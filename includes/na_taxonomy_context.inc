<?php

/**
 * get term from current context. Extract tid form url or current node.
 */
class taxonomy_context {

  private $vid = null;

  public function __construct($vid) {
    $this->vid = $vid;
    return $this;
  }

  /**
   * return tid of current context, or FALSE if not if is found;
   */
  public function get_term() {

    // extract tid from taxonomy/term/% pages.
    if (arg(0) == 'taxonomy' && arg(1) == 'term' && is_numeric(arg(2))) {
      $term = taxonomy_term_load(arg(2));
      if ($term->vid == $this->vid) {
        return $term;
      }
    }

    // extract tid from node/% pages.
    if (arg(0) == 'node' && is_numeric(arg(1)) && !arg(2)) {
      $results = db_query('SELECT tid FROM {taxonomy_index} WHERE nid = :nid', array(':nid' => arg(1)));
      foreach ($results as $result) {
        return taxonomy_term_load($result->tid);
      }
    }

  }

}

