<?php

/**
 * @file mass_contact plugin type groupping_method.
 *
 * Select users by their role.
 */

$plugin = array(
  // Function executed to retrieve list of users by category settings.
  // In case of roles we receive unserialized settings with selected roles.
  'create_recipient_list_callback' => 'mass_contact_role_create_recipient_list',

  // Function to prepare Category description for Recipients column
  // on categories list admin page.
  'mass_contact_admin_categories_callback' => 'mass_contact_role_admin_categories',

  // Next three callbacks used to maintain form of add/edit category.
  'mass_contact_admin_edit' => 'mass_contact_role_admin_edit',
  'mass_contact_admin_edit_validate' => 'mass_contact_role_admin_edit_validate',
  'mass_contact_admin_edit_submit' => 'mass_contact_role_admin_edit_submit',
);

/**
 * Callback to retrieve users by roles.
 *
 * Get the user IDs for all the users in all the roles included in the
 * category.
 */
function mass_contact_role_create_recipient_list($recipients) {
  $groups = array();
  if (isset($recipients['mass_contact_role'])) {
    $groups = $recipients['mass_contact_role'];
  }

  if (!empty($groups[0]) && $groups[0] == 2) {
    $query = "SELECT u.uid FROM {users} u LEFT JOIN {users_roles} ur ON u.uid = ur.uid";
    $result = db_query($query)->fetchAllAssoc('uid', PDO::FETCH_ASSOC);
  }
  else {
    $query = "SELECT u.uid FROM {users} u LEFT JOIN {users_roles} ur ON u.uid = ur.uid WHERE ur.rid IN (:ur_rids)";
    $result = db_query($query, array(':ur_rids' => $groups))->fetchAllAssoc('uid', PDO::FETCH_ASSOC);
  }
  $uids = array();
  foreach ($result as $record) {
    $uids[] = $record['uid'];
  }
  return $uids;
}

/**
 * Callback to prepare role names for admin/config/system/mass_contact page
 * column Recipients.
 */
function mass_contact_role_admin_categories($recipients) {
  $groups = array();
  if (isset($recipients['mass_contact_role']) && !empty($recipients['mass_contact_role'])) {
    $groups = $recipients['mass_contact_role'];
  }
  else {
    return;
  }

  $result = db_query('SELECT name FROM {role} WHERE rid IN (:rids)', array(':rids' => $groups));

  $role_names = array();
  foreach ($result as $record) {
    $role_names[] = $record->name;
  }

  return t('Roles: %roles', array('%roles' => implode(', ', $role_names)));
}

/**
 * Form element for Category add/edit page.
 *
 * @param array $roles
 * @return type
 */
function mass_contact_role_admin_edit($recipients) {
  $roles = array();
  if (isset($recipients['mass_contact_role']) && !empty($recipients['mass_contact_role'])) {
    $roles = $recipients['mass_contact_role'];
  }

  // Get a list of all roles, except for the anonymous user role.
  $allroles = db_select('role', 'r')
    ->fields('r', array('rid', 'name'))
    ->condition('rid', 1, '>')
    ->orderBy('name', 'ASC')
    ->execute();
  foreach ($allroles as $roleobj) {
    $onerid = $roleobj->rid;
    $onename = $roleobj->name;
    $rolesarray[$onerid] = $onename;
  }
  $form_element = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles to receive email'),
    '#options' => $rolesarray,
    '#default_value' => $roles,
    '#description' => t('These roles will be added to the mailing list. Note: if you check "authenticated users", other roles will not be added, as they will receive the email anyway.'),
  );

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
function mass_contact_role_admin_edit_validate($form, &$form_state) {
  $selected_roles = array_filter($form_state['values']['recipients']['mass_contact_role']);
  return empty($selected_roles);
}

/**
 * Add/edit form submit callback. Should return piece of data that will be
 * saved to mass_contact table in recepients field.
 *
 * @param type $form
 * @param type $form_state
 */
function mass_contact_role_admin_edit_submit($form, &$form_state) {
  $roles = $form_state['values']['recipients']['mass_contact_role'];

  // If all authenticated users are already added,
  // simply return authenticated users rid.
  if ($roles[2] == 2) {
    return array(2);
  }
  // Remove roles that were not selected.
  $roles = array_filter($roles);

  return $roles;
}
