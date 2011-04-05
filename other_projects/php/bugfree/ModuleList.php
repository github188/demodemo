<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * list project's modules.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

/* */
if($_GET['ProjectID'] != '')
{
    $_SESSION['TestCurrentProjectID'] = $_GET['ProjectID'];
}
$ProjectID = $_SESSION['TestCurrentProjectID'];
$TestMode = baseGetTestMode();
$ModuleType = $TestMode == 'Bug' ? 'Bug' : 'Case';
$ModuleTree = testGetTreeModuleList($ProjectID, $TestMode . 'List.php?', $ModuleType);

$TPL->assign('ModuleTree', $ModuleTree);

$TPL->display('ModuleList.tpl');
?>
