<?php
/**
 * @file
 *
 * Helpers function for taxonomy and vocabulary.
 */

class na_helper_taxonomy {

  /**
   * Return nodes associated to tids. This is mainly a copy paste of native drupal
   * taxonomy_node_select, except that function can take several tids as argument,
   * and take care of dupplicate if any (when a node is associated to several terms, it would appear
   * several times when passing an array of tids to "taxonomy_select_nodes").
   *
   * We also care of pager, because count query have also to take in account that a node may be associated
   * to several terms.
   *
   * WARNING : offset options does not work with paginated queries. This seems to come
   * from native drupal pager.
   *
   * @param array $tids
   *   an array of term ids to match
   * @param array $options
   *   - pager : if TRUE, add a pager.
   *   - offset : (int) skip results at beginning (Does not work with drupal pager !)
   *   - limit : sql LIMIT.
   *   - order : order by. Take an array as arguments.
   *
   * @return array
   *   nids.
   */
  static function nodes_select($tids, $options = array()) {

    if (!variable_get('taxonomy_maintain_index_table', TRUE)) {
      return array();
    }

    // default values for options.
    // please not that offset cannot work with pager queries...
    $options_default = array(
      'pager' => TRUE,
      'offset' => 0,
      'limit' => 10,
      'order' =>  array('t.sticky' => 'DESC', 't.created' => 'DESC')
    );

    $options = array_merge($options_default, $options);

    $query = db_select('taxonomy_index', 't');
    $query->addTag('node_access');
    $query->condition('tid', $tids);

    // avoid dupplicate, as a node may be attached to several terms.
    // Adding this here ensure that our "count_query" will use a distinct,
    // and so return a right total items number for pager.
    if ($options['pager']) {
      $count_query = clone $query;
      $count_query->addExpression('COUNT(DISTINCT(t.nid))');
      $query = $query->extend('PagerDefault');
      $query->setCountQuery($count_query);
      $query->limit($options['limit']);
    }
    else {
      $query->range($options['offset'], $options['limit']);
    }

    $query->addField('t', 'nid');
    foreach ($options['order'] as $field => $direction) {
      $query->orderBy($field, $direction);
      // ORDER BY fields need to be loaded too, assume they are in the form table_alias.name
      list($table_alias, $name) = explode('.', $field);
      $query->addField($table_alias, $name);
    }
    $nids = $query->execute()->fetchCol();

    return $nids;
  }

  /**
   * Return a nested array (a "tree") of terms instead of a "flat" array list.
   *
   * @param $terms
   *   an array of terms as returned by taxonomy_get_tree()
   *
   * @return (array)
   *   nested array of terms, according to their hierarchy.
   *
   * @see taxonomy_get_nested_tree.
   */
  static function nested_tree_get($terms, $parent = 0, $max_depth = NULL, $load_entities = FALSE, $load_nids = FALSE, $parents_index = array(), $depth = 0) {
    foreach($terms as $term) {
      foreach($term->parents as $term_parent) {
        if ($term_parent == $parent) {
          $return[$term->tid] = $term;
        }
        else {
          $parents_index[$term_parent][$term->tid] = $term;
        }
      }
    }

    foreach($return as &$term) {
      $term->children = array();
      if (isset($parents_index[$term->tid]) && (is_null($max_depth) || $depth < $max_depth)) {
        $term->children = self::nested_tree_get($parents_index[$term->tid], $term->tid, $max_depth, $load_entities, $parents_index, $depth + 1);
      }
    }

    return $return;
  }

  /**
   * rebuild a node breadcrumb based on the hierarchy of the term in
   * taxonomy/term/* pages. (NOT in nodes pages)
   * 
   * @param int $tid
   *   current tid.
   * @param $title_add bool
   *  if FALSE, remove title from the end of the breadcrumb
   */
  static function breadcrumb_term_page_set($tid, $title_add = TRUE) {
    $current = (object) array(
      'tid' => $tid,
    );
    $breadcrumb = array();
    if ($title_add) {
      $breadcrumb[] = drupal_get_title();
    }
    while ($parents = taxonomy_get_parents($current->tid)) {
      $current = array_shift($parents);
      $breadcrumb[] = l($current->name, 'taxonomy/term/' . $current->tid);
    }
    $breadcrumb[] = l(t('Home'), NULL);
    $breadcrumb = array_reverse($breadcrumb);
    drupal_set_breadcrumb($breadcrumb);
  }

}

