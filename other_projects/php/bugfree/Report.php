<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * report bug, case, result.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

$ReportMode = $_GET['ReportMode'] == '' ? $_SESSION['TestMode'] : $_GET['ReportMode'];

$TPL->assign('ReportMode', $ReportMode);

/* Display. */
$TPL->assign('HeaderTitle', $ReportMode . ' ' . $_LANG['ReportForms']);
$TPL->display('ReportIndex.tpl');
?>
