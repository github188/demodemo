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
	exit('Access Denied (cache_cache.php)');
}

$_CACHE['cache'] = array (
  'zdjb' => 
  array (
    'id' => 'zdjb',
    'title' => '总点击榜',
    'sql' => 'SELECT n.id AS bid, n.title, ne.click_total, n.path
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_external` AS ne
ON n.id = ne.book_id
ORDER BY ne.click_total DESC
LIMIT 10',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'ztjb' => 
  array (
    'id' => 'ztjb',
    'title' => '总推荐榜',
    'sql' => 'SELECT n.id AS bid, n.title, ne.commend_total, n.path
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_external` AS ne
ON n.id = ne.book_id
ORDER BY ne.commend_total DESC
LIMIT 10
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'zscb' => 
  array (
    'id' => 'zscb',
    'title' => '总收藏榜',
    'sql' => 'SELECT n.id AS bid, n.title, ne.favo_total, n.path
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_external` AS ne
ON n.id = ne.book_id
ORDER BY ne.favo_total DESC
LIMIT 10
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'ydjb' => 
  array (
    'id' => 'ydjb',
    'title' => '月点击榜',
    'sql' => 'SELECT n.id AS bid, n.title, ne.click_month, n.path
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_external` AS ne
ON n.id = ne.book_id
ORDER BY ne.click_month DESC
LIMIT 10
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'ytjb' => 
  array (
    'id' => 'ytjb',
    'title' => '月推荐榜',
    'sql' => 'SELECT n.id AS bid, n.title, ne.commend_month, n.path
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_external` AS ne
ON n.id = ne.book_id
ORDER BY ne.commend_month DESC
LIMIT 10
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'qltjb' => 
  array (
    'id' => 'qltjb',
    'title' => '强力推荐榜',
    'sql' => 'SELECT n.id AS bid, n.title, n.path, nc.dateline
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_commend` AS nc
ON n.id = nc.book_id AND nc.commend = 2
ORDER BY nc.dateline DESC
LIMIT 3
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'bjtjl' => 
  array (
    'id' => 'bjtjl',
    'title' => '编辑推荐栏',
    'sql' => 'SELECT n.id AS bid, n.title, n.path, nc.dateline
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_commend` AS nc
ON n.id = nc.book_id AND nc.commend = 3
ORDER BY nc.dateline DESC
LIMIT 3
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'sflbt' => 
  array (
    'id' => 'sflbt',
    'title' => '十个分类标题',
    'sql' => 'SELECT title ,id As subject_id
FROM {db_prefix}read8_class_novel
WHERE category = \'subject\' AND active = 1 AND id IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
ORDER BY id ASC
',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'mgl' => 
  array (
    'id' => 'mgl',
    'title' => '每个分类内容 ',
    'sql' => 'SELECT n.title, n.author, n.intro, n.subject,n.subject As subject_id, n.id As bid, n.path
FROM `{db_prefix}read8_novel` As n
WHERE (SELECT Count( * )
FROM `{db_prefix}read8_novel`
WHERE subject = n.subject AND id < n.id) <2 And n.subject In(1,2,3,4,5,6,7,8,9,10)
',
    'twod' => '1',
    'group' => 'subject',
    'with' => 'subject,subject_id',
    'active' => '1',
  ),
  'dpflbt' => 
  array (
    'id' => 'dpflbt',
    'title' => '短篇分类标题',
    'sql' => 'SELECT title, id As subject_id
FROM {db_prefix}read8_class_story
WHERE category = \'subject\' AND active = 1 AND id IN (1, 2, 3, 4, 5, 6)
ORDER BY id ASC',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'dpmgl' => 
  array (
    'id' => 'dpmgl',
    'title' => '短篇每个分类内容',
    'sql' => 'SELECT n.title, n.author, n.description, n.id As bid, n.subject, n.subject As subject_id
FROM `{db_prefix}read8_story` As n
WHERE (SELECT Count( * )
FROM `{db_prefix}read8_story`
WHERE subject = n.subject AND id < n.id) <1 And n.subject In(1,2,3,4,5,6)',
    'twod' => '1',
    'group' => 'subject',
    'with' => 'subject,subject_id',
    'active' => '1',
  ),
  'trwyg' => 
  array (
    'id' => 'trwyg',
    'title' => '同人网游阁',
    'sql' => 'SELECT n.title, n.author, n.intro, n.subject, n.id As bid, n.path
FROM `{db_prefix}read8_novel` As n
WHERE (SELECT Count( * )
FROM `{db_prefix}read8_novel`
WHERE subject = n.subject AND id < n.id) <2 And n.subject In(11, 12)',
    'twod' => '1',
    'group' => 'subject',
    'with' => 'subject',
    'active' => '1',
  ),
  'tydmg' => 
  array (
    'id' => 'tydmg',
    'title' => '体育耽美阁',
    'sql' => 'SELECT n.title, n.author, n.intro, n.subject, n.id As bid, n.path
FROM `{db_prefix}read8_novel` As n
WHERE (SELECT Count( * )
FROM `{db_prefix}read8_novel`
WHERE subject = n.subject AND id < n.id) <2 And n.subject In(13, 14)',
    'twod' => '1',
    'group' => 'subject',
    'with' => 'subject',
    'active' => '1',
  ),
  'fmtj' => 
  array (
    'id' => 'fmtj',
    'title' => '封面推荐',
    'sql' => 'SELECT n.path, n.title, n.id As bid
FROM `{db_prefix}read8_novel` As n
LEFT JOIN `{db_prefix}read8_novel_commend` As nc
ON n.id=nc.book_id
WHERE nc.commend=1
LIMIT 6',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'digg' => 
  array (
    'id' => 'digg',
    'title' => 'digg',
    'sql' => 'SELECT n.id AS bid, n.title, ne.digg_today, n.path
FROM `{db_prefix}read8_novel` AS n
LEFT JOIN `{db_prefix}read8_novel_external` AS ne
ON n.id = ne.book_id
ORDER BY ne.digg_today DESC
LIMIT 10',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
  'zxpl' => 
  array (
    'id' => 'zxpl',
    'title' => '最新评论',
    'sql' => 'SELECT n.id AS bid, n.title, nc.dateline, nc.username, n.path
FROM `{db_prefix}read8_novel` AS n
RIGHT JOIN `{db_prefix}read8_novel_comment` AS nc
ON n.id = nc.book_id
ORDER BY nc.dateline DESC
LIMIT 10',
    'twod' => '0',
    'group' => '',
    'with' => '',
    'active' => '1',
  ),
);