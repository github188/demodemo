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
if(!defined('IN_Read8')) {
	exit('Access Denied (book.func.php)');
}

/*
	书籍处理函数库
*/


function _novel($sc = 0, $cc = 0, $finished = 0, $initial = '', $words = 0, $orderbycreate = false){
	global $db, $page;

	if($page < 1)
		$page = 1;

	$sql = array(
		'field' => 'n.id          AS bid,
					n.title,
					n.pinyin,
					n.initial     AS bp_i,
					n.author,
					n.author_id,
					n.subject     AS bp_t,
					n.content,
					n.dateline    AS bp_d,
					n.words       AS bp_w,
					n.path,
					n.lastupdate,
					n.lastchapter,
					np.title      AS chapter_title,
					np.priority',
		'from'  => $db->table(__TAB_NOVEL__, 'n'),
		'join'  => array('table' => $db->table(__TAB_NOVEL_CHAPTER__, 'np'), 'on' => 'n.id = np.book_id AND n.lastchapter = np.id'),
		'order' => 'n.'.($orderbycreate ? 'dateline' : 'lastupdate').' DESC',
		'limit' => 20,
		'page'  => $page,
		'filter'=> '_novel_callback',
		'counter'=>1,
	);

	if($sc)
		$sql['where']['n.subject'] = $sc;

	if($cc)
		$sql['where']['n.content'] = $cc;

	if($finished)
		$sql['where']['n.state']   = 3;

	if($initial)
		$sql['where']['n.initial'] = $initial;

	if($words)
		$sql['where']['n.words']   = $words;

	return $db->select($sql);
}
function _novel_callback($book){
	$book['InfoURL']    = novel_url($book);
	$book['ChapterURL'] = novel_url($book, $book['priority']);
	$book['AuthorURL']  = author_url($book['author_id']);
	$book['subject']    = $book['bp_t'];

	return html_show(convert_novel_classes($book), false);
}



function is_story($bsid){
	global $_CATEGORY;

	return $_CATEGORY['INDEX']['subject'][$bsid]['description'];
}


function novel_get_id_by_name($title){
	global $db;

	$b = $db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_NOVEL__,
		'where' => array('title' => $title),
	));
	if($b)
		return $b['id'];

	return false;
}
function novel_get_chapter_title_by_priority($bid, $priority){
	global $db;

	$c = $db->selectOne(array(
		'field' => 'title',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array(
			'book_id'  => $bid,
			'priority' => $priority,
		),
	));
	if($c)
		return $c['title'];

	return false;
}

function novel_get_chapter_priority_by_id($bid, $cid){
	global $db;

	$c = $db->selectOne(array(
		'field' => 'priority',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array(
			'book_id' => $bid,
			'id'      => $cid,
		),
	));
	if($c)
		return $c['priority'];

	return false;
}


function novel_clear($bid){
	global $db, $Global;

	if(!($bookinfo = novel_exists($bid)))
		back('查无此书');

	// TODO: 日志记录

	// 删除静态文件
	$abspath = ROOT.novel_url($bookinfo, 0);
	$filelist = rlistdir($abspath);
	foreach($filelist as $v)
		if(substr($v, -5) == '.html')
			unlink($abspath.'/'.$val);

	//删除附件
	$attachs = $db->select(array(
		'field' => 'file',
		'from'  => __TAB_NOVEL_ATTACH__,
		'where' => array('book_id' => $bid),
	));
	foreach($attachs as $attach)
		unlink(ROOT."$Global[attach_dir]/$attach[file]");

	//删除数据库
	$sql = array(
		__TAB_NOVEL_CHAPTER__,
		__TAB_NOVEL_VOLUME__,
		__TAB_NOVEL_ATTACH__,
	);
	foreach($sql as $val)
		$db->delete(array(
			'book_id' => $bid,
		), $val);

	//UpdateCache('bookmain');
}



