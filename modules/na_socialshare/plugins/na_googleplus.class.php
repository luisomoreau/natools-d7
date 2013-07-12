<?php
/**
 * @file
 *
 * Google plus buttons
 * @see na_socialshare.class.php
 */


/**
 * Googleplus Share implementation.
 */
class na_googleplus extends na_socialshare {

  static function help_url() {
    return 'https://developers.google.com/+/plugins/+1button/#plusonetag-parameters';
  }

  static function params_custom() {
    return array();
  }

  function script() {
    return '<script type="text/javascript">
    window.___gcfg = {lang: \'fr\'};
    (function() {
          var po = document.createElement(\'script\'); po.type = \'text/javascript\'; po.async = true;
              po.src = \'https://apis.google.com/js/plusone.js\';
              var s = document.getElementsByTagName(\'script\')[0]; s.parentNode.insertBefore(po, s);
                })();
    </script>';
  }

  function html() {
    return '<div class="g-plusone"' . $this->params_to_html_attributes($this->params) . '></div>';
  }

}

