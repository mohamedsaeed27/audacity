/**********************************************************************

  Audacity: A Digital Audio Editor

  LegacyBlockFile.cpp

  Dominic Mazzoni

  Audacity 1.1.0 block file format:

  1. Header tag: 20 bytes
     "AudacityBlockFile110"
  2. 64K summaries (min, max, RMS, each a 4-byte float)
  3. 256 summaries (min, max, RMS, each a 4-byte float)

**********************************************************************/

#include <float.h>
#include <math.h>

#include <wx/ffile.h>
#include <wx/utils.h>

#include "LegacyBlockFile.h"
#include "../FileFormats.h"

#include "sndfile.h"


/// Construct a LegacyBlockFile memory structure that will point to an
/// existing block file.  This file must exist and be a valid block file.
///
/// @param existingFile The disk file this LegacyBlockFile should use.
LegacyBlockFile::LegacyBlockFile(wxFileName existingFile,
                                 sampleFormat format,
                                 sampleCount summaryLen,
                                 sampleCount len):
   BlockFile(existingFile, len),
   mFormat(format)
{
   if( !existingFile.FileExists() )
      // throw an exception?
      ;

   printf("LegacyBlockFile!\n");

   mSummaryInfo.bytesPerFrame = sizeof(float) * 3; /* min, max, rms */
   mSummaryInfo.totalSummaryBytes = summaryLen;
   mSummaryInfo.offset64K = 20; /* legacy header tag len */
   mSummaryInfo.frames64K = (summaryLen-20) / 256;
   mSummaryInfo.offset256 = mSummaryInfo.offset64K +
      (mSummaryInfo.frames64K * mSummaryInfo.bytesPerFrame);
   mSummaryInfo.frames256 =
      (summaryLen - 20 -
       (mSummaryInfo.frames64K * mSummaryInfo.bytesPerFrame)) /
      mSummaryInfo.bytesPerFrame;

   //
   // Compute the min, max, and RMS of the block from the
   // 64K summary data
   //

   float *data = new float[mSummaryInfo.frames64K * 3];
   wxFFile summaryFile;
   if( !summaryFile.Open(mFileName.GetFullPath(), "rb") ) {


      printf("Couldn't open '%s'\n", (const char *)mFileName.GetFullPath());


      delete[] data;
      return;
   }
   summaryFile.Seek(mSummaryInfo.offset64K);
   int read = summaryFile.Read(data,
                               mSummaryInfo.frames64K *
                               mSummaryInfo.bytesPerFrame);

   int count = read / mSummaryInfo.bytesPerFrame;

   mMin = FLT_MAX;
   mMax = FLT_MIN;
   float sumsq = 0;

   for(int i=0; i<count; i++) {
      if (data[3*i] < mMin)
         mMin = data[3*i];
      if (data[3*i+1] > mMax)
         mMax = data[3*i+1];
      sumsq += data[3*i+2]*data[3*i+2];
   }
   mRMS = sqrt(sumsq / count);

   delete[] data;

   printf("%s: min=%.3f max=%.3f, rms=%.3f\n",
          (const char *)mFileName.GetFullPath(), mMin, mMax, mRMS);
}

LegacyBlockFile::~LegacyBlockFile()
{
}

/// Read the summary section of the disk file.
///
/// @param *data The buffer to write the data to.  It must be at least
/// mSummaryinfo.totalSummaryBytes long.
bool LegacyBlockFile::ReadSummary(void *data)
{
   wxFFile summaryFile;

   if( !summaryFile.Open(mFileName.GetFullPath(), "rb") )
      return false;

   int read = summaryFile.Read(data, (size_t)mSummaryInfo.totalSummaryBytes);

   return (read == mSummaryInfo.totalSummaryBytes);
}

