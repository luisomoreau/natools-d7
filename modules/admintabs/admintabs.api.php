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
    '#title' => t('Dashboard'),

    // if you want different role to land on different tabs according to their role,
    // define a 'default tab' and associate a role id to a tab path.
    // In this example, user with role 4 will be redirected to "tips" page when landing
    // on "admin/dashboard" page.
    // Do not define this key if you want all users to land on first tab by default.
    '#default tab' => array(
      3 => 'overview',
      4 => 'tips',
    ),

    // now define all the tabs we need inside this tabset.
    '#tabs' => array(

      // here is is minimum required to make appear a tab.
      // remember that these are drupal menu_local_tasks, so you need
      // to define at least TWO tabs to make something appear.
      
      // tab overview
      'overview' => array('title' => 'Overview'),
      // tab brand
      'tips' => array('title' => 'Tips'),

      // define a sub taks to "brands". here we override type to use
      // MENU_LOCAL_ACTION type.
      // We use a custom "redirect" key, so that user is redirect to node/add/tip form
      // from this links. a "destination" parameter will be set in url to bring him back to dashboard.
      'tips/add-tip' => array(
        'title' => 'Add a tip',
        'redirect' => 'node/add/tip',
        'type' => MENU_LOCAL_ACTION,
      ),
    ),
  );

  return $tabsets;
}

