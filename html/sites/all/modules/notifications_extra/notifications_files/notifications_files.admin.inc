<?php
// $Id: notifications_files.admin.inc,v 1.1.2.1 2009/06/28 05:06:38 danepowell Exp $

/**
 * @file
 * Administration tab for Notifications Attached Files
 */

/**
 *  Site-wide settings form.
 */
function notifications_files_settings_form() {
  // Enable / disable for subscription types
  $form['notifications_files_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Send attachments for content types'),
    '#options' => notifications_content_types('name'),
    '#default_value' => variable_get('notifications_files_types', array()),
    '#description' => t('Attachments to checked content types will be distributed with notifications'),
  );

  return system_settings_form($form);
}
