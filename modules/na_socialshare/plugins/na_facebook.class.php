<?php
/**
 * @file
 * facebook buttons
 *
 * @see na_socialshare.class.php
 */

/**
 * Base class for all facebook buttons
 */
abstract class na_facebook extends na_socialshare {

  function help_url() {
    return 'http://developers.facebook.com/docs/plugins';
  }

  function script() {
    // div id=fb_root seems to required here ?
    return '<div id="fb-root"></div>
      <script>(function(d, s, id) {
        var js, fjs = d.getElementsByTagName(s)[0];
        if (d.getElementById(id)) return;
        js = d.createElement(s); js.id = id;
        js.src = "//connect.facebook.net/fr_FR/all.js#xfbml=1";
        fjs.parentNode.insertBefore(js, fjs);
        }(document, \'script\', \'facebook-jssdk\'));
      </script>';
  }

}

/**
 * facebook Like button implementation.
 */
class na_facebooklike extends na_facebook {

  function help_url() {
    return 'https://developers.facebook.com/docs/reference/plugins/like/';
  }

  function title() {
    return "Facebook Like";
  }

  function html() {
    return '<div class="fb-like" ' . $this->params_to_html_attributes($this->params) . '></div>';
  }

}

/**
 * Facebook send button implementation.
 */
class na_facebooksend extends na_facebook {

  function help_url() {
    return 'https://developers.facebook.com/docs/reference/plugins/send/';
  }

  function title() {
    return "Facebook Send";
  }

  function html() {
    return '<div class="fb-send" ' . $this->params_to_html_attributes($this->params) . '></div>';
  }

}

/**
 * Facebook comments button implementation.
 */
class na_facebookcomment extends na_facebook {

  function title() {
    return "Facebook Comment";
  }

  function __construct($params = array()) {
    // set default url to current page if data-href is empty.
    global $base_root;
    if (!isset($params['data-href'])) $params['data-href'] = $base_root . request_uri();
    parent::__construct($params);
  }

  function html() {
    return '<div class="fb-comments" ' . $this->params_to_html_attributes($this->params) . '></div>';
  }

}

