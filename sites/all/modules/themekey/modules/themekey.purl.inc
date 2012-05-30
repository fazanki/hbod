<?php

/**
 * @file
 *
 *
 * @see themekey.module
 *
 * @author Markus Kalkbrenner | bio.logis GmbH
 *   @see http://drupal.org/user/124705
 */


/**
 * Implements hook_themekey_properties().
 *
 * Provides additional properties for the ThemeKey module:
 * - purl:active_provider
 *
 * @return
 *   array of themekey properties
 */
function themekey_purl_themekey_properties() {

  // Attributes for properties
  $attributes = array();

  $attributes['purl:active_provider'] = array(
    'description' => t('Purl: Active Provider - The active provider that triggered the current id.'),
#    'validator' => 'themekey_validator_purl_providers',
    // TODO page cache support depends on the purl processors executed
    'page cache' => THEMEKEY_PAGECACHE_UNSUPPORTED,
  );

  // Mapping functions
  $maps = array();

  $maps[] = array(
    'src' => 'system:dummy',
    'dst' => 'purl:active_provider',
    'callback' => 'themekey_purl_dummy2active_provider',
  );
  return array('attributes' => $attributes, 'maps' => $maps);
}


/**
 * ThemeKey mapping function to set a
 * ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:dummy
 * dst: purl:active_provider
 *
 * @return
 *   string
 *   or NULL if no value could be mapped
 */
function themekey_purl_dummy2active_provider() {
  // @see http://drupal.org/node/1529892
  purl_init();

  $active_providers = array();
  foreach (purl_active()->get() as $method => $items) {
    if (is_array($items)) {
      foreach ($items as $item) {
        $active_providers[] = $item->provider;
      }
    }
  }

  if (!empty($active_providers)) {
    return $active_providers;
  }

  return NULL;
}
