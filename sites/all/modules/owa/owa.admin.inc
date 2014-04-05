<?php

/**
 * @file
 * Administrative page callbacks for the owa module.
 */

/**
 * Implementation of hook_admin_settings() for configuring the module
 */
function owa_admin_settings_form($form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['account']['owa_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => variable_get('owa_account', ''),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#description' => t('This ID is unique to each site you want to track separately.'),
  );

  $form['account']['owa_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL of Open Web Analytics'),
    '#default_value' => variable_get('owa_base_url', ''),
    '#required' => TRUE,
    '#description' => t('This is the url of your Open Web Analytics installation.'),
  );

  // Standard tracking configurations.
  $form['user_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('User specific tracking settings'),
    '#collapsible' => TRUE,
  );
  $form['user_vis_settings']['owa_custom'] = array(
    '#type' => 'radios',
    '#title' => t('Custom tracking settings'),
    '#options' => array(
      t('Users cannot control whether they are tracked or not.'),
      t('Track users by default, but let individual users to opt out.'),
      t('Do not track users by default, but let individual users to opt in.')
    ),
    '#description' => t('Allow individual users to customize the visibility of tracking in their account settings. Only users with %permission permission are allowed to set their own preference.', array('%permission' => t('opt-in or out of tracking'))),
    '#default_value' => variable_get('owa_custom', 0),
  );

  // Render the role overview.
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific tracking settings'),
    '#collapsible' => TRUE,
  );

  $form['role_vis_settings']['owa_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('owa_visibility_roles', 0),
  );

  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings']['owa_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('owa_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $access = user_access('use PHP for tracking visibility in owa');
  $visibility = variable_get('owa_visibility', 0);
  $pages = variable_get('owa_pages', '');

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['owa_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['owa_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
      '#wysiwyg' => FALSE,
      '#rows' => 10,
    );
  }

  // Link specific configurations.
  $form['linktracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['linktracking']['owa_track_page_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track page views'),
    '#default_value' => variable_get('owa_track_page_view', 1),
    '#description' => t('Enables tracking of page views.')
  );
  $form['linktracking']['owa_track_clicks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track clicks'),
    '#default_value' => variable_get('owa_track_clicks', 1),
    '#description' => t('Tracker can be configured to track any clicks that occur on any DOM element of your web pages.')
  );
  $form['linktracking']['owa_track_dom_stream'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track dom stream'),
    '#default_value' => variable_get('owa_track_dom_stream', 1),
    '#description' => t('Track the domstream of users.')
  );
  $form['linktracking']['owa_track_across_domain'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track across domains'),
    '#default_value' => variable_get('owa_track_across_domain', 0),
    '#description' => t('This will share the users cookie across other domains. (Maybe a security risk)')
  );

  $form['owa_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#default_value' => variable_get('owa_debug', 0),
    '#description' => t('This will send debug information to the javascript console.')
  );

  return system_settings_form($form);
}

function owa_admin_settings_form_validate($form, &$form_state) {
  // Trim some text values.
  $form_state['values']['owa_account'] = trim($form_state['values']['owa_account']);
  $form_state['values']['owa_pages'] = trim($form_state['values']['owa_pages']);
  $form_state['values']['owa_base_url'] = trim($form_state['values']['owa_base_url']);
}