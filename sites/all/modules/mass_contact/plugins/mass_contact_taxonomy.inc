<?php

/**
 * @file mass_contact plugin type groupping_method.
 *
 * Select users by taxonomy terms.
 */

$plugin = array(
  'create_recipient_list_callback' => 'mass_contact_taxonomy_create_recipient_list',
  'mass_contact_admin_categories_callback' => 'mass_contact_taxonomy_admin_categories',
  'mass_contact_admin_edit' => 'mass_contact_taxonomy_admin_edit',
  'mass_contact_admin_edit_validate' => 'mass_contact_taxonomy_admin_edit_validate',
  'mass_contact_admin_edit_submit' => 'mass_contact_taxonomy_admin_edit_submit',
);

/**
 * Callback to retrieve users by taxonomy terms.
 *
 * Get the user IDs for all the users that have specified taxonomy terms
 * attached to user object.
 */
function mass_contact_taxonomy_create_recipient_list($recipients) {
  $tids = array();
  if (!isset($recipients['mass_contact_taxonomy']) || empty($recipients['mass_contact_taxonomy'])) {
    return array();
  }
  $tids = $recipients['mass_contact_taxonomy'];

  // Query to select users by term.
  // Check all fields of user entity and if field is taxonomy term reference,
  // add condition.
  $query = new EntityFieldQuery();
  $user_fields = field_info_instances('user');
  foreach ($user_fields['user'] as $user_field_instance) {
    $field = field_info_field($user_field_instance['field_name']);
    if ($field['module'] == 'taxonomy') {
      $query->fieldCondition($field['field_name'], 'tid', $tids, 'IN');
    }
  }
  $result = $query->execute();
  // Collect uids.
  $uids = array();
  if (isset($result['user'])) {
    $uids = array_keys($result['user']);
  }

  return $uids;
}

/**
 * Callback to prepare taxonomy term names for admin/config/system/mass_contact
 * page column Recipients.
 */
function mass_contact_taxonomy_admin_categories($recipients) {
  $tids = array();
  if (!isset($recipients['mass_contact_taxonomy']) || empty($recipients['mass_contact_taxonomy'])) {
    return;
  }
  $tids = $recipients['mass_contact_taxonomy'];

  $terms = taxonomy_term_load_multiple($tids);
  if (empty($terms)) {
    return;
  }

  $term_names = array();
  foreach ($terms as $term) {
    $term_names[] = $term->name;
  }

  return t('Taxonomy terms: %terms', array('%terms' => implode(', ', $term_names)));
}

/**
 * Form element for Category add/edit page.
 *
 * @param array $terms
 * @return type
 */
function mass_contact_taxonomy_admin_edit($recipients) {
  // Prepare array of field_name => vocabulary of taxonomy fields.
  $user_fields = field_info_instances('user');
  $taxonomy_fields = array();
  foreach ($user_fields['user'] as $user_field_instance) {
    $field = field_info_field($user_field_instance['field_name']);
    if ($field['module'] == 'taxonomy') {
      $taxonomy_fields[$field['field_name']] = $field['settings']['allowed_values'][0]['vocabulary'];
    }
  }

  $form_element = array();
  foreach ($taxonomy_fields as $field_name => $vocabulary_name) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
    $terms = taxonomy_get_tree($vocabulary->vid);
    $options = array();
    foreach ($terms as $term) {
      $options[$term->tid] = check_plain($term->name);
    }
    $default_value = array();
    if (isset($recipients['mass_contact_taxonomy'])) {
      $default_value = $recipients['mass_contact_taxonomy'];
    }
    $form_element[$field_name] = array(
      '#type' => 'checkboxes',
      '#title' => t('Taxonomy vocabulary: %vocabulary', array('%vocabulary' => $vocabulary->name)),
      '#options' => $options,
      '#default_value' => $default_value,
    );
  }
  return $form_element;
}

/**
 * Add/edit validation callback. Set form error and return whether selection
 * is empty or not.
 *
 * @param array $form
 * @param array $form_state
 * @return bool
 */
function mass_contact_taxonomy_admin_edit_validate($form, &$form_state) {
  if (!empty($form_state['values']['recipients']['mass_contact_taxonomy'])) {
    foreach ($form_state['values']['recipients']['mass_contact_taxonomy'] as $term_values) {
      $term_values_filtered = array_filter($term_values);
      if (!empty($term_values_filtered)) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/**
 * Add/edit form submit callback. Should return piece of data that will be
 * saved to mass_contact table in recepients field.
 *
 * @param type $form
 * @param type $form_state
 */
function mass_contact_taxonomy_admin_edit_submit($form, &$form_state) {
  $terms = array();
  if (!empty($form_state['values']['recipients']['mass_contact_taxonomy'])) {
    foreach ($form_state['values']['recipients']['mass_contact_taxonomy'] as $term_values) {
      $terms += array_filter($term_values);
    }
  }
  return $terms;
}
