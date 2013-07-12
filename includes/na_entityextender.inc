<?php
/**
 * @file
 *
 * Add useful methods to an entity object for theming, in a node.tpl.php for example,
 * allowing to print field in right language / display mode in a easy way.
 * Some kind of light "entityMetadataWrappe" from entity API. 
 * This class should be use only for theming purpose, use entity API for more advanced things.
 *
 * @code
 *
 * // make node an entityExtender class object. 
 * $node = entityExtender::create('node', $node);
 *
 * // this will comport as usual. So you MUST satinize value  before printing it.
 * print $node->title();
 * 
 * // display a sanitized node field, in right language, in full display mode and wihout ANY html
 * print $node->field_render('field_my_field', 'full')
 *
 * // return RAW (NOT SANITIZED) value for a field. The second param is a subkey of field array.
 * print $node->field_value('field_my_field', 'value');
 * 
 * // This returns the fully loaded collections from a field collection field.
 * $field_collections = $node->field_load_collections('field_collections');
 *
 * // return all fully loaded nodes referenced by field "field_ref_parent".
 * $node->field_load_entityreference('field_ref_parent', TRUE)
 * 
 * // return a fully load term form a term_reference field.
 * $node->field_load_termreference('field_my_term');
 *
 * // render an image with a imagestyle preset.
 * $node->field_render_image('field_my_image', 'my_image_style_preset')
 *
 * @endcode
 */

class na_entityextender {

  // full entity object : ex : full node object.
  private $entity = NULL;
  // type of entity : "node", "taxonomy_term" etc...
  private $entity_type = NULL;
  // let us now which type of entity we extends, for debugging purposes.
  public $entityextender_entity_type = 'node';

  /**
   * this is the "constructor" : return NULL if $entity does not exists.
   */
  static function create($entity_type, $entity) {
    // entity seems not to be set, return NULL.
    if (!$entity) {
      return NULL;
    }
    // this entity is already an entityExtender object, return it as it is.
    if (get_class($entity) == __CLASS__ ) {
      return $entity;
    }
    // in all other cases, return a new entityExtender object.
    return new entityExtender($entity_type, $entity);
  }

  /**
   * Create an entitExtender object : copy properties from received stdClass object (e.g : a node).
   */
  private function __construct($entity_type, $entity) {
    $this->entityextender_entity_type = $this->entity_type = $entity_type;
    $this->entity = $entity;
    foreach ($entity as $property => $value) {
      $this->$property = $value;
    }
  }

  /**
   * Render a field with a specific display mode.
   * That means that you can change rendering of this method 
   * by using "manage fields" tab in content type administration.
   * Only render minimal html fo the field, without label (you have to print it manually if needed)
   *
   * @param $field_name (string)
   *    field name to render
   * @param $display (string or array)
   *    name of display mode, or array of settings as asked by field_view_value()
   *    This is REQUIRED, so that we know which display is changing rendering of our fields,
   *    in "manage display" fields admin.
   * @param $multiple (bool) 
   *   if FALSE, return fetched value as a string.
   *   if TRUE, return fetched values into an array.
   *
   * @return
   *   string if single value, array if multiple values.
   */
  public function field_render($field_name, $display, $multiple = FALSE) {
    $items = field_get_items($this->entity_type, $this->entity, $field_name);
    if (!$items) return FALSE;
    if (!$multiple) {    
      $output = field_view_value($this->entity_type, $this->entity, $field_name, reset($items), $display);	
      return render($output);
    }
    else {
      $values = array();
      foreach ($items as $item) {
        $output = field_view_value($this->entity_type, $this->entity, $field_name, $item, $display);
        $values[] = render($output);
      }
      return $values;
    }
  }

  /**
   * Return field raw values, as found in entity object.
   * @param $field_name (string)
   *   name of the fieldApi field.
   * @param $key (string)
   *   a key of field sub-array (maybe "value", "target_id", "tid", "iso2" according to the field type.
   *   return all the keys if no key is specified.
   * @param $mulitple (bool)
   *   if FALSE, return fetched value as a string.
   *   if TRUE, return fetched values into an array.
   */
  public function field_value($field_name, $key = NULL, $multiple = FALSE) {
    $items = field_get_items($this->entity_type, $this->entity, $field_name);
    if (!$items) return FALSE;
    $values = array();
    foreach ($items as $item) {
      $values[] = $key ? $item[$key] : $item;
    }
    return $multiple ? $values : reset($values);
  }

  /**
   * Fully load entities from a "field collection" field.
   * Returned objects are ew_entity_extender objects.
   */
  public function field_load_collections($field_name, $multiple = FALSE) {
    $items = field_get_items($this->entity_type, $this->entity, $field_name);
    if (!$items) return FALSE;
    $ids = array_map(function($val){return $val['value'];}, $items);
    $entities = entity_load('field_collection_item', $ids);
    return $this->entities_return_wrapper('field_collection_item', $entities, $multiple);
  }

  /**
   * Helper to load an entityreference from a field api field.
   *
   * Please not that function return an object, so you can write something like this :
   * print strip_tags$mynode->field_load_entityreference('field_ref_parent')->title;
   * It will load node referenced by field_ref_parent and display its title.
   */
  public function field_load_entityreference($field_name, $multiple = FALSE, $type = 'node') {
    $target_ids = $this->field_value($field_name, 'target_id', $multiple);
    if ($target_ids) {
     // entity_load need an array, ensure we always have an array here.
      if (!$multiple) $target_ids = (array)$target_ids;
      $entities = entity_load($type, $target_ids);
      return $this->entities_return_wrapper($type, $entities, $multiple);
    }
    else {
      return FALSE;
    }
  }

  public function field_load_termreference($field_name, $multiple = FALSE) {
    $target_ids = $this->field_value($field_name, 'tid', $multiple);
    // entity_load need an array, ensure we always have an array here.
    if (!$multiple) $target_ids = (array)$target_ids;
    if ($target_ids) {
      $entities = taxonomy_term_load_multiple($target_ids);
      return $this->entities_return_wrapper('taxonomy_term', $entities, $multiple);
    }
    else {
      return FALSE;
    }
  }

  public function field_load_file($field_name, $multiple = FALSE) {
    $fids = $this->field_value($field_name, 'fid', $multiple);
    // entity_load need an array, ensure we always have an array here.
    if (!$multiple) $fids = (array)$fids;
    if ($fids) {
      $files = file_load_multiple($fids);
      // add real url to file object.
      foreach ($files as $file) {
        $file->url = strip_tags(file_create_url($file->uri));
      }
      return $multiple ? $files : reset($files);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Helper function to return loaded entities as entityExtender object.
   * @param $type (string)
   *   type of entity
   * @param $entities (object)
   *   fully loaded entities
   * @param $multiple (bool)
   */
  private function entities_return_wrapper($type, $entities, $multiple) {
    if ($multiple) {
      return array_map(function($val)use($type){return entityExtender::create($type, $val);}, $entities);
    }
    else {
      return entityExtender::create($type, reset($entities));
    }
  }

  /**
   * Helper to simply display an image with a specific preset imagestyle. Use directly field_render() method if
   * you need more control with display options.
   * @param $field_name
   *   name of the field to render
   * @param $image_style (string)
   *   machine name of image style prese to use to render image.
   */
  public function field_render_image($field_name, $image_style, $multiple = FALSE) {
    $output = $this->field_render($field_name, array('settings'=> array('image_style' => $image_style)), $multiple);
    return $output;
  }

}


