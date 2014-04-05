// Get our owa settings.
var owa = Drupal.settings.owa;

// Set base url
var owa_baseUrl = owa.baseURL;

// Prep our commands variable.
var owa_cmds = owa_cmds || [];

// Add the siteID
owa_cmds.push(['setSiteId', owa.id]);

// Add the items we want to track.
if (owa.trackPageView) {
  owa_cmds.push(['trackPageView']);
}

if (owa.trackClicks) {
  owa_cmds.push(['trackClicks']);
}

if (owa.trackDomStream) {
  owa_cmds.push(['trackDomStream']);
}

// Debug to console
if (owa.debug) {
  owa_cmds.push(['setDebug', true]);
}

(function ($) {
  // Need to wait for our site to load before we can check for clicks and add our tracker code.
  $(document).ready(function() {
    // Add our javascript to the end.
    $('script:last').append('<script type="text/javascript" async=true src="' + owa_baseUrl + 'modules/base/js/owa.tracker-combined-min.js" />');

    // Track across domains
    if (owa.trackAcrossDomain) {
      $('a').each(function() {
        var isInternal = new RegExp("^(https?):\/\/" + window.location.host, "i");
        var isAbsolute = new RegExp("(https?):\/\/", "i");

        if (isAbsolute.test(this.href) && !isInternal.test(this.href)) {
          $(this).click(function() {
            OWATracker.shareStateByLink(this.href);
            return false;
          });
        }
      });
    }
  });
})(jQuery);
