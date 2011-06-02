<div class="dongtai">
    <div class="dongtai_scroll">
	    <ul>
				<li> 
					<span><strong id='s1'>金烂烂 生成了他的后宫图</strong><br/>3秒前</span>
                    <span><strong id='s2'>金烂烂 生成了他的后宫图</strong><br/>4秒前</span> 
				</li> 
				<li> 
					<span><strong id='s3'>金烂烂 生成了他的后宫图</strong><br/>5秒前</span>
                    <span><strong id='s4'>金烂烂 生成了他的后宫图</strong><br/>6秒前</span> 
				</li> 
				<li> 
					<span><strong id='s5'>金烂烂 生成了他的后宫图</strong><br/>1秒前</span>
                    <span><strong id='s6'>金烂烂 生成了他的后宫图</strong><br/>4秒前</span> 
				</li> 
	    </ul>
    </div>
</div>
<script >
$(document).ready(function(){
    jQuery.getJSON("http://littlefun.sinaapp.com/gong/last_gong_jsonp/?callback=?",
    function(data){
        $('#s1').html(data[0]['user_profile']['name'] + " 生成了他的后宫图");
        $('#s2').html(data[1]['user_profile']['name'] + " 生成了他的后宫图");
        $('#s3').html(data[2]['user_profile']['name'] + " 生成了他的后宫图");
        $('#s4').html(data[3]['user_profile']['name'] + " 生成了他的后宫图");
        $('#s5').html(data[4]['user_profile']['name'] + " 生成了他的后宫图");
        $('#s6').html(data[5]['user_profile']['name'] + " 生成了他的后宫图");

        $(".dongtai_scroll").jCarouselLite({
            speed:500,
            scroll:1,
            auto:4000,
            visible:1,
            vertical:true
        });
    },
    'json');
});
</script>
