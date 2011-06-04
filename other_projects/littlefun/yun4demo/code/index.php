<?php
        session_start();
	$begin_time = microtime();
	define('DEBUG',TRUE);
	sae_set_display_errors(True);
	
	//xhprof —— facebook 搞的一个测试php性能的扩展
	define('XHPROF',False);
	if(XHPROF){
		sae_xhprof_start();
	}
	//定义YunPHP的类库的路径
	$dir_yunphp = 'YunPHP';
	$dir_protected = 'protected';
	
	//决定是否让系统使用memcache,
	define('USE_MEM',TRUE);
	
	//定义量目录，DOCROOT为项目的目录，YUNPHP为系统目录
	define('WEBROOT',realpath(dirname(__FILE__)));
	define('YUNPHP',WEBROOT.'/'.$dir_yunphp.'/');
	define('DOCROOT',WEBROOT.'/'.$dir_protected.'/');
	define('SITE_ROOT',realpath(dirname(__FILE__)).'/');
	
	
	define('RUNTIME','saemc:/');
	require_once 'YunPHP/main.php';
	if(XHPROF){
		sae_xhprof_end();
	}
	
	$end_time = microtime();
	$escaped_time = $end_time - $begin_time;
//	echo "escaped time $escaped_time";
	