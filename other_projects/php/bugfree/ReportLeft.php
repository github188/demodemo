<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * report left sripts.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

$ReportMode = $_GET['ReportMode'] == '' ? 'Bug' : $_GET['ReportMode'];

$TPL->assign('ReportMode', $ReportMode);
$TPL->assign('ReportTypeList', $_LANG["{$ReportMode}ReportType"]);
$TPL->assign('ReportTypeListChecked', @array_flip($_SESSION[$ReportMode.'ReportTypeList']));

/* Display. */
$TPL->display('ReportLeft.tpl');
?>