function _novel_comment($bid, $cid = false){
	global $db;

	if(!ris_int($bid))
		return false;

	if(!($book = novel_exists($bid)))
		return false;

	$sql = array(
		'field' => 'nc.id,
					nc.book_id    AS bid,
					nc.chapter_id AS cid,
					nc.uid,
					nc.username,
					nc.title,
					nc.content,
					nc.ip,
					nc.digest,
					nc.sticky,
					nc.highlight,
					nc.dateline,
					n.title       AS book_title,
					n.pinyin,
					n.path',
		'from'  => $db->table(__TAB_NOVEL_COMMENT__, 'nc'),
		'join'  => array('table' => $db->table(__TAB_NOVEL__, 'n'), 'on' => 'n.id = nc.book_id'),
		'where' => array(
			'nc.book_id' => $bid,
			'nc.active'  => 1,
		),
		'order' => 'nc.dateline DESC',
		'filter'=> 'comment_filter',
	);

	if(false !== $cid) {
		if(!ris_int($cid))
			return false;

		if(!($chapter = novel_chapter_exists($bid, $cid)))
			return false;

		$sql['where']['chapter_id'] = $cid;
	}

	return $db->select($sql);
}

function comment_filter($comment){
	if(isset($comment['path'])) {
		$comment['InfoURL']    = novel_url($comment);
		$comment['ChapterURL'] = novel_url($comment, $comment['cid']);
	}else {
		$comment['InfoURL']    = story_url($comment);
	}

	return html_show($comment);
}


function novel_chapter_delete($bid, $cid, $abspath = false){
	global $db;

	// TODO: 日志

	if(!$abspath) {
		if(!($bookinfo = novel_exists($bid)))
			return false;

		$abspath = ROOT.novel_url($bookinfo, 0);
	}


	if(!($chapter_info = novel_chapter_exists($bid, $cid)))
		return false;

	$priority = $chapter_info['priority'];

	$rows = $db->select(array(
		'field' => 'id, file',
		'from'  => __TAB_NOVEL_ATTACH__,
		'where' => array(
			'chapter_id' => $cid,
			'book_id'    => $bid,
		),
	));
	foreach($rows as $row) {
		if(!unlink($abspath.'/'.$row['id'].'.'.$row['file']))
			trigger_error('删除附件 '.str_replace(ROOT, '', $abspath.'/'.$row['id'].'.'.$row['file']).' 失败，请检查文件是否存在或是否有删除权限', E_USER_NOTICE);

		$db->delete(array(
			'id' => $row['id'],
		), __TAB_NOVEL_ATTACH__);
	}

	$db->delete(array(
		'book_id' => $bid,
		'id'      => $cid,
	), __TAB_NOVEL_CHAPTER__);

	//UpdateBookcid($bid);//更新书籍的最新章节id

	unlink($abspath.'/'.$priority.'.html');

	//更新该章节后的所有章节的静态页面
	novel_chapter_rename($priority + 1, $abspath, false);

	//修改数据库
	$sql="UPDATE ".__TAB_NOVEL_CHAPTER__." SET priority = priority - 1 WHERE priority > $priority AND `book_id` = '$bid'";
	$db->execUpBySql($sql);
	$sql="UPDATE ".__TAB_NOVEL_VOLUME__." SET chapter_priority = chapter_priority - 1 WHERE chapter_priority > $priority AND `book_id` = '$bid'";
	$db->execUpBySql($sql);
}

function novel_chapter_exists($bid, $cid){
	global $db;

	return $db->selectOne(array(
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array(
			'book_id' => $bid,
			'id'      => $cid,
		),
	));
}

