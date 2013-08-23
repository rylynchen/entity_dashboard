<?php

/**
 * This field handler aggregates operations that can be done on a model
 * under a single field providing a more flexible way to present them in a view
 */
class entity_dashboard_handler_entity_dashboard_operations_field extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['did'] = 'did';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (!user_access('administer or forward')) {
      return;
    }
    $did = $this->get_value($values, 'did');
    $view = array(
      'title' => l(t('View'), 'entity_dashboard/'.$did),
      'html' => TRUE,
    );
    $links = array_merge(array('view' => $view), menu_contextual_links('entity_dashboard', 'entity_dashboard', array($did)));
    $links['entity_dashboard-edit']['title'] = 'Edit';
    $links['entity_dashboard-delete']['title'] = 'Delete';
    return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
  }
}
