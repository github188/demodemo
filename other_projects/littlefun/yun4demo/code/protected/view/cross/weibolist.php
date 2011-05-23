<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">  
<html xmlns="http://www.w3.org/1999/xhtml">  
    <head> 
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"> 
        <title> 登陆</title> 
         <link rel='stylesheet' type='text/css' media='all' href='/css/help.css' /> 
    </head> 
    <body> 
    <div id="main"> 
        <div id="content"> 
            <h1><?php echo $me['name']; ?> 你好~ 欢迎来到YunPHP 新浪微博开放API demo页面 </h1> 
            <strong> <a href="/weibo/source" target="_blank">查看源码</a></strong> 
            <h2>发送新微博</h2> 
            <form action="/weibo/weibolist" method="post"> 
            <input type="text" name="text" style="width:300px" /> 
            &nbsp;<input type="submit" /> 
            </form> 

            <h2>发送图片微博</h2> 
            <form action="/weibo/weibolist" method="post" > 
            <input type="text" name="text" style="width:300px" value="文字内容" /> 
            <input type="text" name="pic" style="width:300px" value="图片url" /> 
            &nbsp;<input type="submit" /> 
            </form> 
	    <h2>好友列表</h2>
            <?php if( is_array( $friends ) ): ?> 
            <?php foreach( $friends as $item ): ?> 
            <div style="padding:10px;margin:5px;border:1px solid #ccc;float:left;"> 
            <img src='<?=$item['profile_image_url'];?>' /> <br/>
            <?=$item['name'];?> 
            </div> 
            <?php endforeach; ?> 
            <?php endif; ?> 
            <div style="clear:both;"></div>
            <div class="important"><?=$flag;?></div> 
            <?php if( is_array( $ms ) ): ?> 
            <?php foreach( $ms as $item ): ?> 
            <div style="padding:10px;margin:5px;border:1px solid #ccc"> 
            <?=$item['text'];?> 
            </div> 
            <?php endforeach; ?> 
            <?php endif; ?> 
        </div> 
    </div> 
    </body> 
</html> 