function _novel_chapter($bid, $priority = 0){
	global $db;

	if(!ris_int($bid))
		return false;

	if(!$book = novel_exists($bid))
		return false;

	//获得volume array
	$rows = $db->select(array(
		'field' => 'title, chapter_priority',
		'from'  => __TAB_NOVEL_VOLUME__,
		'where' => array('book_id' => $bid),
		'order' => 'chapter_priority',
	));
	foreach($rows as $row)
		$volumes[$row['chapter_priority']] = $row['title'];


	//章节生成
	$rows = $db->select(array(
		'field' => 'title, priority, vip',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array('book_id' => $bid),
		'order' => 'priority ASC',
	));
	$i = 0;
	$chapters = array();
	foreach($rows as $row) {
		//生成书籍目录
		if(!empty($volumes[$row['priority']]))
			$chapters[$i]['volume_title'] = $volumes[$row['priority']];

		$chapters[$i]['vip']      = $row['vip'];
		$chapters[$i]['priority'] = $row['priority'];
		$chapters[$i]['title']    = $row['title'];

		$i++;
	}

	$priority = $row['priority'] + 1;

	return $chapters;
}

function novel_rebuild_index($bid){
	global $db;

	if(!ris_int($bid))
		return false;

	if(!$book = html_show(novel_exists($bid), false))
		return false;

	$chapters = html_show(_novel_chapter($bid), false);

	//替换index.html
	$tpl = new AOKOTpl('../../html/template/index');
	$tpl->assign(array(
		'C_booktitle'  => $book['title'],
		'C_authorname' => $book['author'],
		'C_bookid'     => $bid,
		'chapterlist'  => $chapters,
	));

	include _cache('ads');
	$tpl->assign($_CACHE['ads']);

	rfow(ROOT.novel_url($book, 'index'), $tpl->output(1));

	$priority = 0;
	if($row = $db->selectOne(array(//最大章节id
		'field' => 'MAX(priority) AS priority',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array('book_id' => $bid),
	))) {
		$priority = $row['priority'];
	}
	$priority++;//新章节的顺序id

	//生成最后章节的跳转页
	r_cp(ROOT.novel_url($book, 'index'), ROOT.novel_url($book, $priority));
}

function novel_volume_tidy($bid){
	global $db;

	if(!ris_int($bid))
		return false;

	$vols = $db->select(array(
		'field' => 'id, chapter_priority',
		'from'  => __TAB_NOVEL_VOLUME__,
		'where' => array('book_id' => $bid),
		'order' => 'chapter_priority ASC',
	));
	foreach($vols as $row) {
		$sql="UPDATE ".__TAB_NOVEL_CHAPTER__." SET volume_id = '".$row['id']."' WHERE book_id = $bid AND priority > ".($row['chapter_priority'] - 1);
		$db->execUpBySql($sql);
	}
}

function novel_exists($bid){
	global $db;

	if(ris_int($bid)) {
		$where['id']    = $bid;
	}else {
		$where['title'] = $bid;
	}

	return $db->selectOne(array(
		'field' => '*, id AS bid, subject AS subject_id, content AS content_id',
		'from'  => __TAB_NOVEL__,
		'where' => $where,
	));
}


function get_novel_by_id($bid){
	if(!$bid || !ris_int($bid))
		return false;

	global $db;
	return $db->selectOne(array(
		'field' => '*, id AS bid',
		'from'  => __TAB_NOVEL__,
		'where' => array(
			'id' => $bid,
		),
	));
}


function novel_ext($bid){
	global $db;

	return $db->selectOne(array(
		'field' => 'click_today + click_total     AS click,
					vote_today + vote_total       AS vote,
					rate_total                    AS rate,
					favo_today + favo_month       AS favorite,
					commend_today + commend_total AS commend,
					digg_today + digg_total       AS digg,
					bury_today + bury_total       AS bury',
		'from'  => __TAB_NOVEL_EXT__,
		'where' => array('book_id' => $bid),
	));
}

