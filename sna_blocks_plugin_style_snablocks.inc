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
      '#title' => t('View page display id.'),
      '#default_value' => $this->options['sna_view_display_id'] ? $this->options['sna_view_display_id'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('Archive View page display id. expample - page or page_1 etc.'),
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
    $sna_settings = variable_get('sna_settings', array());
    $current_setting = $form_state['view']->display_handler->display->display_options['style_options'];
    // Check for page display.
    if (strtolower(substr($form_state['values']['style_options']['sna_view_display_id'],0,4)) != 'page') {
      form_set_error('sna_view_display_id', t('Invalid page display.'));
    }
    else {
      // Validate viewname and display_id.
      $query = db_select('views_view', 'V');
      $query->Fields('V', array('vid', 'name'));
      $query->addJoin('INNER', 'views_display', 'VD', 'V.vid = VD.vid');
      $query->addField('VD', 'display_title');
      $query->condition('V.name', $form_state['values']['style_options']['sna_view_name']);
      $query->condition('VD.id', $form_state['values']['style_options']['sna_view_display_id']);
      $result = $query->execute()->fetchAll();
      if (!$result) {
        form_set_error('sna_view_display_id', t('Invalid page display.'));
      }
    }
    // Check is the page aleardy in use.
    $entered_key = $form_state['values']['style_options']['sna_view_name'] . '_' . $form_state['values']['style_options']['sna_view_display_id'];
    if (!empty($current_setting)) {
      // Existing settins.
      $saved_key = $current_setting['sna_view_name'] . '_' . $current_setting['sna_view_display_id'];
      if ($entered_key != $saved_key) {
        if ($sna_settings[$entered_key]) {
          form_set_error('sna_view_display_id', t('Page is already in use!'));
        }
      }
    }
    else {
      // New settings.
      if ($sna_settings[$entered_key]) {
        form_set_error('sna_view_display_id', t('Page is already in use!'));
      }
    }
  }

  // Save settings on submit.
  function options_submit(&$form, &$form_state) {
    $sna_settings = variable_get('sna_settings', array());
    $key = $form_state['values']['style_options']['sna_view_name'] . '_' . $form_state['values']['style_options']['sna_view_display_id'];
    unset($sna_settings[$key]);
    $sna_settings[$key] = $form_state['values']['style_options']['field_name'];
    variable_set('sna_settings', $sna_settings);
  }
}
