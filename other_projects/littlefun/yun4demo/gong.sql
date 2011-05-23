DROP TABLE IF EXISTS `sina_gong`;

CREATE TABLE `sina_gong` (
  `uid` int(11) NOT NULL,
  `result` text,
  `update_time` datetime DEFAULT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
