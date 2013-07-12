<?php
/**
 * @file
 *
 * Helpers function around entities and field API.
 */


class na_helper_entity {

  /**
   * Helper function to let us know is a boolean field from field_api is "0" or "1".
   * Does *not* work with multiple values field.
   *
   * @param string $entity_type
   *   'node', 'user', etc..
   * @param object $entity
   *   fully loaded entity object
   * @param $field
   *   field name to inspect.
   *
   * @return (bool)
   *   TRUE if this node is flagged as "chapter" with field "field_page_chapter".
   */
  static function field_is_checked($entity_type, $entity, $field) {
    $is_checked = FALSE;
    if ($items = field_get_items($entity_type, $entity, $field)) {
      if ($items[0]['value'] == 1) {
        $is_checked = TRUE;
      }
    }
    return $is_checked;
  }

}

