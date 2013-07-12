<?php
/**
 * API to display drupal menus in a easy way. Useful when you want to create
 * complex templates for menus, to keep full control over html.
 * Usage example :
 * @code
 * $menu = new menuapi('menu-ew-site-navigation-en');
 * // choose which level you want to print, with which depth.
 * // All links will be shown for level 1, but on next levels, only children
 * // of current active menu item will be displayed.
 * $menu->min_depth = 1; 
 * $menu->max_depth = 2;
 * $menu->class_active = 'active-link';
 * // return an array of links, without any html.
 * $links = $menu->get_links(); 
 * @endcode
 *
 * Links are array of the form :
 * array(  
 *   'plid'     => (int) parent menu mlid,
 *   'title'    => (string) link title,
 *   'href'     => (string) link href,
 *   'path'     => (string) link path,
 *   'hidden'   => (bool) indicates if this link is hidden or not,
 *   'weight'   => (int) weight of link in menu
 *   'language' => (string) link language,
 *   'children' => (array) may contains link if any children are found,
 *   'active'   => (bool) TRUE if this link is in active trail,
 *   'classes' => (string) contains useful classes for theming. e.g : "first active". It's up to you to print that i your custom template.
 * );
 *
 * @FIXME make sure this code is i18n compatible.
 */
class na_menuapi {

  // machine name of the menu to build.
  protected $menu_name;

  // show hidden links ?
  public $show_hidden = FALSE;  

  // options to build menus. See menu_build_tree() for documentation about that.
  public $min_depth = 1;
  public $max_depth = 1;
  public $only_active_trail = FALSE;  

  // Default classes for "active", "first" and "last" link of a menu.
  // you may change that accordin to our needs.
  public $class_active = 'active';
  public $class_first  = 'first';
  public $class_last   = 'last';

  /**
   * @param $menu_name (string)
   *   Machine name of the menu
   */
  function __construct($menu_name) {
    $this->menu_name = $menu_name;
  }

  /**
   * @return 
   *   current active mlids (item menu ids).
   */
  protected function mlids_trail() {
    // use menu_tree_get_path, so that menu_tree_set_path can be used with our module.
    if ($path = menu_tree_get_path($this->menu_name)) {
      $link = db_query("SELECT * FROM {menu_links} WHERE link_path=:path", array('path' => $path))->fetchObject();
      $mlids = array();
      for ($i =1; $i < 10; $i++) {
        $p = "p$i";
        if ($link->$p != 0) {
          $mlids[] = $link->$p;
        }
      }
      return $mlids;
    }
    else {
      $mlids = array();  
      foreach (menu_get_active_trail() as $link) {
        if($link['href'] != '<front>') {     
          $mlids[] = $link['mlid'];
        }  
      }   
      return $mlids;
    }
  }

  /**
   * Once we removed unwanted menu items (ex : when displaying
   * only level 2 of a menu, we display only links on current active trail),
   * we can add classes that will make theming easier.
   */
  protected function add_classes(&$menu_tree_section) {
    $total = count($menu_tree_section);
    if (!$total) return; 
    $i = 1; 
    foreach ($menu_tree_section as &$link) {
      $classes = array();
      if ($i == 1) {
        $link->first = TRUE;
        $classes[] = $this->class_first;
      }
      if ($i == $total) {
        $link->last = TRUE;
        $classes[] = $this->class_last;
      }
      if ($link->active) {
        $classes[] = $this->class_active;
      }
      $link->classes = implode(' ', $classes);
      $i++;
    }
  }

  /**
   * Build a simplifie menu tree.
   * @return array
   *   array of links.
   */
  public function get_links($menu_tree = array()) {  	

    //dpm(menu_get_active_trail());

    if (!$menu_tree) {	   
      $params = array(
        'min_depth' => $this->min_depth,
        'max_depth' => $this->max_depth,
        'active_trail' => $this->mlids_trail(),		 
        // TRUE seems to be only usefull for building breadcrumb
        'only_active_trail' => FALSE, 
      );   	   
      $menu_tree = menu_build_tree($this->menu_name, $params);	   
      $menu_tree_section = array();	   
    }
    $i = 1;
    foreach ($menu_tree as $datas) {

      $link = (object)$datas['link'];

      // skip if we asked for a specific language, and item link does not belong to this language.
      // leave "und" items links, as they have to be displayed , whatever the language is.
      // if ($this->language != 'und' && $link->language != 'und' && $link->language != $this->language) continue;

      // do no display hidden links, unless this is asked.
      if ($this->show_hidden == FALSE && $link->hidden == TRUE) continue;
      //
      // only keep items that are child of an active parent menu item.
      if ($this->min_depth > 1 && $link->plid != 0 && !in_array($link->plid, $this->mlids_trail())) continue;

      $first = ($i == 1) ? TRUE : FALSE;
      $last  = ($i == count($datas)) ? TRUE : FALSE;
      $active = $link->in_active_trail;

      $item = array(  
        'plid'     => $link->plid,
        'title'    => $link->link_title,
        'href'     => $link->href,
        'path'     => $link->link_path,
        'hidden'   => $link->hidden,
        'weight'   => $link->weight,
        'language' => $link->language,
        'children' => array(),
        'active'   => $active,
      );

      // objects are much easier to use when building templates.
      $menu_tree_section[$link->mlid] = (object)$item;

      // if children are found, recurse.
      if ($datas['below']) {	    
        $menu_tree_section[$link->mlid]->children = $this->get_links($datas['below'], $link->mlid);		
      }
      $i++;
    }

    // add some properties and a "classes" properties to make things simpler for theming.
    $this->add_classes($menu_tree_section);

    return $menu_tree_section;
  }

}


