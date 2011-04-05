<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * top navigation.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

sysXajaxRegister("xSetTestMode,xSetCurrentProject");

$OnChangeStr = 'onchange="';
$OnChangeStr .= 'xajax_xSetCurrentProject(this.value);parent.LeftTopFrame.location=\'ModuleList.php?ProjectID=\'+idObj(\'TopNavProjectList\').value;';
$OnChangeStr .= 'parent.RightBottomFrame.location=\'' . $_SESSION['TestMode'] . 'List.php?ProjectID=\'+idObj(\'TopNavProjectList\').value;';
$OnChangeStr .= '"';
$TestCurrentProjectID = $_COOKIE['TestCurrentProjectID'] != '' ? $_COOKIE['TestCurrentProjectID'] : $_SESSION['TestCurrentProjectID'];
$ProjectListSelect = testGetValidProjectSelectList('TopNavProjectList', $TestCurrentProjectID, $OnChangeStr);

$TPL->assign('TestRealName', $_SESSION['TestRealName']);
$TPL->assign('TestIsAdmin', $_SESSION['TestIsAdmin']);
$TPL->assign('TestIsProjectAdmin', $_SESSION['TestIsProjectAdmin']);
$TPL->assign('TopNavProjectList', $ProjectListSelect);

$TPL->assign('TestMode', $_SESSION['TestMode']);

$TPL->display('TopNav.tpl');
?>
