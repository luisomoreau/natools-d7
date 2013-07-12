<?php
/**
 * @file
 * Twitter buttons
 *
 * @see na_socialshare.class.php
 */

/* =======================
     TWITTER BUTTONS
   ======================= */

/**
 * Base class for all twitter buttons.
 */
abstract class na_twitter extends na_socialshare {

  static function help_url() {
     return "https://dev.twitter.com/docs/tweet-button";
  }

  // all twitter buttons embed the same js.
  function script() {
    return '<script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>';
  }

}

/**
 * Twitter share implementation.
 */
class na_twittershare extends na_twitter {

  static function params_custom() {
    return array();
  }

  function html() {
    return '<a href="https://twitter.com/share" class="twitter-share-button"' . $this->params_to_html_attributes($this->params) . '>Tweet</a>';
  }

}

/**
 * Twitter follow implementation.
 */
class na_twitterfollow extends na_twitter {

  static function params_custom() {
    return array(
      'account' => 'Required  : Machine name of twitter Account to follow. This will be used to construct url to this account.',
    );
  }

  function html() {
    return sprintf('<a href="https://twitter.com/%s" class="twitter-follow-button"' . $this->params_to_html_attributes($this->params) . ' >Follow @%s</a>', $this->params['account'], $this->params['account']);
  }

}

/**
 * Twitter hashtag implementation.
 */
class na_twitterhashtag extends na_twitter {

  static function params_custom() {
    return array(
      'hashtag' => 'Required  : hashtag to show.',
    );
  }

  function html() {
    return sprintf('<a href="https://twitter.com/intent?button_hashtag=%s" class="twitter-hashtag-button"' . $this->params_to_html_attributes($this->params) . ' >Tweet #%hashtag</a>', $this->params['hashtag'], $this->params['hashtag']);
  }

}

