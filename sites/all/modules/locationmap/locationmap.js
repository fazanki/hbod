(function($){
  Drupal.behaviors.locationmap = {
    attach: function(context, settings) {
      if (GBrowserIsCompatible()) {
        var locationmap = new GMap2(document.getElementById("locationmap_map"));
        target_point = new GLatLng(Drupal.settings.locationmap.lat, Drupal.settings.locationmap.lng);
        locationmap.setCenter(target_point, parseInt(Drupal.settings.locationmap.zoom));
        var geocoder = new GClientGeocoder();
        var admin = Drupal.settings.locationmap.admin;
        address = Drupal.settings.locationmap.address;
        description = Drupal.settings.locationmap.info;
        locationmap.addControl(new GMapTypeControl());
        locationmap.addControl(new GSmallMapControl());
        Drupal.settings.locationmap.marker = new GMarker(target_point, {draggable: (admin == true)});
        locationmap.addOverlay(Drupal.settings.locationmap.marker);
        locationmap.setMapType(eval(Drupal.settings.locationmap.type));
        GEvent.addListener(Drupal.settings.locationmap.marker, "click", function() {
        Drupal.settings.locationmap.marker.openInfoWindowHtml(description);
        });
        
        // Allow fine tuning of the marker position in admin mode.
        if (admin) {
          GEvent.addListener(Drupal.settings.locationmap.marker, "dragstart", function() {
            Drupal.settings.locationmap.marker.closeInfoWindow();
          });
          GEvent.addListener(Drupal.settings.locationmap.marker, "dragend", function() {
            Drupal.settings.locationmap.marker.openInfoWindowHtml(description);
            latlng = Drupal.settings.locationmap.marker.getLatLng();
            $('#edit-locationmap-lat').val(latlng.lat());
            $('#edit-locationmap-lng').val(latlng.lng());
          });
          GEvent.addListener(locationmap, "zoomend", function() {
            $('#edit-locationmap-zoom').val(locationmap.getZoom());
          });
        };
      }
    }
  };
})(jQuery);