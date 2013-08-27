<?php

/**
 * Implements hook_views_default_views().
 */
function entity_dashboard_views_default_views() {
	$view = new view();
	$view->name = 'entity_dashborads';
	$view->description = 'Entity dashboards manage list';
	$view->tag = 'default';
	$view->base_table = 'entity_dashboard';
	$view->human_name = 'entity_dashborads';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'none';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'did' => 'did',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
	  'did' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	);
	/* Field: Entity Dashboard: Entity dashboard ID */
	$handler->display->display_options['fields']['did']['id'] = 'did';
	$handler->display->display_options['fields']['did']['table'] = 'entity_dashboard';
	$handler->display->display_options['fields']['did']['field'] = 'did';
	/* Field: Entity Dashboard: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'entity_dashboard';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	/* Field: Entity Dashboard: Path */
	$handler->display->display_options['fields']['path']['id'] = 'path';
	$handler->display->display_options['fields']['path']['table'] = 'entity_dashboard';
	$handler->display->display_options['fields']['path']['field'] = 'path';
	$handler->display->display_options['fields']['path']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['path']['alter']['path'] = '[path]';
	/* Field: Entity Dashboard: Theme */
	$handler->display->display_options['fields']['theme']['id'] = 'theme';
	$handler->display->display_options['fields']['theme']['table'] = 'entity_dashboard';
	$handler->display->display_options['fields']['theme']['field'] = 'theme';
	/* Field: Entity Dashboard: ThemeKey ID */
	$handler->display->display_options['fields']['themekey_id']['id'] = 'themekey_id';
	$handler->display->display_options['fields']['themekey_id']['table'] = 'entity_dashboard';
	$handler->display->display_options['fields']['themekey_id']['field'] = 'themekey_id';
	/* Field: Entity Dashboard: Operations links */
	$handler->display->display_options['fields']['operations']['id'] = 'operations';
	$handler->display->display_options['fields']['operations']['table'] = 'entity_dashboard';
	$handler->display->display_options['fields']['operations']['field'] = 'operations';
	$handler->display->display_options['fields']['operations']['label'] = 'Operations';

  $views[$view->name] = $view;
  return $views;
}