function novel_create($b, $cover = false){
	global $db, $_SYSTEM;

	// title, author, author_id, subject, content, style, role_lead, role_support, license, description, tag, intro
	if(isset($b['id']))
		unset($b['id']);

	if(isset($b['sample']))
		unset($b['sample']);

	if(isset($b['verifytime']))
		unset($b['verifytime']);

	if(isset($b['book_id']))
		unset($b['book_id']);

	if(isset($b['cover']))
		unset($b['cover']);

	$b['pinyin']     = pinyin_unique(pinyin($b['title']));
	$b['initial']    = pinyin($b['title'], true);
	$b['dateline']   = TIMESTAMP;
	$b['lastupdate'] = TIMESTAMP;
	$b['hash']       = md5($b['title']);

	$path = '';

	//	可能的元素
	//		首字母			bp_i
	//		日期			bp_d
	//		书名字数		bp_w
	//		分类			bp_t
	switch($_SYSTEM['SEO']['htmlsubdir'][0]) {
		case 'i':
			$path .= strtoupper($b['initial']);
			break;
		case 'd':
			$path .= rdate(TIMESTAMP, $_SYSTEM['SEO']['htmlsubdirdateformat']);
			break;
		case 'w':
			$path .= rmb_strlen($b['title'], true);
			break;
		case 't':
			$path .= $b['subject'];
			break;
	}
	$path .= '/';
	switch($_SYSTEM['SEO']['htmlsubdir'][1]) {
		case 'i':
			$path .= strtoupper($b['initial']);
			break;
		case 'd':
			$path .= rdate(TIMESTAMP, $_SYSTEM['SEO']['htmlsubdirdateformat']);
			break;
		case 'w':
			$path .= rmb_strlen($b['title'], true);
			break;
		case 't':
			$path .= $b['subject'];
			break;
	}

	$path .= '/'.$b['pinyin'];
	$b['path'] = $path;

	$bid = $db->insert($b, __TAB_NOVEL__);

	// 点击 投票 评分 收藏 初始化
	$db->insert(array(
		'book_id' => $bid,
	), __TAB_NOVEL_EXT__);

	// 创建目录
	rmkdir($_SYSTEM['SEO']['htmlpath'].'/'.$path);

	if($cover)
		r_cp(ROOT.$cover, ROOT.$_SYSTEM['SEO']['htmlpath'].'/'.$path.'/bookface.jpg');

	return $bid;
}


function commend($commend, $subject = false, $story = false){
	global $db;

	$where['nc.commend'] = $commend;
	if($subject && ris_int($subject))
		$where['n.subject'] = $subject;

	return $db->select(array(
		'field' => 'n.*, n.id AS bid',
		'from'  => $db->table($story ? __TAB_STORY_COMMEND__ : __TAB_NOVEL_COMMEND__, 'nc'),
		'join'  => array('table' => $db->table($story ? __TAB_STORY__ : __TAB_NOVEL__, 'n'), 'on' => 'nc.book_id = n.id'),
		'where' => $where,
		'filter'=> 'commend_callback',
	));
}

function commend_callback($row){
	if(isset($row['path'])) {
		$row['InfoURL']  = novel_url($row);
		$row['HTMLPATH'] = novel_url($row, 0);
	}else {
		$row['InfoURL']  = story_url($row);
	}

	return $row;
}




function _story($sc = 0, $cc = 0){
	global $db, $page;

	if($page < 1)
		$page = 1;

	$sql = array(
		'field' => '*, id AS bid',
		'from'  => __TAB_STORY__,
		'where' => array(
			'state' => 3,
		),
		'order' => 'lastupdate DESC',
		'limit' => 20,
		'page'  => $page,
		'filter'=> '_story_callback',
		'counter'=>1,
	);

	if($sc)
		$sql['where']['subject'] = $sc;

	if($cc)
		$sql['where']['content'] = $cc;

	return $db->select($sql);
}
function _story_callback($book){
	$book['URL']    = story_url($book);

	return convert_story_classes(html_show($book));
}


