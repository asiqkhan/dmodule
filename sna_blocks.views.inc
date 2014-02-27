<?php
/**
 * @file
 * Defines the View Style Plugins for Views Slideshow module.
 */

/**
 * Implements hook_views_plugins().
 */
function sna_blocks_views_plugins() {
  return array(
    'style' => array(
      'snablocks' => array(
        'title' => t('Simple Node Archive'),
        'help' => t('Display the results as a slideshow.'),
        'handler' => 'sna_blocks_plugin_style_snablocks',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'path' => drupal_get_path('module', 'sna_blocks'),
        'theme' => 'sna_blocks',
        'theme path' => drupal_get_path('module', 'sna_blocks') . '/theme',
        'theme file' => 'sna_blocks.theme.inc',
      ),
    ),
  );
}

/**
 * Implements hook_views_query_alter().
 */
function sna_blocks_views_query_alter(&$view, &$query) {
  $key = $view->name . '_' . $view->current_display;
  $sna_settings = variable_get('sna_settings', array());
  if (isset($sna_settings[$key]) && $sna_settings[$key]) {
    $archive_page = $view->display_handler->display->display_options['path'];;
    $display_year = $display_month = '';
    $arg = explode('/', $_GET['q']);
    $display_year = $arg[0] == $archive_page && isset($arg[1]) && $arg[1] != '' && is_numeric($arg[1]) ? $arg[1] : '';
    $display_month = $arg[0] == $archive_page && isset($arg[2]) && $arg[2] != '00' && is_numeric($arg[2]) ? $arg[2] : '';

    if (!empty($display_year)) {
      $query->where[1]['conditions'][] = array(
        'field' => "FROM_UNIXTIME(node.created, '%Y') = " . $display_year,
        'value' => array(),
        'operator' => 'formula',
      );
    }

    if (!empty($display_month)) {
      $query->where[1]['conditions'][] = array(
        'field' => "FROM_UNIXTIME(node.created, '%m') = " . $display_month,
        'value' => array(),
        'operator' => 'formula',
      );
    }
  }
}