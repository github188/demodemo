-- phpMyAdmin SQL Dump
-- version 2.11.5-rc1
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2008 年 07 月 31 日 12:04
-- 服务器版本: 5.0.45
-- PHP 版本: 5.2.5


--
-- 数据库: `Read8_demo`
--

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_advertise`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_advertise` (
  `pos` varchar(255) NOT NULL default '',
  `ad` mediumtext NOT NULL,
  PRIMARY KEY  (`pos`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='广告投放';

--
-- 导出表中的数据 `rd8_read8_advertise`
--

INSERT INTO `rd8_read8_advertise` VALUES('index_1', '<a href="http://www.read8.net/" target="_blank"><img src="images/banner_1.jpg" alt="" /></a>');
INSERT INTO `rd8_read8_advertise` VALUES('index_2', '<a href="http://bbs.read8.net/" target="_blank"><img src="images/banner_2.jpg" alt="" /></a>');
INSERT INTO `rd8_read8_advertise` VALUES('read_1', '');
INSERT INTO `rd8_read8_advertise` VALUES('read_2', '');

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_announce`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_announce` (
  `id` smallint(5) NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `content` text NOT NULL,
  `dateline` int(10) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `dateline` (`dateline`),
  KEY `title` (`title`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='网站公告表结构';

--
-- 导出表中的数据 `rd8_read8_announce`
--

INSERT INTO `rd8_read8_announce` VALUES(1, '欢迎大家访问本书网，祝您看书开心', '<span style="color: #95A3FD;">欢迎大家访问本书网，祝您看书开心。</span>', 1207966654);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_api`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_api` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `address` varchar(255) NOT NULL default '',
  `code` varchar(255) NOT NULL default '',
  `charset` enum('GBK','UTF-8','BIG5') NOT NULL default 'UTF-8',
  `gate` enum('Discuz!','PHPWind') NOT NULL default 'PHPWind',
  `active` tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `address` (`address`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='通行证';

--
-- 导出表中的数据 `rd8_read8_api`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_author`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_author` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `pseudonym` varchar(255) NOT NULL default '',
  `pinyin` varchar(255) NOT NULL default '',
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `avatar` varchar(255) NOT NULL default '',
  `column_name` varchar(255) NOT NULL default '',
  `intro` text NOT NULL,
  `announcement` text NOT NULL,
  `sample` text NOT NULL,
  `vip` tinyint(1) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `pseudonym` (`pseudonym`),
  UNIQUE KEY `pinyin` (`pinyin`),
  UNIQUE KEY `uid` (`uid`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`),
  KEY `vip` (`vip`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='作家';

--
-- 导出表中的数据 `rd8_read8_author`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_author_commend`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_author_commend` (
  `author_id` mediumint(8) unsigned NOT NULL default '0',
  `book_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`author_id`,`book_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='作家推荐栏';

--
-- 导出表中的数据 `rd8_read8_author_commend`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_author_interview`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_author_interview` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `author_id` mediumint(8) unsigned NOT NULL default '0',
  `title` varchar(255) NOT NULL default '',
  `content` longtext NOT NULL,
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) unsigned NOT NULL default '1',
  PRIMARY KEY  (`id`),
  KEY `author_id` (`author_id`),
  KEY `title` (`title`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='作家访谈';

--
-- 导出表中的数据 `rd8_read8_author_interview`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_author_link`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_author_link` (
  `author_id` mediumint(8) unsigned NOT NULL default '0',
  `lid` tinyint(1) unsigned NOT NULL auto_increment,
  `link_author` mediumint(8) unsigned NOT NULL default '0',
  PRIMARY KEY  (`author_id`,`lid`),
  KEY `link_author` (`link_author`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='作家友链';

--
-- 导出表中的数据 `rd8_read8_author_link`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_banlist`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_banlist` (
  `id` mediumint(8) NOT NULL auto_increment,
  `kind` varchar(255) NOT NULL default '',
  `word` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `kind` (`kind`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='封禁列表';

--
-- 导出表中的数据 `rd8_read8_banlist`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_cache`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_cache` (
  `id` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL default '',
  `sql` mediumtext NOT NULL,
  `twod` tinyint(1) unsigned NOT NULL default '0',
  `group` varchar(255) NOT NULL default '',
  `with` varchar(255) NOT NULL default '',
  `active` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `active` (`active`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='自定义缓存';

--
-- 导出表中的数据 `rd8_read8_cache`
--

INSERT INTO `rd8_read8_cache` VALUES('zdjb', '总点击榜', 'SELECT n.id AS bid, n.title, ne.click_total, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_external` AS ne\r\nON n.id = ne.book_id\r\nORDER BY ne.click_total DESC\r\nLIMIT 10', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('ztjb', '总推荐榜', 'SELECT n.id AS bid, n.title, ne.commend_total, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_external` AS ne\r\nON n.id = ne.book_id\r\nORDER BY ne.commend_total DESC\r\nLIMIT 10\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('zscb', '总收藏榜', 'SELECT n.id AS bid, n.title, ne.favo_total, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_external` AS ne\r\nON n.id = ne.book_id\r\nORDER BY ne.favo_total DESC\r\nLIMIT 10\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('ydjb', '月点击榜', 'SELECT n.id AS bid, n.title, ne.click_month, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_external` AS ne\r\nON n.id = ne.book_id\r\nORDER BY ne.click_month DESC\r\nLIMIT 10\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('ytjb', '月推荐榜', 'SELECT n.id AS bid, n.title, ne.commend_month, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_external` AS ne\r\nON n.id = ne.book_id\r\nORDER BY ne.commend_month DESC\r\nLIMIT 10\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('qltjb', '强力推荐榜', 'SELECT n.id AS bid, n.title, n.path, nc.dateline\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_commend` AS nc\r\nON n.id = nc.book_id AND nc.commend = 2\r\nORDER BY nc.dateline DESC\r\nLIMIT 3\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('bjtjl', '编辑推荐栏', 'SELECT n.id AS bid, n.title, n.path, nc.dateline\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_commend` AS nc\r\nON n.id = nc.book_id AND nc.commend = 3\r\nORDER BY nc.dateline DESC\r\nLIMIT 3\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('sflbt', '十个分类标题', 'SELECT title ,id As subject_id\r\nFROM {db_prefix}read8_class_novel\r\nWHERE category = ''subject'' AND active = 1 AND id IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)\r\nORDER BY id ASC\r\n', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('mgl', '每个分类内容 ', 'SELECT n.title, n.author, n.intro, n.subject,n.subject As subject_id, n.id As bid, n.path\r\nFROM `{db_prefix}read8_novel` As n\r\nWHERE (SELECT Count( * )\r\nFROM `{db_prefix}read8_novel`\r\nWHERE subject = n.subject AND id < n.id) <2 And n.subject In(1,2,3,4,5,6,7,8,9,10)\r\n', 1, 'subject', 'subject,subject_id', 1);
INSERT INTO `rd8_read8_cache` VALUES('dpflbt', '短篇分类标题', 'SELECT title, id As subject_id\r\nFROM {db_prefix}read8_class_story\r\nWHERE category = ''subject'' AND active = 1 AND id IN (1, 2, 3, 4, 5, 6)\r\nORDER BY id ASC', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('dpmgl', '短篇每个分类内容', 'SELECT n.title, n.author, n.description, n.id As bid, n.subject, n.subject As subject_id\r\nFROM `{db_prefix}read8_story` As n\r\nWHERE (SELECT Count( * )\r\nFROM `{db_prefix}read8_story`\r\nWHERE subject = n.subject AND id < n.id) <1 And n.subject In(1,2,3,4,5,6)', 1, 'subject', 'subject,subject_id', 1);
INSERT INTO `rd8_read8_cache` VALUES('trwyg', '同人网游阁', 'SELECT n.title, n.author, n.intro, n.subject, n.id As bid, n.path\r\nFROM `{db_prefix}read8_novel` As n\r\nWHERE (SELECT Count( * )\r\nFROM `{db_prefix}read8_novel`\r\nWHERE subject = n.subject AND id < n.id) <2 And n.subject In(11, 12)', 1, 'subject', 'subject', 1);
INSERT INTO `rd8_read8_cache` VALUES('tydmg', '体育耽美阁', 'SELECT n.title, n.author, n.intro, n.subject, n.id As bid, n.path\r\nFROM `{db_prefix}read8_novel` As n\r\nWHERE (SELECT Count( * )\r\nFROM `{db_prefix}read8_novel`\r\nWHERE subject = n.subject AND id < n.id) <2 And n.subject In(13, 14)', 1, 'subject', 'subject', 1);
INSERT INTO `rd8_read8_cache` VALUES('fmtj', '封面推荐', 'SELECT n.path, n.title, n.id As bid\r\nFROM `{db_prefix}read8_novel` As n\r\nLEFT JOIN `{db_prefix}read8_novel_commend` As nc\r\nON n.id=nc.book_id\r\nWHERE nc.commend=1\r\nLIMIT 6', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('digg', 'digg', 'SELECT n.id AS bid, n.title, ne.digg_today, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nLEFT JOIN `{db_prefix}read8_novel_external` AS ne\r\nON n.id = ne.book_id\r\nORDER BY ne.digg_today DESC\r\nLIMIT 10', 0, '', '', 1);
INSERT INTO `rd8_read8_cache` VALUES('zxpl', '最新评论', 'SELECT n.id AS bid, n.title, nc.dateline, nc.username, n.path\r\nFROM `{db_prefix}read8_novel` AS n\r\nRIGHT JOIN `{db_prefix}read8_novel_comment` AS nc\r\nON n.id = nc.book_id\r\nORDER BY nc.dateline DESC\r\nLIMIT 10', 0, '', '', 1);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_class_novel`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_class_novel` (
  `category` varchar(255) NOT NULL default '',
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `priority` smallint(5) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '1',
  `deltime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`category`,`id`),
  KEY `title` (`title`),
  KEY `priority` (`priority`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`),
  KEY `deltime` (`deltime`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='长篇分类';

--
-- 导出表中的数据 `rd8_read8_class_novel`
--

INSERT INTO `rd8_read8_class_novel` VALUES('subject', 1, '言情', 1, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 2, '武侠', 2, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 3, '奇幻', 3, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 4, '恐怖', 4, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 5, '历史', 5, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 6, '传奇', 6, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 7, '军事', 7, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 8, '科幻', 8, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 9, '童话', 9, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 10, '侦探', 10, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 11, '同人', 11, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 12, '网游', 12, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 13, '体育', 13, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('subject', 14, '耽美', 14, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 1, '转世重生', 1, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 2, '魔法校园', 2, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 3, '王朝争霸', 3, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 4, '异术超能', 4, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 5, '远古神话', 5, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 6, '异世冒险', 6, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 7, '吸血家族', 7, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 8, '变身情缘', 8, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 9, '传统武侠', 9, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 10, '谐趣武侠', 10, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 11, '浪子异侠', 11, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 12, '古典仙侠', 12, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 13, '奇幻修真', 13, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 14, '现代修真', 14, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 15, '穿越时空', 15, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 16, '浪漫轻喜', 16, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 17, '宫闱情仇', 17, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 18, '快意江湖', 18, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 19, '菁菁校园', 19, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 20, '爱在职场', 20, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 21, '现代都市', 21, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 22, '冒险推理', 22, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 23, '灵异恐怖', 23, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 24, '架空历史', 24, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 25, '商海沉浮', 25, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 26, '宦海风云', 26, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 27, '都市异能', 27, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 28, '现代战争', 28, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 29, '战争幻想', 29, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 30, '特种军旅', 30, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 31, '品味人生', 31, 0, 1, 0);
INSERT INTO `rd8_read8_class_novel` VALUES('content', 32, '拯救世界', 32, 0, 1, 0);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_class_story`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_class_story` (
  `category` varchar(255) NOT NULL default '',
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `priority` smallint(5) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '1',
  `deltime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`category`,`id`),
  KEY `title` (`title`),
  KEY `priority` (`priority`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`),
  KEY `deltime` (`deltime`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='短篇分类';

--
-- 导出表中的数据 `rd8_read8_class_story`
--

INSERT INTO `rd8_read8_class_story` VALUES('subject', 1, '散文', 1, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('subject', 2, '诗歌', 2, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('subject', 3, '歌词', 3, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('subject', 4, '评论', 4, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('subject', 5, '随笔', 5, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('subject', 6, '剧本', 6, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 1, '在水一方', 1, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 2, '悠然我思', 2, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 3, '天涯萍踪', 3, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 4, '动感都市', 4, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 5, '倾城之恋', 5, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 6, '弹指江湖', 6, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 7, '百味人生', 7, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 8, '故事新编', 8, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 9, '魅影奇幻', 9, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 10, '纯真年代', 10, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 11, '另类先锋', 11, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 12, '尖峰时刻', 12, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 13, '思想钩沉', 13, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 14, '声色文武', 14, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 15, '剧本中心', 15, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 16, '假语村言', 16, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 17, '无心快语', 17, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 18, '原创歌词', 18, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 19, '诗风词韵', 19, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 20, '如歌行板', 20, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 21, '青青子衿', 21, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 22, '情感日记', 22, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 23, '记事日记', 23, 0, 1, 0);
INSERT INTO `rd8_read8_class_story` VALUES('content', 24, '随感日记', 24, 0, 1, 0);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_class_system`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_class_system` (
  `category` varchar(255) NOT NULL default '',
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `description` text NOT NULL,
  PRIMARY KEY  (`category`,`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='系统分类';

--
-- 导出表中的数据 `rd8_read8_class_system`
--

INSERT INTO `rd8_read8_class_system` VALUES('style', 1, '恶搞', '');
INSERT INTO `rd8_read8_class_system` VALUES('style', 2, '喜剧', '');
INSERT INTO `rd8_read8_class_system` VALUES('style', 3, '正剧', '');
INSERT INTO `rd8_read8_class_system` VALUES('style', 4, '悲剧', '');
INSERT INTO `rd8_read8_class_system` VALUES('state', 1, '正在连载', '');
INSERT INTO `rd8_read8_class_system` VALUES('state', 2, '暂停更新', '');
INSERT INTO `rd8_read8_class_system` VALUES('state', 3, '已完结', '');
INSERT INTO `rd8_read8_class_system` VALUES('state', 4, '锁定', '');
INSERT INTO `rd8_read8_class_system` VALUES('state', 5, '冻结', '1');
INSERT INTO `rd8_read8_class_system` VALUES('state', 6, '审核中', '1');
INSERT INTO `rd8_read8_class_system` VALUES('license', 1, '本站独发', '本作品独家发表于读吧书网，不会再发表于其他任何网站，若无本站书面授权，任何个人和组织不得转载。');
INSERT INTO `rd8_read8_class_system` VALUES('license', 2, '本站首发', '本作品首发于读吧书网，本作品任何章节在其他任何网站发表更新的时间至少比本站晚一个月，并且在其他网站发表时，要明确写明是本站首发。');
INSERT INTO `rd8_read8_class_system` VALUES('license', 3, '他站首发', '本作品首发于其他网站。');

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_collect_rule_multi`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_collect_rule_multi` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `parent` varchar(255) NOT NULL default '',
  `page_list` varchar(255) NOT NULL default '',
  `page_start` int(10) unsigned NOT NULL default '0',
  `page_end` int(10) unsigned NOT NULL default '0',
  `bid_start` varchar(255) NOT NULL default '',
  `bid_end` varchar(255) NOT NULL default '',
  `bid_regexp` varchar(255) NOT NULL default '',
  `lastupdate` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `parent` (`parent`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='批量采集规则';

--
-- 导出表中的数据 `rd8_read8_collect_rule_multi`
--

INSERT INTO `rd8_read8_collect_rule_multi` VALUES(1, '起点最新全本', 'qidian', 'http://www.qidian.com/Book/BookStore.aspx?Type=1&ChannelId=-1&SubCategoryId=-1&PageIndex={read8page}&OrderId=6&P=All ', 1, 0, '', '', '|new Book\\(''(\\d+)|isU', 1214928241);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(2, '起点中文网新手周榜', 'qidian', 'http://www.qidian.com/Book/BookPotentialList.aspx?Type=Book', 0, 0, '', '', '|new Book\\(''(\\d+)|isU', 1214928237);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(3, '起点中文网强力推荐榜', 'qidian', 'http://www.qidian.com/Book/CommendList.aspx?GroupId=3&ChannelId=-1&PageIndex={read8page}', 1, 6, '', '', '|href=''/Book/(\\d+)\\.aspx'' title=''|isU', 1214928227);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(4, '起点中文网新人周榜', 'qidian', 'http://www.qidian.com/Book/BookPotentialList.aspx?Type=Author', 0, 0, '', '', '|new Book\\(''(\\d+)|isU', 1214928215);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(5, '起点中文网女频周点击榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=3', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922506);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(6, '起点中文网女频总点击榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=1', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922501);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(7, '起点中文网女频月点击榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=2', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922486);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(8, '起点中文网女频总推荐榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=4', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922537);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(9, '起点中文网女频月推荐榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=4', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922561);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(10, '起点中文网女频周推荐榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=6', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922596);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(11, '起点中文网女频总收藏榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=7', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922622);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(12, '起点中文网女频总字数榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=10', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922685);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(13, '起点中文网女频新人周潜力榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=9', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922740);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(14, '起点中文网女频新书周潜力榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=8', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922826);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(15, '起点中文网女频入站时间榜', 'qidian', 'http://mm.qidian.com/MMWeb//TopTen.aspx?type=11', 0, 0, '', '', '|href=''/MMWeb/(\\d+)\\.aspx'' target=''_blank'' class=''act01''|isU', 1214922861);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(16, '起点中文网玄幻奇幻周点击榜', 'qidian', 'http://www.qidian.com/Book/BookSortList.aspx?Type=Hit&ChannelId=1&F=W', 0, 0, '', '', '|new Book\\(''(\\d+)|isU', 1214923323);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(17, '起点中文网武侠仙侠周点击榜', 'qidian', 'http://www.qidian.com/Book/BookSortList.aspx?Type=Hit&ChannelId=2&F=W', 0, 0, '', '', '|new Book\\(''(\\d+)|isU', 1214923710);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(18, '起点中文网都市言情周点击榜', 'qidian', 'http://www.qidian.com/Book/BookSortList.aspx?Type=Hit&ChannelId=3&F=W', 0, 0, '', '', '|new Book\\(''(\\d+)|isU', 1214923745);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(19, '逐浪自力更生榜', 'zhulang', 'http://www.zhulang.com/bookarray.php?flag=1&page={read8page}', 1, 4, '', '', '|class="h3"><a href=''/(\\d+)/index\\.html''|isU', 1214927664);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(20, '逐浪周点击数榜', 'zhulang', 'http://www.zhulang.com/bookarray.php?flag=2&page={read8page}', 1, 4, '', '', '|class="h3"><a href=''/(\\d+)/index\\.html''|isU', 1214927656);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(21, '逐浪周投票数榜', 'zhulang', 'http://www.zhulang.com/bookarray.php?flag=3&page={read8page}', 1, 4, '', '', '|class="h3"><a href=''/(\\d+)/index\\.html''|isU', 1214927712);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(22, '逐浪新书潜力榜', 'zhulang', 'http://www.zhulang.com/bookarray.php?flag=4&page={read8page}', 1, 4, '', '', '|class="h3"><a href=''/(\\d+)/index\\.html''|isU', 1214927745);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(23, '逐浪总投票数榜', 'zhulang', 'http://www.zhulang.com/bookarray.php?flag=6&page={read8page}', 1, 4, '', '', '|class="h3"><a href=''/(\\d+)/index\\.html''|isU', 1214927805);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(24, '逐浪总点击数榜', 'zhulang', 'http://www.zhulang.com/bookarray.php?flag=7&page={read8page}', 1, 4, '', '', '|class="h3"><a href=''/(\\d+)/index\\.html''|isU', 1214927845);
INSERT INTO `rd8_read8_collect_rule_multi` VALUES(25, '逐浪最近更新小说', 'zhulang', 'http://s.zhulang.com/w_book_list.php?bState=all&classa=&classb=&initial=&total=57495&pageNum={read8page}', 1, 1000, '', '', '|class="h3"><a href="http://book\\.zhulang\\.com/(\\d+)/index\\.html|isU', 1214928077);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_collect_rule_single`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_collect_rule_single` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `mark` varchar(255) NOT NULL default '',
  `title` varchar(255) NOT NULL default '',
  `address` varchar(255) NOT NULL default '',
  `page_info` varchar(255) NOT NULL default '',
  `general_regexp_1` varchar(255) NOT NULL default '',
  `title_start` varchar(255) NOT NULL default '',
  `title_end` varchar(255) NOT NULL default '',
  `title_regexp` varchar(255) NOT NULL default '',
  `author_start` varchar(255) NOT NULL default '',
  `author_end` varchar(255) NOT NULL default '',
  `author_regexp` varchar(255) NOT NULL default '',
  `subject_start` varchar(255) NOT NULL default '',
  `subject_end` varchar(255) NOT NULL default '',
  `subject_regexp` varchar(255) NOT NULL default '',
  `subject_pos` mediumtext NOT NULL,
  `content_start` varchar(255) NOT NULL default '',
  `content_end` varchar(255) NOT NULL default '',
  `content_regexp` varchar(255) NOT NULL default '',
  `content_pos` mediumtext NOT NULL,
  `intro_start` varchar(255) NOT NULL default '',
  `intro_end` varchar(255) NOT NULL default '',
  `intro_regexp` varchar(255) NOT NULL default '',
  `cover_start` varchar(255) NOT NULL default '',
  `cover_end` varchar(255) NOT NULL default '',
  `cover_prefix` varchar(255) NOT NULL default '',
  `cover_regexp` varchar(255) NOT NULL default '',
  `page_list` varchar(255) NOT NULL default '',
  `general_regexp_2` varchar(255) NOT NULL default '',
  `volume_start` varchar(255) NOT NULL default '',
  `volume_end` varchar(255) NOT NULL default '',
  `volume_regexp` varchar(255) NOT NULL default '',
  `chapter_link_start` varchar(255) NOT NULL default '',
  `chapter_link_end` varchar(255) NOT NULL default '',
  `chapter_title_start` varchar(255) NOT NULL default '',
  `chapter_title_end` varchar(255) NOT NULL default '',
  `chapter_regexp` varchar(255) NOT NULL default '',
  `chapter_title_pos` tinyint(1) unsigned NOT NULL default '2',
  `page_read` varchar(255) NOT NULL default '',
  `general_regexp_3` varchar(255) NOT NULL default '',
  `page_final` varchar(255) NOT NULL default '',
  `text_start` varchar(255) NOT NULL default '',
  `text_end` varchar(255) NOT NULL default '',
  `text_regexp` varchar(255) NOT NULL default '',
  `replacements` mediumtext NOT NULL,
  `image` tinyint(1) unsigned NOT NULL default '0',
  `image_prefix` varchar(255) NOT NULL default '',
  `compression` tinyint(1) unsigned NOT NULL default '0',
  `charsets` tinyint(1) unsigned NOT NULL default '0',
  `lastupdate` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `mark` (`mark`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='站点采集规则';

--
-- 导出表中的数据 `rd8_read8_collect_rule_single`
--

INSERT INTO `rd8_read8_collect_rule_single` VALUES(1, 'qidian', '起点中文网', 'http://www.qidian.com/', 'http://www.qidian.com/Book/{read8bid}.aspx', '', 'zsfontBT">', '&nbsp;', '', '作者：', '</a>', '', '作品大类：', '</td>', '', 'YTowOnt9', '类别：', '</a>', '', 'YTowOnt9', 'height="20px">', '作品关键字：', '', 'http://image.cmfu.com/books/', '" width', 'http://image.cmfu.com/books/', '', 'http://www.qidian.com/BookReader/{read8bid}.aspx', '', 'class=''chat''>', '[', '', '', '', '', '', '|href=''\\d+,(\\d+)\\.aspx[^>]+>(.+)</a>|iU', 2, 'http://www.qidian.com/BookReader/{read8bid},{read8cv1}.aspx', '|files\\.qidian\\.com/Author(\\d+)|i', 'http://files.qidian.com/Author{read8g3v1}/{read8bid}/{read8cv1}.txt', 'document.write(''', ''');', '', 'YTo2OntpOjA7YTo0OntzOjE6ImYiO3M6Mzoi44CAIjtzOjE6InQiO3M6NDoiwqDCoCI7czoxOiJzIjtpOjA7czo0OiJmdW5jIjtzOjExOiJzdHJfcmVwbGFjZSI7fWk6MTthOjQ6e3M6MToiZiI7czozOiI8cD4iO3M6MToidCI7czoyOiJcbiI7czoxOiJzIjtpOjA7czo0OiJmdW5jIjtzOjExOiJzdHJfcmVwbGFjZSI7fWk6MjthOjQ6e3M6MToiZiI7czo0OiI8YnI+IjtzOjE6InQiO3M6MjoiXG4iO3M6MToicyI7aTowO3M6NDoiZnVuYyI7czoxMToic3RyX3JlcGxhY2UiO31pOjM7YTo0OntzOjE6ImYiO3M6Mjk6InzjgJDotbfngrkuKuetvue6puS9nOWTgeOAkXxpIjtzOjE6InQiO3M6MDoiIjtzOjE6InMiO2k6MTtzOjQ6ImZ1bmMiO3M6MTI6InByZWdfcmVwbGFjZSI7fWk6NDthOjQ6e3M6MToiZiI7czoyMDoifOS9nOWTgeWFs+mUruWtly4qfGkiO3M6MToidCI7czowOiIiO3M6MToicyI7aToxO3M6NDoiZnVuYyI7czoxMjoicHJlZ19yZXBsYWNlIjt9aTo1O2E6NDp7czoxOiJmIjtzOjE4OiJ8LipxaWRpYW5cLmNvbS4qfGkiO3M6MToidCI7czowOiIiO3M6MToicyI7aToxO3M6NDoiZnVuYyI7czoxMjoicHJlZ19yZXBsYWNlIjt9fQ==', 0, '', 0, 0, 1211941905);
INSERT INTO `rd8_read8_collect_rule_single` VALUES(2, 'zhulang', '逐浪小说', 'http://www.zhulang.com/', 'http://www.zhulang.com/{read8bid}/index.html', '', '<strong>《', '》', '', '作　　者：', '</a>', '', '作品类型：', '</td>', '', 'YTowOnt9', '', '', '', 'YTowOnt9', '<li class="h1"><font color="#000"><strong>', '</li>', '', 'http://images.zhulang.com/book_cover', '"', 'http://images.zhulang.com/book_cover', '', 'http://book.zhulang.com/{read8bid}/index.html', '', 'bottomborder">', '[', '', '', '', '', '', '|href=".+/\\d+/(\\d+)\\.html[^>]+>(.+)</a>|iU', 2, 'http://book.zhulang.com/{read8bid}/{read8cv1}.html', '', '', '</h2></div>', '</div>', '', 'YToyOntpOjA7YTo0OntzOjE6ImYiO3M6Mzoi44CAIjtzOjE6InQiO3M6NDoiwqDCoCI7czoxOiJzIjtpOjA7czo0OiJmdW5jIjtzOjExOiJzdHJfcmVwbGFjZSI7fWk6MTthOjQ6e3M6MToiZiI7czo4OiI8YnI+PGJyPiI7czoxOiJ0IjtzOjI6IlxuIjtzOjE6InMiO2k6MDtzOjQ6ImZ1bmMiO3M6MTE6InN0cl9yZXBsYWNlIjt9fQ==', 0, '', 0, 0, 1214116376);
INSERT INTO `rd8_read8_collect_rule_single` VALUES(3, 'cuiweiju', '翠微居', 'http://www.cuiweiju.com/', 'http://www.cuiweiju.com/modules/article/articleinfo.php?id={read8bid}', '|article/html/(\\d+)/|i', 'style="text-align:center"><h1>', '</h1>', '', '作　　者：', '</a>', '', '', '', '', 'YTowOnt9', '类　　别：', '</h2>', '', 'YTowOnt9', '内容简介：', '本书公告：', '', '.jpg" target="_blank"><img src="', '"', '', '', 'http://read.cuiweiju.com/files/article/html/{read8g1v1}/{read8bid}/index.html', '', 'class="vcss">', '</td', '', '', '', '', '', '|"(\\d+)\\.html"[^>]*>(.*)</a>|iU', 2, 'http://read.cuiweiju.com/files/article/html/{read8g1v1}/{read8bid}/{read8cv1}.html', '', '', '<div id="info">', '<br>', '', 'YToyOntpOjA7YTo0OntzOjE6ImYiO3M6Mjk6InzkvZzogIXvvJpbXjxdKzwvZGl2PltePl0rPnxpIjtzOjE6InQiO3M6MDoiIjtzOjE6InMiO2k6MTtzOjQ6ImZ1bmMiO3M6MTI6InByZWdfcmVwbGFjZSI7fWk6MTthOjQ6e3M6MToiZiI7czoyNDoiJm5ic3A7Jm5ic3A7Jm5ic3A7Jm5ic3A7IjtzOjE6InQiO3M6NDoiICAgICI7czoxOiJzIjtpOjA7czo0OiJmdW5jIjtzOjExOiJzdHJfcmVwbGFjZSI7fX0=', 0, '', 0, 0, 1217589839);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_favorite`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_favorite` (
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `fid` mediumint(8) unsigned NOT NULL auto_increment,
  `gid` tinyint(1) unsigned NOT NULL default '0',
  `bid` int(10) unsigned NOT NULL default '0',
  `cid` int(10) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`uid`,`fid`),
  KEY `gid` (`gid`),
  KEY `bid` (`bid`),
  KEY `cid` (`cid`),
  KEY `dateline` (`dateline`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='收藏';

--
-- 导出表中的数据 `rd8_read8_favorite`
--

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_favorite_category`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_favorite_category` (
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `gid` tinyint(1) unsigned NOT NULL auto_increment,
  `category` char(19) NOT NULL default '',
  PRIMARY KEY  (`uid`,`gid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='收藏分类';

--
-- 导出表中的数据 `rd8_read8_favorite_category`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_focus`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_focus` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `url` varchar(255) NOT NULL default '',
  `description` varchar(255) NOT NULL default '',
  `link` varchar(255) NOT NULL default '',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) unsigned NOT NULL default '1',
  PRIMARY KEY  (`id`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='首页焦点图';

--
-- 导出表中的数据 `rd8_read8_focus`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_help`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_help` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `title` varchar(255) NOT NULL default '',
  `content` text NOT NULL,
  `dateline` int(10) unsigned NOT NULL default '0',
  `reply` text NOT NULL,
  `replytime` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) unsigned NOT NULL default '1',
  PRIMARY KEY  (`id`),
  KEY `uid` (`uid`),
  KEY `title` (`title`),
  KEY `dateline` (`dateline`),
  KEY `replytime` (`replytime`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='求助投诉';

--
-- 导出表中的数据 `rd8_read8_help`
--

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_justnow`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_justnow` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `username` char(255) NOT NULL default '',
  `book_id` int(10) unsigned NOT NULL default '0',
  `book_title` char(255) NOT NULL default '',
  `dateline` int(10) unsigned NOT NULL default '0',
  `module` char(10) NOT NULL default '',
  `action` char(10) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `uid` (`uid`),
  KEY `book_id` (`book_id`),
  KEY `dateline` (`dateline`),
  KEY `module` (`module`),
  KEY `action` (`action`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='就在刚才';

--
-- 导出表中的数据 `rd8_read8_justnow`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_links`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_links` (
  `id` tinyint(255) unsigned NOT NULL auto_increment,
  `name` varchar(255) NOT NULL default '',
  `url` varchar(255) NOT NULL default '',
  `logo` varchar(255) NOT NULL default '',
  `apply` tinyint(1) unsigned NOT NULL default '0',
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `username` varchar(255) NOT NULL default '',
  `address` varchar(255) NOT NULL default '',
  `ps` text NOT NULL,
  `priority` tinyint(255) unsigned NOT NULL default '255',
  PRIMARY KEY  (`id`),
  KEY `uid` (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='书网友情链接';

--
-- 导出表中的数据 `rd8_read8_links`
--

INSERT INTO `rd8_read8_links` VALUES(1, '澳柯网信', 'www.aokosoft.com', '', 0, 0, '', '', '', 1);
INSERT INTO `rd8_read8_links` VALUES(2, '读吧书网', 'www.read8.net', '', 0, 0, '', '', '', 2);
INSERT INTO `rd8_read8_links` VALUES(3, '第七世界', 'www.7world.cn', '', 0, 0, '', '', '', 3);
INSERT INTO `rd8_read8_links` VALUES(4, '下棋啊', 'www.xiaqia.com', '', 0, 0, '', '', '', 4);

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `pinyin` varchar(255) NOT NULL default '',
  `initial` char(2) NOT NULL default '',
  `author` varchar(255) NOT NULL default '',
  `author_id` mediumint(8) unsigned NOT NULL default '0',
  `typewriter` varchar(255) NOT NULL default '',
  `typewriter_id` mediumint(8) unsigned NOT NULL default '0',
  `subject` tinyint(1) unsigned NOT NULL default '0',
  `content` tinyint(1) unsigned NOT NULL default '0',
  `style` tinyint(1) unsigned NOT NULL default '0',
  `role_lead` varchar(255) NOT NULL default '',
  `role_support` varchar(255) NOT NULL default '',
  `license` tinyint(1) unsigned NOT NULL default '3',
  `description` varchar(255) NOT NULL default '',
  `tag` varchar(255) NOT NULL default '',
  `path` varchar(255) NOT NULL default '',
  `intro` text NOT NULL,
  `state` tinyint(1) unsigned NOT NULL default '1',
  `vip` tinyint(1) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  `lastupdate` int(10) unsigned NOT NULL default '0',
  `lastchapter` smallint(5) unsigned NOT NULL default '0',
  `words` mediumint(8) unsigned NOT NULL default '0',
  `hash` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `pinyin` (`pinyin`),
  KEY `initial` (`initial`),
  KEY `author` (`author`),
  KEY `author_id` (`author_id`),
  KEY `subject` (`subject`),
  KEY `content` (`content`),
  KEY `style` (`style`),
  KEY `role_lead` (`role_lead`),
  KEY `role_support` (`role_support`),
  KEY `license` (`license`),
  KEY `description` (`description`),
  KEY `tag` (`tag`),
  KEY `state` (`state`),
  KEY `dateline` (`dateline`),
  KEY `lastupdate` (`lastupdate`),
  KEY `lastchapter` (`lastchapter`),
  KEY `words` (`words`),
  KEY `hash` (`hash`),
  KEY `typewriter_id` (`typewriter_id`),
  KEY `path` (`path`),
  KEY `title` (`title`),
  KEY `vip` (`vip`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='小说';

--
-- 导出表中的数据 `rd8_read8_novel`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_apply`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_apply` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `author` varchar(255) NOT NULL default '',
  `author_id` mediumint(8) unsigned NOT NULL default '0',
  `subject` tinyint(1) unsigned NOT NULL default '0',
  `content` tinyint(1) unsigned NOT NULL default '0',
  `style` tinyint(1) unsigned NOT NULL default '0',
  `role_lead` varchar(255) NOT NULL default '',
  `role_support` varchar(255) NOT NULL default '',
  `license` tinyint(1) unsigned NOT NULL default '0',
  `description` varchar(255) NOT NULL default '',
  `tag` varchar(255) NOT NULL default '',
  `intro` text NOT NULL,
  `sample` text NOT NULL,
  `dateline` int(10) unsigned NOT NULL default '0',
  `verifytime` int(10) unsigned NOT NULL default '0',
  `book_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `author_id` (`author_id`),
  KEY `subject` (`subject`),
  KEY `content` (`content`),
  KEY `style` (`style`),
  KEY `role_lead` (`role_lead`),
  KEY `role_support` (`role_support`),
  KEY `license` (`license`),
  KEY `description` (`description`),
  KEY `tag` (`tag`),
  KEY `dateline` (`dateline`),
  KEY `verifytime` (`verifytime`),
  KEY `book_id` (`book_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='小说申请';

--
-- 导出表中的数据 `rd8_read8_novel_apply`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_attach`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_attach` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `book_id` int(10) unsigned NOT NULL default '0',
  `chapter_id` int(10) unsigned NOT NULL default '0',
  `file` char(3) NOT NULL,
  `dateline` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `book_id` (`book_id`,`chapter_id`),
  KEY `dateline` (`dateline`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='小说附件';

--
-- 导出表中的数据 `rd8_read8_novel_attach`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_chapter`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_chapter` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `priority` smallint(5) unsigned NOT NULL default '0',
  `volume_id` tinyint(1) unsigned NOT NULL default '0',
  `description` varchar(255) NOT NULL default '',
  `locked` tinyint(1) unsigned NOT NULL default '0',
  `vip` varchar(255) NOT NULL default '',
  `dateline` int(10) unsigned NOT NULL default '0',
  `lastupdate` int(10) unsigned NOT NULL default '0',
  `words` smallint(5) unsigned NOT NULL default '0',
  `hash` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`book_id`,`id`),
  KEY `priority` (`priority`),
  KEY `volume_id` (`volume_id`),
  KEY `locked` (`locked`),
  KEY `vip` (`vip`),
  KEY `dateline` (`dateline`),
  KEY `lastupdate` (`lastupdate`),
  KEY `words` (`words`),
  KEY `hash` (`hash`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='小说章节';

--
-- 导出表中的数据 `rd8_read8_novel_chapter`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_commend`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_commend` (
  `commend` tinyint(1) unsigned NOT NULL default '0',
  `book_id` int(10) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`commend`,`book_id`),
  KEY `dateline` (`dateline`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='小说推荐';

--
-- 导出表中的数据 `rd8_read8_novel_commend`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_comment`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_comment` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `chapter_id` smallint(5) unsigned NOT NULL default '0',
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `username` varchar(255) NOT NULL default '',
  `title` varchar(255) NOT NULL default '',
  `content` text NOT NULL,
  `ip` int(10) unsigned NOT NULL default '0',
  `digest` tinyint(1) unsigned NOT NULL default '0',
  `sticky` tinyint(1) unsigned NOT NULL default '0',
  `highlight` varchar(255) NOT NULL default '',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (`book_id`,`id`),
  KEY `chapter_id` (`chapter_id`),
  KEY `uid` (`uid`),
  KEY `title` (`title`),
  KEY `ip` (`ip`),
  KEY `digest` (`digest`),
  KEY `sticky` (`sticky`),
  KEY `highlight` (`highlight`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`),
  FULLTEXT KEY `content` (`content`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='小说评论';


--
-- 导出表中的数据 `rd8_read8_novel_comment`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_draft`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_draft` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `book_id` int(10) unsigned NOT NULL default '0',
  `title` varchar(255) NOT NULL default '',
  `description` varchar(255) NOT NULL default '',
  `insertbefore` smallint(5) unsigned NOT NULL default '0',
  `volumetitle` varchar(255) NOT NULL default '',
  `vip` tinyint(1) unsigned NOT NULL default '0',
  `article` mediumtext NOT NULL,
  `dateline` int(10) unsigned NOT NULL default '0',
  `pubtime` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) unsigned NOT NULL default '1',
  PRIMARY KEY  (`id`),
  KEY `book_id` (`book_id`),
  KEY `dateline` (`dateline`),
  KEY `pubtime` (`pubtime`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='小说定时发布';

--
-- 导出表中的数据 `rd8_read8_novel_draft`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_external`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_external` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `click_today` int(10) unsigned NOT NULL default '0',
  `click_week` int(10) unsigned NOT NULL default '0',
  `click_month` int(10) unsigned NOT NULL default '0',
  `click_total` int(10) unsigned NOT NULL default '0',
  `vote_today` int(10) unsigned NOT NULL default '0',
  `vote_week` int(10) unsigned NOT NULL default '0',
  `vote_month` int(10) unsigned NOT NULL default '0',
  `vote_total` int(10) unsigned NOT NULL default '0',
  `rate_week` int(10) unsigned NOT NULL default '0',
  `rate_month` int(10) unsigned NOT NULL default '0',
  `rate_total` int(10) unsigned NOT NULL default '0',
  `favo_today` int(10) unsigned NOT NULL default '0',
  `favo_week` int(10) unsigned NOT NULL default '0',
  `favo_month` int(10) unsigned NOT NULL default '0',
  `favo_total` int(10) unsigned NOT NULL default '0',
  `commend_today` int(10) unsigned NOT NULL default '0',
  `commend_week` int(10) unsigned NOT NULL default '0',
  `commend_month` int(10) unsigned NOT NULL default '0',
  `commend_total` int(10) unsigned NOT NULL default '0',
  `digg_today` int(10) unsigned NOT NULL default '0',
  `digg_week` int(10) unsigned NOT NULL default '0',
  `digg_month` int(10) unsigned NOT NULL default '0',
  `digg_total` int(10) unsigned NOT NULL default '0',
  `bury_today` int(10) unsigned NOT NULL default '0',
  `bury_week` int(10) unsigned NOT NULL default '0',
  `bury_month` int(10) unsigned NOT NULL default '0',
  `bury_total` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`book_id`),
  KEY `click_week` (`click_week`),
  KEY `click_month` (`click_month`),
  KEY `click_total` (`click_total`),
  KEY `vote_week` (`vote_week`),
  KEY `vote_month` (`vote_month`),
  KEY `vote_total` (`vote_total`),
  KEY `rate_week` (`rate_week`),
  KEY `rate_month` (`rate_month`),
  KEY `rate_total` (`rate_total`),
  KEY `favo_week` (`favo_week`),
  KEY `favo_month` (`favo_month`),
  KEY `favo_total` (`favo_total`),
  KEY `commend_week` (`commend_week`),
  KEY `commend_month` (`commend_month`),
  KEY `commend_total` (`commend_total`),
  KEY `digg_week` (`digg_week`),
  KEY `digg_month` (`digg_month`),
  KEY `digg_total` (`digg_total`),
  KEY `bury_week` (`bury_week`),
  KEY `bury_month` (`bury_month`),
  KEY `bury_total` (`bury_total`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='小说扩展数据';

--
-- 导出表中的数据 `rd8_read8_novel_external`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_rate`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_rate` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `rate` tinyint(1) unsigned NOT NULL default '0',
  KEY `book_id` (`book_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='小说评分记录';

--
-- 导出表中的数据 `rd8_read8_novel_rate`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_novel_volume`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_novel_volume` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `id` tinyint(1) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `chapter_priority` smallint(5) unsigned NOT NULL default '0',
  PRIMARY KEY  (`book_id`,`id`),
  UNIQUE KEY `title` (`title`),
  KEY `chapter_priority` (`chapter_priority`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='小说卷';

--
-- 导出表中的数据 `rd8_read8_novel_volume`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_pay`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_pay` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `price` mediumint(8) unsigned NOT NULL default '0',
  `vmoney` mediumint(8) unsigned NOT NULL default '0',
  `gate` char(3) NOT NULL default 'ALI',
  `state` tinyint(1) unsigned NOT NULL default '0',
  `suctime` int(10) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '1',
  `deltime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `uid` (`uid`),
  KEY `price` (`price`),
  KEY `vmoney` (`vmoney`),
  KEY `state` (`state`),
  KEY `suctime` (`suctime`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`),
  KEY `deltime` (`deltime`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='充值记录';

--
-- 导出表中的数据 `rd8_read8_pay`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_pm`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_pm` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `from` varchar(255) NOT NULL default '',
  `from_id` mediumint(8) unsigned NOT NULL default '0',
  `to` varchar(255) NOT NULL default '',
  `to_id` mediumint(8) unsigned NOT NULL default '0',
  `title` varchar(255) NOT NULL default '',
  `content` mediumtext NOT NULL,
  `dateline` int(10) unsigned NOT NULL default '0',
  `isread` tinyint(1) unsigned NOT NULL default '0',
  `isinbox` tinyint(1) NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `from` (`from`),
  KEY `from_id` (`from_id`),
  KEY `to` (`to`),
  KEY `to_id` (`to_id`),
  KEY `title` (`title`),
  KEY `dateline` (`dateline`),
  KEY `isread` (`isread`),
  KEY `isinbox` (`isinbox`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='站内短信';

--
-- 导出表中的数据 `rd8_read8_pm`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_sessions`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_sessions` (
  `sid` char(32) NOT NULL,
  `ip` char(15) NOT NULL,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `username` char(255) NOT NULL,
  `expire` int(10) NOT NULL default '0',
  `secode` char(255) NOT NULL default '',
  UNIQUE KEY `sid` (`sid`),
  KEY `uid` (`uid`),
  KEY `expire` (`expire`)
) ENGINE=MEMORY DEFAULT CHARSET=utf8 COMMENT='在线列表';

--
-- 导出表中的数据 `rd8_read8_sessions`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_sessions_acp`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_sessions_acp` (
  `id` mediumint(8) unsigned NOT NULL default '0',
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `name` varchar(255) NOT NULL default '',
  `pswd` varchar(32) NOT NULL default '',
  `privilege` text NOT NULL,
  `loginip` varchar(20) NOT NULL default '',
  `isfounder` smallint(1) unsigned NOT NULL default '0',
  `expire` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`),
  UNIQUE KEY `uid` (`uid`),
  KEY `expire` (`expire`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='管理员在线表';

--
-- 导出表中的数据 `rd8_read8_sessions_acp`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_shop`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_shop` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `book_id` int(10) unsigned NOT NULL default '0',
  `chapter_id` int(10) unsigned NOT NULL default '0',
  `user_id` int(10) unsigned NOT NULL default '0',
  `price` int(10) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `book_id` (`book_id`),
  KEY `chapter_id` (`chapter_id`),
  KEY `user_id` (`user_id`),
  KEY `price` (`price`),
  KEY `dateline` (`dateline`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='VIP 购买记录';

--
-- 导出表中的数据 `rd8_read8_shop`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_story`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_story` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `title` varchar(255) NOT NULL default '',
  `author` varchar(255) NOT NULL default '',
  `typewriter` mediumint(8) unsigned NOT NULL default '0',
  `subject` tinyint(1) unsigned NOT NULL default '0',
  `content` tinyint(1) unsigned NOT NULL default '0',
  `description` varchar(255) NOT NULL default '',
  `tag` varchar(255) NOT NULL default '',
  `body` mediumtext NOT NULL,
  `state` tinyint(1) unsigned NOT NULL default '1',
  `dateline` int(10) unsigned NOT NULL default '0',
  `lastupdate` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `title` (`title`),
  KEY `author` (`author`),
  KEY `typewriter` (`typewriter`),
  KEY `subject` (`subject`),
  KEY `content` (`content`),
  KEY `description` (`description`),
  KEY `tag` (`tag`),
  KEY `state` (`state`),
  KEY `dateline` (`dateline`),
  KEY `lastupdate` (`lastupdate`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='短篇文学';

--
-- 导出表中的数据 `rd8_read8_story`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_story_commend`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_story_commend` (
  `commend` tinyint(1) unsigned NOT NULL default '0',
  `book_id` int(10) unsigned NOT NULL default '0',
  `dateline` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`commend`,`book_id`),
  KEY `dateline` (`dateline`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='短篇文学推荐';

--
-- 导出表中的数据 `rd8_read8_story_commend`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_story_comment`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_story_comment` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `username` varchar(255) NOT NULL default '',
  `title` varchar(255) NOT NULL default '',
  `content` text NOT NULL,
  `ip` int(10) unsigned NOT NULL default '0',
  `digest` tinyint(1) unsigned NOT NULL default '0',
  `sticky` tinyint(1) unsigned NOT NULL default '0',
  `highlight` varchar(255) NOT NULL default '',
  `dateline` int(10) unsigned NOT NULL default '0',
  `active` tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (`book_id`,`id`),
  KEY `uid` (`uid`),
  KEY `title` (`title`),
  KEY `ip` (`ip`),
  KEY `digest` (`digest`),
  KEY `sticky` (`sticky`),
  KEY `highlight` (`highlight`),
  KEY `dateline` (`dateline`),
  KEY `active` (`active`),
  FULLTEXT KEY `content` (`content`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='短篇文学评论';

--
-- 导出表中的数据 `rd8_read8_story_comment`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_story_external`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_story_external` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `click_today` int(10) unsigned NOT NULL default '0',
  `click_week` int(10) unsigned NOT NULL default '0',
  `click_month` int(10) unsigned NOT NULL default '0',
  `click_total` int(10) unsigned NOT NULL default '0',
  `vote_today` int(10) unsigned NOT NULL default '0',
  `vote_week` int(10) unsigned NOT NULL default '0',
  `vote_month` int(10) unsigned NOT NULL default '0',
  `vote_total` int(10) unsigned NOT NULL default '0',
  `rate_week` int(10) unsigned NOT NULL default '0',
  `rate_month` int(10) unsigned NOT NULL default '0',
  `rate_total` int(10) unsigned NOT NULL default '0',
  `favo_today` int(10) unsigned NOT NULL default '0',
  `favo_week` int(10) unsigned NOT NULL default '0',
  `favo_month` int(10) unsigned NOT NULL default '0',
  `favo_total` int(10) unsigned NOT NULL default '0',
  `commend_today` int(10) unsigned NOT NULL default '0',
  `commend_week` int(10) unsigned NOT NULL default '0',
  `commend_month` int(10) unsigned NOT NULL default '0',
  `commend_total` int(10) unsigned NOT NULL default '0',
  `digg_today` int(10) unsigned NOT NULL default '0',
  `digg_week` int(10) unsigned NOT NULL default '0',
  `digg_month` int(10) unsigned NOT NULL default '0',
  `digg_total` int(10) unsigned NOT NULL default '0',
  `bury_today` int(10) unsigned NOT NULL default '0',
  `bury_week` int(10) unsigned NOT NULL default '0',
  `bury_month` int(10) unsigned NOT NULL default '0',
  `bury_total` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`book_id`),
  KEY `click_week` (`click_week`),
  KEY `click_month` (`click_month`),
  KEY `click_total` (`click_total`),
  KEY `vote_week` (`vote_week`),
  KEY `vote_month` (`vote_month`),
  KEY `vote_total` (`vote_total`),
  KEY `rate_week` (`rate_week`),
  KEY `rate_month` (`rate_month`),
  KEY `rate_total` (`rate_total`),
  KEY `favo_week` (`favo_week`),
  KEY `favo_month` (`favo_month`),
  KEY `favo_total` (`favo_total`),
  KEY `commend_week` (`commend_week`),
  KEY `commend_month` (`commend_month`),
  KEY `commend_total` (`commend_total`),
  KEY `digg_week` (`digg_week`),
  KEY `digg_month` (`digg_month`),
  KEY `digg_total` (`digg_total`),
  KEY `bury_week` (`bury_week`),
  KEY `bury_month` (`bury_month`),
  KEY `bury_total` (`bury_total`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='短篇文学扩展数据';

--
-- 导出表中的数据 `rd8_read8_story_external`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_story_rate`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_story_rate` (
  `book_id` int(10) unsigned NOT NULL default '0',
  `rate` tinyint(1) unsigned NOT NULL default '0',
  KEY `book_id` (`book_id`),
  KEY `rate` (`rate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='短篇文学评分';

--
-- 导出表中的数据 `rd8_read8_story_rate`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_system`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_system` (
  `config` varchar(20) NOT NULL default '',
  `index` varchar(50) NOT NULL default '',
  `value` mediumtext NULL,
  PRIMARY KEY  (`config`,`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='系统设置';

--
-- 导出表中的数据 `rd8_read8_system`
--

INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'SITE_NAME', '');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'SITE_ADDR', '');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'BBS_URL', 'http://bbs.read8.net');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'copyright', '版权所有&nbsp;&copy;&nbsp;{NOW_YEAR} <a href="{SITE_ADDR}">{SITE_NAME}</a>');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'copyright_system', '技术支持：{read8cn}  版本：{read8ver}');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'copyright_system_sn', '');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'miibeian', '');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'TIME_ZONE', '+8');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'TIME_DELAY', '0');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'open', '1');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'message', '网站升级中，请稍候访问');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'api', 'none');
INSERT INTO `rd8_read8_system` VALUES('SYSTEM', 'task', '0');

INSERT INTO `rd8_read8_system` VALUES('USER', 'open', '1');
INSERT INTO `rd8_read8_system` VALUES('USER', 'verify', '0');
INSERT INTO `rd8_read8_system` VALUES('USER', 'minlength', '3');
INSERT INTO `rd8_read8_system` VALUES('USER', 'maxlength', '12');
INSERT INTO `rd8_read8_system` VALUES('USER', 'favorite', '30');

INSERT INTO `rd8_read8_system` VALUES('AUTHOR', 'verify', '1');
INSERT INTO `rd8_read8_system` VALUES('AUTHOR', 'size', '51200');
INSERT INTO `rd8_read8_system` VALUES('AUTHOR', 'commend', '20');
INSERT INTO `rd8_read8_system` VALUES('AUTHOR', 'link', '20');

INSERT INTO `rd8_read8_system` VALUES('READ', 'fulltext', '1');
INSERT INTO `rd8_read8_system` VALUES('READ', 'fulltext_guest', '0');
INSERT INTO `rd8_read8_system` VALUES('READ', 'download_package', '1');
INSERT INTO `rd8_read8_system` VALUES('READ', 'download_package_guest', '0');
INSERT INTO `rd8_read8_system` VALUES('READ', 'download_text', '1');
INSERT INTO `rd8_read8_system` VALUES('READ', 'download_text_guest', '0');

INSERT INTO `rd8_read8_system` VALUES('COMMENT', 'open', '1');
INSERT INTO `rd8_read8_system` VALUES('COMMENT', 'chapter', '1');
INSERT INTO `rd8_read8_system` VALUES('COMMENT', 'guest', '0');
INSERT INTO `rd8_read8_system` VALUES('COMMENT', 'minlength', '8');
INSERT INTO `rd8_read8_system` VALUES('COMMENT', 'maxlength', '10000');

INSERT INTO `rd8_read8_system` VALUES('DISPLAY', 'template', 'basic');
INSERT INTO `rd8_read8_system` VALUES('DISPLAY', 'statcode', '');
INSERT INTO `rd8_read8_system` VALUES('DISPLAY', 'speedtrace', '1');
INSERT INTO `rd8_read8_system` VALUES('DISPLAY', 'announce', '8');

INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'open', '1');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'size', '51200');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'format', '15');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'watermark_text', 'http://www.read8.net');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'watermark', '9');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'watermark_type', '0');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'watermark_color', '0 0 0');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'watermark_image', 'images/watermark.png');
INSERT INTO `rd8_read8_system` VALUES('UPLOAD', 'watermark_alpha', '65');

INSERT INTO `rd8_read8_system` VALUES('FOPEN', 'emulate', '');
INSERT INTO `rd8_read8_system` VALUES('FOPEN', 'curl', '');
INSERT INTO `rd8_read8_system` VALUES('FOPEN', 'agent', '');
INSERT INTO `rd8_read8_system` VALUES('FOPEN', 'proxy', '');
INSERT INTO `rd8_read8_system` VALUES('FOPEN', 'cookie', '');

INSERT INTO `rd8_read8_system` VALUES('SEO', 'htmlpath', 'html');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'htmlsubdir', 'id');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'htmlsubdirdateformat', 'Ymd');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'title', '');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'keywords', '');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'description', '');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'novel', '0');
INSERT INTO `rd8_read8_system` VALUES('SEO', 'author', '0');

INSERT INTO `rd8_read8_system` VALUES('SAFE', 'secode', '0');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'secode_length', '5');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'acp_expire', '120');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'acp_pswd_minlength', '8');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'acp_pswd_expire', '30');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'acp_pswd_change', '2');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'acp_pswd_history', '20');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'chmod_dir', '420');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'chmod_file', '493');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'prefix', 'R35f5gj8w');
INSERT INTO `rd8_read8_system` VALUES('SAFE', 'code', '1w748b74H945h74U');

INSERT INTO `rd8_read8_system` VALUES('INFO', 'html', '1,0,0,0,0,0,0,0');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'about', '本书网由澳柯网信科技发展有限公司旗下的读吧书网提供程序及技术支持。\r\n\r\n<a href="http://www.aokosoft.com" rel="external"><img src="http://www.aokosoft.com/img/aokosoft.png"></a>\r\n\r\n<a href="http://www.read8.net" rel="external"><img src="http://www.read8.net/images/logo1.gif"></a>\r\n\r\n<a href="http://www.read8.net" rel="external">www.Read8.net</a>提供技术支持。\r\n\r\n如需下载和使用书网整站程序请浏览我们的产品网站：读吧书网官方网站。');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'guide', '本站是一个文学爱好者和在线阅读的最佳书网，丰富的功能和完善的vip，将给您带来前所未有的速度和喜悦。\r\n\r\n欢迎访问本书网，若有问题，请联系站长。');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'contact', '广告联系、作者咨询、版权事宜联系本站（请站长自行修改此段信息）\r\n\r\n书网技术支持&书网程序购买：www.read8.net\r\n\r\n读吧书网合作咨询电话：13731199020(仅接受电话) 13810291982(仅接受短信) 郑先生。');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'pay', '支付说明:  \r\n	1元人民币=100虚拟币。会员最低支付款额为10元。建议在汇款后加一位小数，方便核对。\r\n	在线支付购买了本站的虚拟币之后，您可以自由在我们站内定购任何VIP服务。\r\n\r\n在线支付说明:\r\n	支持几十种国内外银行卡的在线支付功能，可实时增加您账上的金额，快捷方便。\r\n	选择好购买类别后点“确认购买”，在确定后转到银行网关选择您的银行卡并支付。\r\n	银行支付确认后会自动转回本站，此时交易结束，你可以检查你的本站币是否到帐。\r\n	支付过程中如遇到网络速');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'fee', '兑换说明：\r\n	兑换金额必须大于人民币100元，且必须是它的整数倍，否则不予受理。\r\n	对于兑换，在申请之前必须在作家保密资料中详细填写您的汇款方式，否则无法受理。');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'novel', '新书申请说明：\r\n	欢迎发布新书，请务必填好相应选项。尤其是书籍类型这个不可更改选项。\r\n	对于作品的授权类型，请慎重选择，谨防侵权。\r\n	以上注意填写错误的，恕不审批。');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'author', '作家申请说明：\r\n	欢迎申请驻站作家。成为我们的驻站作家，当您的书籍电子章节被他人购买阅读时，您就可以收到相应的反馈分成。请附上5000字的小说以便我们审阅。\r\n	以上注意填写错误的，恕不审批。');
INSERT INTO `rd8_read8_system` VALUES('INFO', 'eula', '用户注册说明：\r\n	欢迎访问本站。请自律，勿发布不良信息，遵守道德规范。\r\n\r\n	欢迎您加入我们的大家庭。这里是一方爱书的天空。\r\n\r\n			   To be a better man.\r\n\r\n	更多协议请修改后台信息设置。');


INSERT INTO `rd8_read8_system` VALUES('RANK', 'click_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'click_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'click_total_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'vote_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'vote_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'vote_total_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'rate_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'rate_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'rate_total_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'favo_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'favo_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'favo_total_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'commend_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'commend_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'commend_total_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'digg_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'digg_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'digg_total_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'bury_week_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'bury_month_story', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'bury_total_story', '10');

INSERT INTO `rd8_read8_system` VALUES('RANK', 'bury_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'bury_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'bury_week_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'digg_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'digg_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'digg_week_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'commend_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'commend_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'commend_week_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'favo_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'favo_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'favo_week_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'rate_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'rate_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'rate_week_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'vote_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'vote_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'vote_week_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'click_total_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'click_month_novel', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'click_week_novel', '10');

INSERT INTO `rd8_read8_system` VALUES('RANK', 'novel_1', '6');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'novel_2', '3');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'novel_3', '3');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'novel_4', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'novel_5', '10');

INSERT INTO `rd8_read8_system` VALUES('RANK', 'story_1', '6');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'story_2', '3');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'story_3', '3');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'story_4', '10');
INSERT INTO `rd8_read8_system` VALUES('RANK', 'story_5', '10');

INSERT INTO `rd8_read8_system` VALUES('OFFICIAL', 'username', '');
INSERT INTO `rd8_read8_system` VALUES('OFFICIAL', 'password', '');

INSERT INTO `rd8_read8_system` VALUES('STORY', 'open', '1');
INSERT INTO `rd8_read8_system` VALUES('STORY', 'verify', '0');

INSERT INTO `rd8_read8_system` VALUES('PAY', 'ali_seller_email', 'zairwolf@163.com');
INSERT INTO `rd8_read8_system` VALUES('PAY', 'ali_partner', '2088002029290264');
INSERT INTO `rd8_read8_system` VALUES('PAY', 'ali_sign', 'kuip37avpflqhb94zdbxq79cd6jlacti');
INSERT INTO `rd8_read8_system` VALUES('PAY', 'unit', '读吧币');
INSERT INTO `rd8_read8_system` VALUES('PAY', 'rate', '100');

INSERT INTO `rd8_read8_system` VALUES('MOBILE', 'username', '');
INSERT INTO `rd8_read8_system` VALUES('MOBILE', 'password', '');
INSERT INTO `rd8_read8_system` VALUES('MOBILE', 'novel_update', '1');
INSERT INTO `rd8_read8_system` VALUES('MOBILE', 'novel_comment', '1');

INSERT INTO `rd8_read8_system` VALUES('VIP', 'rebate', '0.7');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'expire', '180');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'price', '3');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'copyright', '仅供 [rd8vip] 阅读');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'copyright_font', 'simhei.ttf');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'copyright_size', '12');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'line', '100');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'font', 'simhei.ttf');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'size', '12');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'background', '255 255 255');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'background_alpha', '127');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'color', '0 0 0');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'mark_font', 'simhei.ttf');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'mark_size', '12');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'mark_color', '255 0 0');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'buy_picture', 'images/purchase.jpg');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'area_buy', '214,176,314,202');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'area_login', '65,176,148,202');
INSERT INTO `rd8_read8_system` VALUES('VIP', 'area_pay', '380,176,463,202');

-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_user`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_user` (
  `uid` mediumint(8) NOT NULL auto_increment,
  `username` varchar(255) NOT NULL default '',
  `pwd` varchar(32) NOT NULL default '',
  `email` varchar(255) NOT NULL default '',
  `credit` int(10) NOT NULL default '0',
  `vmoney` int(10) NOT NULL default '0',
  `sms` int(10) NOT NULL default '0',
  `dateline` int(10) NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`uid`),
  UNIQUE KEY `username` (`username`),
  KEY `dateline` (`dateline`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='用户';

--
-- 导出表中的数据 `rd8_read8_user`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_user_acp`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_user_acp` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `name` varchar(255) NOT NULL default '',
  `pswd` varchar(32) NOT NULL default '',
  `privilege` mediumtext NOT NULL,
  `lastlogin` int(10) unsigned NOT NULL default '0',
  `lastloginip` varchar(20) NOT NULL default '',
  `adduser` mediumint(8) unsigned NOT NULL default '0',
  `accessip` text NOT NULL,
  `error` smallint(1) unsigned NOT NULL default '0',
  `isfounder` smallint(1) unsigned NOT NULL default '0',
  `history` text NOT NULL,
  `changepswdtime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`),
  UNIQUE KEY `uid` (`uid`),
  KEY `pswd` (`pswd`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='管理员';

--
-- 导出表中的数据 `rd8_read8_user_acp`
--


-- --------------------------------------------------------

--
-- 表的结构 `rd8_read8_user_typewriter`
--

CREATE TABLE IF NOT EXISTS `rd8_read8_user_typewriter` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `uid` mediumint(8) unsigned NOT NULL default '0',
  `novel_add` tinyint(1) unsigned NOT NULL default '0',
  `novel_edit` tinyint(1) unsigned NOT NULL default '0',
  `novel_del` tinyint(1) NOT NULL default '0',
  `chapter_add` tinyint(1) unsigned NOT NULL default '0',
  `chapter_edit` tinyint(1) unsigned NOT NULL default '0',
  `chapter_del` tinyint(1) NOT NULL default '0',
  `overstep` tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `uid` (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='录入员';

--
-- 导出表中的数据 `rd8_read8_user_typewriter`
--

