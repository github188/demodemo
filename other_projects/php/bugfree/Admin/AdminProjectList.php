<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin project list.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();

if($_REQUEST['reset'])
{
   $_SESSION['SearchProject']='';
}

$Where = '';
if($_SESSION['TestIsAdmin'])
{
    $Where = '1';
}
elseif($_SESSION['TestIsProjectAdmin'])
{
    $Where = " ProjectManagers LIKE '%," . mysql_real_escape_string(mysql_real_escape_string($_SESSION['TestUserName'])) . ",%'";
}

if(isset($_GET['SearchProject']))
{
    $SearchProject = trim($_GET['SearchProject']);
    $_SESSION['SearchProject'] =  $SearchProject;
}

if($SearchProject != '')
{
    $Where .= " AND ( BINARY ProjectName like '%{$SearchProject}%') ";
}
else
{
    if($_SESSION['SearchProject'] != '')
    {
       $SearchProject =  $_SESSION['SearchProject'];
       $Where .= " AND ( BINARY ProjectName like '%{$SearchProject}%')";
    }
}

/* Get pagination */
$Pagination = new Page('TestProject', '', '', '', 'WHERE ' . $Where . ' ORDER BY DisplayOrder DESC, ProjectID DESC', '', $MyDB);
$LimitNum = $Pagination->LimitNum();

/* Get project list */
$ProjectList = testGetProjectList($Where, ' DisplayOrder DESC, ProjectID DESC', $LimitNum);
$UserList = testGetUserList();

/* Get project list */
foreach($ProjectList as $ProjectID => $ProjectInfo)
{
    $GroupACL = dbGetList('TestGroup', '', "GroupID " . dbCreateIN($ProjectInfo['ProjectGroupIDs']));
    $ProjectGroupList = array();
    foreach($GroupACL as $Key => $GroupInfo)
    {
        $ProjectGroupList[$GroupInfo['GroupID']] = $GroupInfo['GroupName'];
    }
    asort($ProjectGroupList);
    $ProjectManagerNameList = array();
    foreach($ProjectInfo['ProjectManagerList'] as $UserName)
    {
        $ProjectManagerNameList[$UserName] = $UserList[$UserName]['PreAppendName'];
    }
    asort($ProjectManagerNameList);
    $ProjectList[$ProjectID]['ProjectGroupListHTML'] = htmlSelect($ProjectGroupList, 'ProjectGroupList','', '', 'class="FullSelect"');
    $ProjectList[$ProjectID]['ProjectManagerListHTML'] = htmlSelect($ProjectManagerNameList, 'ProjectManagerNameList','', '', 'class="FullSelect"');
    $ProjectList[$ProjectID]['LastEditedByName'] = $UserList[$ProjectInfo['LastEditedBy']]['RealName'];
    $ProjectList[$ProjectID]['AddedByName'] = $UserList[$ProjectInfo['AddedBy']]['RealName'];
}

/* Assign */
$TPL->assign('PaginationHtml', $Pagination->show('right', 'margin-right:20px'));
$TPL->assign('ProjectList', $ProjectList);
$TPL->assign('UserList', $UserList);
$TPL->assign('SearchProject', $_SESSION['SearchProject']);
/* Display the template file. */
$TPL->assign('NavActivePro', ' class="Active"');
$TPL->display('Admin/ProjectList.tpl');
?>
