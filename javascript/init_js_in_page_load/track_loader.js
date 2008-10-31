var inited_event = false;
var ajax_root = 'http://10.56.117.81'
var current_tc = ''
var bug_window

// init for track loader
var init = function(){
	if(inited_event) return;
	inited_event = true;
	
	bug_window = document.createElement("iframe")
	bug_window.src = ajax_root + '/bug/list';
	bug_window.id = 'bug_window';
	bug_window.scrolling='no';
	
	var main = document.getElementsByTagName('body')[0];
	var style = bug_window.style;
	style['position'] = 'absolute';
	style['top'] = '20px';
	style['right'] = '20px';
	style['width'] = '300px';
	style['height'] = '250px';
	style['zIndex'] = '9';
		
	main.insertBefore(bug_window, main.firstChild);
	
	onEvent(document, "click", event_handler);
	onEvent(window, "scroll", event_scroll);
	//onEvent(window, "resize", event_resize);	
}

var main = document.getElementsByTagName('body');
//document.onreadystatechange=init;	
if(window.addEventListener){
	onEvent(document, "DOMContentLoaded", init);
}else { // IE	
	onEvent(document, "readystatechange", function(){
	   if (document.readyState=="complete")
	   {
	      init();
	   }		
	});
}

//function for window scroll
function event_scroll() {
	var scrollPos; 
	if (typeof window.pageYOffset != 'undefined') { 
	   scrollPos = window.pageYOffset; 
	} 
	else if (typeof document.compatMode != 'undefined' && 
	     document.compatMode != 'BackCompat') { 
	   scrollPos = document.documentElement.scrollTop; 
	} 
	else if (typeof document.body != 'undefined') { 
	   scrollPos = document.body.scrollTop; 
	} 
	var style = bug_window.style;
	style['top'] = scrollPos + 20 + 'px';
}


function onEvent(o, name, handler){
	if(window.addEventListener){
		o.addEventListener(name, handler, false);
	} else { // IE
		o.attachEvent('on' + name, handler);
	}
}
