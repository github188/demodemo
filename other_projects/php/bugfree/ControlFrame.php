<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * control frame.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

$TPL->assign('CloseLeftFrame', $_COOKIE['CloseLeftFrame']);

$TPL->display('ControlFrame.tpl');
?>
