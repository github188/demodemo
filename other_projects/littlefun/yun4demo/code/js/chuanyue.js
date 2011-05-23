// JavaScript Document
//页面右侧关注切换
$(function(){
	$(".gz_scroll").jCarouselLite({
		btnNext: ".more",
		speed:300,
		scroll:1,
		start:nStart,
		visible:1,
		beforeStart:function(pa){
			if(pa.prevObject.length==2){
				//翻到最后一页，更新网友
				refresh_friend();
			}
		}
	});
	$("#tit_tab span").each(function(index){
		$(this).click(function(){
			$(this).addClass("hover").siblings().removeClass("hover");
			$("#list_tab > div").eq(index).show().siblings().hide();
		});
	});
	window.setTimeout(msgmove,3000);
	$(".messagewrap ul").hover(function() {
		$(this).attr("name", "hovered"); // 鼠标经过设置ul的name值为"hovered"
		}, function() {
			$(this).removeAttr("name");
		});
});

/**
 * 更新好友
 */
	function refresh_friend(){
		//按钮等待
		$(".mainra a").unbind();
		$(".mainra img").css("height","25px");
		$(".mainra img").attr("src","/cross/images/change_load.gif");
		
		var uid = $("#myself").val();
		var url =  "friends_list.php";
		var param = {"uid":uid,"refresh":1};
		$.post(url,param,function(result){
			$(".gz_scroll").html(result);
			$(".gz_scroll").jCarouselLite({
				btnNext: ".more",
				speed:300,
				scroll:1,
				visible:1,
				beforeStart:function(pa){
					if(pa.prevObject.length==2){
						//翻到最后一页，更新网友
						refresh_friend();
					}
				}
				//按钮可点
			});
			$(".mainra img").attr("src","../images/change_03.gif");
		});
	}

	function msgmove() {
		var isIE = !!window.ActiveXObject;
		var isIE6 = isIE && !window.XMLHttpRequest;
		if ($(".messagewrap ul").attr("name") != "hovered") {
			// 判断ul的name属性是否为"hovered"，决定下面代码块是否运行，以实现鼠标经过暂停滚动。
			var height = $(".messagewrap li:last").height();
			if (isIE6) {
				// 判断IE6，jQuery的animate动画和opacity透明在一起使用在IE6中会出现中文重影现象，
				// 所以在ie6中实行透明的禁用。
				$(".messagewrap li:last").css( {
					"height" : 0
				});
			} else {
				$(".messagewrap li:last").css( {
					"opacity" : 0,
					"height" : 0
				});
				// 列表最后的li透明和高度设置为0
			}
			$(".messagewrap li:first").before($(".messagewrap li:last"));
			// 把列表最后的li提升到顶部，实现有限列表项无限循环滚动显示
			$(".messagewrap li:first").animate( {
				"height" : height
			}, 300);
			// 列表顶部的li高度逐渐变高以把下面的li推下去
			if (!isIE6) {
				$(".messagewrap li:first").animate( {
					"opacity" : "1"
				}, 300);
				//在非IE6浏览器中设置透明淡入效果
			}
		}
		setTimeout("msgmove()", 5000);
		//设置5秒滚动一次
	}


//发布到新浪微博，参数为微博名
function weibo_Submit(weiboName){
	var content=$("#test_Text").val();
	var imgurl=$("#test_Img").attr("src");
	if(!weiboName){ 
		var username=$.trim($("#weibo_Name").val());
		if (username.length == 0 || username == '请输入微博名' || username.replace(/[ ]/g, "") == '') {
				tishi("请输入微博名！");
				return false;
		}
		weiboName=username; 
	}
	$("#send_Other img").attr("src","../images/"+platForm+"_send_load.gif");
	if(content!='' && imgurl!=''){	
		var url="sendweibo.php";
		var param={
			"weiboName":weiboName,
			"content":content,
			"imgurl":imgurl
		};
		$.post(url,param,function(result){
			if(result.error){
				tishi("发微博失败，您可能重复发送了！");
				$("#send_Other img").attr("src","../images/"+platForm+"_send_03.gif");
				return false;
			}else{
				if(result=="repeat"){
					tishi_success("你已经发过该微博喽！");
				}else{
					tishi_success("发微博成功！");
				}
				$("#send_Other img").attr("src","../images/"+platForm+"_send_03.gif");
			}
		});
	}else{
		tishi("请输入微博内容！");
		$("#send_Other img").attr("src","../images/"+platForm+"_send_03.gif");
	}
}

//自己穿越的
function testMyself(){
//	var url="cross_op.php";
//	var myself=$("#myself").val();
//	var param = {
//		"uid" : myself,
//		"op" : "result"
//	};
//	$("#test_Myself img").attr("src","../images/meload.gif");
//	$.post(url,param,function(result){
//		if(result.error){
//			myError(result.error);
//		}
//		else{
//			var img = "cross_op.php?uid="+result.uid+"&op=img&r="+Math.random();
//			$("#test_Img").attr("src",img);
//			$("#test_Text").val(result.label);
//			$(".maintextarea").show();
//			if($("#send_Weibo").attr("checked")){
//				weibo_Submit(myself);
//			}
//		}
//		$("#test_Myself img").attr("src","../images/me_03.gif");
//	});
	var myself=$("#myself").val();
	if($("#send_Weibo").attr("checked")){
		document.location.href="cross.php?uid="+myself+"&bs=1";
	}else{
		document.location.href="cross.php?uid="+myself;
	}
}

//穿越按钮
function chuanYue(){
	var url="cross_op.php";
	var username=$.trim($("#weibo_Name").val());
	if (username.length == 0 || username == '请输入微博名') {
		tishi("请输入微博名！");
		return false;
	}
	document.location.href=encodeURIComponent(username)+".html";
//	var param={
//			"uid":username,
//			"op":"result"
//	};
//	$("#chuanyue img").attr("src","../images/cy_load.gif");
//	$.post(url,param,function(result){
//		if(result.error){
//			myError(result.error);
//		}
//		else{
//			var img = "cross_op.php?uid="+result.uid+"&op=img";
//			$("#test_Img").attr("src",img);
//			$("#test_Text").val(result.label);
//			$(".maintextarea").show();
//		}
//		$("#chuanyue img").attr("src","../images/cy_btn_03.jpg");
//	});
}

//可能出现的错误信息
function myError(error){
	switch(error){
		case "timeOut":
		tishi("登录超时，<a href=''>请重新登录</a>");
		break;
	case "noUserName":
		tishi("请输入微博名");
		break;
	default:
		tishi("操作失败：" + error);
		break;
	}
}

//文本字数不得超过140字
function st_context() {
	var countnum = $("#test_Text").val().length;
	if (countnum > 140) {
		tishi("微博字数不得多于140个字！");
	}
	;
}

//错误提示
function tishi(str){
	$("#ts_con").html(str);
	$("#tishi").show();
	$("#ts_con").css("background-image", 'url(../images/09.gif)');
	window.setTimeout("tishi_close()", 4000);
}

//成功提示
function tishi_success(str){
	$("#ts_con").html(str);
	$("#tishi").show();
	$("#ts_con").css("background-image", 'url(../images/12.gif)');
	window.setTimeout("tishi_close()", 4000);
}

//关闭提示
function tishi_close(){
	$("#tishi").hide();
}

//加关注
function tracking() {
	var url = "follow.php";
	var param = {};
	$.post(url, param, function(result) {
		tishi_success("关注成功！");
	});
}