<?php

/**
 * @file
 * Extended multilanguage administration and module settings UI.
 */

/**
 * Form builder function.
 */
function language_switcher_admin_settings($form) {
  // Content selection options.
  $form['selection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content selection'),
  );
  $form['selection']['language_switcher_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Content selection mode'),
    '#default_value' => variable_get('language_switcher_mode', 'off'),
    '#options' => _language_switcher_selection_mode(),
    '#description' => t('Determines which content to show depending on the current page language and the default language of the site.'),
  );

  return system_settings_form($form);
}

// List of selection modes
function _language_switcher_selection_mode() {
  return array(
    'strict' => t('Only current language.'),
    'multi' => t('Selected languages from language block.'),
    'off' => t('All content. No language conditions apply.'),
  );
}