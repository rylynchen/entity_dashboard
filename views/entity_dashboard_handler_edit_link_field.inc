<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class entity_dashboard_handler_edit_link_field extends entity_dashboard_handler_link_field {
  
  function construct() {
    parent::construct();
  }

  function render($values) {
    // Check access
    if (!user_access('administer entity dashboard')) {
      return;
    }
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    $id = $values->{$this->aliases['did']};
    return l(t('Edit'), 'entity_dashboard/' . $did . '/edit');
  }
}
