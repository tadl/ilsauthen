<?php
// $Id$

/**
 * Define admin settings form
 */
function ilsauthen_admin_settings() {

  $form['ilsauthen_enable_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable external account logging'),
    '#description' => t('Enabling this option logs usernames (and corresponding passwords) that are managed by or routed to external authentication. Use for testing purposes only.'),
    '#default_value' => variable_get('ilsauthen_enable_logging', 0),
    '#weight' => -20,
  );

  $enable_logging = variable_get('ilsauthen_enable_logging', 0);
  if ($enable_logging) {
    $form['logging_warning'] = array(
      '#prefix' => '<div class="messages error">',
      '#value' => t('Warning: Usernames and passwords for external accounts are being logged. Turn off <em>Enable external account logging</em>, above, unless you are testing a driver.'),
      '#suffix' => '</div>',
      '#weight' => -20,
    );
   }

  $form['ilsauthen_show_no_mail_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t("Warn user if they haven't set an email address"),
    '#description' => t("Drupal requires users to register an email addresse in their profile. If a driver is unable to get the user's email address from the external authentication source, enabling this option will display a warning until the user adds their email address to their profile."),
    '#default_value' => variable_get('ilsauthen_show_no_mail_warning', 1),
    '#weight' => -20,
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default roles for external accounts'),
    '#weight' => -15,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $roles = user_roles(TRUE);
  // Remove 'authenticated user' (rid 2), which is always the second element in the roles array. We are 
  // only left with roles created by the site administrator. We do this since Drupal will always add a 
  // new user to the authenticated user role.
  unset($roles[2]); 
  $form['roles']['ilsauthen_default_roles'] = array(
    '#title' => t('Default roles'),
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#default_value' => variable_get('ilsauthen_default_roles', array()),
    '#description' => t('The roles that accounts created using external authentication are placed in. Does not apply to local Drupal accounts.')
  );

  $drivers = ilsauthen_get_drivers();
  $form['ilsauthen_driver'] = array(
    '#type' => 'select',
    '#title' => t('Driver'),
    '#default_value' => variable_get('ilsauthen_driver', 'sample'),
    '#options' => $drivers,
    '#description' => t('Driver-specific settings will appear below after you have saved this form. Settings for drivers that are not selected are retained.'),
    '#weight' => -12,
    '#ahah' => array(
       'path' => 'ilsauthen/savesetting/js',
       'wrapper' => 'warn-save-settings-wrapper',
       'event' => 'change',
     ),
  );

  $form['warning'] = array(
    '#type' => 'markup', 
    '#value' => '<div id="warn-save-settings-wrapper"></div>',
    '#weight' => -10 
  );

  return system_settings_form($form);
}

/**
 * Get list of all drivers.
 */
function ilsauthen_get_drivers() {
  $drivers = array();
  $module_dir = drupal_get_path('module', 'ilsauthen');
  $driver_files = file_scan_directory($module_dir .'/drivers', '\.inc$');
  foreach ($driver_files as $driver) {
    $drivers[$driver->name] = $driver->name;
  }
  return $drivers;
}
