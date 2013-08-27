<?php

/**
 * Display dashboard admin page
 */
function entity_dashboard_manage() {
  $view = views_get_view('entity_dashborads', 'default');
  $view->override_url = $_GET['q'];
  return $view->preview();
}

/**
 * Returns a dashboard submission form.
 */
function entity_dashboard_add() {
  $entityDashboard = entity_get_controller('entity_dashboard')->create();
  $output = drupal_get_form('entity_dashboard_form', $entityDashboard);
  return $output;
}

/**
 * Dashboard view page.
 */
function entity_dashboard_page_view($entityDashboard, $view_mode = 'full') {
  if ($view_mode == 'full') {
    $build['entity_dashboard'][$entityDashboard->did]['title'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Title:</div><div class="field-items"><div class="field-item even">'.filter_xss($entityDashboard->title).'</div></div></div>',
      '#weight' => -20,
    );
    $build['entity_dashboard'][$entityDashboard->did]['path'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Path:</div><div class="field-items"><div class="field-item even">'.l(filter_xss($entityDashboard->path), $entityDashboard->path).'</div></div></div>',
      '#weight' => -20,
    );
    $build['entity_dashboard'][$entityDashboard->did]['theme'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Theme:<div class="field-items"><div class="field-item even">'.filter_xss($entityDashboard->theme).'</div></div></div>',
      '#weight' => -10,
    );
    $build['entity_dashboard'][$entityDashboard->did]['module'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Module:<div class="field-items"><div class="field-item even">'.filter_xss($entityDashboard->module).'</div></div></div>',
      '#weight' => -10,
    );
  }
  return $build;
}

/**
 * Dashboard edit page.
 */
function entity_dashboard_page_edit($dashboard) {
  return drupal_get_form('entity_dashboard_form', $dashboard);
}

/**
 * Confirm form for deleting a dashboard.
 */
function entity_dashboard_delete_confirm_form($form, &$form_state, $dashboard) {
  $form_state += array('dashboard' => $dashboard);
  $confirm_question = t('Are you sure you want to delete dashboard for Path : %path ?', array('%path' => $dashboard->path));
  return confirm_form($form, $confirm_question, 'entity_dashboard/'.$dashboard->did);
}

/**
 * Dashboard delete confirm form submit handler.
 */
function entity_dashboard_delete_confirm_form_submit($form, &$form_state) {
  $dashboard = $form_state['dashboard'];
  $dashboard->delete();
  drupal_set_message(t('Delete dashboard for Path : %path .', array('%path' => $dashboard->path)));
  $form_state['redirect'] = 'admin/structure/entity_dashboard';
}

/**
 * Page callback: Displays the dashboard.
 *
 * @param $launch_customize
 *   Whether to launch in customization mode right away. TRUE or FALSE.
 */
function entity_dashboard_page($path, $launch_customize = FALSE) {
  $entityDashboard = entity_dashboard_load_by_path($path);
  $js_settings = array(
    'dashboard' => array(
      'drawer' => url($entityDashboard->path.'/drawer/'.$entityDashboard->did),
      'blockContent' => url($entityDashboard->path.'/block-content'),
      'updatePath' => url($entityDashboard->path.'/update'),
      'formToken' => drupal_get_token('entity-dashboard-update'),
      'entityDashboardPath' => current_path(),
      'launchCustomize' => $launch_customize,
      'dashboard' => url($entityDashboard->path),
      'emptyBlockText' => t('(empty)'),
      'emptyRegionTextInactive' => t('This dashboard region is empty. Click <em>Customize dashboard</em> to add blocks to it.'),
      'emptyRegionTextActive' => t('DRAG HERE'),
      'accessCustomize' => user_access('config entity dashboard'),
    ),
  );
  $build = array(
    '#theme' => 'entity_dashboard_demo',
    '#message' => t('To customize the dashboard page, move blocks to the dashboard regions on the <a href="@dashboard">Dashboard administration page</a>, or enable JavaScript on this page to use the drag-and-drop interface.', array('@dashboard' => url($entityDashboard->path.'/configure'))),
    '#access' => 1,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'entity_dashboard') . '/entity_dashboard.js',
        array('data' => $js_settings, 'type' => 'setting'),
      ),
      'library' => array(array('system', 'ui.sortable')),
    ),
  );
  return $build;
}

/**
 * Page callback: Builds the page for administering dashboard blocks.
 *
 * This page reuses the Block module's administration form but limits editing
 * to blocks that are available to appear on the dashboard.
 *
 * @see block_admin_display()
 * @see block_admin_display_form()
 * @see dashboard_form_dashboard_admin_display_form_alter()
 * @see template_preprocess_dashboard_admin_display_form()
 */
