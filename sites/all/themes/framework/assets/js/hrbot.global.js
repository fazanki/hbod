var HRBOT = HRBOT || {};


HRBOT.Page = (function ($){
    
    Page = {
        
        registry: [],

        register: function ( where, what ){
        //debugger;
            if( !(where == '*' || "className" in where || "id" in where) && !!what )
                throw new Error('Parameters missing for HRBOT.Page.register');

            var key = where == '*' ? '*' : (where.className ? "." + where.className : '#' + where.id);
            Page.registry[key] = Page.registry[key] || [];

            Page.registry[key].push(what);
        },

        run: function (){
            Page.runAll(Page.registry['*'] || []);
            var classes = $(document.body).attr('className').split(' ');
            for( var i = 0, cl = classes.length; i < cl; i++ ){
                Page.runAll(Page.registry['.' + classes[i]] || []);
            }
            
            Page.runAll(Page.registry[ '#' + $(document.body).attr('id')] || []);
        },
        
        runAll: function ( group ){
            if( !!group ){
                for( var i = 0, gl = group.length; i < gl; i++ ){
                    if( group[i] instanceof Function )
                        group[i]();
                }
            }
        }
    }

    return Page;
})(jQuery);

jQuery(function (){
    HRBOT.Page.run();
    
})