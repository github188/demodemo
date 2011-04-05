<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * index.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

/* Set the URL to display in RightBottom. */
$TestMode = in_array($_GET['TestMode'], array('Bug','Case','Result')) ? $_GET['TestMode'] : $_SESSION['TestMode'];
$TestMode =  $TestMode == '' ? 'Bug' : $TestMode;
$_SESSION['TestMode'] = $TestMode;

/* Assign. */
$TPL->assign('TestMode', $TestMode);
$TPL->assign('CloseLeftFrame', $_COOKIE['CloseLeftFrame']);

/* Display. */
$TPL->display('Index.tpl');
?>