function entity_dashboard_page_blocks($path) {
  global $theme_key;
  // $use_theme = variable_get('theme_default','none');
  $entityDashboard = entity_dashboard_load_by_path($path);
  drupal_theme_initialize();
  module_load_include('inc', 'block', 'block.admin');

  $blocks = array();
  $available_blocks = entity_dashboard_available_blocks();
  $entityDashboard_blocks = db_select('entity_dashboard_block', 'edb')
    ->fields('edb')->condition('edb.did',  $entityDashboard->did)
    ->execute()
    ->fetchAllAssoc('bid', PDO::FETCH_ASSOC);
  $entityDashboard_blocks = empty($entityDashboard_blocks) ? block_admin_display_prepare_blocks($theme_key) : $entityDashboard_blocks;
  $dashboard_regions = _entity_dashboard_region_descriptions();
  $blocks = array();
  $_blocks = block_admin_display_prepare_blocks($theme_key);
  foreach ($_blocks as $block) {
    if (in_array($block['module'] . '_' . $block['delta'], $available_blocks)) {
      $blocks[] = $block;
    }
  }
  return drupal_get_form('entity_dashboard_admin_display_form', $blocks, $theme_key, $dashboard_regions);
}

/**
 * Ajax callback: Shows disabled blocks in the dashboard customization mode.
 */
function entity_dashboard_show_disabled($path) {
  global $theme_key;
  // $theme = variable_get('theme_default','none');
  $entityDashboard = entity_dashboard_load_by_path($path);

  $available = entity_dashboard_available_blocks();
  $entity_dashboard_blocks = db_select('entity_dashboard_block')
    ->condition('theme', $theme_key)
    ->condition('status', 1)
    ->condition('region ', 'entity_dashboard_inactive', '<>')
    ->condition('did', $entityDashboard->did)
    ->fields('entity_dashboard_block')
    ->execute();
  foreach ($entity_dashboard_blocks as $item) {
    unset($available[$item->module . '_' . $item->delta]);
  }
  // Blocks are not necessarily initialized at this point.
  $blocks = _block_rehash();

  foreach ($blocks as $key => $block) {
    $id = $block['module'] . '_' . $block['delta'];
    if (!isset($available[$id]) || !$available[$id]) {
      unset($blocks[$key]);
    }
  }

  // Theme the output and end the page request.
  print theme('entity_dashboard_disabled_blocks', array('blocks' => $blocks));
  drupal_exit();
}

/**
 * Ajax callback: Displays the rendered contents of a specific block.
 *
 * @param $module
 *   The block's module name.
 * @param $delta
 *   The block's delta.
 */
function entity_dashboard_show_block_content($path, $module, $delta) {
  global $theme_key;
  // $theme = variable_get('theme_default', 'none');
  $entityDashboard = entity_dashboard_load_by_path($path);
  drupal_theme_initialize();

  $blocks = array();
  $block_object = db_query("SELECT * FROM {block} WHERE theme = :theme AND module = :module AND delta = :delta", array(
    ":theme" => $theme_key,
    ":module" => $module,
    ":delta" => $delta,
    ))
    ->fetchObject();
  $block_object->enabled = TRUE;
  $block_object->page_match = TRUE;
  $blocks[$module . "_" . $delta] = $block_object;
  $block_content = _block_render_blocks($blocks);
  $build = _block_get_renderable_array($block_content);
  $rendered_block = drupal_render($build);
  print $rendered_block;
  drupal_exit();
}

/**
 * Sets the new weight of each region according to the drag-and-drop order.
 */
function entity_dashboard_update($path) {
  global $theme_key;
  // $theme = variable_get('theme_default', 'none');
  $entityDashboard = entity_dashboard_load_by_path($path);
  drupal_theme_initialize();
  // Check the form token to make sure we have a valid request.
  if (!empty($_REQUEST['form_token']) && drupal_valid_token($_REQUEST['form_token'], 'entity-dashboard-update') && !empty($_REQUEST['entity_dashboard_path']) && $entityDashboard = entity_dashboard_load_by_path($_REQUEST['entity_dashboard_path'])) {
    parse_str($_REQUEST['regions'], $regions);
    $bids = array();
    foreach ($regions as $region_name => $blocks) {
      if ($region_name == 'disabled_blocks') {
        $region_name = 'entity_dashboard_inactive';
      }
      foreach ($blocks as $weight => $block_string) {
        preg_match('/block-([^-]+)-(.+)/', $block_string, $matches);
        $block = new stdClass();
        $block->module = str_replace('-', '_', $matches[1]);
        $block->delta = $matches[2];

        $block->region = $region_name;
        $block->weight = $weight;
        $block->status = 1;

        if ($block->region !== 'disabled_blocks') {
          $delta_obj = db_query("SELECT delta FROM {block} WHERE module = :module AND delta = :delta AND theme = :theme LIMIT 1", array(
            ':module' => $block->module, 
            ':delta'  => $block->delta,
            ':theme' => $theme_key,
          ))->fetchObject();
          $block->delta = $delta_obj->delta;
          
          db_merge('entity_dashboard_block')
            ->key(array(
              'module'  => $block->module,
              'delta'   => $block->delta,
              'theme'   => $theme_key,
              'did'     => $entityDashboard->did,
            ))
            ->fields(array(
              'status'  => $block->status,
              'weight'  => $block->weight,
              'region'  => $block->region,
              'pages'   => '',
            ))
            ->execute();
        }
        else {
          db_delete('entity_dashboard_block')
            ->condition('did', $entityDashboard->did)
            ->condition('module', $block->module)
            ->condition('delta', $block->delta)
            ->execute();
        }
      }
    }
    drupal_set_message(t('The configuration options have been saved.'), 'status', FALSE);
  }
  drupal_exit();
}