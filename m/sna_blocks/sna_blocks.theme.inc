<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

use Drupal\Component\Utility\Html;
use Drupal\Component\Utility\SafeMarkup;
use Drupal\Component\Utility\String;
use Drupal\Component\Utility\Xss;
use Drupal\Core\Template\Attribute;
use Drupal\Core\Url;

/**
 * Prepares variables for view templates.
 *
 * Default template: sna-blocks-view-simplenodearchive.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - view: A ViewExecutable object.
 *   - rows: The raw row data.
 *   - options: An array of options. Each option contains:
 *     - separator: A string to be placed between inline fields to keep them
 *       visually distinct.
 */
function template_preprocess_sna_blocks_view_simplenodearchive(&$variables) {
  $view = $variables['view'];
  $argument = $view->argument[$view->build_info['summary_level']];

  $url_options = array();

  if (!empty($view->exposed_raw_input)) {
    $url_options['query'] = $view->exposed_raw_input;
  }

  $count = 0;
  $active_urls = array(
    // Force system path.
    \Drupal::url('<current>', [], ['alias' => TRUE]),
    // Could be an alias.
    \Drupal::url('<current>'),
  );
  $active_urls = array_combine($active_urls, $active_urls);

  // Collect all arguments for each row, to be able to alter them for example
  // by the validator. This is not done per single argument value, because
  // this could cause performance problems.
  $row_args = array();
  foreach ($variables['rows'] as $id => $row) {
    $row_args[$id] = $argument->summaryArgument($row);
  }
  $argument->processSummaryArguments($row_args);

  foreach ($variables['rows'] as $id => $row) {
    // Only false on first time.
    if ($count++) {
      $variables['rows'][$id]->separator = Xss::filterAdmin($variables['options']['separator']);
    }
    $variables['rows'][$id]->attributes = array();
    $variables['rows'][$id]->link = $argument->summaryName($row);
    $args = $view->args;
    $args[$argument->position] = $row_args[$id];

    $base_path = NULL;
    if (!empty($argument->options['summary_options']['base_path'])) {
      $base_path = $argument->options['summary_options']['base_path'];
    }
    $variables['rows'][$id]->url = _url($view->getUrl($args, $base_path), $url_options);
    $variables['rows'][$id]->count = intval($row->{$argument->count_alias});
    if (isset($active_urls[$variables['rows'][$id]->url])) {
      $variables['rows'][$id]->attributes['class'][] = 'active';
    }
    $variables['rows'][$id]->attributes = new Attribute($variables['rows'][$id]->attributes);
  }
  $variables['archive_block'] = 'No Result found';
}
