/* GS JavaScript Framework - Sitewide
 *
 * @author Andy Hawkes, Mehmet Duran, Franco Zanki 
 * @GS = Goldman Sachs	
 */

var HRBOT = HRBOT || {};

HRBOT.Sitewide = (function ($){
    HRBOT.Sitewide = function(){
	    
        this.createGoogleMap = function() {
            var myLatlng = new google.maps.LatLng(45.819229,15.975151);
            var myOptions = {
                zoom: 15,
                center: myLatlng,
                mapTypeId: google.maps.MapTypeId.ROADMAP
                }
            var map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
            var marker = new google.maps.Marker({ position: myLatlng, map: map, title:"Hello World!"});
            google.maps.event.addListener(marker, 'click', function() {map.setZoom(8);});
            $('#map_canvas').css({"width":"700px","height":"300px"});
		}
    }

    return new HRBOT.Sitewide();
})(jQuery);

HRBOT.Page.register('*', HRBOT.Sitewide.createGoogleMap);



//GS.Page.register({'className': 'blog'}, GS.Sitewide.blogBg);
