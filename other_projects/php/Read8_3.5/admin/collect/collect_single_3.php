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
	exit('Access Denied (collect_single_3.php)');
}

if(empty($multi)) {
	$multi = 0;
}
if(empty($jump)) {
	$jump  = 20;
}
if(empty($clear)) {
	$clear = 0;
}
if(empty($sp)) {
	$sp = 0;
}

if(!$multi) {
	cookie('jump',  $jump,  31536000);
	cookie('clear', $clear, 31536000);
}

if(empty($target) || !ris_int($target)) {
	if($multi) {
		_collect_output_js('update_info(\'开始采集\', \'目标站错误\', \'multi_'.$target.'_'.$read8bid.'_start\', \'Red\');');
		return false;
	}else {
		_collect_error('请选择目标站');
	}
}
if(empty($read8bid) || !ris_int($read8bid)) {
	if($multi) {
		_collect_output_js('update_info(\'开始采集\', \'目标书籍错误\', \'multi_'.$target.'_'.$read8bid.'_start\', \'Red\');');
		return false;
	}else {
		_collect_error('请设置目标书籍');
	}
}

if(!(include _collect_data($target, $read8bid))) {
	if($multi) {
		_collect_output_js('update_info(\'开始采集\', \'读取采集缓存失败\', \'multi_'.$target.'_'.$read8bid.'_start\', \'Red\');');
		return false;
	}else {
		_collect_error('读取采集缓存失败');
	}
}


if($novel_exists) {
	if($clear && !$sp) {
		novel_clear($novel_exists['bid']);
	}
}elseif(!$sp) {
	$postfields = array(
		'title',
		'author',
		'typewriter',
		'subject',
		'content',
		'cover',
		'intro',
	);
	foreach($postfields as $v) {
		if(isset($_POST[$v]) && trim($_POST[$v]) != $b[$v]) {
			if($v == 'typewriter') {
				if($u = $db->selectOne(array(
					'field' => 'uid',
					'from'  => __TAB_USER__,
					'where' => array(
						'username' => trim($_POST[$v]),
					),
				))) {
					$b[$v]       = trim($_POST[$v]);
					$b[$v.'_id'] = $u['uid'];
					cookie($v,       $b[$v],       31536000);
					cookie($v.'_id', $b[$v.'_id'], 31536000);
				}else {
					// display error messages
				}
			}else {
				$b[$v] = trim($_POST[$v]);
			}
		}
	}

	$cover = rforr($b['cover'], false);
	if($cover) {
		$f = 'data/cache/cover_'.rrand(20);
		if(!rfow(ROOT.$f, $cover))
			$f = false;
	}else {
		$f = false;
	}

	include _class('subject', 'novel');
	include _class('content', 'novel');

	if(!ris_int($b['subject']) || !isset($_CATEGORY['NOVEL']['IDX']['subject'][$b['subject']]))
		$b['subject'] = $_CATEGORY['NOVEL']['subject'][0]['id'];

	if(!ris_int($b['content']) || !isset($_CATEGORY['NOVEL']['IDX']['content'][$b['content']]))
		$b['content'] = $_CATEGORY['NOVEL']['content'][0]['id'];

	$bid = novel_create($b, $f);

	if($f)
		unlink(ROOT.$f);

	if($bid) {
		$novel_exists = novel_exists($bid);
	}else {
		if($multi) {
			_collect_output_js('update_info(\'开始采集\', \'新建书籍失败\', \'multi_'.$target.'_'.$read8bid.'_start\', \'Red\');');
			return false;
		}else {
			_collect_error('新建书籍失败<br />'.$db->error());
		}
	}
}else {
	$novel_exists = novel_exists($b['title']);
}


$bid  = $novel_exists['bid'];
$path = novel_url($novel_exists, 0);

if(!$multi) {
	_collect_js();

	ob_end_flush();
	ob_implicit_flush();
	$tpl = new AOKOTpl($pfile.'_step_'.$step);
	$tpl->output();

}

_collect_output_js('create_progress(false, \'状态\');');

if($multi) {
	_collect_output_js('display_pause(\'在采集过程中您可以随时 <a href="?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos='.$multi_pos.'&multi_ppos=1&sp='.($cc + 1).'" style="color: Red;">点击这里暂停采集</a>\');');
}else {
	_collect_output_js('display_pause(\'在采集过程中您可以随时 <a href="?ptype='.$ptype.'&pfile='.$pfile.'&step=4&target='.$target.'&read8bid='.$read8bid.'&multi='.$multi.'" style="color: Red;">点击这里暂停采集</a>\');');
}

