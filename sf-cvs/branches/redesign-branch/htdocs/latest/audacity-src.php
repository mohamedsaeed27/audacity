<?

# perform a redirect to the latest version, so links to it can remain static

// logging
$d = date("Y-m-d G:i:s");
$fp = fopen("download_stats.txt", "a");
fwrite($fp, "$d\tsrc\n");
fclose($fp);

include 'mirror.inc.php';
include 'versions.inc.php';
header('Location: '.mirror.'audacity-src-'.src_version.'.tar.gz');

?>
