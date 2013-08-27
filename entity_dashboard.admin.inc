<?php

/**
 * Entity dashboard admin settings.
 */
function entity_dashboard_settings_block() {
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
    '#required'       => TRUE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  $form['#submit'] = array('entity_dashboard_settings_block_submit');
  return $form;
}

/**
 * Handler entity dashboard admin settings form submit.
 */
function entity_dashboard_settings_block_submit($form, &$form_state) {
  variable_set('entity_dashboard_available_blocks', serialize(array_filter($form_state['values']['available_blocks'])));
  drupal_set_message(t('Your configure have been saved.'));
}

/**
 * Entity dashboard admin settings.
 */
function entity_dashboard_settings_theme() {
  $themes = array();
  $available_themes = entity_dashboard_available_themes();
  foreach (system_list('theme') as $key => $theme) {
    if ($theme->status) {
      $themes[$key] = $theme;
    }
  }
  $form = array();
  $form['themes'] = array(
    '#tree' => TRUE,
  );
  foreach ($themes as $key => $theme) {
    $available = (boolean)isset($available_themes[$key]);
    $form['themes'][$key] = array(
      '#title' => $theme->name,
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => !$available,
      '#tree' => TRUE,
    );
    $form['themes'][$key]['available'] = array(
      '#title' => T('Available'),
      '#type' => 'checkbox',
      '#default_value' => $available,
    );
    $form['themes'][$key]['regions'] = array(
      '#title' => T('Allow control regions'),
      '#type' => 'textarea',
      '#default_value' => $available ? implode("\n", $available_themes[$key]) : '',
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  $form['#validate'] = array('entity_dashboard_settings_theme_validate');
  $form['#submit'] = array('entity_dashboard_settings_theme_submit');
  return $form;
}

/**
 * Handler entity dashboard admin settings form validate.
 */
function entity_dashboard_settings_theme_validate($form, &$form_state) {
  foreach ($form_state['values']['themes'] as $key => $theme) {
    $theme_regions = array_keys(system_region_list($key));
    if ($theme['available']) {
      $_regions = explode("\n", $theme['regions']);
      $regions = array();
      $invalid = FALSE;
      foreach ($_regions as $_region) {
        $region = trim($_region);
        if (in_array($region, $theme_regions)) {
          $regions[] = $region;
        }
        else {
          $invalid = TRUE;
          break;
        }
      }
      if ($invalid) {
        form_error($form['themes'][$key]['regions'], t('Invalid regions.'));
      }
      else {
        form_set_value($form['themes'][$key]['regions'], $regions, $form_state);
      }

    }
  }
}

/**
 * Handler entity dashboard admin settings form submit.
 */
function entity_dashboard_settings_theme_submit($form, &$form_state) {
  $available_themes = array();
  foreach ($form_state['values']['themes'] as $key => $theme) {
    if ($theme['available']) {
      $available_themes[$key] = $theme['regions'];
    }
  }
  variable_set('entity_dashboard_available_themes', serialize($available_themes));
  drupal_static_reset('_entity_dashboard_regions');
  drupal_set_message(t('Your configure have been saved.'));
}