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
      '#title' => t('Archive view name'),
      '#default_value' => $this->options['sna_view_name'] ? $this->options['sna_view_name'] : $this->view->name,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
    );
    $form['sna_view_display_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Archive view display ID'),
      '#default_value' => $this->options['sna_view_display_id'] ? $this->options['sna_view_display_id'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
    );
    $form['sna_view_page_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Archive view Page URL'),
      '#default_value' => $this->options['sna_view_page_url'] ? $this->options['sna_view_page_url'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
    );
    $form['sna_user_jquery'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['sna_user_jquery'] ? $this->options['sna_user_jquery'] : 0 ,
      '#title' => t('Use Jquery to theme archive blocks.'),
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