<?php

/**
 * @file
 * Webform Localization i18n_string integration.
 */

/**
 * Provides interface with the i18n_string module.
 * Based in patch http://drupal.org/node/245424#comment-5244256
 * by Calin Marian. Further development sponsored by Riot Games.
 *
 * @author German Martin <gmartin.prg@gmail.com>
 */

/**
 * Translates the component properties that are translatable.
 *
 * These are found in under 'translated_strings' in the 'extra' array of the
 * component, which is build when the component is inserted / updated, or
 * when all webform strings are updated from
 * admin/config/regional/translate/i18n_string.
 *
 * @param array $element
 *   The FAPI renderable array of the component instance.
 * @param array $component
 *   The component.
 */
function _webform_localization_translate_component(&$element, $component) {
  if (isset($component['extra']['translated_strings']) && is_array($component['extra']['translated_strings'])) {
    foreach ($component['extra']['translated_strings'] as $name) {
      $name_list = explode(':', $name);
      $current_element = &$element;
      if (strpos($name_list[3], '[') !== FALSE) {
        // The property is deeper in the renderable array, we must extract the
        // the place where it is.
        list ($children, $property) = explode(']#', $name_list[3]);
        // Remove the '[' from the begining of the string.
        $children = drupal_substr($children, 1);
        $children_array = explode('][', $children);
        foreach ($children_array as $child) {
          if (isset($current_element[$child])) {
            $current_element = &$current_element[$child];
          }
          else {
            continue;
          }
        }
      }
      else {
        // Remove the '#' from the begining of the property, for consistency.
        $property = drupal_substr($name_list[3], 1);
      }
      if (strpos($property, '-') !== FALSE) {
        // If property is array, we extract the key from the property.
        list ($property, $key) = explode('-', $property);
        if (isset($current_element['#' . $property][$key])) {
          $current_element['#' . $property][$key] = webform_tt($name, $current_element['#' . $property][$key]);
        }
      }
      else {
        // If we are dealing with option groups.
        if (isset($name_list[4]) && strpos($name_list[4], '/-') !== FALSE) {
          $option_group = str_replace('/-', '', $name_list[4]);
          // If it's a element.
          if (isset($name_list[5])) {
            $current_element['#' . $property][$option_group][$name_list[5]] = webform_tt($name, $current_element['#' . $property][$option_group][$name_list[5]]);
          }
          else {
            // If it's a option group we translate the key.
            $translated_option_group = webform_tt($name, $option_group);
            if ($translated_option_group != $option_group) {
              _webform_localization_array_key_replace($current_element['#' . $property], $option_group, $translated_option_group);
            }
          }
        }
        else {
          // Else we can treat the property as string.
          if (isset($current_element['#' . $property])) {
            if ($property == 'markup' && $current_element['#type'] == 'markup') {
              $current_element['#' . $property] = i18n_string($name, $current_element['#' . $property], array('format' => $current_element['#format']));
            }
            else {
              $current_element['#' . $property] = webform_tt($name, $current_element['#' . $property]);
            }
          }
        }
      }
    }
  }
}

/**
 * Update / create translation source for all the translatable poperties.
 *
 * @param array $component
 *   A webform component.
 */
function webform_localization_component_update_translation_strings(&$component) {
  // Fill in the the default values for the missing properties.
  module_load_include('inc', 'webform', 'includes/webform.components');
  webform_component_defaults($component);
  // Render the 'render' FAPI array for the component.
  $element = webform_component_invoke($component['type'], 'render', $component, NULL, 'html');
  // Parse the renderable array to find the translatable properties and
  // update / create translation source for them.
  $component['extra']['translated_strings'] = _webform_localization_component_translation_parse($element, $component);
  // Render the 'display' FAPI array for the component.
  $element = webform_component_invoke($component['type'], 'display', $component, NULL, 'html');
  // Parse the renderable array to find the translatable properties and
  // update / create translation source for them.
  $component['extra']['translated_strings'] = array_merge($component['extra']['translated_strings'], array_diff(_webform_localization_component_translation_parse($element, $component), $component['extra']['translated_strings']));
}

