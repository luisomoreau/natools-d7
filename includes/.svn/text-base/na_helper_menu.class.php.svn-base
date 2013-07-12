<?php
/**
 * @file
 *
 * helper functions to manipulate drupal menu.
 */
class na_helper_menu {

  /**
   * Get direct children menu items for a parent menu item.   *
   * This function is NOT recursive. Consider using drupal native
   * menu_build_tree() if you need more features.
   *
   * @param string $path
   *   internal drupal path
   *
   * @param string $menu_name
   *   in which menu name we have to look for. This is required as
   *   the same link may be in several different menus with different
   *   hierarchies.
   *
   * @return
   *   array of nids of nodes that are part of children items menus.
   *
   * @TODO this function is not recursive for the moment, on only first level of children.
   * Use menu_node_views module if you need that.
   */
  static function children_get($path, $menu_name) {

    $query = db_select('menu_links', 'ml_parent');
    $query->join('menu_links', 'ml_children', 'ml_parent.mlid = ml_children.plid');
    $query->fields('ml_children');
    $query->condition('ml_parent.link_path', $path);
    $query->condition('ml_parent.menu_name', $menu_name);
    $query->condition('ml_children.menu_name', $menu_name);
    $query->orderBy('ml_children.weight', 'ASC');
    $query->orderBy('ml_children.link_title', 'ASC');
    $result = $query->execute();
   
    $items = array();
    foreach ($result as $row) {
      $items[] = $row;
    }
    return $items;
  }

  /**
   * Return only a part of a tree of a menu.
   *
   * @param $tree (array)
   *   Full tree af a menu, as returned by menu_tree_data()
   * @param $menu_link (array)
   *    Fully loaded menu link item. all items unders this one will be returned.
   *
   * @return array
   *   Part of tree under $menu_link item.
   *
   * @see menu_tree_data()
   */
  static function subtree_get($tree, $menu_link) {
    $parents = array();
    for ($i = 1; $i <= $menu_link['depth']; $i++) {
      $parents[] = $menu_link["p$i"];
      $parents[] = 'below';
    }
    $sub_tree = drupal_array_get_nested_value($tree, $parents);
    return $sub_tree;
  }


  /**
   * @param $nid (int)
   *   find "parent" node of this nid.
   *
   * @param $menu_name
   *   In which menu we have to looking for our parent.
   *
   * @return (mixed)
   *   nid (int) of the parent node or FALSE if no nid is found.
   */
  static function node_parent_get($nid, $menu_name) {
    // we are not using menu_node table as this is not mandatory to get the job done.
    $path = "node/$nid";
    $query = "SELECT ml2.link_path FROM  {menu_links} ml1 
      JOIN {menu_links} ml2 ON ml1.plid = ml2.mlid 
      WHERE ml1.link_path = :link_path";
    $result = db_query($query, array(':link_path' => $path))->fetchField();
    if ($result) {
      $nid = str_replace('node/', '', $result);
      return (int) $nid;
    }
    else {
      return FALSE;
    }
  }

  /**
   * @param $nid
   *   Nid of a node that is part of a drupal menu.
   * @return
   *   array of nids of nodes that are part of children items menus.
   *
   * @TODO this function is not recursive for the moment, on only first level of childs.
   * Use menu_node_views module if you need that.
   */
  static function node_children_get($nid) {

    if (!is_numeric($nid)) {
      trigger_error("nid argument should be numeric.");
      return;
    }

    // build link_path of our node, as it is stored in menu_links table.
    $node_path = "node/$nid";

    // find in menu_links table mlid (menu link id) associated to this node in the menu.
    // @FIXME if there is several menus, we will have bugs...
    $query = db_select('menu_links', 'ml');
    $query->addField('ml','mlid');
    $query->condition('ml.link_path', $node_path);
    $mlid = $query->execute()->fetchField();

    if (!$mlid) {
      return array();
    }

    // now, find all children items menu for our mlid; but only pathes that correspond to nodes.
    // we can't easily do that because we know that router_path is "node/%" for viewing node.
    $query = db_select('menu_links', 'ml');
    $query->join('menu_router', 'mr', 'ml.router_path = mr.path');
    $query->addField('ml','link_path');
    $query->condition('mr.path', 'node/%');
    $query->condition('ml.plid', $mlid);
    $query->orderBy('ml.weight');
    $rows = $query->execute();

    // rows contains all our children link_path pointing to nodes.
    // Extract nids from link_pathes.
    $nids = array();
    foreach ($rows as $row) {
      $nid = str_replace('node/', '', $row->link_path);
      if (is_numeric($nid)) {
        $nids[] = $nid;
      }
    }
    return $nids;

  }


}

