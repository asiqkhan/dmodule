<?php
/**
 * @file
 * Defines the View Style Plugins for Views Slideshow module.
 */


/**
 * Implements hook_views_plugins().
 */
function sna_blocks_views_plugins() {
  dsm(drupal_get_path('module', 'sna_blocks'));
  return array(
    'style' => array(
      'snablocks' => array(
        'title' => t('SNA Blocks'),
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