/**
 * @file
 * JavaScript for the Hover Preview module.
 */
(function ($) {

/**
 * Drupal Hover Preview behavior.
 */
Drupal.behaviors.hoverPreview = {
  attach: function (context, settings) {

    // Register the Image Preview action.
    $('img.hover-preview-imgpreview').once('hover-preview-imgpreview', function() {
      $(this).imgPreview({
        srcAttr: 'data-hover-preview',
        containerID: 'hover-preview-imgpreview'
      });
    });

    // Register the Image Replacement action.
    $('img.hover-preview-replace').once('hover-preview-replace', function() {
      // Preload the hover preview image.
      var hover = $(this).attr('data-hover-preview');
      $('<img/>').attr('src', hover);
      $(this).data('hoverPreview', hover);

      // Invoke the hover action on the image.
      $(this).hover(function() {
        // Swap the images whenever the user hovers their mouse.
        var backup = $(this).data('hoverPreview');
        $(this).data('hoverPreview', $(this).attr('src'));
        $(this).attr('src', backup);
      });
    });

  }
};

})(jQuery);
