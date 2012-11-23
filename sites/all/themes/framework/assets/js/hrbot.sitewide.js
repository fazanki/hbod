/* GS JavaScript Framework - Sitewide
 *
 * @author Andy Hawkes, Mehmet Duran, Franco Zanki 
 * @GS = Goldman Sachs	
 */

var HRBOT = HRBOT || {};

HRBOT.Sitewide = (function ($){
    HRBOT.Sitewide = function(){
        
		
		
    }

 
})(jQuery);


/* JavaScript MVC Template
 *
 * @author Franco Zanki
 * @param HRBOT = Hrvatsko Botanicko Drustvo	
 */

var HRBOT = HRBOT || {};

HRBOT.Sitewide            = HRBOT.Sitewide || {};
HRBOT.Sitewide.Model      = HRBOT.Sitewide.Model || {};
HRBOT.Sitewide.View       = HRBOT.Sitewide.View || {};
HRBOT.Sitewide.Controller = HRBOT.Sitewide.Controller || {};


/**
 *  Model - defines the state of the application
 *        - purely for storing data, no logic should go here
 */
HRBOT.Sitewide.Model = (function($) {	

    var response;

	return {

		setResponse: function setResponse(res) {
			response = res;
		},

		getResponse: function getResponse() {
			return response;
		},

                SOME_CONSTANT: "SOME_CONSTANT"

	}

})(jQuery);


/**
 *  View - contains functions that update the DOM
 *
 */
HRBOT.Sitewide.View = (function($) {

	var View = {
		createGoogleMap: function createGoogleMap () {
		    
            var myLatlng = new google.maps.LatLng(45.807025,15.970881);
            var myOptions = {
                zoom: 16,
                center: myLatlng,
                mapTypeId: google.maps.MapTypeId.ROADMAP
                }
            var map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
            var marker = new google.maps.Marker({ position: myLatlng, map: map, title:"Hello World!"});
            google.maps.event.addListener(marker, 'click', function() {map.setZoom(8);});
            $('#map_canvas').css({"width":"720px","height":"300px"});
		},

		createTabs: function createTabs() {
            var links   = $('.field-name-field-tab-link .field-item');
            var tabs    = $('.field-name-field-body-tab .field-item');
           
            if (links.length == tabs.length) {
                // remove label fields 
                $('.field-name-field-tab-link, .field-name-field-body-tab').find('.field-label').remove();
                
                // loop through the tab fiels and create tab links
                var linksHtml = [];
                var navHtml = "";
                links.each(function(i){
                    var linkText = $(this).text();
                    var linkHtml = (i == 0) ? '<li><a class="tabControl selected" href="tab'+i+'">'+linkText+'</a></li>' : '<li><a class="tabControl" href="tab'+i+'">'+linkText+'</a></li>';
                    linksHtml.push(linkHtml);
                });
                //remove all drupal structure rubish
                $('.field-name-field-tab-link *').remove();
                
                // create new html for tabs
                var navHtml = "";
                $.each(linksHtml, function(i) {
                    navHtml += linksHtml[i];
                });
                $('.field-name-field-tab-link').append($("<nav><ul>"+navHtml+"</ul></nav>"));
                // loop through the tabs assign the id and hide all exept the first one
                tabs.each(function(i){
                    $(this).attr('id', 'tab'+i);
                    if (i != 0) {
                        $(this).hide();
                    }
                });
                
                // bind the link events
                $('.tabControl').bind('click', function(e) {
                    e.preventDefault();
                    e.stopPropagation();
                    $(' .field-name-field-tab-link').find('a').removeClass('selected');
                    $(this).addClass('selected');
                    var tabId = $(this).attr('href');
                    tabs.hide();
                    $('#'+tabId).show();
                })
            } else {
                throw new Error('nuber of link and tags are not equal');
            }
            
            
		},

		// hit area ON
		expandHitAreaOn: function expandHitAreaOn (){
		    $(this).css({
		        //'background':'#f0f0f0',
		        'cursor':'pointer'
		            })
		    $(this).addClass('graditent');	
		},
		// hit area off
		expandHitAreaOff: function expandHitAreaOff (){
		    $(this).removeClass('graditent')
		},

		placeholder: function placeholder($element, value) {
			$element.attr('placeholder', value)

		}

	}

	return {

		getView: function getView() {
			return View;
		}

	};

})(jQuery);


/**
 *  Controller - contains logic code of application
 *             - controls the flow of execution
 */
HRBOT.Sitewide.Controller = (function($) {

    // short-cut variables for accessing Model and View, which are in separate closures
	var Model = HRBOT.Sitewide.Model,
	     View = HRBOT.Sitewide.View.getView();

	/**
	 *  General-purpose helper functions
	 */
	var Helper = {

		convertXtoY: function convertXtoY() {

			// some generic helper function

		}
	};

	var Validation = {
	    
	    // register fucntion only with that body class
        validateBodyClass: function validateBodyClass(className) {
            if($('body.'+className).length > 0) {
                return true;
            } else {
                return false;
            }
        },
        
		
		fieldsValid: function fieldsValid() {

			// validation logic

			return isValid;
		}

	};

	/**
	 *  Functions that bind events and functions that get triggered from events
	 */
	var Event = {
	    
	    callGoogleMap: function callGoogleMap() {
		    if (Validation.validateBodyClass('page-node-18') || Validation.validateBodyClass('page-node-126')) {
		        View.createGoogleMap();
		    }
		},
		
		createTabs:function createTabs() {
		   
		    if(Validation.validateBodyClass('node-type-tabbed-content')) {
		        
		        View.createTabs();
		    }
		},

		saveClick: function saveClick(e) {

			if(Validation.fieldsValid()) {


			}

		},
		
		expandHitAreaClick: function expandHitAreaClick () {
		   var hitlink = $(this).find('.node-readmore a').attr('href')
		   window.location = hitlink;
		},

		addPlaceholder: function addPlaceholder() {
			var value = ($('html').attr('lang') === 'en' || $('html').attr('lang') == undefined) ? "Search" : "Tražilica";
			View.placeholder($("#edit-search-block-form--2"), value);
		},
		
		callFacebookFeed: function () {

			// $.getJSON('https://graph.facebook.com/index/feed?access_token=365711886855103|fcXIIWs6W_pkWItIIFpbLe4BHm4', function(data) {
  	// 			var items = [];
  	// 			//debugger;
  	// 			$.each(data, function(key, val) {
   //  				items.push('<li id="' + key + '">' + val + '</li>');
  	// 			});

  	// 			$('<ul/>', {
   //  				'class': 'my-new-list',
   //  				html: items.join('')
  	// 			})
  	// 			//.appendTo('body');
			// });

		},

		bind: function bind() {
			$('article.node').hover(
			    View.expandHitAreaOn,
			    View.expandHitAreaOff
			).bind('click', Event.expandHitAreaClick);
			
		}

	};

	/**
	 *  The return block is what gets exposed to global scope
	 */
	return {

		init: function init() {
			//Event.callFacebookFeed();
      		Event.callGoogleMap();
      		Event.createTabs()
			Event.bind();
			Event.addPlaceholder();
			//Event.callFacebookFeed():
			
			

			// execute any View functions needed on page load.

            //View.updateDOM1();
		}
	};

})(jQuery);
