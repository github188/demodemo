$('pagecolor').innerHTML = '背景颜色：<select name="pc" id="pc" onchange="javascript:changePageColor(this.options[this.selectedIndex].value);"><option style="background-color: #EBF3E6" value="EBF3E6">默认设置</option><option style="background-color: #E9FAFF" value="E9FAFF">淡蓝海洋</option><option style="background-color: #FFFFED" value="FFFFED">明黄清俊</option><option style="background-color: #EEFAEE" value="EEFAEE">绿意淡雅</option><option style="background-color: #FCEFFF" value="FCEFFF">红粉世家</option><option style="background-color: #FFFFFF" value="FFFFFF">白雪天地</option><option style="background-color: #EFEFEF" value="EFEFEF">灰色世界</option></select>';

setPageColor();

function changePageColor(pagecolor) {//改变背景颜色
	document.body.style.cssText="background: #"+pagecolor+";";
	setCookie("pagecolor",pagecolor);
}

function setPageColor() {//设置背景颜色
	var pagecolor = getCookie("pagecolor");
	var pagecolor_index = 0;
	switch(pagecolor) {
		case "EBF3E6":pagecolor_index=0;break;
		case "E9FAFF":pagecolor_index=1;break;
		case "FFFFED":pagecolor_index=2;break;
		case "EEFAEE":pagecolor_index=3;break;
		case "FCEFFF":pagecolor_index=4;break;
		case "FFFFFF":pagecolor_index=5;break;
		case "EFEFEF":pagecolor_index=6;break;
		default:pagecolor_index=0;
	}
	if(pagecolor) {
		document.body.style.cssText = "background: #"+pagecolor+";";
		$("pc").selectedIndex = pagecolor_index;
	}
}

var font_lang;

$('rd8_font_lang').innerHTML='<span class="rd8_a" onclick="javascript:changeFontLang(1);"><img src="'+G_siteurl+'/images/tc.gif" align="absmiddle" />正體中文</span>';

if(getCookie("font_lang")=="1") initlang();
function initlang(){
	font_lang = getFontLang();
	var_array = new Array("font_lang");
	parseHTML(var_array);
	if(getCookie("font_lang")=="1") document.body.innerHTML = s2t(document.body.innerHTML);
}

function getFontLang(){//取得繁简转换控件
	var font_lang = '';
	if(getCookie("font_lang")=="1") font_lang='<span class="rd8_a" onclick="javascript:changeFontLang(0);"><img src="'+G_siteurl+'/images/sc.gif" align="absmiddle" />简体中文</span>';
	else font_lang = '<span class="rd8_a" onclick="javascript:changeFontLang(1);"><img src="'+G_siteurl+'/images/tc.gif" align="absmiddle" />正體中文</span>';
	$("rd8_font_lang").innerHTML = font_lang;
	return font_lang;
}

function changeFontLang(font_lang){//改变字体繁简
	setCookie("font_lang",font_lang);
	top.location = top.location;
}

function parseHTML(var_array){//替换变量
	if(!var_array) return;
	var divs;
	for(var i = 0; i < var_array.length; i++) {
		divs = document.getElementsByName(var_array[i]);
		var j = 0;
		while(divs[j]) {
			eval("divs["+j+"].innerHTML="+var_array[i]);
			j++;
		}
	}
}
