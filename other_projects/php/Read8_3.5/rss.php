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

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

header('Content-Type: application/rss+xml; charset='.SYSCHARSET);

echo '<'.'?xml version="1.0" encoding="'.SYSCHARSET.'"?'.'>';
?>
<rss version="2.0">
	<channel>
		<title><?php echo html_show($_SYSTEM['SYSTEM']['SITE_NAME']); ?></title>
		<link><?php echo $_SYSTEM['SYSTEM']['SITE_ADDR']; ?>/</link>
		<description><?php echo html_show($_SYSTEM['SEO']['description']); ?></description>
		<language>zh-cn</language>
		<copyright><![CDATA[<?php echo   str_replace(array(
									'{SITE_NAME}',
									'{SITE_ADDR}',
									'{BBS_URL}',
									'{NOW_YEAR}',
								), array(
									html_show($_SYSTEM['SYSTEM']['SITE_NAME']),
									$_SYSTEM['SYSTEM']['SITE_ADDR'],
									$_SYSTEM['SYSTEM']['BBS_URL'],
									rdate(TIMESTAMP, 'Y'),
								), $_SYSTEM['SYSTEM']['copyright']); ?>]]></copyright>
		<generator>读吧书网 <?php echo SYS_VERSION; ?></generator>
		<powered><![CDATA[<?php echo str_replace(array(
								'{read8cn}',
								'{read8en}',
								'{read8ver}',
							), array(
								'<a href="http://www.read8.net" rel="external" id="by">读吧书网</a>',
								'<a href="http://www.read8.net" rel="external" id="by">Read8.Net</a>',
								SYS_VERSION,
							), $_SYSTEM['SYSTEM']['copyright_system']); ?>]]></powered>
		<image>
			<url>images/logo.gif</url>
			<title><?php echo html_show($_SYSTEM['SYSTEM']['SITE_NAME']); ?></title>
			<link><?php echo $_SYSTEM['SYSTEM']['SITE_ADDR']; ?></link>
		</image>
<?php
include _cache('rss', 3600);

foreach($_CACHE['rss'] as $feed) {
?>
		<item>
			<title><?php echo $feed['title'] ?></title>
			<description><![CDATA[<?php echo $feed['intro']; ?>]]></description>
			<link><?php echo novel_url($feed); ?></link>
			<pubDate><?php echo gmdate(DATE_FORMAT, $feed['lastupdate']).' GMT'; ?></pubDate>
			<author><?php echo $feed['author']; ?></author>
		</item>
<?php
}
?>
	</channel>
</rss>

