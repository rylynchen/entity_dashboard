<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class entityDashboardMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['title'] = array(
      'label' => t('Title'),
      'description' => t('The dashboard page title.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer entity dashboard',
      'schema field' => 'title',
    );
    $properties['path'] = array(
      'label' => t('Path'),
      'description' => t('The dashboard path.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer entity dashboard',
      'schema field' => 'path',
    );
    $properties['theme'] = array(
      'label' => t('Theme'),
      'description' => t('The dashboard theme.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer entity dashboard',
      'schema field' => 'theme',
    );
    $properties['themekey_id'] = array(
      'label' => t('ThemeKey ID'),
      'description' => t('The dashboard theme.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer entity dashboard',
      'schema field' => 'themekey_id',
    );
    return $info;
  }
}