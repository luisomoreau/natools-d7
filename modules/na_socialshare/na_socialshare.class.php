<?php
/**
 * @file
 *
 * Base class to implements social buttons. Extends this class to create
 * widgets.
 * How to use example :
 * @code
 *   $params = array('data-send' => FALSE, 'data-layout' => 'button_count');
 *   $facebook_like = new na_facebooklike($params);
 *   print $facebook_like->render();
 * @endcode
 */

/**
 * Base class for all social widgets.
 */
abstract class na_socialshare {

  /**
   * Associative array of options to display af facebook button, twitter etc...
   */
  protected $params = array();

  // this variable help us to print each needed js only ONE time per page.
  static protected $js_already_included = array();

  /**
   * @param array $params
   * an associative array of options for the button. E.g :
   * array('data-send' => FALSE, 'data-layout' => 'button_count', etc...);
   */
  function __construct($params = array()) {
    foreach ($params as $key => $value) {
       $this->params[$key] = htmlentities($value, ENT_QUOTES);
    }
  }

  /**
   * @return string
   * Url to documentation for existing parameters. E.g : http://developers.facebook.com/docs/plugins/
   */
  abstract function help_url();


  /**
   * @return string
   * Title of this widget (to display on admin for example)
   */
  abstract function title();

  /**
   * @return array of custom params.
   *
   * You can pass custom params to the constructor. Add in this function their
   * documentation, so that user now available params when configuring a block.
   * Make sur your custom param has not the same key as an existing param !
   *
   * For example, for "twitter follow" you may specify which account to follow
   * in the link, and this is not passed as a classic param from twitter API. 
   * You have to return an array of the form :
   *
   * @code
   * return array('variable_name' => 'Description of what this variable does in your class.');
   * @endcode
   */
  function params_custom() {return array();}

  /**
   * set params array to a string that we may insert in html attributes, like
   * data-show="false" data-cout="true" 
   * This is how facebook, googleplus and twitter handle options for their share links
   * in html5 version.
   * @TODO we could use drupal_attributes. Because it would be our only dependance
   * to Drupal API in our classes for now, keep our own version.
   */
  protected function params_to_html_attributes($params) {
    $output = array();
    foreach ($this->params as $key => $value) {
      $output[] = sprintf('%s="%s"', $key, $value);
    }
    return implode(' ', $output);
  }

  /**
   * social buttons will may need to embed javascript (and a bit of html for facebook).
   * In some cases, we won't want to load script until the page is fully loaded, so we separate
   * js from html.
   * Today, facebook, google plus or twitter are loaded in a "async" way so we can just print
   * js at the same time as html with them, in their html5 version.
   */
  abstract function script(); 

  /**
   * Display html of the social share service.
   *
   * only return part of the code that will display social widget. Js should not be added here.
   */
  abstract function html();

  // By default, render html and js at the same time. That's okay if js is injecting
  // a script tag by js at the end of page loading. If not, print html et "script" separately at the right places.
  function render() {
    $output = '';

    // Make sure each embedded script is called only once per page.
    $cache_key = md5($this->script());
    if (!isset(self::$js_already_included[$cache_key])) {
      $output .= $this->script() . "\r\n";
      self::$js_already_included[$cache_key] = 1;
    }

    $output .= $this->html();
    return $output;
  }

}