function story_exists($bid){
	global $db;

	if(!$bid || !ris_int($bid))
		return false;

	$where['id']    = $bid;

	return $db->selectOne(array(
		'field' => '*, id AS bid',
		'from'  => __TAB_STORY__,
		'where' => $where,
	));
}

function story_ext($bid){
	global $db;

	return $db->selectOne(array(
		'field' => 'click_today + click_total     AS click,
					vote_today + vote_total       AS vote,
					rate_total                    AS rate,
					favo_today + favo_month       AS favorite,
					commend_today + commend_total AS commend,
					digg_today + digg_total       AS digg,
					bury_today + bury_total       AS bury',
		'from'  => __TAB_STORY_EXT__,
		'where' => array('book_id' => $bid),
	));
}

function _story_comment($bid){
	global $db;

	if(!ris_int($bid))
		return false;

	if(!($book = story_exists($bid)))
		return false;

	return $db->select(array(
		'field' => 'nc.id,
					nc.book_id    AS bid,
					nc.uid,
					nc.username,
					nc.title,
					nc.content,
					nc.ip,
					nc.digest,
					nc.sticky,
					nc.highlight,
					nc.dateline,
					n.title       AS book_title',
		'from'  => $db->table(__TAB_STORY_COMMENT__, 'nc'),
		'join'  => array('table' => $db->table(__TAB_STORY__, 'n'), 'on' => 'n.id = nc.book_id'),
		'where' => array(
			'nc.book_id' => $bid,
			'nc.active'  => 1,
		),
		'order' => 'nc.dateline DESC',
		'filter'=> 'comment_filter',
	));
}




function get_novel_by_author_id($aid){
	if(!$aid || !ris_int($aid))
		return false;

	global $db;
	return $db->select(array(
		'field' => 'id, title, description, subject, content, style, license, state, dateline, lastupdate, words',
		'from'  => __TAB_NOVEL__,
		'where' => array('author_id' => $aid),
		'filter'=> 'convert_novel_classes',
	));
}





function novel_content($filename){
	$content  = rfor($filename);
	$oldstart = '<!--- r8artlbegin --->';
	$oldend   = '<!--- r8artlend --->';
	$startpos = '<!-- r8artlbegin -->';
	$endpos   = '<!-- r8artlend -->';

	if(false === strpos($content, $oldstart))
		return rsubstr($content, $startpos, $endpos);

	return rsubstr($content, $oldstart, $oldend);
}



function new_attach($attachs, $book, $cid){
	global $db, $_SYSTEM, $format;

	$img = '';
	foreach($attachs as $attach) {
		if(upload_status($attach['error'], true))
			continue;

		if($attach['size'] > $_SYSTEM['UPLOAD']['size'])
			continue;

		$fext = rfilename($attach['name'], true);
		if(!in_array($fext, $format))
			continue;

		$aid = $db->insert(array(
			'book_id'    => $book['bid'],
			'chapter_id' => $cid,
			'file'       => $fext,
			'dateline'   => TIMESTAMP,
		), __TAB_NOVEL_ATTACH__);

		$file = $aid.'.'.$fext;
		$path = ROOT.novel_url($book, 0).'/'.$file;

		rmove_uploaded_file($attach['tmp_name'], $path);
		if('jpg' == $fext || 'jpeg' == $fext) {
			if($_SYSTEM['UPLOAD']['watermark']) {
				if($_SYSTEM['UPLOAD']['watermark_type']) {
					ImageWaterMarkText($path, $_SYSTEM['UPLOAD']['watermark'], $_SYSTEM['UPLOAD']['watermark_text'], $_SYSTEM['UPLOAD']['watermark_color']);
				}else {
					ImageWaterMarkImage($path, $_SYSTEM['UPLOAD']['watermark'], ROOT.$_SYSTEM['UPLOAD']['watermark_image'], $_SYSTEM['UPLOAD']['watermark_alpha']);
				}
			}
		}
		$img .= '<img src="'.$file.'" alt="" />'.LF;
	}
	return $img;
}

