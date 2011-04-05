<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin edit user.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();
sysXajaxRegister("xAdminAddUser,xAdminEditUser");

$ActionType = $_GET['ActionType'];
if($ActionType != 'EditUser')
{
    $ActionType = 'AddUser';
}
else
{
    $UserInfo = dbGetRow('TestUser', '', "UserID = '{$_GET[UserID]}'");
    $GroupACL = dbGetList('TestGroup', '', "GroupUser LIKE '%," . mysql_real_escape_string(mysql_real_escape_string($UserInfo['UserName'])) . ",%'");
    $UserGroupList = array();
    foreach($GroupACL as $Key => $GroupInfo)
    {
        $UserGroupList[$GroupInfo['GroupID']] = $GroupInfo['GroupName'];
    }

    if(isset($_GET['IsDroped']))
    {
        if($_GET['IsDroped'] == '0')
        {
            /* Update group info*/
            foreach($GroupACL as $GroupInfo)
            {
                $NewGroupUsers = str_replace(',' . $UserInfo['UserName'], '' , $GroupInfo['GroupUser']);
                if($NewGroupUsers == ',') $NewGroupUsers = '';
                $NewGroupUsers = mysql_real_escape_string($NewGroupUsers);
                dbUpdateRow('TestGroup', 'GroupUser', "'{$NewGroupUsers}'",  "GroupID = '{$GroupInfo[GroupID]}'");                   

                $NewGroupManagers = str_replace(',' . $UserInfo['UserName'], '' , $GroupInfo['GroupManagers']);
                if($NewGroupManagers == ',') $NewGroupManagers = '';
                $NewGroupManagers = mysql_real_escape_string($NewGroupManagers);
                dbUpdateRow('TestGroup', 'GroupManagers', "'{$NewGroupManagers}'",  "GroupID = '{$GroupInfo[GroupID]}'");                   

            } 

            /* update project info*/
            $ProjectAdminACL = dbGetList('TestProject', '', "ProjectManagers LIKE '%," . mysql_real_escape_string(mysql_real_escape_string($UserInfo['UserName'])) . ",%'");
            foreach($ProjectAdminACL as $ProjectInfo)
            {
                $NewProjectManagers = str_replace(',' . $UserInfo['UserName'], '' , $ProjectInfo['ProjectManagers']);
                if($NewProjectManagers == ',') $NewProjectManagers = '';
                $NewProjectManagers = mysql_real_escape_string($NewProjectManagers);
                dbUpdateRow('TestProject', 'ProjectManagers', "'{$NewProjectManagers}'",  "ProjectID = '{$ProjectInfo[ProjectID]}'");                   
            } 
            
            dbUpdateRow('TestUser', 'IsDroped', "'1'", 'LastEditedBy', "'{$_SESSION[TestUserName]}'", 'LastDate', 'now()', "UserID = '{$_GET[UserID]}'");
        }
        elseif($_GET['IsDroped'] == '1')
        {
            dbUpdateRow('TestUser', 'IsDroped', "'0'", 'LastEditedBy', "'{$_SESSION[TestUserName]}'", 'LastDate', 'now()', "UserID = '{$_GET[UserID]}'");
        }
    
        // add update group
        $BackUrl = $_SERVER['HTTP_REFERER'] ? $_SERVER['HTTP_REFERER'] : 'AdminUserList.php';
        jsGoTo($BackUrl);
        exit;
    }
}

/* Create select html code */
$GroupList = testGetGroupList('GroupID <> 1');
$ACLAttrib = 'multiple="multiple" size="8" class="MultiSelect"';
$UserInfo['AuthModeName'] = $_LANG['AuthMode'][$UserInfo['AuthMode']];
$TPL->assign('UserInfo', $UserInfo);

/* Display the template file. */
$TPL->assign('NavActiveUser', ' class="Active"');
$TPL->assign('ActionType', $ActionType);
$TPL->display('Admin/User.tpl');
?>
