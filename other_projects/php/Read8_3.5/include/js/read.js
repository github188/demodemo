var font_size;

function setKeys(bid,cid,lcid,ncid) {
	common_load();
	if(document.attachEvent) {
		document.attachEvent("onkeydown",function (event){
				if(event.keyCode==37) location.href=lcid+".html";
				if(event.keyCode==39) location.href=ncid+".html";
				if(event.keyCode==13) location.href="index.html";
			}
		);
	}else {
		document.addEventListener("keydown",function (event){
				if(event.keyCode==37) location.href=lcid+".html";
				if(event.keyCode==39) location.href=ncid+".html";
				if(event.keyCode==13) location.href="index.html";
			}
		,true);
	}
}

function common_load(){
	externallinks();
	$('fontsize').innerHTML = '字体大小：<select name="fs" id="fs" onchange="javascript:changeFontSize(this.options[this.selectedIndex].value);"><option value="12">12</option><option value="14">14</option><option value="16" selected="selected">16</option><option value="18">18</option><option value="20">20</option><option value="22">22</option><option value="24">24</option></select>';
	setFontSize();
}

function changeFontSize(font_size) {//改变字体大小
	$("content").style.cssText="font-size: "+font_size+"px;";
	setCookie("font_size",font_size);
}

function setFontSize() {//设置字体大小
	var font_size = getCookie("font_size");
	var font_size_index = 0;
	switch(font_size) {
		case "12":font_size_index=0;break;
		case "14":font_size_index=1;break;
		case "16":font_size_index=2;break;
		case "18":font_size_index=3;break;
		case "20":font_size_index=4;break;
		case "22":font_size_index=5;break;
		case "24":font_size_index=6;break;
		default:font_size_index=1;
	}
	if(font_size) {
		$("content").style.cssText = "font-size: "+font_size+"px;";
		$("fs").selectedIndex = font_size_index;
	}
}

var locate = document.documentElement.scrollTop;
var timer;
function scroller(){
	if(locate == document.documentElement.scrollTop) {
		locate++;
		scroll(0,locate);
		clearTimeout(timer);
		timer = setTimeout("scroller()",1);
	}
}
if(document.attachEvent) {
	document.attachEvent("ondblclick",function (event){
			scroller();
		}
	);
	document.attachEvent("onclick",function (event){
			clearInterval(timer);
			locate = document.documentElement.scrollTop;
		}
	);
}else {
	document.addEventListener("dblclick",function (event){
			scroller();
		}
	,true);
	document.addEventListener("click",function (event){
			clearInterval(timer);
			locate = document.documentElement.scrollTop;
		}
	,true);
}
