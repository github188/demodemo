<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin edit group.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
@set_time_limit(0);
@ini_set('memory_limit', -1);

require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();

sysXajaxRegister("xAdminAddGroup,xAdminEditGroup");

/* Get User List and Project List */
$UserList = testGetUserList();
uasort($UserList, 'testCmpPreAppendName');

$ActionType = $_GET['ActionType'];
if($ActionType != 'EditGroup')
{
    $ActionType = 'AddGroup';
}
else
{
    /* Get group info */
    $GroupInfo = array_pop(testGetGroupList("GroupID='{$_GET[GroupID]}'"));
    if(!$_SESSION['TestIsAdmin'] && !preg_match('/,'. mysql_real_escape_string($_SESSION['TestUserName']) . ',/', $GroupInfo['GroupManagers']) && $_SESSION['TestUserName'] != $GroupInfo['AddedBy'])
    {
        sysErrorMsg();
    }

    /* Delet Group */
    if($_GET['DropGroupID'] > 1)
    {
        dbDeleteRow('TestGroup', "GroupID = '$_GET[DropGroupID]'");
        $BackUrl = $_SERVER['HTTP_REFERER'] ? $_SERVER['HTTP_REFERER'] : 'AdminGroupList.php';
        jsGoTo($BackUrl);
        exit;
    }
    
    uasort($GroupInfo['GroupUserList'], 'testCmpPreAppendName');
    uasort($GroupInfo['GroupManagerList'], 'testCmpPreAppendName');
    
    $GroupInfo['GroupUserNames'] = join(',', array_keys($GroupInfo['GroupUserList']));
    $GroupInfo['GroupManagerNames'] = join(',', array_keys($GroupInfo['GroupManagerList']));
    $GroupInfo['GroupProjectIDs'] = join(',', array_keys($GroupInfo['GroupProjectList']));
}

/* Create select html code */
$ACLAttrib = 'multiple="multiple" size="8" class="MultiSelect" style="width:220px"';
$SelectUserList = htmlSelect($UserList, 'UserList', '', '', $ACLAttrib, "UserName,PreAppendName");
$SelectUserList2 = htmlSelect($UserList, 'UserList2', '', '', $ACLAttrib, "UserName,PreAppendName");
$SelectGroupUserList = htmlSelect($GroupInfo['GroupUserList'], 'GroupUserList', '', '', $ACLAttrib);
$SelectGroupManagerList = htmlSelect($GroupInfo['GroupManagerList'], 'GroupManagerList', '', '', $ACLAttrib);

/* Tpl assign */
$TPL->assign('GroupInfo', $GroupInfo);
$TPL->assign('UserList', $SelectUserList);
$TPL->assign('GroupManagerList', $SelectGroupManagerList);
$TPL->assign('UserList2', $SelectUserList2);
$TPL->assign('GroupUserList', $SelectGroupUserList);

/* Display the template file. */
$TPL->assign('NavActiveGroup', ' class="Active"');
$TPL->assign('ActionType', $ActionType);
$TPL->display("Admin/Group.tpl");
?>
