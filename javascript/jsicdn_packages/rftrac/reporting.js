

function build_diff_dialog(){
    var dialog = $("<div id='diff_show'>" +
                              "<div class='head'><a class='close'>[X]</a></div>" +
                              "<div class='body'></div>" +                           
                              "</div>");
	$(["table class='meta'>",
	"<tr><th>Build Name:</th><td class='bn'></td></tr>",
	"<tr><th>SUT name:</th><td class='sn'></td></tr>",
	"<tr><th>SUT version:</th><td class='sv'></td></tr>",
	"<tr><th>Start time:</th><td class='st'></td></tr>",
	"<tr><th>Elapsed:</th><td class='el'></td></tr>", 
	"<tr><th>Trac Status:</th><td class='ts'></td></tr>",
	"<tr><th>Execute By:</th><td class='eb'></td></tr>",
	"</table>"].join("")).appendTo(".body", dialog);    
	
    function diff_show_model(e){
        this.e = e;
        this._build_name = $('.bn', e);
        this._sut_name = $('.sn', e);
        this._sut_version = $('.sv', e);
        this._starttime = $('.st', e);
        this._elapsed = $('.el', e);
        this._trac_status = $('.ts', e);
        this._execute_user = $('.eb', e);
    }
    
	diff_show_model.prototype = {
	    build_name: function(){return this._build_name.html.apply(this._build_name, arguments);},
	    sut_name: function(){return this._sut_name.html.apply(this._sut_name, arguments);},
	    sut_version: function(){return this._sut_version.html.apply(this._sut_version, arguments);},
	    starttime: function(){return this._starttime.html.apply(this._starttime, arguments);},
	    elapsed: function(){return this._elapsed.html.apply(this._elapsed, arguments);},
	    trac_status: function(){return this._trac_status.html.apply(this._trac_status, arguments);},
	    execute_user: function(){return this._execute_user.html.apply(this._execute_user, arguments);}
	}
	
   dialog.model = new diff_show_model(dialog);
	
   dialog.css({display:'none',
               position:'absolute',
               background:'#f7f7ff',
               border:'3px ridge #eee',
               width:'300px',
               height:'200px'
               });
   dialog.css("z-index", 10);
   dialog.draggable({handle:'.head'});
   
   $(".head", dialog).css({background:'#c6d8ff', cursor:'move'});
     
   $("a.close", dialog).click(function(){ $("#diff_show").hide(); });   
   $("body").append(dialog);   
   
   return dialog;
}

function show_diff_testcase(){
    var diff_url = this.href, p = $(this).offset();
    function testcase_loading(){
        $.getJSON(diff_url, [], function(data){
            var m = rfTrac.show_diff.model; 
            m.build_name(data['build_name']);
            m.sut_name(data['sut_name']);
            m.sut_version(data['sut_version']);
            m.starttime(data['starttime']);
            m.elapsed(data['elapsed']);
            m.trac_status(data['trac_status']);
            m.execute_user(data['execute_user']);
        });
    }
    
    if(rfTrac.show_diff.css('display') == 'none'){
        rfTrac.show_diff.css({top:p.top, left:p.left + $(this).width() + 20}).
        show('fast', testcase_loading);
    }else {
        testcase_loading();
    }
}

 /**
   * @public
   * @param  <Event> e
   */
function setup_diff_reporting(endpoint, api_key){
    //testcase_comments = show_testcase_comments;
    
    if (typeof rfTrac === 'undefined'){
        rfTrac = {endpoint:endpoint, api_key:api_key};
        //$(document).click(on_click_event);
        rfTrac.show_diff = build_diff_dialog();
                              
        $("td.diff a").hover(show_diff_testcase, function(){});
    }
}