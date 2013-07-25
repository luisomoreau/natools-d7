(function ($) {

  Drupal.behaviors.opishare = {

    attach: function (context, settings) {

      $(".social-share-links a").click(function(e){
        e.preventDefault();
        var h = $(this).attr("data-popup-height"),
        w = $(this).data("popup-width");
        window.open(
          $(this).attr("href"),
          "share",
          "top="+((screen.height-h)/2)+",left="+((screen.width-w)/2)+",width="+w+",height="+h
        );
      });

    }
  };

})(jQuery);

