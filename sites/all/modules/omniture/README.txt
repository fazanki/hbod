Module: Omniture Integration
Author: Greg Knaddison (greggles) http://knaddison.com
Co-Maintainer: Matthew Tucker (ultimateboy)
Co-Maintainer: Alexander Ross (bleen18) http://bleen.net
Based on Google Analytics module by Mike Carter www.ixis.co.uk


Description
===========
Adds the Omniture statistics system to your website.


Requirements
============

* Omniture user account


Installation
============
* Copy the 'omniture' module directory in to your Drupal
modules directory as usual.


Customization
=============
Here is an example of module code that you can use to create variables more
suited to tracking your needs by utilizing hook_omniture_variables(). This code
should go in your custom module's .module file and modified accordingly. For
illustration purposes we are adding a setting to the omniture administration
form to allow site administrators to control whether or not they want to track
our custom "referring_search_engine" variable.
Note: Do not forget to rename the functions.

<?php

  /**
   * Implements hook_omniture_variables().
   */
  function mymodule_omniture_variables() {
    // Initialize a variables array to be returned by this hook.
    $variables = array();

    if (variable_get('omniture_track_search_engine', 0)) {
      $variables['referring_search_engine'] = 'none';

      // Create a list of possible search engines that my site cares about.
      $search_engines = array('google.com', 'yahoo.com', 'bing.com', 'ask.com');

      // Get the refering URL.
      $referer = $_SERVER['HTTP_REFERER'];

      // Check the refering URL to see if the request is coming from a search engine
      // and if it is, change the value of my "referring_search_engine" variable.
      foreach ($search_strings as $engine)  {
        if (stripos($referer, $engine) !== FALSE) {
          $variables['referring_search_engine'] = $engine;
          break;
        }
      }
    }

    return $variables;
  }

  /**
   * Implements hook_form_FORM_ID_alter().
   */
  function mymodule_form_omniture_admin_settings_alter(&$form, &$form_state) {
    $form['general']['omniture_track_search_engine'] = array(
      '#type' => 'checkbox',
      '#title' => t('Track the referring search engine for every request'),
      '#default_value' => variable_get('omniture_track_search_engine', 0),
    );
  }

?>

Usage
=====
You will also need to define what user roles should be tracked.
Simply tick the roles you would like to monitor.

You can also add JavaScript code in the "Advanced" section of the settings.

All pages will now have the required JavaScript added to the
HTML footer can confirm this by viewing the page source from
your browser.

'admin/' pages are automatically ignored by this module.
