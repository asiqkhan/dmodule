<?php

class sna_blocks_plugin_style_snablocks extends views_plugin_style {
  // Set default options
  function option_definition() {
    dsm("options");
    $options = parent::option_definition();

    // call every module using hook_views_slideshow_option_definition and merge
    // it's return value with the other default options.
    return array_merge($options, module_invoke_all('sna_blocks_option_definition'));
  }

  // Build the settings form for the view.
  function options_form(&$form, &$form_state) {

  }
}