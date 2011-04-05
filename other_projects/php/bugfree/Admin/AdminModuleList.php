<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin module list.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();

sysXajaxRegister("xProjectSetSlaveModule,xSetModuleOwner,xProjectSetAssignedUser,xAdminAddModule,xAdminEditModule");

$ProjectID = $_GET['ProjectID'];
if($_SESSION['TestIsProjectAdmin'])
{
    $ProjectInfo = current(testGetProjectList("ProjectID = '{$_GET[ProjectID]}' AND ProjectManagers LIKE '%," . mysql_real_escape_string(mysql_real_escape_string($_SESSION['TestUserName'])) . ",%'"));
}
elseif($_SESSION['TestIsAdmin'])
{
    $ProjectInfo = current(testGetProjectList("ProjectID = '{$_GET[ProjectID]}'"));
}
if(empty($ProjectInfo))
{
    sysErrorMsg();
}
$ModuleID = $_GET['ModuleID'];
$ModuleType = $_GET['ModuleType'];
$ModuleType == "Bug" ? $ModuleType = "Bug" : $ModuleType = "Case";

$ModuleInfo = testGetModuleInfo($ModuleID);

$ModuleTree = testGetAdminTreeModuleList($ProjectID, '?', $ModuleType);

$OnChangeStr = 'onchange="';
$OnChangeStr .= 'xajax_xProjectSetSlaveModule(this.value, \'SlaveModuleList\', \'ParentModuleID\', \''. $ModuleType .'\');xajax_xProjectSetAssignedUser(this.value, \'AssignedToUserList\', \'EditModuleOwner\');';
$OnChangeStr .= '"';
$SelectProjectList = testGetAllProjectSelectList('ProjectID', $ProjectID, $OnChangeStr);

// ModuleList for edit moudle
$OnChangeStr = 'onchange="';
$OnChangeStr .= 'xajax_xSetModuleOwner(this.value, \'EditModuleOwner\');';
$OnChangeStr .= '"';
$SelectEditModuleList = testGetSelectModuleList($ProjectID, 'ParentModuleID', $ModuleInfo['ParentID'], $OnChangeStr, $ModuleType);

// ModuleList for add module
$OnChangeStr = 'onchange="';
$OnChangeStr .= 'xajax_xSetModuleOwner(this.value, \'AddModuleOwner\');';
$OnChangeStr .= '"';
$SelectAddModuleList = testGetSelectModuleList($ProjectID, 'ParentModuleID', $ModuleID, $OnChangeStr, $ModuleType);

$ProjectUserList = testGetProjectUserList($ProjectID, true);

// UserList for edit module
$SelectEditModuleUserList = htmlSelect($ProjectUserList, 'EditModuleOwner', '',$ModuleInfo['ModuleOwner'], 'class="NormalSelect"');

// UserList for add module
$SelectAddModuleUserList = htmlSelect($ProjectUserList, 'AddModuleOwner', '',$ModuleInfo['ModuleOwner'], 'class="NormalSelect"');

/* Assign */
$TPL->assign('ModuleID', $ModuleID);
$TPL->assign('ProjectID', $ProjectID);
$TPL->assign('ModuleType', $ModuleType);
$TPL->assign('ProjectInfo', $ProjectInfo);
$TPL->assign('ModuleInfo', $ModuleInfo);
$TPL->assign('ModuleList', $ModuleTree);
$TPL->assign('SelectProjectList', $SelectProjectList);
$TPL->assign('SelectEditModuleList', $SelectEditModuleList);
$TPL->assign('SelectAddModuleList', $SelectAddModuleList);
$TPL->assign('SelectEditModuleUserList', $SelectEditModuleUserList);
$TPL->assign('SelectAddModuleUserList', $SelectAddModuleUserList);

/* Display the template file. */
$TPL->assign('NavActivePro', ' class="Active"');
$TPL->display('Admin/ModuleList.tpl');
?>
