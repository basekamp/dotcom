<?php
// $Id: views_fluid_grid.views.inc,v 1.1.2.1 2009/09/27 13:03:45 markuspetrux Exp $

/**
 * @file
 * Integration with Views.
 */

/**
 * Implementation of hook_views_plugins().
 */
function views_fluid_grid_views_plugins() {
  return array(
    'module' => 'views_fluid_grid', // This just tells our themes are elsewhere.
    'style' => array(
      'fluid_grid' => array(
        'title' => t('Fluid grid'),
        'help' => t('Displays items in a fluid grid.'),
        'handler' => 'views_fluid_grid_plugin_style',
        'path' => drupal_get_path('module', 'views_fluid_grid') . '/views',
        'theme path' => drupal_get_path('module', 'views_fluid_grid') . '/theme',
        'theme file' => 'theme.inc',
        'theme' => 'views_fluid_grid_plugin_style',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        // @todo: Add advanced_help support.
        //'help topic' => 'style-fluid-grid',
      ),
    ),
  );
}
