CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

The jQuery Easing module makes the jQuery Easing plugin 
(http://gsgd.co.uk/sandbox/jquery/easing/) available for Drupal 
modules to use. It provides no functionality of it's own and should 
only be installed if another module requires it.

INSTALLATION
------------

1. Create a directory named 'easing' under your site's libraries 
   directory (either sites/all/libraries or sites/SITENAME/libraries).

2. Download the jQuery Easing plugin from 
   http://gsgd.co.uk/sandbox/jquery/easing/ and put it in this 
   directory.
   
3. Move/rename (or copy, or symlink) this file to remove the version 
   number from the file name.
    
   eg. `mv jquery.easing.1.3.js jquery.easing.js`
   
   (See http://drupal.org/node/964986 for why this is necessary.)
   
4. Enable the jQuery Easing module as usual. If inastalled correctly, 
   you should see the library version number at admin/reports/status.