/**
 * Parse a component renderable array to find the translatable properties.
 *
 * Create / update or remove translation source for translatable properties
 * of a webform component.
 *
 * @param array $element
 *   The renderable array to be parsed.
 * @param array $component
 *   The component which was rendered.
 * @return
 *   An array of translatabled webform properties.
 *
 */
function _webform_localization_component_translation_parse($element, $component) {
  $translated_properies = array();
  if (!isset($element['#parents'])) {
    $element['#parents'] = array();
  }

  if (isset($element['#translatable']) && is_array($element['#translatable'])) {
    foreach ($element['#translatable'] as $key) {
      if (isset($element['#' . $key]) && $element['#' . $key] != '') {
        if (isset($element['#parents']) && count($element['#parents'])) {
          $property = '[' . implode('][', $element['#parents']) . ']#' . $key;
        }
        else {
          $property = '#' . $key;
        }
        if (is_array($element['#' . $key])) {
          // If the translatable property is an array, we translate the
          // children.
          foreach ($element['#' . $key] as $elem_key => $elem_value) {
            // If the child if an array, we translate the elements.
            if (is_array($elem_value)) {
              foreach ($elem_value as $k => $v) {
                $name = implode(':',
                                array(
                                      'webform',
                                      $component['nid'],
                                      $component['cid'],
                                      $property,
                                      '/-' . $elem_key . '/-',
                                      $k,
                                      )
                                );
                $translated_properies[] = $name;
                webform_tt($name, $v, NULL, TRUE);
              }
              $name = implode(':',
                              array(
                                    'webform',
                                    $component['nid'],
                                    $component['cid'],
                                    $property,
                                    '/-' . $elem_key . '/-',
                                    )
                              );
              $translated_properies[] = $name;
              webform_tt($name, $elem_key, NULL, TRUE);
            }
            else {
              // If the child is not an array.
              $name = implode(':', array('webform',
                    $component['nid'], $component['cid'],
                    $property . '-' . $elem_key));
              $translated_properies[] = $name;
              webform_tt($name, $elem_value, NULL, TRUE);
            }
          }
        }
        else {
          /**
           * If the translatable property is not an array,
           * it can be treated as a string.
           */
          $name = implode(
                          ':',
                          array(
                                'webform',
                                $component['nid'],
                                $component['cid'],
                                $property
                                )
                          );
          $translated_properies[] = $name;
          webform_tt($name, $element['#' . $key], NULL, TRUE);
        }
      }
    }
  }

  // Recursevly call the function on the children, after adding the children
  // name to its #parents array.
  foreach (element_children($element) as $child) {
    $element[$child]['#parents'] = $element['#parents'];
    $element[$child]['#parents'][] = $child;
    // Add the translated propertied to the list.
    $translated_properies = array_merge(
      $translated_properies,
      _webform_localization_component_translation_parse($element[$child], $component)
      );
  }

  return $translated_properies;
}

/**
 * Delete translation source for all the translatable poperties
 *
 * Process components matching webforms configuration.
 */
function webform_localization_delete_all_strings() {
  $query = db_select('webform_component', 'wc');
  $query->fields('wc');
  $query->condition('wl.expose_strings', 0, '=');
  $query->innerJoin('webform_localization', 'wl', 'wc.nid = wl.nid');
  $components = $query->execute()->fetchAllAssoc('cid');
  foreach ($components as $component) {
    $component = (array) $component;
    $component['extra'] = unserialize($component['extra']);

    webform_localization_component_delete_translation_strings($component);

    $component['extra'] = serialize($component['extra']);
    drupal_write_record('webform_component', $component, array('nid', 'cid'));
  }
}

