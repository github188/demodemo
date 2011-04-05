<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * list group.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();

if($_REQUEST['reset'])
{
   $_SESSION['SearchGroup']='';
}



$Where = '';
$Where = '1';

if(isset($_GET['SearchGroup']))
{
    $SearchGroup = trim($_GET['SearchGroup']);
    $_SESSION['SearchGroup'] =  $SearchGroup;
}

if($SearchGroup != '')
{
    $Where .= " AND ( BINARY GroupName like '%{$SearchGroup}%') ";
}
else
{
    if($_SESSION['SearchGroup'] != '')
    {
       $SearchGroup =  $_SESSION['SearchGroup'];
       $Where .= " AND ( BINARY GroupName like '%{$SearchGroup}%')";
    }
}

/* Get pagination */
$Pagination = new Page('TestGroup', '', '', '', 'WHERE ' . $Where . ' ORDER BY GroupID DESC', '', $MyDB);
$LimitNum = $Pagination->LimitNum();

/* Get group list */
$GroupList = testGetGroupList($Where, 'GroupID DESC', $LimitNum);
foreach($GroupList as $key => $GroupInfo)
{
    if($_SESSION['TestIsAdmin'] || (preg_match('/,'. mysql_real_escape_string($_SESSION['TestUserName']) . ',/', $GroupInfo['GroupManagers'])) || $_SESSION['TestUserName'] == $GroupInfo['AddedBy'])
    {
        $GroupList[$key]['IsEditable'] = true;
    }
    else
    {
        $GroupList[$key]['IsEditable'] = false;
    }
}

$UserList = testGetOneDimUserList();

/* Assign */
$TPL->assign('PaginationHtml', $Pagination->show('right', 'margin-right:20px'));
$TPL->assign('GroupList', $GroupList);
$TPL->assign('UserList', $UserList);
$TPL->assign('SearchGroup', $_SESSION['SearchGroup']);

/* Display the template file. */
$TPL->assign('NavActiveGroup', ' class="Active"');
$TPL->display('Admin/GroupList.tpl');
?>