for($i = 1; $i <= 5; $i++)
	$use[] = 'read8cv'.$i;


for($i = $sp; $i < $sp + $jump; $i++) {
	$priority = $i + 1;

	set_time_limit(120);

	_collect_output_js('refresh_progress(false, '.(round($i / $cc * 100)).');');

	if(isset($original_volumes[$priority])) {
		_collect_output_js('volume_info(\''.$original_volumes[$priority]['volume_title'].'\', '.intval($original_volumes[$priority]['volume_exists'] && !$clear).');');
		if($original_volumes[$priority]['volume_exists'] && !$clear) {
			// update volume
			$db->update(array(
				'title'            => $original_volumes[$priority]['volume_title'],
				'chapter_priority' => $priority,
			), array(
				'book_id'          => $bid,
				'id'               => $original_volumes[$priority]['volume_exists'],
			), __TAB_NOVEL_VOLUME__);
		}else {
			// insert volume
			$db->insert(array(
				'book_id'          => $bid,
				'title'            => $original_volumes[$priority]['volume_title'],
				'chapter_priority' => $priority,
			), __TAB_NOVEL_VOLUME__);
		}
	}

	if(!isset($original_chapters[$i])) {
		// finished
		if($multi) {
			_collect_output_js('display_info(\'整理数据\', \'tidy\');');

			novel_volume_tidy($bid);
			_collect_output_js('update_info(\'整理数据\', \'生成目录\', \'tidy\', \'Blue\');');

			novel_rebuild_index($bid);
			_collect_output_js('update_info(\'整理数据\', \'完成\', \'tidy\', \'Green\');');

			_collect_output_js('display_info(\'清除缓存\', \'cache\');');
			unlink(_collect_data($target, $read8bid));
			_collect_output_js('update_info(\'清除缓存\', \'完成\', \'cache\', \'Green\');');

			if(empty($multi_ppos)) {
				_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos='.($multi_pos + 1).'\');');
			}else {
				_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos=0\');');
			}
		}else {
			_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=4&target='.$target.'&read8bid='.$read8bid.'&multi='.$multi.'\');');
		}
		return true;
	}

	extract($original_chapters[$i]);

	if($insert && !$clear) {
		_collect_output_js('insert_chapter('.$insert.');');
		novel_chapter_rename($priority, ROOT.$path, true, $insert);
		$db->execUpBySql("UPDATE ".__TAB_NOVEL_CHAPTER__." SET priority = priority + $insert WHERE priority >= '$priority' AND `book_id` = '$bid'");
	}

	_collect_output_js('display_info(\''.$title.'\', '.$priority.');');

	if($exists && !$clear) {
		_collect_output_js('update_info(\''.$title.'\', \'跳过\', '.$priority.', \'Gray\');');
		continue;
	}

	// init info
	_collect_output_js('update_info(\''.$title.'\', \'读取页面\', '.$priority.', \'Blue\');');

	// get address
	$address = _collect_get_address($rule['page_read'], $use);
	// get page
	$page_read = _collect_get_page($rule, $address);
	if(!$page_read) {
		if($multi) {
			_collect_output_js('update_info(\'开始采集\', \'读取目标页面失败 <a href="'.$address.'" target="_blank">'.$address.'</a>\', \'multi_'.$target.'_'.$read8bid.'_start\', \'Red\');');
			return false;
		}else {
			_collect_error('读取目标页面失败<br /><a href="'.$address.'" target="_blank">'.$address.'</a>');
		}
	}

	if($rule['page_final']) {
		_collect_output_js('update_info(\''.$title.'\', \'读取内容页面\', '.$priority.', \'Blue\');');

		$us2 = $use;
		$us2[] = 'read8g3v1';
		$us2[] = 'read8g3v2';
		extract(_collect_get_general_regexp($page_read, $rule, 3, 2));

		$address = _collect_get_address($rule['page_final'], $us2);

		$page_read = _collect_get_page($rule, $address);
		if(!$page_read) {
			if($multi) {
			_collect_output_js('update_info(\'开始采集\', \'读取目标页面失败 <a href="'.$address.'" target="_blank">'.$address.'</a>\', \'multi_'.$target.'_'.$read8bid.'_start\', \'Red\');');
			return false;
			}else {
				_collect_error('读取目标页面失败<br /><a href="'.$address.'" target="_blank">'.$address.'</a>');
			}
		}
	}

	_collect_output_js('update_info(\''.$title.'\', \'分析内容\', '.$priority.', \'Blue\');');
	$content = rtrim(strip_tags(_collect_replace($rule, _collect_get_info_by_tag($page_read, $rule, 'text')), '<img>'));

	$images = array();
	if(preg_match_all('|<img [^>]*src\s*=\s*["\']?([^"\' ]+)[ "\'][^>]+>|iU', $content, $matches)) {
		foreach($matches[0] as $k => $v) {
			$images[$k] = $matches[1][$k];
			$content = str_replace($v, '[img]'.($k + 1).'[/img]', $content);
		}
	}

	$content = html_show($content);

	_collect_output_js('update_info(\''.$title.'\', \'写入数据库\', '.$priority.', \'Blue\');');
	$abschapterid = $db->insert(array(//章节绝对id
		'book_id'     => $bid,
		'title'       => $title,
		'priority'    => $priority,
		'dateline'    => TIMESTAMP,
		'words'       => rmb_strlen($content, true),
		'hash'        => md5($title),
	), __TAB_NOVEL_CHAPTER__);

	if($images) {
		if($rule['image']) {
			require_once ROOT.'include/image.func.php';
			include _config('upload');
			$f = ROOT.'data/cache/collect_image';

			foreach($images as $k => $v) {
				_collect_output_js('update_info(\''.$title.'\', \'下载图片 '.$k.'\', '.$priority.', \'Blue\');');
				rfow($f, _collect_get_page($rule, $rule['image_prefix'].$v, true));
				$image_info = GetImageType($f);
				if(!$image_info) {
					$content = str_replace('[img]'.($k + 1).'[/img]', '', $content);
					continue;
				}

				$aid = $db->insert(array(
					'book_id'    => $bid,
					'chapter_id' => $abschapterid,
					'file'       => $image_info[0],
					'dateline'   => TIMESTAMP,
				), __TAB_NOVEL_ATTACH__);

				$image = ROOT.$path.'/'.$aid.'.'.$image_info[0];
				r_cp($f, $image);

				if('jpg' == $image_info[0] || 'jpeg' == $image_info[0]) {
					if($_SYSTEM['UPLOAD']['watermark']) {
						if($_SYSTEM['UPLOAD']['watermark_type']) {
							ImageWaterMarkText($image, $_SYSTEM['UPLOAD']['watermark'], $_SYSTEM['UPLOAD']['watermark_text'], $_SYSTEM['UPLOAD']['watermark_color']);
						}else {
							ImageWaterMarkImage($image, $_SYSTEM['UPLOAD']['watermark'], ROOT.$_SYSTEM['UPLOAD']['watermark_image'], $_SYSTEM['UPLOAD']['watermark_alpha']);
						}
					}
				}

				$content = str_replace('[img]'.($k + 1).'[/img]', '<img src="'.$aid.'.'.$image_info[0].'" alt="" />', $content);
			}
		}else {
			foreach($images as $k => $v) {
				$content = str_replace('[img]'.($k + 1).'[/img]', '<img src="'.$rule['image_prefix'].$v.'" alt="" />', $content);
			}
		}
	}

	_collect_output_js('update_info(\''.$title.'\', \'写入文件\', '.$priority.', \'Blue\');');
	$tpl = new AOKOTpl('../../html/template/content');
	$tpl->assign(array(
		'chapter_title' => stripslashes($title),
		'book_title'    => $novel_exists['title'],
		'C_authorname'  => $novel_exists['author'],
		'bid'           => $bid,
		'cid'           => $abschapterid,
		'C_lchapterid'  => $priority - 1,
		'C_nchapterid'  => $priority + 1,
		'C_tchapterid'  => $priority,
		'C_content'     => $content,
	));
	rfow(ROOT.novel_url($novel_exists, $priority), $tpl->output(1));

	_collect_output_js('update_info(\''.$title.'\', \'完成\', '.$priority.', \'Green\');');
}

if(isset($abschapterid)) {
	$db->update(array(
		'lastupdate'  => TIMESTAMP,
		'lastchapter' => $abschapterid,
	), array(
		'id'          => $bid,
	), __TAB_NOVEL__);
}

if($multi) {
	_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos='.$multi_pos.'&sp='.($sp + $jump).'\');');
}else {
	_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&target='.$target.'&read8bid='.$read8bid.'&multi='.$multi.'&jump='.$jump.'&clear='.$clear.'&sp='.($sp + $jump).'\');');
}

return true;
