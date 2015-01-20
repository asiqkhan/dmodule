<?php

/**
 * @file
 * Definition of Drupal\sna_blocks\Plugin\views\style\SimpleNodeArchive.
 */

namespace Drupal\sna_blocks\Plugin\views\style;

use Drupal\views\Plugin\views\style\StylePluginBase;
use Drupal\Core\Annotation\Plugin;
use Drupal\Core\Annotation\Translation;
use Drupal\Core\Form\FormStateInterface;

/**
 * The default style plugin for summaries.
 *
 * @ingroup views_style_plugins
 *
 * @ViewsStyle(
 *   id = "sna_blocks",
 *   title = @Translation("Simple Node Archive"),
 *   help = @Translation("Displays result in archive formatted, with month and year that link to achive page."),
 *   display_types = {"normal"}
 * )
 */
class SimpleNodeArchive extends StylePluginBase {
  /**
   * Overrides Drupal\views\Plugin\Plugin::$usesOptions.
   */
  protected $usesOptions = TRUE;

  /**
   * Does the style plugin allows to use style plugins.
   *
   * @var bool
   */
  protected $usesRowPlugin = TRUE;

  /**
   * Does the style plugin support custom css class for the rows.
   *
   * @var bool
   */
  protected $usesRowClass = TRUE;
  
  /**
   * Does the style plugin support grouping of rows.
   *
   * @var bool
   */
  protected $usesGrouping = FALSE;

  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['field_name'] = array('default' => 'node_created');
    $options['sna_view_name'] = array('default' => '');
    $options['sna_view_display_id'] = array('default' => '');
    $options['sna_view_page_url'] = array('default' => '');
    $options['sna_use_jquery'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    $form['sna_blocks_wrapper'] = array(
      '#markup' => '<b>Note:</b> As the archive settings for each block is unique. It is recommended to override view "Style options".',
    );

    $default_field_name = $this->options['field_name'] ? $this->options['field_name'] : 'node_created';

    $form['field_name'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Archived date field machine name'),
      '#default_value' => $default_field_name,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => $this->t('Achived block will created based on this field. Default value is node posted date "node_created". You have to add the field to views "FIELDS" list.'),
    );

    $form['sna_view_name'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('View machine name'),
      '#default_value' => $this->options['sna_view_name'] ? $this->options['sna_view_name'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => $this->t('The machine name of the view whose page is used to display archive result.'),
      '#prefix' => '<br>A view page is required to display achive result. You need to create a page and then use below setting for page.<hr>',
    );
    $form['sna_view_display_id'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('View page display id.'),
      '#default_value' => $this->options['sna_view_display_id'] ? $this->options['sna_view_display_id'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => $this->t('The view page display id, e.g. page_1'),
    );
    $form['sna_view_page_url'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('View page display URL.'),
      '#default_value' => $this->options['sna_view_page_url'] ? $this->options['sna_view_page_url'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => $this->t('View page path.'),
    );
  }
}
