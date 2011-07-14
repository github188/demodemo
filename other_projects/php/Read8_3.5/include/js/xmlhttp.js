/*
	rimy ajax
	Dec. 17th. 2007
	(C) 2007 rimy.org
*/

var r_xmlhttp;
var r_ajax_timedOut = false;
var r_ajax_requestTimeLimit = 15;

function r_ajax_request(URI, request_handler, method, request_string){
	r_xmlhttp = false;

	if(!method) method = "GET";
	method = method.toUpperCase();
	if(method == "GET") request_string = null;

	if(typeof XMLHttpRequest != "undefined") {
		r_xmlhttp = new XMLHttpRequest();
	}else {
		var aVersion = new Array(
			"MSXML2.XMLHTTP.6.0",
			"MSXML2.XMLHTTP.5.0",
			"MSXML2.XMLHTTP.4.0",
			"MSXML2.XMLHTTP.3.0",
			"MSXML2.XMLHTTP.2.0",
			"MSXML2.XMLHTTP",
			"Microsoft.XMLHTTP"
		);
		var length = aVersion.length;
		for(var i = 0; i < length; i++) {
			try {
				r_xmlhttp = new ActiveXObject(aVersion[i]);
				break;
			}catch(exception) {
			}
		}
	}

	if(!r_xmlhttp) {
		alert("Can't create XMLHttpRequest.");
		return false;
	}

	r_xmlhttp.open(method, URI, true);
	eval("r_xmlhttp.onreadystatechange = r_" + request_handler + "_ajax_handler");
	if(method == "POST") r_xmlhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	r_xmlhttp.send(request_string);
	r_ajax_timedOut = setTimeout(r_ajax_timeout, r_ajax_requestTimeLimit * 1000);
	return true;
}

function r_ajax_timeout(clear){
	clearTimeout(r_ajax_timedOut);
	r_ajax_timedOut = false;
	if(!clear) {
		alert("XMLHttpRequest timed out.");
		r_xmlhttp = false;
	}
}

function r_ajax_handler(){
	if(r_xmlhttp.readyState == 4) {
		if(r_xmlhttp.status == 200) {
			r_ajax_timeout(true);
			return true;
		}else {
			alert("Unkonwn error");
			return false;
		}
	}else {
		return false;
	}
}