/**
 * Remove translation source for all the translatable poperties.
 *
 * @param array $component
 *   A webform component array.
 */
function webform_localization_component_delete_translation_strings($component) {
  if (isset($component['extra']['translated_strings'])) {
    foreach ($component['extra']['translated_strings'] as $name) {
      i18n_string_remove($name);
    }
  }
}

/**
 * Update / create translation source for general webform poperties.
 *
 * @param array $properties
 *   The form_state values that have been saved.
 */
function webform_localization_update_translation_strings($properties) {
  if (!empty($properties['confirmation']['value'])) {
    $name = implode(':', array('webform', $properties['nid'], 'confirmation'));
    webform_tt($name, $properties['confirmation']['value'], NULL, TRUE);
  }
  if (!empty($properties['submit_text'])) {
    $name = implode(':', array('webform', $properties['nid'], 'submit_text'));
    webform_tt($name, $properties['submit_text'], NULL, TRUE);
  }

  // Allow to translate the redirect url if it's not set to none or the
  // default confirmation page.
  if (!in_array($properties['redirect_url'], array('<confirmation>', '<none>'))) {
    $name = implode(':', array('webform', $properties['nid'], 'redirect_url'));
    webform_tt($name, $properties['redirect_url'], NULL, TRUE);
  }
}

/**
 * Translate general webform properties.
 *
 * @param $node
 *   A node object.
 */
function webform_localization_translate_strings(&$node, $update = FALSE) {
  $name = implode(':', array('webform',
        $node->webform['nid'],
        'confirmation'));
  $node->webform['confirmation'] = webform_tt(
          $name,
          $node->webform['confirmation'],
          NULL,
          $update);
  $name = implode(
          ':',
          array(
            'webform',
            $node->webform['nid'],
            'submit_text'));
  $node->webform['submit_text'] = webform_tt(
          $name,
          $node->webform['submit_text'],
          NULL,
          $update);

  // Allow to translate the redirect url if it's not set to none or the
  // default confirmation page.
  if (!in_array($node->webform['redirect_url'], array('<confirmation>', '<none>'))) {
    $name = implode(':', array('webform', $node->webform['nid'], 'redirect_url'));
    $node->webform['redirect_url'] = webform_tt($name, $node->webform['redirect_url'], NULL, $update);
  }
}

/**
 * Update / create translation source for webform email poperties.
 *
 * @param array $properties
 *   The form_state values that have been saved.
 */
function webform_localization_emails_update_translation_string($properties) {
  $nid = $properties['node']->webform['nid'];
  $eid = $properties['eid'];
  if (!empty($properties['subject_custom'])) {
    $name = implode(':',
            array('webform',
              $nid,
              'email',
              $eid,
              'subject_custom'));
    webform_tt($name, $properties['subject_custom'], NULL, TRUE);
  }
  // Allow to translate the mail recipients if not based on a component.
  if (!empty($properties['email']) && !is_numeric($properties['email'])) {
    $name = implode(':', array('webform', $nid, 'email', $eid, 'email'));
    webform_tt($name, $properties['email'], NULL, TRUE);
  }
  if (!empty($properties['from_name_custom'])) {
    $name = implode(':', array('webform',
          $nid,
          'email',
          $eid,
          'from_name_custom'));
    webform_tt($name, $properties['from_name_custom'], NULL, TRUE);
  }
  if (!empty($properties['template'])) {
    $name = implode(':', array('webform', $nid, 'email', $eid, 'template'));
    webform_tt($name, $properties['template'], NULL, TRUE);
  }
}

/**
 * Update / create translation source for webform email poperties.
 *
 * @param $emails
 *   An array of webform emails.
 * @param $nid
 *   The node Id of the webform.
 */
