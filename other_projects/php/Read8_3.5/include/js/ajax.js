function AJAX_Init(){
	if(typeof XMLHttpRequest != "undefined") {
		return new XMLHttpRequest();
	}else {
		var aVersion = new Array(
			"Microsoft.XMLHTTP",
			"MSXML2.XMLHTTP",
			"MSXML2.XMLHTTP.2.0",
			"MSXML2.XMLHTTP.3.0",
			"MSXML2.XMLHTTP.4.0",
			"MSXML2.XMLHTTP.5.0",
			"MSXML2.XMLHTTP.6.0"
		);
		for(var i = 0; i < aVersion.length; i++) {
			try{
				var xmlHttp = new ActiveXObject(aVersion[i]);
			}catch(exception) {
			}
		}
		return xmlHttp;
	}
}

/*function AJAX(){
	var art = 'global';

	if(typeof XMLHttpRequest != "undefined") {
		var ajax = new XMLHttpRequest();
	}else {
		var aVersion = ["MSXML2.XMLHTTP.6.0",
						"MSXML2.XMLHTTP.5.0",
						"MSXML2.XMLHTTP.4.0",
						"MSXML2.XMLHTTP.3.0",
						"MSXML2.XMLHTTP.2.0",
						"MSXML2.XMLHTTP",
						"Microsoft.XMLHTTP"];
		for(var i=0;i<aVersion.length;i++) {
			try{
				var xmlHttp = new ActiveXObject(aVersion[i]);
			}catch(exception) {
				var ajax = xmlHttp;
			}
		}
	}

	this.request = function(url, method, parameters){
		ajax.open(method,url,true);
		alert('request '+art);
		ajax.onreadystatechange = function(){
			alert('ready '+art);
			if(ajax.readyState == 4 && ajax.status == 200) {
				alert('change '+art);
				art = ajax.responseText;
				alert('end '+art);
				//alert(this);
				//this.response();
			}
		}
		if(method == "POST") ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		ajax.send(parameters);
	};

	this.response = function(){
		//alert(this);
		alert('response '+art);
		return art;
	};
}*/