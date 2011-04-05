<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin user log list.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin('SysAdmin');

if($_REQUEST['reset'])
{
   $_SESSION['SearchLog']='';
}

$Where = '1 ';

if(isset($_GET['SearchLog']))
{
    $SearchLog = trim($_GET['SearchLog']);
    $_SESSION['SearchLog'] =  $SearchLog;
}

if($SearchLog != '')
{
    $Where .= " AND ( BINARY UserName like '%{$SearchLog}%') ";
}
else
{
    if($_SESSION['SearchLog'] != '')
    {
       $SearchLog =  $_SESSION['SearchLog'];
       $Where .= " AND ( BINARY UserName like '%{$SearchLog}%')";
    }
}


/* Get pagination */
$Pagination = new Page('TestUserLog', '', '', '', 'WHERE ' . $Where . ' ORDER BY LogID DESC', '', $MyDB);
$LimitNum = $Pagination->LimitNum();

/* Get userlog list */
$UserLogList = dbGetList('TestUserLog', '*', $Where, '', 'LogID DESC', $LimitNum);

/* Assign */
$TPL->assign('PaginationHtml', $Pagination->show('right', 'margin-right:20px'));
$TPL->assign('UserLogList', $UserLogList);
$TPL->assign('SearchLog', $_SESSION['SearchLog']);
/* Display the template file. */
$TPL->assign('NavActiveUserLog', ' class="Active"');
$TPL->display('Admin/UserLogList.tpl');
?>
