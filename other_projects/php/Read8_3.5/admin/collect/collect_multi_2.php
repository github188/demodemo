<?php
/**
*
*  Copyright:  (c) 2004 - 2008 [AOKOSOFT] Read8
*  Web Site:   http://www.aokosoft.com http://www.read8.net
*  Powered by: [AOKOSOFT] Read8 Group
*  Support:    http://bbs.read8.net
*  License:    http://www.read8.net/license
*
*/
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_COLLECT') || !isset($PHP_SELF)) {
	exit('Access Denied (collect_multi_2.php)');
}

if(empty($multi) || !in_array($multi, array('regexp', 'text', 'range'))) {
	_collect_error('请选择采集方式');
}
if(empty($jump)) {
	$jump  = 20;
}

cookie('multi', $multi, 31536000);

require ROOT.'admin/'.$pf.'/'.$pfile.'_'.$step.'_'.$multi.'.php';


_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos=1\');');