/// Read the data portion of the block file using libsndfile.  Convert it
/// to the given format if it is not already.
///
/// @param data   The buffer where the data will be stored
/// @param format The format the data will be stored in
/// @param start  The offset in this block file
/// @param len    The number of samples to read
int LegacyBlockFile::ReadData(samplePtr data, sampleFormat format,
                              sampleCount start, sampleCount len)
{
   SF_INFO info;

   memset(&info, 0, sizeof(info));

   switch(mFormat) {
   case int16Sample:
      info.format =
         SF_FORMAT_RAW | SF_FORMAT_PCM_16 | SF_ENDIAN_CPU;
      break;
   default:
   case floatSample:
      info.format =
         SF_FORMAT_RAW | SF_FORMAT_FLOAT | SF_ENDIAN_CPU;
      break;
   case int24Sample:
      info.format = SF_FORMAT_RAW | SF_FORMAT_PCM_32 | SF_ENDIAN_CPU;
      break;
   }
   info.samplerate = 44100; // Doesn't matter
   info.channels = 1;
   info.frames = mLen + (mSummaryInfo.totalSummaryBytes /
                         SAMPLE_SIZE(mFormat));
   
   SNDFILE *sf = sf_open(mFileName.GetFullPath(), SFM_READ, &info);

   if (!sf)
      return 0;

   sf_count_t seekstart = start +
      (mSummaryInfo.totalSummaryBytes / SAMPLE_SIZE(mFormat));
   sf_seek(sf, seekstart , SEEK_SET);

   samplePtr buffer = NewSamples(len, floatSample);

   int framesRead = 0;

   // If both the src and dest formats are integer formats,
   // read integers from the file (otherwise we would be
   // converting to float and back, which is unneccesary)
   if (format == int16Sample &&
       sf_subtype_is_integer(info.format)) {
      framesRead = sf_readf_short(sf, (short *)data, len);
   }
   else
   if (format == int24Sample &&
       sf_subtype_is_integer(info.format))
   {
      framesRead = sf_readf_int(sf, (int *)data, len);

      // libsndfile gave us the 3 byte sample in the 3 most
      // significant bytes -- we want it in the 3 least
      // significant bytes.  The cast to unsigned prevents
      // sign extension.
      int *intPtr = (int *)data;
      for( int i = 0; i < framesRead; i++ )
         intPtr[i] = (unsigned int)intPtr[i] >> 8;
   }
   else {
      // Otherwise, let libsndfile handle the conversion and
      // scaling, and pass us normalized data as floats.  We can
      // then convert to whatever format we want.
      framesRead = sf_readf_float(sf, (float *)buffer, len);
      CopySamples(buffer, floatSample,
                  (samplePtr)data, format, framesRead);
   }

   DeleteSamples(buffer);

   sf_close(sf);

   return framesRead;
}

void LegacyBlockFile::SaveXML(int depth, wxFFile &xmlFile)
{
   for(int i = 0; i < depth; i++)
      xmlFile.Write("\t");
   xmlFile.Write("<legacyblockfile ");
   xmlFile.Write(wxString::Format("filename='%s' ", mFileName.GetFullName().c_str()));
   xmlFile.Write(wxString::Format("len='%d' ", mLen));
   xmlFile.Write(wxString::Format("summarylen='%d' ", mSummaryInfo.totalSummaryBytes));
   xmlFile.Write("/>\n");
}

/// static
BlockFile *LegacyBlockFile::BuildFromXML(wxString projDir, const char **attrs,
                                         sampleFormat format)
{
   wxFileName fileName;
   sampleCount len = 0;
   sampleCount summaryLen = 0;

   while(*attrs)
   {
       const char *attr =  *attrs++;
       const char *value = *attrs++;

       if( !strcmp(attr, "name") )
          fileName.Assign(projDir, value);
       if( !strcmp(attr, "len") )
          len = atoi(value);
       if( !strcmp(attr, "summarylen") )
          summaryLen = atoi(value);
   }

   return new LegacyBlockFile(fileName, format, summaryLen, len);
}

/// Create a copy of this BlockFile, but using a different disk file.
///
/// @param newFileName The name of the new file to use.
BlockFile *LegacyBlockFile::Copy(wxFileName newFileName)
{
   BlockFile *newBlockFile = new LegacyBlockFile(newFileName,
                                                 mFormat,
                                                 mSummaryInfo.totalSummaryBytes,
                                                 mLen);

   return newBlockFile;
}

int LegacyBlockFile::GetSpaceUsage()
{
   wxFFile dataFile(mFileName.GetFullPath());
   return dataFile.Length();
}
