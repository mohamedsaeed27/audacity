/**********************************************************************

  Audacity: A Digital Audio Editor

  Experimental.h

  Dominic Mazzoni
  James Crook

  Used for includes and #defines for experimental features.

  When the features become mainstream the include files will 
  move out of here and into the files which need them.  The
  #defines will then be retired.



  JKC: This file solves a problem of how to avoid forking the
  code base when working on new features e.g:
    - Additional displays in Audacity
    - Modular architecture.
  Add #defines in here for the new features, and make your code
  conditional on those #defines.

**********************************************************************/

#ifndef __EXPERIMENTAL__
#define __EXPERIMENTAL__

//Uncomment the next #define to enable experimental features.
#define EXPERIMENTAL_FEATURES

// Comment out the next two lines if you want to disable 'experimental theming'
// Work in progress, June-2008.
//#define EXPERIMENTAL_THEMING
#if IS_BETA
//August 2009 - Theming not locked down enough enough for a stable release.
// we're doing betas which are leading up to an RC.
//#define EXPERIMENTAL_THEME_PREFS
#endif

//Next line enables Mic monitoring at times when it was previously off.
//More work is needed as after recording or playing it results in an 
//unwanted record-cursor on the wave track.
//#define EXPERIMENTAL_EXTRA_MONITORING

//#define EXPERIMENTAL_ROLL_UP_DIALOG
//#define RIGHT_ALIGNED_TEXTBOXES
//#define EXPERIMENTAL_VOICE_DETECTION

// Effect categorisation. Adds support for arranging effects in categories
// and displaying those categories as submenus in the Effect menu.
// This was a 2008 GSoC project that was making good progress at the half-way point
// but then the student didn't contribute after that.  It needs a bit of work to finish it off.
// As a minimum, if this is turned on for a release,
// it should have an easy mechanism to disable it at run-time, such as a menu item or a pref,
// preferrably disabled until other work is done.  Martyn 22/12/2008.
//#define EFFECT_CATEGORIES

// Andreas Micheler, 20.Nov 2007: 
// A spectrumLogF-like view mode with notes quantization.
// Just select the "Find Notes" checkbox in the spectrum prefs 
// to activate it instead of the Spectrum log(f) mode.
//#define EXPERIMENTAL_FIND_NOTES

// AM, 22.Nov 2007
// Skip Points support in the spectrum view mode.
//#define EXPERIMENTAL_FFT_SKIP_POINTS

// AM, 22.Nov 2007: 
// A Frequency Grid for the Spectrum Log(f) & Find Notes modes
//#define EXPERIMENTAL_FFT_Y_GRID

// Andy Coder, 03.Mar 2009:
// Allow keyboard seeking before initial playback position
//#define EXPERIMENTAL_SEEK_BEHIND_CURSOR

// Philip Van Baren 01 July 2009
// Replace RealFFT() and PowerSpectrum function to use (faster) RealFFTf function
#define EXPERIMENTAL_USE_REALFFTF

// RBD, 1 Sep 2008
// Enables MIDI Output of NoteTrack (MIDI) data during playback
// USE_MIDI must be defined in order for EXPERIMENTAL_MIDI_OUT to work
#ifdef USE_MIDI
//#define EXPERIMENTAL_MIDI_OUT
#endif

// USE_MIDI must be defined in order for EXPERIMENTAL_SCOREALIGN to work
#ifdef USE_MIDI
//#define EXPERIMENTAL_SCOREALIGN
#endif

#ifdef EXPERIMENTAL_FEATURES
   // The first experimental feature is a notebook that adds
   // a tabbed divider to the project.
   //#define EXPERIMENTAL_NOTEBOOK
   // The notebook in turn can contain:
   // 1. The Nyquist Inspector, which is a browser for the objects in 
   // Audacity.
   //#define EXPERIMENTAL_NYQUIST_INSPECTOR
   // 2. The Vocal Studio, a screen for working with vocal sounds
   // particularly vowel sounds.
   //#define EXPERIMENTAL_VOCAL_STUDIO
   // 3. The Audacity Tester is an extended version of the benchmarks
   // display.  The crucial idea is to be able to compare waveforms
   // where effects have been applied by audacity but using different 
   // block-sizes.  This should give high confidence that we don't
   // suffer from end-effects on buffers, e.g. losing one sample on
   // each buffer.
   //#define EXPERIMENTAL_AUDACITY_TESTER

   // A long term plan is to use dso's and dlls for Audacity extensions
   // These are 'WX' plug ins that manage their own displays using
   // wxWindows.
   //#define EXPERIMENTAL_WX_PLUG_INS
#endif

//If you want any of these files, ask JKC.  They are not
//yet checked in to Audacity CVS as of 10-Oct-2004
#ifdef EXPERIMENTAL_NOTEBOOK
#include "widgets/GuiFactory.h"
#include "widgets/APanel.h"
extern void AddPages(   AudacityProject * pProj, GuiFactory & Factory,  wxNotebook  * pNotebook );
#endif

#ifdef EXPERIMENTAL_NYQUIST_INSPECTOR
#include "NyquistAdapter.h"
#endif

#ifdef EXPERIMENTAL_AUDACITY_TESTER
#endif

#define EXPERIMENTAL_LYRICS_WINDOW //vvv Vaughan, 2009-07-06
#define EXPERIMENTAL_MIXER_BOARD //vvv Vaughan, 2009-07-07

#if USE_PORTMIXER
   //Automatically tries to find an acceptable input volume
   //#define AUTOMATED_INPUT_LEVEL_ADJUSTMENT
#endif

#endif

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag:

