<?php

/**
 * Entity dashboard admin settings.
 */
function entity_dashboard_settings() {
  drupal_theme_initialize();
  module_load_include('inc', 'block', 'block.admin');
  
  $blocks = array();
  foreach (block_admin_display_prepare_blocks(variable_get('theme_default','none')) as $block) {
    $blocks[$block['module'] . '_' . $block['delta']] = $block['info'];
  } 
  $form = array();
  
  $form['available_blocks'] = array(
    '#type'           => 'checkboxes',
    '#title'          => t('Available blocks'),
    '#options'        => $blocks,
    '#default_value'  => entity_dashboard_available_blocks(),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  $form['#submit'] = array('entity_dashboard_settings_submit');
  return $form;
}

/**
 * Handler entity dashboard admin settings form submit.
 */
function entity_dashboard_settings_submit($form, &$form_state) {
  variable_set('entity_dashboard_available_blocks', serialize(array_filter($form_state['values']['available_blocks'])));
  drupal_set_message(t('Your configure have been saved.'));
}