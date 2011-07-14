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
	exit('Access Denied (collect_single_2.php)');
}

if(empty($multi)) {
	$multi = 0;
}
if(empty($jump)) {
	$jump  = 20;
}

if(empty($target) || !ris_int($target)) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'目标站错误\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('请选择目标站');
	}
}
if(empty($read8bid) || !ris_int($read8bid)) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'目标书籍错误\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('请设置目标书籍');
	}
}

$rule = _collect_rule($target);

// general vars
$use = array(
	'read8bid',
);

$address = _collect_get_address($rule['page_info'], $use);
$page_info = _collect_get_page($rule, $address);
if(!$page_info) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'读取目标页面失败 <a href="'.$address.'" target="_blank">'.$address.'</a>\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('读取目标页面失败<br /><a href="'.$address.'" target="_blank">'.$address.'</a>');
	}
}

extract(_collect_get_general_regexp($page_info, $rule, 1));

$b = array(
	'title'   => _collect_get_info_by_tag($page_info, $rule, 'title'),
	'author'  => _collect_get_info_by_tag($page_info, $rule, 'author'),
	'subject' => _collect_get_info_by_tag($page_info, $rule, 'subject'),
	'content' => _collect_get_info_by_tag($page_info, $rule, 'content'),
	'intro'   => _collect_get_info_by_tag($page_info, $rule, 'intro'),
	'cover'   => _collect_get_info_by_tag($page_info, $rule, 'cover'),
);

$b = html_show_decode($b);

$b['intro'] = _collect_replace($rule, $b['intro']);

//$b = html_show(super_trim(rstrip_tags($b)));
$b['title']   = trim(strip_tags($b['title']));
$b['author']  = trim(strip_tags($b['author']));
$b['subject'] = trim(strip_tags($b['subject']));
$b['content'] = trim(strip_tags($b['content']));
$b['intro']   = '    '.trim(strip_tags($b['intro']));
$b['cover']   = $rule['cover_prefix'].trim(strip_tags($b['cover']));


if(!$b['title']) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'无法取得书籍名称\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('无法取得书籍名称');
	}
}else {
	if(in_array($b['title'], _collect_blacklist())) {
		if($multi) {
			_collect_output_js('update_info(\'读取基本信息\', \'该书被列入黑名单\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
			return false;
		}else {
			_collect_error('该书被列入黑名单，若要采集，请从黑名单中删除');
		}
	}
}
if(!$b['author']) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'无法取得书籍作者\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('无法取得书籍作者');
	}
}

$typewriter    = _cookie('typewriter');
$typewriter_id = _cookie('typewriter_id');
if($typewriter && $typewriter_id && $db->selectOne(array(
	'field' => 'uid',
	'from'  => __TAB_USER__,
	'where' => array(
		'uid'      => $typewriter_id,
		'username' => $typewriter,
	),
))) {
	$b['typewriter']    = $typewriter;
	$b['typewriter_id'] = $typewriter_id;
}else {
	$b['typewriter']    = $Global['F_username'];
	$b['typewriter_id'] = $Global['F_uid'];
}

$b = html_show($b, false);

$idx = array_search($b['subject'], $_CATEGORY['NOVEL']['IDX']['subject']);
if($idx) {
	$b['subject'] = $idx;
}else {
	foreach($rule['subject_pos'] as $v) {
		if($b['subject'] != $v['f'])
			continue;

		$b['subject'] = $v['t'];
		break;
	}
}

$idx = array_search($b['content'], $_CATEGORY['NOVEL']['IDX']['content']);
if($idx) {
	$b['content'] = $idx;
}else {
	foreach($rule['content_pos'] as $v) {
		if($b['content'] != $v['f'])
			continue;

		$b['content'] = $v['t'];
		break;
	}
}

$subjects = array();
foreach($_CATEGORY['NOVEL']['IDX']['subject'] as $v => $t) {
	$subjects[] = array(
		'v' => $v,
		't' => $t,
		's' => $v == $b['subject'] ? ' selected="selected"' : '',
	);
}

$contents = array();
foreach($_CATEGORY['NOVEL']['IDX']['content'] as $v => $t) {
	$contents[] = array(
		'v' => $v,
		't' => $t,
		's' => $v == $b['content'] ? ' selected="selected"' : '',
	);
}

$max_priority = 0;
$exist_volumes = $exist_chapters = $chapters_priority = array();

$book = novel_exists($b['title']);
if($book) {
	if($book['state'] != 1) {
		if($multi) {
			_collect_output_js('update_info(\'读取基本信息\', \'书籍状态错误，只能更新正在连载的书籍\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
			return false;
		}else {
			_collect_error('书籍状态错误，只能更新正在连载的书籍');
		}
	}
	$b = html_show($book, false);
	$b['cover'] = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/'.$_SYSTEM['SEO']['htmlpath'].'/'.$b['path'].'/bookface.jpg';
	$novel_exists = true;

	// exist_volumes
	$rows = $db->select(array(
		'field' => 'id, title',
		'from'  => __TAB_NOVEL_VOLUME__,
		'where' => array(
			'book_id' => $book['bid'],
		),
	));
	foreach($rows as $row)
		$exist_volumes[$row['id']] = $row['title'];

	// exist_chapters
	$rows = $db->select(array(
		'field' => 'volume_id, title',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array(
			'book_id' => $book['bid'],
		),
	));
	foreach($rows as $row)
		$exist_chapters[$row['title']] = $row['volume_id'];

	// max_priority
	$row = $db->selectOne(array(
		'field' => 'MAX(priority) AS max_priority',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array(
			'book_id' => $book['bid'],
		),
	));
	$max_priority = $row['max_priority'];
}else {
	$novel_exists = false;
}

$address = _collect_get_address($rule['page_list'], $use);
$page_list = _collect_get_page($rule, $address);
if(!$page_list) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'读取目标页面失败 <a href="'.$address.'" target="_blank">'.$address.'</a>\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('读取目标页面失败<br /><a href="'.$address.'" target="_blank">'.$address.'</a>');
	}
}

