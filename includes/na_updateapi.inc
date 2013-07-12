<?php
/**
 * @file
 *
 * Class of helpers to create quickly hook_update_N to deploy configuration
 * or content to different servers.
 *
 * Function must follow this naming convention to make this class more intuitive
 * {object_name}_{action}.
 */

class na_updateapi {

  // if TRUE, all code will except function that actually writes something in database.
  public $logs = '';

  public function log($success, $message) {
    $success = $success ? 'message' : 'erreur';
    $this->logs .= sprintf('%s : %s <br />', $success, $message);
  }

  // add a message that will be displayed when update_N finished.
  public function __construct() {
  }

  /**
   * Create new terms for in a vocabulary.
   *
   * @param $vocabulary_name (string)
   *   machine name of the vocabulary in which terms will be added.
   * @param $terms (array)
   *   array of terms ARRAYS. will be converted to object to use with taxonomy_term_save.
   * @param $update_existing (bool) 
   *   if TRUE, if a term with the same name already exists, it will be updated rather
   *   than insert a new term with same name.
   *
   * example of term object: do not add vid key, funcion will find it automatically
   * @code   
   * $terms = array(
   *   array('name' => 'Vines and grapes', 'parent' => array(8)),
   *   array('name' => 'Still wine vinification'),
   *   array('name' => 'Sparkling-wine vinification'),
   *   array('name' => 'Bottle ageing'),
   * );
   *  @endcode
   */
  public function taxonomy_term_create($vocabulary_name, $terms, $update_existing = TRUE) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
    $vid = $vocabulary->vid;
    foreach ($terms as $term) {
      $term = (object)$term;
      // set 0 as parent by default.
      $parent = isset($term->parent) ? $term->parent : array(0);
      $result = reset(taxonomy_get_term_by_name($term->name, $vocabulary_name));
      if ($result && $update_existing) {
        $this->log(TRUE, "$term->name already exists. Will be upated. ");
        $term->tid = $result->tid;
      }
      $term->vid = $vocabulary->vid;
      taxonomy_term_save($term);
      $this->log(TRUE, "try to save $term->name in $vocabulary_name");
    }
  }

  /**
   * Update a drupal block, registered in block table.
   *
   * @param $block array :
   * - module (required)
   * - delta (required)
   * - theme (required)
   * - visibility
   * - region
   * - status
   * - pages
   */
  public function block_update($block) {
    $required_keys = array('module', 'delta', 'theme');
    foreach ($required_keys as $required_key) {
     if (!in_array($required_key, array_keys($params))) {
       trigger_error("$required key param is mandatory", E_USER_ERROR);
     }
    }
    drupal_write_record('block', $block);
  }

  /**
   * enable a module. 
   * @param $module_list : an arra of module names.
   * @param $enable_dependencies: If TRUE, dependencies will automatically be added 
   * and enabled in the correct order. This incurs a significant performance cost, 
   * so use FALSE if you know $module_list is already complete and in the correct order.
   */
  public function module_enable($module_list, $enable_dependencies = TRUE) {
    $result = module_enable($module_list, $enable_dependencies);
    if ($result) {
      $module_list_string = implode(', ', $module_list);
      $this->log(TRUE, "enable $module_list_string" );
    }
    else {
      $this->log(FALSE, "missing dependencies for $module_list_string");
    }
  }

  /**
   * Delete ALL instances of a field. Use with caution.
   */
  public function field_delete_field($field_name) {
    field_delete_field($field_name);
    field_purge_batch();
  }

  /**
   * Save a new translation to database
   * @param string $string_source : original string
   * @param string $string_translated : translated string
   * @param string $language : target language
   * @param boolean $override : Set it to TRUE to override an existing translation.
   */
  public function translation_save($string_source, $string_translated, $language, $override = FALSE) {
    // search for source string.
    $lid = db_query('SELECT lid FROM {locales_source} WHERE source = :string_source', array(':string_source' => $string_source))->fetchField();
    if (!$lid) {
      return;
    }

    // search for an existing translation.
    $translation = db_query('SELECT translation from {locales_target} WHERE lid = :lid', array(':lid' => $lid))->fetchField();

    // if there is an existing translation and we asked for an override, update.
    if ($translation && $override) {
      $result = db_update('locales_target')->fields(array(
        'translation' => $string_translated,
      ))
      ->condition('lid', $lid)
      ->condition('language', $language)
      ->execute();
    }

    // if no translation is found, insert our new translation.
    if (!$translation) {
      db_insert('locales_target')->fields(array(
        'lid' => $lid,
        'translation' => $string_translated,
        'language' => $language,
      ))->execute();
    }

    _locale_invalidate_js();
    cache_clear_all('locale:', 'cache', TRUE);

  }

}


