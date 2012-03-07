Hover Preview Image Style
=========================

Requires Drupal 7.x.

This module provides a mouse hover state for images. This is similar to
functionality you would find on many shopping websites.

The implementation of this module goes a bit further than just a javascript
hover. The idea is you create multiple imagecache presets, and this will allow
you to hover from any preset to any other preset. 

Here's a step-by-step of how to use it.

1) Create at least two image styles, one for your thumbnail/static/small
image, and one for your larger hover image. You can create as many presets as
you like, but for this example we'll use two.

Preset #1 = Thumbnail = Scale and Crop, size of 80x80
Preset #2 = BigImage = Scale and Crop, size of 200x200


2) After saving your presets, you now have new choices available for the field
output. In the Field Display settings, choose Hover Preview as the display.


3) Now you must configure how the Hover Preview is presented:
- The image style
- What to link the image to
- The Hover Preview action (image popup preview or image replacement)
- The Hover Preview style when showing the previewed image

