<?php

/**
 * @file tabsapi.php
 *
 * Allow to create quickly local tasks with drupal hook_menu.
 * This is usefull to create an administration with tabs.
 * 
 * All information in this array are passed to the hook_menu, so every key
 * - redirect : tab will be a redirection to path specified as a value
 *
 * This hook generates menu items but default callback page returns nothing (empty string)
 * it's up to the developper to fill created page with blocks, or
 * to define a custom callback for his tab if he want to.
 */
function hook_admintabs_info() {
  return array(
    'admin/dashboard' => array('title' => 'Dashboard'),
    'admin/dashboard/news' => array('title' => 'News'),
    'admin/dashboard/articles' => array('title' => 'Articles'),
    'admin/dashboard/pages' => array('title' => 'Pages'),
  );
}


