<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * edit user info.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("Include/Init.inc.php");

$UserInfo = dbGetRow('TestUser', '', "UserName = '" . mysql_real_escape_string($_SESSION['TestUserName']) . "'");
$isReceiveEmail = ($UserInfo['NoticeFlag'] & 2) == 2 ? true : false;
$isReceiveWangwang = ($UserInfo['NoticeFlag'] & 1) == 1 ? true : false;

$TPL->assign('Disabled', false);
if($UserInfo['AuthMode'] == 'LDAP')
{
    $TPL->assign('Disabled', true);
}
$TPL->assign('UserInfo', $UserInfo);
$TPL->assign('isReceiveEmail', $isReceiveEmail);

$TPL->display('EditMyInfo.tpl');
?>
