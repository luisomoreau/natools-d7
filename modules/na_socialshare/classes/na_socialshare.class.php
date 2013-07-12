<?php
/**
 * @file
 *
 * Base class to implements social buttons.
 *
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
   * @params
   * an associative array of options for the button. E.g :
   * array('data-send' => FALSE, 'data-layout' => 'button_count', etc...);
   */
  function __construct($params = array()) {
    if (!is_array($params) && !$params) return;
    foreach ($params as $key => $value) {
       // encode quotes.
       $this->params[$key] = htmlentities($value, ENT_QUOTES);
    }
  }

  /**
   * Must return an url to the page listing all existing paramaters, for ex :
   * http://developers.facebook.com/docs/plugins/
   * This is a valuable information for building an administration page for those buttons.
   */
  abstract static function help_url();

  /**
   * You button may contains custom params, not documented by their API or for you own needs.
   * Pass them the same way you pass normal params to the constructor, this is just a way to
   * "document" them, so that user know which params he is able to pass. Be sure that you custom
   * param has not the same key as an existing params.
   *
   * For example, for "twitter follow" you may specify which account to follow
   * in the link, and this is not passed as a classic param from their API. 
   * You must return
   * an array of the form :
   *
   * @code
   * return array(
   *   'variable_name' => 'Description of what this variable does in your class.',
   * );
   * @endcode
   */
  abstract static function params_custom();  

  /**
   * set params array to a string that we may insert in html attributes, like
   * data-show="false" data-cout="true" 
   * This is how facebook, googleplus and twitter handle options for their share links
   * in html5 version.
   * @TODO we could use drupal_attributes. Because it would be the only dependance
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
   * only return part of the code that will display social widget. Js must not be added here.
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

