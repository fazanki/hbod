<!DOCTYPE html>
<!--[if lt IE 7]> <html class="ie6 ie" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"> <![endif]-->
<!--[if IE 7]>    <html class="ie7 ie" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"> <![endif]-->
<!--[if IE 8]>    <html class="ie8 ie" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"> <![endif]-->
<!--[if gt IE 8]> <!--> <html class="" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"> <!--<![endif]-->
<head>
  <?php print $head; ?>
  <!-- Set the viewport width to device width for mobile -->
  <meta name="viewport" content="width=device-width" />
  <title><?php print $head_title; ?></title>
  <meta name="title" content="Hrvatsko botaničko društvo – botaničari Hrvatske" />
  <meta name="description" content="Djelovanje  Hrvatskog botaničkog društva usmjereno je k promicanju i razvitku botaničkih znanosti na popularnoj, stručnoj i znanstvenoj razini preko projekata i predavanja. Društvo se zalaže za očuvanje i zaštitu flore, vegetacije i staništa te ukupne biološke raznolikosti Hrvatske. Ono sudjeluje u povezivanju i poticanju stručne i znanstvene suradnje sa studentskim, građanskim i drugim organizacijama na Sveučilištu, Republici Hrvatskoj i svijetu." />
  <meta name="keywords" content="Hrvatsko botaničko društvo, botaničko društvo, botanika, botaničari, udruga, strukovna udruga, Zagreb, Botanički vrt" />
  <meta name="language" content="<?php print $language->language; ?>" />
  <meta name="robots" content="index, follow, NOODP" />
  <?php print $styles; ?>
  
  
  
  <?php print $scripts; ?>
  
  <!-- IE Fix for HTML5 Tags -->
  <!--[if lt IE 9]>
    <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
  <![endif]-->
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
  <!-- script type="text/javascript" src="http://maps.googleapis.com/maps/api/js?key=AIzaSyCDHoUUHq9l6fQhUEyxYveS2SLvIetA8Jk&sensor=true"></script -->
  <script type="text/javascript" src="http://maps.googleapis.com/maps/api/js?sensor=true"></script>
    <script type="text/javascript">
        if(HRBOT.Sitewide && HRBOT.Sitewide.Controller) {
            HRBOT.Sitewide.Controller.init();
        }
    </script>

    <script type="text/javascript">

    //   var _gaq = _gaq || [];
    //   _gaq.push(['_setAccount', 'UA-37059057-1']);
    //   _gaq.push(['_setDomainName', 'hbod.hr']);
    //   _gaq.push(['_trackPageview']);

    //   (function() {
    //     var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
    //     ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    //     var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
    //   })();

     </script>
    
</html>