//window.onerror = function Nopop(){
	//return true;
//};

var ie = document.all && !window.opera;
	iebody = (document.compatMode=="CSS1Compat") ? document.documentElement : document.body;
	docheight = (ie || iebody) ? iebody.clientHeight : window.innerHeight;

//iebody.style.background = '#c1e4d8';

function checkall(form, refer, prefix) {
	var form = document.getElementById(form);
	for(var i = 0; i < form.elements.length; i++) {
		var e = form.elements[i];
		switch(refer.name) {
			case "chkall":value = true;break;
			case "unchkall":value = false;break;
			case "opposite":value = !e.checked;break;
			default :value = refer.checked;
		}
		if(e.name != checkall && (!prefix || (prefix && e.name.match(prefix)))) e.checked = value;
	}
}
function cfmoption(word, loc){
	if(confirm(word)) location.href = loc;
}
function promptoption(word, cname, loc){
	if(str = prompt(word,cname)) location.href = loc + encodeURIComponent(str);
}
function externallinks() {
	if(!document.getElementsByTagName) return;
	var anchors = document.getElementsByTagName("a");
	for(var i = 0; i < anchors.length; i++) {
		var anchor = anchors[i];
		if(anchor.getAttribute("href") && anchor.getAttribute("rel")) {
			switch(anchor.getAttribute("rel")) {
				//alert(anchor.getAttribute("rel"));
				case "external":anchor.target = "_blank";break;
				//case "self":anchor.target = "_self";break;
				case "parent":anchor.target = "_top";break;
			}
		}
	}
}

function getadverts(siteurl){
	var pos = new Array(
		'header',
		'bookinfo',
		'list_top',
		'list_foot',
		'read_top',
		'read_foot'
	);
	for(var i = 0; i < pos.length; i++) {
		try{
			//alert(pos[i]);
			//$('advert_'+pos[i]).innerHTML='<script type="text/javascript" src="'+siteurl+'/dataquote.php?getad='+pos[i]+'"></script>';
			$('advert_'+pos[i]).src = siteurl + '/dataquote.php?getad='+pos[i];
			//alert($('advert_'+pos[i]).innerHTML);
		}catch(exception) {
		}
	}
}

function setCookie(name,value){//设置cookie
	var t = new Date();
	t.setTime(t.getTime()+1*3600000);
	document.cookie = name + "=" + value + ";expires=" + t.toGMTString();
}
function getCookieVal(offset){//取得offset位置的cookie值
	var endstr = document.cookie.indexOf (";", offset);
	if(endstr == -1) endstr = document.cookie.length;
	return unescape(document.cookie.substring(offset, endstr));
}
function getCookie(name){//取得名称为name的cookie值
	var arg = name + "=";
	var alen = arg.length;
	var clen = document.cookie.length;
	var i = 0;
	while(i < clen) {
		var j = i + alen;
		if (document.cookie.substring(i, j) == arg) return getCookieVal(j);
		i = document.cookie.indexOf(" ", i) + 1;
		if(i == 0) break;
	}
	return null;
}

function chtmlspecialchars(str){
	return new String(str).split('&').join('&amp;').split('<').join('&lt;').split('>').join('&gt;').split('"').join('&quot;').split("'").join('&#039;').split("\r\n").join('<br />').split("\n").join('<br />').split("\t").join('&nbsp;&nbsp;&nbsp;&nbsp;');
}

function cstrlen(str){
	var len = 0;
	for(var i = 0; i < str.length; i++) {
		len += str.charCodeAt(i) < 0 || str.charCodeAt(i) > 255 ? 2 : 1;
	}
	return len;
}
function cstrlines(str){
	return new String(str).split("\n").length;
}

function $(objId){
	return document.getElementById(objId);
}


var HighLight = new Object();
HighLight.oStyle = 'rd8_col0';
HighLight.hStyle = 'rd8_col1';
HighLight.sStyle = 'rd8_col2';

HighLight.over   = function (trId){
	$(trId).className = this.hStyle;
};
HighLight.out    = function (trId, inputId){
	if(inputId && $(inputId).checked) {
		$(trId).className = this.sStyle;
	}else {
		$(trId).className = this.oStyle;
	}
};
HighLight.click  = function (inputId){
	$(inputId).checked = !$(inputId).checked;
};
HighLight.change = function (trId, inputId){
	if($(inputId).checked) {
		$(trId).className = this.sStyle;
	}else {
		$(trId).className = this.oStyle;
	}
};


function addBrowserFavorite(title, url){
	if(ie)
		window.external.AddFavorite(url, title);
	else
		window.sidebar.addPanel(title, url, '');
}

function setHomePage(url){
	if(ie) {
		document.body.style.behavior='url(#default#homepage)';
		document.body.setHomePage(url);
	}else {
		try {
			netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
		}catch(e) {
			alert('由于您浏览器的安全设置，本次操作无法完成' + "\n" + '请打开 about:config 修改 signed.applets.codebase_principal_support 的值为 true 并在弹出警告时选择是');
		}
		var prefs = Components.classes['@mozilla.org/preferences-service;1'].getService(Components. interfaces.nsIPrefBranch);
			prefs.setCharPref('browser.startup.homepage', url);
	}
}


function intval(value){
	if(isNaN(value)) {
		return 0;
	}else {
		return Math.round(value);
	}
}

function calclen(value, maxlength, minlength){
	var length = cstrlen(value);

	if(length < minlength) {
		$('rd8_lengthlimit').innerHTML = '<span style="color: Red;">长度不足 ' + length + ' / ' + minlength + '</span>';
	}else if(length > maxlength) {
		$('rd8_lengthlimit').innerHTML = '<span style="color: Red;">内容过长 ' + length + ' / ' + maxlength + '</span>';
	}else {
		$('rd8_lengthlimit').innerHTML = '字数统计 ' + length + ' / ' + maxlength;
	}
}