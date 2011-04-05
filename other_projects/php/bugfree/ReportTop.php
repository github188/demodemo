<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * report top scripts.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

$ReportModeList = array('Bug'=>'Bug','Case'=>'Case', 'Result' => 'Result');
$ReportMode = $_GET['ReportMode'];
if(!in_array($ReportMode, $ReportModeList))
{
    $ReportMode = 'Bug';
}
$Attrib = 'onchange="parent.top.location=\'Report.php?ReportMode=\'+this.value' . ';"';
$ReportModeSelect = htmlSelect($ReportModeList, 'ReportMode','', $ReportMode,$Attrib);

$TPL->assign('ReportModeList', $ReportModeSelect);
$TPL->assign('ReportMode', $ReportMode);

/* Display. */
$TPL->display('ReportTop.tpl');
?>