$chapters = $original_volumes = $original_chapters = array();

$volumes = _collect_tag_split($page_list, $rule, 'volume');
$priority = 1;
$previnsert = 0;// 上一章是否插入
foreach($volumes as $volume) {
	$volume_title = _collect_get_info_by_tag($volume, $rule, 'volume');
	$chapter_info = _collect_get_info_by_regexp($volume, $rule, 'chapter');

	if(!$volume_title && !$chapter_info)
		continue;

	if(empty($chapter_info[$rule['chapter_title_pos']]))
		continue;

	unset($chapter_info[0]);

	$volume_title = html_show(trim(strip_tags(html_show_decode($volume_title))), false);
	if($volume_title) {
		$original_volumes[$priority] = array(
			'volume_title'  => $volume_title,
			'volume_exists' => array_search($volume_title, $exist_volumes),
		);
	}

	$chapters_info = array();
	foreach($chapter_info[$rule['chapter_title_pos']] as $k => $chapter) {
		$title  = html_show(trim(strip_tags(html_show_decode($chapter))), false);
		$exist  = isset($exist_chapters[$title]);
		if(!$exist && $priority < $max_priority) {// 不存在而且顺序小于现有章节数为插入
			$max_priority++;// 总章节增加
			if($previnsert) {// 上一章也是插入
				$insert = 0;// 不计插入
				$original_chapters[$k - $previnsert]['insert'] = $previnsert + 1;// 第一次插入章节数量增加
				$previnsert++;
			}else {
				$insert = $previnsert = 1;
			}
		}else {
			$insert = $previnsert = 0;
		}

		$chapters_info[] = array(
			'title'    => $title,
			'exists'   => $exist,
		);
		$original_chapters[] = array(
			'title'    => $title,
			'read8cv1' => isset($chapter_info[1][$k]) ? $chapter_info[1][$k] : '',
			'read8cv2' => isset($chapter_info[2][$k]) ? $chapter_info[2][$k] : '',
			'read8cv3' => isset($chapter_info[3][$k]) ? $chapter_info[3][$k] : '',
			'read8cv4' => isset($chapter_info[4][$k]) ? $chapter_info[4][$k] : '',
			'read8cv5' => isset($chapter_info[5][$k]) ? $chapter_info[5][$k] : '',
			'exists'   => $exist,
			'insert'   => $insert,
		);
		$priority++;
	}
	$chapters[] = array(
		'volume'        => $volume_title,
		'volume_exists' => array_search($volume, $exist_volumes),
		'chapter_info'  => $chapters_info,
	);
}
if(!$chapters) {
	if($multi) {
		_collect_output_js('update_info(\'读取基本信息\', \'无法取得章节列表\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Red\');');
		return false;
	}else {
		_collect_error('无法取得章节列表');
	}
}


// collect cache
$chd  = '/*'.LF.
		HT.'target: '.$target.LF.
		HT.'rule:   '.wordwrap(base64_encode(serialize($rule)), 60, LF.HT.HT.HT, true).LF.
		'*/'.LF.
		LF;

$chd .= '$rule = '.var_export($rule, true).';'.LF;
$chd .= LF;
$chd .= '$use = '.var_export($use, true).';'.LF;
$chd .= LF;
foreach($use as $k) {
	$chd .= '$'.str_pad($k, 10).'= '.var_export($$k, true).';'.LF;
}
$chd .= LF;
$chd .= '$b = '.var_export($b, true).';'.LF;
$chd .= LF;
$chd .= '$novel_exists = '.($novel_exists ? var_export($book, true) : 'false').';'.LF;
$chd .= LF;
$chd .= '$original_volumes = '.var_export($original_volumes, true).';'.LF;
$chd .= LF;
$chd .= '$original_chapters = '.var_export($original_chapters, true).';'.LF;
$chd .= LF;
$chd .= '$cc = '.var_export(count($original_chapters), true).';'.LF;
$chd .= LF;
$chd .= '$jump = '.var_export($jump, true).';'.LF;
$chd .= LF;
_collect_data($target, $read8bid, $chd);


if($multi) {
	_collect_output_js('update_info(\'读取基本信息\', \'完成\', \'multi_'.$target.'_'.$read8bid.'_info\', \'Green\');');
	return true;
}else {
	$tpl = new AOKOTpl($pfile.'_step_'.$step);
	$tpl->assign($b);
	$tpl->assign(array(
		'target'   => $target,
		'read8bid' => $read8bid,
		'subjects' => $subjects,
		'contents' => $contents,

		'novel_exists' => $novel_exists,

		'jump'  => _cookie('jump')  ? _cookie('jump')  : 20,
		'clear' => _cookie('clear') ? _cookie('clear') : 0,

		'chapter_list' => $chapters,
	));
	$tpl->output();
}