function webform_localization_emails_translation_string_refresh($emails, $nid) {
  foreach ($emails as $email) {
    $eid = $email['eid'];
    if (!empty($email['subject']) && $email['subject'] != 'default') {
      $name = implode(':', array('webform',
            $nid,
            'email',
            $eid,
            'subject_custom'));
      webform_tt($name, $email['subject'], NULL, TRUE);
    }
    // Allow to translate the mail recipients if not based on a component.
    if (!empty($email['email']) && !is_numeric($email['email'])) {
      $name = implode(':', array('webform', $nid, 'email', $eid, 'email'));
      webform_tt($name, $email['email'], NULL, TRUE);
    }
    if (!empty($email['from_name']) && $email['from_name'] != 'default') {
      $name = implode(':', array('webform',
            $nid,
            'email',
            $eid,
            'from_name_custom'));
      webform_tt($name, $email['from_name'], NULL, TRUE);
    }
    if (!empty($email['template']) && $email['template'] != 'default') {
      $name = implode(':', array('webform', $nid, 'email', $eid, 'template'));
      webform_tt($name, $email['template'], NULL, TRUE);
    }
  }
}

/**
 * Translate webform email poperties.
 *
 * @param $node
 *   A node object.
 */
function webform_localization_email_translate_strings(&$node) {
  $nid = $node->webform['nid'];
  foreach ($node->webform['emails'] as $eid => &$email) {
    if (!empty($email['subject']) && $email['subject'] != 'default') {
      $name = implode(':', array('webform',
            $nid,
            'email',
            $eid,
            'subject_custom'));
      $email['subject'] = webform_tt($name, $email['subject']);
    }
    // Allow to translate the mail recipients if not based on a component.
    if (!empty($email['email']) && !is_numeric($email['email'])) {
      $name = implode(':', array('webform', $nid, 'email', $eid, 'email'));
      $email['email'] = webform_tt($name, $email['email']);
    }
    if (!empty($email['from_name']) && $email['from_name'] != 'default') {
      $name = implode(':', array('webform',
            $nid,
            'email',
            $eid,
            'from_name_custom'));
      $email['from_name'] = webform_tt($name, $email['from_name']);
    }
    if (!empty($email['template']) && $email['template'] != 'default') {
      $name = implode(':', array('webform', $nid, 'email', $eid, 'template'));
      $email['template'] = webform_tt($name, $email['template']);
    }
  }
}

/**
 * Remove translation source for webform email poperties.
 *
 * @param $eid
 *   A webform email Id.
 * @param $nid
 *   A node Id.
 */
function webform_localization_emails_delete_translation_string($eid, $nid) {
  $name = implode(':', array('webform', $nid, 'email', $eid, 'subject_custom'));
  i18n_string_remove($name);
  $name = implode(
          ':',
          array('webform',
            $nid,
            'email',
            $eid,
            'from_name_custom'));
  i18n_string_remove($name);
  $name = implode(':', array('webform', $nid, 'email', $eid, 'template'));
  i18n_string_remove($name);
}

/**
 * Translate general webform poperties.
 *
 * @param $node
 *   A node object.
 */
function webform_localization_delete_translate_strings($node) {
  $name = implode(':', array('webform', $node->webform['nid'], 'confirmation'));
  i18n_string_remove($name);
  $name = implode(':', array('webform', $node->webform['nid'], 'submit_text'));
  i18n_string_remove($name);
  foreach ($node->webform['emails'] as $eid => $value) {
    webform_localization_emails_delete_translation_string($eid, $node->nid);
  }
}

/**
 * Helper function to replace an array key and its content.
 *
 * @param $array
 *   Array To process.
 * @param $old_key
 *   Array key to be replaced.
 * @param $new_key
 *   The new array key.
 *
 */
function _webform_localization_array_key_replace(&$array, $old_key, $new_key) {
  $keys = array_keys($array);
  $values = array_values($array);
  foreach ($keys as $k => $v) {
    if ($v == $old_key) {
      $keys[$k] = $new_key;
    }
  }
  $array = array_combine($keys, $values);
}
