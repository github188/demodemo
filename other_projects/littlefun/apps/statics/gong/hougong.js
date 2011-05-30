// JavaScript Document
$(function(){
	$(".dongtai_scroll").jCarouselLite({
	speed:500,
	scroll:1,
	auto:4000,
	visible:1,
	vertical:true
	});
	$("#myfans").jCarouselLite({
	speed:500,
	scroll:1,
	visible:1,
	btnNext: "#fansmore",
	});
	$("#fans_scroll").jCarouselLite({
		speed:500,
		scroll:1,
		visible:1,
		btnNext: "#gzmore",
		//start:nStart,
//		beforeStart:function(pa){
//			if(pa.prevObject.length==2){
//				//翻到最后一页，更新网友
//				refresh_friend();
//			}
//		}
	});
});

////更新好友
//function refresh_friend(){
//	//按钮等待
//	$(".change a").unbind();
//		
//	var uid = $("#myself").val();
//	var url =  "friends_list.php";
//	var param = {"uid":uid,"refresh":1};
//	$.post(url,param,function(result){
//		$(".fans_scroll").html(result);
//		$(".fans_scroll").jCarouselLite({
//			btnNext: ".more",
//			speed:300,
//			scroll:1,
//			visible:1,
//			beforeStart:function(pa){
//				if(pa.prevObject.length==2){
//						//翻到最后一页，更新网友
//					refresh_friend();
//				}
//			}
//				//按钮可点
//		});
//	});
//}
	
	
//生成后宫
function test_huogong(){
	var url="test.php";
	var myselt=$("#myselft").val();
	var param={
			"userId":myselft,
			"result":result
		};
	$.post(url,param,function(result){
			if(result.error){
				myError(result.error);
			}
			else{
				$("#testimg img").attr("src",result.img);
				$("#text_area").val(result.label);
				$(".send_textbox").show();
			}
	});
}

//发布到新浪微博
function weibo_Submit(){
	var content=$("#text_area").val();
	var imgurl=$("#testimg").attr("src");
	if(content!='' && imgurl!=''){	
		var url="sendweibo.php";
		var param={
			"content":content,
			"imgurl":imgurl
		};
		$.post(url,param,function(result){
			if(result.error){
				tishi("发微博失败，您可能重复发送了！");
				return false;
			}else{
				if(result=="repeat"){
					tishi_success("你已经发过该微博喽！");
				}else{
					tishi_success("发微博成功！");
				}
			}
		});
	}
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

//文本字数不得超过140字
function st_context() {
	var countnum = $("#text_area").val().length;
	if (countnum > 140) {
		tishi("微博字数不得多于140个字！");
	}
	;
}