<?php
/**
 * @file tabsapi.php
 *
 * Allow to create quickly local tasks with drupal hook_menu.
 * This is usefull to create an administration with tabs.
 * 
 * All information in this array are passed to the hook_menu, so every key
 * from hook_menu is valid and will be used as usual.
 * Additionnaly, ou may used some custom keys :
 * - redirect : tab will be a redirection to path specified as a value
 *
 * This hook generates menu items but default callback page returns nothing (empty string)
 * it's up to the developper to fill created page with blocks, or
 * to define a custom callback for his tab.
 */
function hook_admintabs_info() {

  $tabsets  = array();

  // create a tabset for admin/dashboard url.
  $tabsets['admin/dashboard'] = array(

    // title for this menu item
    'title' => t('Dashboard'),

    // define default tabs. This may change according to user roles.
    // If you want all role to land on same tab by defaut, set this
    // as array('all' => 'yourtab').
    'default tab' => array(
      3 => 'overview',
      4 => 'tips',
    ),

    // now define all the tabs we need inside this tabset.
    'tabs' => array(

      // here is is minimum required to make appear a tab.
      // remember that these are drupal menu_local_tasks, so you need
      // to define at least TWO tabs to make something appear.
      
      // tab overview
      'overview' => array('title' => 'Overview'),
      // tab brand
      'brands' => array('title' => 'Brands'),

      // define a sub taks to "brands". here we override type to use
      // MENU_LOCAL_ACTION type.
      'brands/add-brand' => array(
        'title' => 'Add a brand',
        'redirect' => 'node/add/brand',
        'type' => MENU_LOCAL_ACTION,
      ),
    ),
  );

  return $tabsets;
}

