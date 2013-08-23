<?php

/**
 * Implements hook_views_data()
 */
function entity_dashboard_views_data_alter(&$data) {
  $data['entity_dashboard']['link_entity_dashboard'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the entity_dashboard.'),
      'handler' => 'entity_dashboard_handler_link_field',
    ),
  );
  $data['entity_dashboard']['edit_entity_dashboard'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the entity_dashboard.'),
      'handler' => 'entity_dashboard_handler_edit_link_field',
    ),
  );
  $data['entity_dashboard']['delete_entity_dashboard'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the entity_dashboard.'),
      'handler' => 'entity_dashboard_handler_delete_link_field',
    ),
  );
  $data['entity_dashboard']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this entity_dashboard.'),
      'handler' => 'entity_dashboard_handler_entity_dashboard_operations_field',
    ),
  );
}