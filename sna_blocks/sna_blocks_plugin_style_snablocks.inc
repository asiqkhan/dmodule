<?php
/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render simple node archive.
 *
 * @ingroup views_style_plugins
 */
class sna_blocks_plugin_style_snablocks extends views_plugin_style {
  // Set default options.
  function option_definition() {
    $options = parent::option_definition();
    $options['sna_view_name'] = array('default' => '');
    $options['sna_view_display_id'] = array('default' => '');
    $options['sna_view_page_url'] = array('default' => '');
    $options['field_name'] = array('default' => 'node_created');
    $options['sna_use_jquery'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  // Build the settings form for the view.
  function options_form(&$form, &$form_state) {
    // Wrap all the form elements to help style the form.
    $form['sna_blocks_wrapper'] = array(
      '#markup' => '<div id="sna-blocks-form-wrapper">',
    );
    $form['sna_view_name'] = array(
      '#type' => 'textfield',
      '#title' => t('View machine name'),
      '#default_value' => $this->options['sna_view_name'] ? $this->options['sna_view_name'] : $this->view->name,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('The machine name of the view whose page is used to display archive result.'),
    );
    $form['sna_view_display_id'] = array(
      '#type' => 'textfield',
      '#title' => t('View page display id.'),
      '#default_value' => $this->options['sna_view_display_id'] ? $this->options['sna_view_display_id'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('The view page display id, e.g. page_1'),
    );
    $form['sna_view_page_url'] = array(
      '#type' => 'textfield',
      '#title' => t('View page display URL.'),
      '#default_value' => $this->options['sna_view_page_url'] ? $this->options['sna_view_page_url'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Archive views page path.'),
    );

    // Set Default value for field_name.
    $disabled_field_name = TRUE;
    $default_field_name = 'node_created';
    if (module_exists('date')) {
      $disabled_field_name = FALSE;
      $default_field_name = $this->options['field_name'] ? $this->options['field_name'] : 'node_created';
    }

    $form['field_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Field Name'),
      '#default_value' => $default_field_name,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#disabled' => $disabled_field_name,
      '#description' => t('Achive will created based on field. Require <a href="http://drupal.org/project/date" target="_blank">Date module </a>'),
    );

    $disabled_jquerymenu_option = TRUE;
    if (module_exists('jquerymenu')) {
      $disabled_jquerymenu_option = FALSE;
    }
    $form['sna_use_jquery'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['sna_use_jquery'] ? $this->options['sna_use_jquery'] : 0 ,
      '#title' => t('check this box if you want to use <a href="http://drupal.org/project/jquerymenu" target="_blank">Jquerymenu module </a> to theme archive blocks.'),
      '#disabled' => $disabled_jquerymenu_option,
    );

    // Wrapper close.
    $form['sna_blocks_wrapper_close'] = array(
      '#markup' => '</div>',
    );
  }

  // Run any validation on the form settings.
  function options_validate(&$form, &$form_state) {
    $view_name = $form_state['values']['style_options']['sna_view_name'];
    $display_id = $form_state['values']['style_options']['sna_view_display_id'];
    $vid = $form_state['view']->vid;
    $view_display_id = $form_state['display_id'];
    try {
      $view = views_get_view($view_name);
      if (isset($view->display[$display_id]) && $view->display[$display_id]->display_plugin == 'page') {
        $style_options = sna_blocks_get_view_style_options($view_name, $display_id);
        if ( !empty($style_options['vid']) && !($vid == $style_options['vid']&& $view_display_id == $style_options['id'])) {
          form_set_error('sna_view_display_id', t('Page is already in use!'));
        }
      }
      else {
        form_set_error('sna_view_display_id', t('Invalid view name or display_id.'));
      }
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage());
    }
  }
}
