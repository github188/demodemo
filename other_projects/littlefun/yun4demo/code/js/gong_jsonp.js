var Gong = {};

$(document).ready(
   function(){
       if(!action){
          load_profile_page();
       }else if(action == 'create_gong'){
       	  load_create_gong();
       }
   }
);

function load_create_gong(){
      $("#user_info").template("user_info" );
      jQuery.getJSON("http://littlefun.sinaapp.com/gong/create_gong_info_jsonp/?"+"&callback=?&uid=" + user_id , function(data) {
          $('#meImg').attr('src', data['me']['profile_image_url']);
          $('#meName').html(data['me']['name']);
          $("#create_my_gong").attr('href', "http://littlefun.sinaapp.com/gong/out_create_gong/" + data['me']['id'] + "/?gender=" + data['me']['gender']);
          
          $("#hot_list li").remove();
          $.tmpl("user_info", data.hot_list).appendTo("#hot_list ul");

          $("#recent_list li").remove();
          $.tmpl("user_info", data.recent_list).appendTo("#recent_list ul");          
      });
             
    window.setTimeout(load_gong_img, 2000);
}

var retry_count = 0;
function load_gong_img(){
    jQuery.getJSON("http://littlefun.sinaapp.com/gong/jsonp/?q=create_gong&callback=?&uid=" + user_id + '&time=' + (new Date()).getTime(),
    function(data){
       if(data['status'] == 'creating'){
            retry_count++;
       	    if(retry_count < 5){
       	       //alert("retry:" + retry_count);
       	       window.setTimeout(load_gong_img, 2000);
       	    }else {
       	       alert("连接超时。");
       	    }
       }else if(data['status'] == 'done'){
       	   $('#test_Text').val(data['text']);
       	   $('#test_Img').attr('src', data['image_url'] + '?time=' + (new Date()).getTime());
       }else {
       	   alert("error:" + data['status'] );
       }
    },
    'json');
}

function load_profile_page(){
      $("#user_info").template( "user_info" );
      jQuery.getJSON("http://littlefun.sinaapp.com/gong/jsonp/?"+"&callback=?&uid=" + user_id , function(data) {
          $('#meImg').attr('src', data['me']['profile_image_url']);
          $('#meName').html(data['me']['name']);
          if(data['gong']){
          	//alert(data['gong']['image_url']);
          	$('#gongImg').attr('src', data['gong']['image_url']);
          }
          //
          $("#create_my_gong").attr('href', "http://littlefun.sinaapp.com/gong/out_create_gong/" + data['me']['id'] + "/?gender=" + data['me']['gender']);
          $("#myfriends li").remove();
	  load_user_info($("#myfriends ul"), data['friends']);
	  Gong.friends = data['friends'];
	  refresh_friends();
	  
          $("#my_fans li").remove();
	  load_user_info($("#my_fans ul"), data['fans']);
	  Gong.fans = data['fans'];	
          $("#my_fans").jCarouselLite({
             speed:500,
             scroll:1,
             visible:1,
             btnNext: "#my_fans_more"
          });
      }); 

      $('#have_gong_friends').click(function(){
          $("#myfriends li").remove();
          if($('#have_gong_friends').attr('checked')){
		var have_gong = [];
		for(var i = 0; i < Gong.friends.length; i++){
		    if(Gong.friends[i]['have_gong']){
		    	have_gong.push(Gong.friends[i]);
		    }
		}
		load_user_info($("#myfriends ul"), have_gong);
          }else {
	       load_user_info($("#myfriends ul"), Gong.friends);
          }
          refresh_friends();
      });
      
      $('#have_gong_fans').click(function(){
          $("#my_fans li").remove();
          if($('#have_gong_fans').attr('checked')){
		var have_gong = [];
		for(var i = 0; i < Gong.fans.length; i++){
		    if(Gong.fans[i]['have_gong']){
		    	have_gong.push(Gong.fans[i]);
		    }
		}
		load_user_info($("#my_fans ul"), have_gong);
          }else {
	       load_user_info($("#my_fans ul"), Gong.fans);
          }
          $("#my_fans").jCarouselLite({
             speed:500,
             scroll:1,
             visible:1,
             btnNext: "#my_fans_more"
          });
      });
      
}

function load_user_info(node, data){
    for(var i = 0; i < data.length; i+=12){
       var page = $("<li>").appendTo(node);
       page.height(750);
       end = i + 12 > data.length ? data.length: (i+12);
       var user_list = data.slice(i, end);
       $.tmpl("user_info", user_list).appendTo(page); //#.appendTo("#myfriends ul");
    };
}

function refresh_friends(){
    $("#myfriends").jCarouselLite({
        speed:500,
        scroll:1,
        visible:1,
        btnNext: "#friends_smore"
    });
}

//发布到新浪微博，参数为微博名
function weibo_Submit(){
    jQuery.getJSON("http://littlefun.sinaapp.com/gong/send_weibo_jsonp/?callback=?&uid=" + user_id + '&time=' + (new Date()).getTime(),
    function(data){
    	var result = data.result;
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
    },
    'json');
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



