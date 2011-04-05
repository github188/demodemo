<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin add project.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();

sysXajaxRegister("xAdminAddProject,xAdminEditProject");

/* Get UserList and GroupList */
$UserList = testGetUserList();
uasort($UserList, 'testCmpPreAppendName');
$GroupList = testGetGroupList();
//$GroupList = sysSortArray($GroupList,'GroupName');

/* Get ActionType */
$ActionType = $_GET['ActionType'];
if($ActionType != 'EditProject')
{
    baseJudgeAdminUserLogin('SysAdmin');
    $ActionType = 'AddProject';
}
else
{
    if(isset($_GET['IsDroped']))
    {
        baseJudgeAdminUserLogin('SysAdmin');
        $IsDroped = (int)!$_GET['IsDroped'];
        dbUpdateRow('TestProject', 'IsDroped', "'$IsDroped'", 'LastEditedBy', "'" . mysql_real_escape_string($_SESSION['TestUserName']) ."'",'LastDate', 'now()', "ProjectID = {$_GET[ProjectID]}");
        dbUpdateRow('TestModule', 'IsDroped', "'$IsDroped'", "ProjectID = {$_GET[ProjectID]}");
        dbUpdateRow('BugInfo', 'IsDroped', "'$IsDroped'", "ProjectID = {$_GET[ProjectID]}");
        dbUpdateRow('CaseInfo', 'IsDroped', "'$IsDroped'", "ProjectID = {$_GET[ProjectID]}");
        dbUpdateRow('ResultInfo', 'IsDroped', "'$IsDroped'", "ProjectID = {$_GET[ProjectID]}");
        // and update bug ,case, result
        $BackUrl = $_SERVER['HTTP_REFERER'] ? $_SERVER['HTTP_REFERER'] : 'AdminProjectList.php';
        jsGoTo($BackUrl);
        exit;
    }
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
    $ProjectInfo['ProjectGroupNameList'] = array();
    foreach($ProjectInfo['ProjectGroupList'] as $GroupID)
    {
        $ProjectInfo['ProjectGroupNameList'][$GroupID] = $GroupList[$GroupID]['GroupName'];
    }
    asort($ProjectInfo['ProjectGroupNameList']);
    $ProjectInfo['ProjectManagerNameList'] = array();
    foreach($ProjectInfo['ProjectManagerList'] as $UserName)
    {
        $ProjectInfo['ProjectManagerNameList'][$UserName] = $UserList[$UserName]['PreAppendName'];
    }
    uasort($ProjectInfo['ProjectManagerNameList'],'testCmpPreAppendName');
    $ProjectGroupIDs = join(',', array_keys($ProjectInfo['ProjectGroupNameList']));
    $ProjectManagers = join(',', array_keys($ProjectInfo['ProjectManagerNameList']));
    $ProjectInfo['NotifyEmail'] = trim($ProjectInfo['NotifyEmail'], ',');
    $TPL->assign('ProjectInfo', $ProjectInfo);
    $TPL->assign('ProjectGroupIDs', $ProjectGroupIDs);
    $TPL->assign('ProjectManagers', $ProjectManagers);
}


/* Create select html code */
$ACLAttrib = 'multiple="multiple" size="8" class="MultiSelect" style="width:220px"';
$SelectUserAttrib = $ACLAttrib;
if($_SESSION['TestIsProjectAdmin'])
{
    $SelectUserAttrib .= ' disabled="disabled" style="background-color:#fff;"';
}
$SelectUserList = htmlSelect($UserList, 'UserList', '', '', $SelectUserAttrib, "UserName,PreAppendName");
$SelectGroupUserList = htmlSelect($ProjectInfo['ProjectManagerNameList'], 'ProjectManagerList', '', '', $SelectUserAttrib);
$SelectGroupList =htmlSelect($GroupList, 'GroupList', '', '', $ACLAttrib, 'GroupID,GroupName');
$SelectProjectGroupList = htmlSelect($ProjectInfo['ProjectGroupNameList'], 'ProjectGroupList', '', '', $ACLAttrib);

/* Tpl assign */
$TPL->assign('UserList', $SelectUserList);
$TPL->assign('GroupUserList', $SelectGroupUserList);
$TPL->assign('GroupList', $SelectGroupList);
$TPL->assign('ProjectGroupList', $SelectProjectGroupList);
$TPL->assign('TestIsAdmin', $_SESSION['TestIsAdmin']);
$TPL->assign('TestIsProjectAdmin', $_SESSION['TestIsProjectAdmin']);

/* Display the template file. */
$TPL->assign('NavActivePro', ' class="Active"');
$TPL->assign('ActionType', $ActionType);
$TPL->display("Admin/Project.tpl");
?>
