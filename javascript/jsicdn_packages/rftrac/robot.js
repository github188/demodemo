
 /**
   * @param  <Event> e
   */
function get_test_case_table(e) {
	for(var i = e; i; i = i.parentNode){
		if(typeof i.tagName != 'string'){
			return undefined
		}			
		if(i.tagName.toLowerCase() == 'table' &&
		   i.className.toLowerCase() == 'test'
		  ) {
			return i;
		}
	}
	return undefined;
}

 /**
   * @param  <Event> e
   */
function get_testcase_log_id(case_table) {
    var meta = case_table.getElementsByTagName('table')[0];
    var name = get_testcase_name(meta);
    var tags = meta.getElementsByTagName('th');
    for(var i = 0; i < tags.length; i++){
        var n = tags.item(i);
        var innerText = n.firstChild.nodeValue;
        if (/Start\s+\/\s+End\s+\/\s+Elapsed/.test(innerText)){
            n = n.nextSibling;
            var time = n.firstChild.nodeValue;
            var times = time.split(/\s*\/\s*/);
            times.pop();
            var start2end = times.join("_");
            break;
        }
    }
    
    name = name.replace(/^\s+|\s+$/, '');
    var case_id = name + "_" + start2end;
    case_id = case_id.toLowerCase();
    return To_md5(case_id);
}

function get_testcase_name(meta){
	
	var tags = meta.getElementsByTagName('th');
	for(var i = 0; i < tags.length; i++){
		var n = tags.item(i);
		var innerText = n.firstChild.nodeValue;
		if (/^Full Name/.test(innerText)){
			n = n.nextSibling;
			var full_name = n.firstChild.nodeValue;
			full_name = full_name.replace(/Test Collection:(.*?\.)/,'');
			full_name = full_name.split('.');
			var test_name = full_name.pop();
			var suite_name = full_name.pop();
			return suite_name + "." + test_name;
		}
	}
	
	return "";
}

 /**
   * @param  <Event> e
   */
function inject_trac_panel(case_table){
	var log_id = get_testcase_log_id(case_table);
	var meta = case_table.getElementsByTagName('table')[0];
	
	//alert(meta.className);
	var meta_item = meta.getElementsByTagName('tr');
	//bodyRef.appendChild(s); 
	
	var trac_panel = rfTrac.trac_panel.cloneNode(true);
	if(meta_item.length > 4){
		meta_item[4].parentNode.insertBefore(trac_panel, meta_item[4]);
	}else {
		meta_item[0].parentNode.appendChild(trac_panel); 
	}
	
	case_table.trac = {panel:trac_panel, 
					   logid:log_id,
					   tcname:get_testcase_name(meta) };
}

 /**
   * @param  <Event> e
   */
function show_testcase_comments(){
	//alert('show_testcase_comments ' + e.tagName); 
	  	
	if(!rfTrac.comments_dialog){
	    function post_comments_with_ajax(){
		    var d = rfTrac.comments_dialog.model;
		    var trac = rfTrac.curTestTrac;	    
	        $.post(rfTrac.endpoint + "trac",
	               {uuid: trac.logid,
	                action: d.category(),
	                text: d.comments(),
	                username: d.user(),
	                bugid: d.bugid(),
	                key: rfTrac.api_key            
	               }, function(e){
	                   if(e.status != "OK"){alert(e.message);}
	                   else{
	                       alert("update testcase successfully.");
	                   }
	               }, 
	               "json");
	          
	    };
		rfTrac.comments_dialog = build_testcase_comments_dialog(post_comments_with_ajax);
	}
	
    var d = rfTrac.comments_dialog.model;
    var trac = rfTrac.curTestTrac;
	
	d.tcname(trac.tcname);
	//d.logid(trac.logid);
	d.category($("select", trac.panel).val());
	//d.user = 'xx';
	d.comments('');
	
	rfTrac.comments_dialog.dialog('open');
}

 /**
 * @param  <Event> e
   */
function on_click_event(e) {
	var t = e.target || e.srcElement;
	
	t = get_test_case_table(t);
	if(typeof t === 'undefined'){ return };
	
	if(typeof(t.trac) === 'undefined'){
		inject_trac_panel(t);
	}
	
	rfTrac.curTestTrac = t.trac;
}


 /**
   * @public
   * @param  <Event> e
   */
function setup_robot_trac(endpoint, api_key, settings){
	//testcase_comments = show_testcase_comments;
	
	if (typeof rfTrac === 'undefined'){
		rfTrac = {endpoint:endpoint, api_key:api_key, settings:settings};
		$(document).click(on_click_event);
		var   trac_panel = $("<tr><th>RF Trac:</th><td>" +
							"<select class='trac_category' onchange='rfTrac.show_comments(event)'>" + 
							"</select>" +
							'&nbsp; <a href="#">comments</a>' +
							'&nbsp; <a href="#">history</a>' + 
							"</td></tr>"
							);
		
		var cate_list = $("select", trac_panel);
		
		$.each(settings.category, function(i, n){
		    var d = $("<option value='" + n[0] + "'>" + n[1] + "</option>").
		              appendTo(cate_list);
        });
		
		rfTrac.trac_panel = trac_panel.get(0);
		
		rfTrac.comments_dialog = undefined;
		rfTrac.show_comments = show_testcase_comments;
		rfTrac.curTestTrac = undefined;
		
		var head = document.getElementsByTagName('head')[0];
		head.appendChild($('<link type="text/css" href="http://jsicdn01.appspot.com/styles/css/ui-lightness/ui.all.css" rel="stylesheet" />').get(0)); 
	}
}


