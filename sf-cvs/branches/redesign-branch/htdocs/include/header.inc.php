<?php
/*
 * Copyright 2004 Matt Brubeck
 * This file is licensed under a Creative Commons license:
 * http://creativecommons.org/licenses/by/2.0/
 */
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="<?=$lang?>">
<head>
  <title><?=_("Audacity")?>: <?=$pageTitle?></title>
  <meta http-equiv="Content-Type" content="text/html; charset=<?=$encoding?>">
  <link rel="stylesheet" href="<?=$sitePath?>/style.css" type="text/css">
</head>
<body>
<p><a class="skip" href="#content"><?=_("Jump to page content")?></a></p>

<form id="search" action="http://google.com/custom" method="get"><p>
  <label for="q"><a href="http://www.google.com/"><img alt="Google" title="<?=_("Powered by Google")?>" src="http://www.google.com/logos/Logo_25wht.gif"></a></label>
  <input name="q" id="q" class="text">
  <input type="hidden" name="sitesearch" value="audacity.sourceforge.net">
  <input type="hidden" name="cof" value="S:http://audacity.sourceforge.net/;AH:center;LH:100;L:http://audacity.sourceforge.net/images/audacity.jpg;LW:253;AWFID:a42d5b7b681df0fd;">
  <input type="hidden" name="domains" value="audacity.sourceforge.net">
  <input type="submit" value="<?=_("Search")?>">
</p></form>

<?php
  $titleStr = _("Audacity: Free Sound Editor and Recording Software");
  $titleImg = "<img title=\"$titleStr\" alt=\"$titleStr\" src=\"$sitePath/images/audacity.jpg\" width=\"253\" height=\"100\">";

  if ($sectionId == "" && $pageId == "") {
    echo "<h1>$titleImg</h1>";
  }
  else {
    echo "<h1><a href=\"$sitePath/\">$titleImg</a></h1>";
  }
?>

<div id="sitenav">
  <h2><?=_("Site Navigation")?></h2>
  <ul>
    <?php
      foreach ($siteNavItems as $navItem) {
        $name = $navItem[0];
        $path = $navItem[1];
        if ($sectionId == $path) {
          $sectionNavItems = $navItem[2];
          ?><li class="selected"><?=$name?></li><?php
        }
        else {
          ?><li><a href="<?=$sitePath?>/<?=$path?>"><?=$name?></a></li><?php
        }
      }
    ?>
  </ul>
</div>

<?php
  if ($sectionNavItems) {
  ?>
  <div id="subnav">
    <?php
      if ($pageId == "") {
        ?><h3 class="selected"><?=$sectionTitle?></h3><?php
      }
      else {
        ?><h3><a href="<?=$sectionPath?>/"><?=$sectionTitle?></a></h3><?php
      }
    ?>
    <ul>
      <?php
        foreach ($sectionNavItems as $navItem) {
          $name = $navItem[0];
          $path = $navItem[1];
          if ($pageId == $path) {
            ?><li class="selected"><?=$name?></li><?php
          }
          else {
            ?><li><a href="<?=$sectionPath?>/<?=$path?>"><?=$name?></a></li><?php
          }
        }
      ?>
    </ul>
  </div>
  <?php
  }
?>

<div id="content"<?php if ($sectionNavItems) echo ' class="afternav"';?>>
