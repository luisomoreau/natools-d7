<?php

/**
 * Helper class to generate dynamic css, where we can
 * use php.
 */
class na_dynamic_css {

  private $file = null;
  private $variables = null;

  /**
   * @param string $file : full Drupal path to css file.
   * @param array $variables
   *   associative array for variables that will be available in our dynamic css file.
   */
  public function __construct($file, $variables) {
    $this->file = $file;
    $this->variables = $variables;
  }

   /**
    * Add our dynamic css to the current page.
    */
   public function inject() {
     extract($this->variables, EXTR_SKIP);
     ob_start();
     include($this->file);
     $dcss = ob_get_contents();
     ob_end_clean();
     drupal_add_css($dcss, array('group' => CSS_THEME, 'weight' => 1000, 'type' => 'inline', 'preprocess' => FALSE));
  }

}

