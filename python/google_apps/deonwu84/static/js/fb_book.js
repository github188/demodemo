var FB = {};
$(document).ready(function(){
    var default_other = "逗号分隔名字."
    $("#fb_plane_record .other").val(default_other);
    $("div.menu a.dialog").click(function(){
        $("#fb_plane_record .other").val(default_other);
        FB.action_plane.dialog("open");
        return false;
    });
    
    $("#fb_plane_record .other").focus(function(){
        if($(this).val() == default_other){
            $(this).val("");
        }
    });
    
    $("#fb_plane_record .money").blur(function(){
        var money = $(this).val();
        if(!money.match(/^\d+(?:\.\d{2})?$/)){
            alert("请输入正确的金额。");
        }
    });
    
    $("#c_all").click(function(){
        $("#fb_plane_record .name_list input").attr("checked", $(this).attr('checked')); 
    });
    
    //date, fb_type="", money="", master="", user_list="", other="", comments=""
    FB.submit_fb_record = function(){
        var p = $("#fb_plane_record");
        
        var user_list = []
        $(".name_list input:checked").each(function(){user_list.push($(this).val())});
        if($("#fb_plane_record .other").val() != default_other){
            //alert($("#fb_plane_record .other").val());
            user_list.push($("#fb_plane_record .other").val());
        }
        
        var money = $(".money", p).val();
        if(!money.match(/^\d+(?:\.\d{2})?$/) || money == '0.00'){
            alert("请输入正确的金额。");
            return false;
        }
        
        if (user_list.length == 0){
            alert("请选择消费人员列表。");
            return false;
        }
                      
	    $.post("/fb_book/action",
	           {date: $(".date", p).val(),
	            fb_type: $(".fb_type", p).val(),
	            money: money,
	            master: $(".master", p).val(),
	            user_list: user_list.join(","),
	            comment: $(".comments", p).val()
	           }, function(e){
	               if(e.status != "OK"){alert(e.message);}
	               else{
	                   alert("账本更新成功！");
	                   FB.action_plane.dialog("close");
	               }
	           }, 
	           "json");    
    };
    
    FB.action_plane = $("#fb_plane_record").dialog({
	modal:true,
	autoOpen: false,
	width: 480,
	height: 400,
	buttons: {
	    "保存记录": function() {
	        //submit_callback();
	        FB.submit_fb_record();
	        //$(this).dialog("close"); 
	    },
	    "取消关闭": function() {
	        $(this).dialog("close"); 
	        }
	    }
	});
	$("#fb_plane_record").css({display:'block'});
});