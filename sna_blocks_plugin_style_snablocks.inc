<?php

class sna_blocks_plugin_style_snablocks extends views_plugin_style {
  // Set default options
  function option_definition() {
    $options = parent::option_definition();
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
      '#description' => t('Achive view machine name.'),
    );
    $form['sna_view_display_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Vew page display ID'),
      '#default_value' => $this->options['sna_view_display_id'] ? $this->options['sna_view_display_id'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Archive View page display id. expample - page or page_1 etc.'),
    );
    $form['sna_view_page_url'] = array(
      '#type' => 'textfield',
      '#title' => t('View Page URL'),
      '#default_value' => $this->options['sna_view_page_url'] ? $this->options['sna_view_page_url'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Archive views page path.'),
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
    // Wrap close
    $form['sna_blocks_wrapper_close'] = array(
      '#markup' => '</div>',
    );
  }

  // Run any necessary actions on submit.
  function options_submit(&$form, &$form_state) {
    $sna_settings = variable_get('sna_settings', array());
    $key = $form_state['values']['style_options']['sna_view_name'] . '_' . $form_state['values']['style_options']['sna_view_display_id'];
    unset($sna_settings[$key]);
    $sna_settings[$key] = 1;
    variable_set('sna_settings', $sna_settings);
  }
}