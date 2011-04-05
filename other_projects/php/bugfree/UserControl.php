<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * user control.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

function xUpdateUserControl($TestMode)
{
    global $_LANG;

    $_SESSION['UpdateUserControl'] = 'UpdateUserControl';
    $LimitNum = '10';

    $UCTestUserName = mysql_real_escape_string($_SESSION['TestUserName']);

    $objResponse = new xajaxResponse();
    sleep(1); //we'll do nothing for two seconds

    $AssignedStr = '';
    $OpenedStr = '';
    $QueryStr = '';

    
    if($TestMode == 'Bug')
    {
        $Columns = 'BugID,BugTitle';
        $Where = "OpenedBy = '{$UCTestUserName}' AND BugStatus <> 'Closed'";
        $Where .= " AND IsDroped = '0' ";
        $OrderBy = 'LastEditedDate DESC';
        $OpenedList = dbGetList('BugInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
        $OpenedList = testSetBugListMultiInfo($OpenedList);

        $Where = "AssignedTo = '{$UCTestUserName}' AND BugStatus <> 'Closed'";
        $Where .= " AND IsDroped = '0' ";
        $AssignedList = dbGetList('BugInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
        $AssignedList = testSetBugListMultiInfo($AssignedList);


        foreach($AssignedList as $Item)
        {
          $AssignedStr .= "{$Item[BugID]}&nbsp;<a href=\"Bug.php?BugID={$Item[BugID]}\" title=\"{$Item[BugTitle]}\" target=\"_blank\">{$Item[UCTitle]}</a><br />";
        }

        foreach($OpenedList as $Item)
        {
          $OpenedStr .= "{$Item[BugID]}&nbsp;<a href=\"Bug.php?BugID={$Item[BugID]}\" title=\"{$Item[BugTitle]}\" target=\"_blank\">{$Item[UCTitle]}</a><br />";
        }
    }
    elseif($TestMode == 'Case')
    {
        $Columns = 'CaseID,CaseTitle';
        $Where = "OpenedBy = '{$UCTestUserName}'";
        $Where .= " AND IsDroped = '0' ";
        $OrderBy = 'LastEditedDate DESC';
        $OpenedList = dbGetList('CaseInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
        $OpenedList = testSetCaseListMultiInfo($OpenedList);

        $Where = "AssignedTo = '{$UCTestUserName}'";
        $Where .= " AND IsDroped = '0' ";
        $AssignedList = dbGetList('CaseInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
        $AssignedList = testSetCaseListMultiInfo($AssignedList);

        foreach($AssignedList as $Item)
        {
          $AssignedStr .= "{$Item[CaseID]}&nbsp;<a href=\"Case.php?CaseID={$Item[CaseID]}\" title=\"{$Item[CaseTitle]}\" target=\"_blank\">{$Item[UCTitle]}</a><br />";
        }

        foreach($OpenedList as $Item)
        {
          $OpenedStr .= "{$Item[CaseID]}&nbsp;<a href=\"Case.php?CaseID={$Item[CaseID]}\" title=\"{$Item[CaseTitle]}\" target=\"_blank\">{$Item[UCTitle]}</a><br />";
        }
    }
    elseif($TestMode == 'Result')
    {
        $Columns = 'ResultID,ResultTitle';
        $Where = "OpenedBy = '{$UCTestUserName}'";
        $Where .= " AND IsDroped = '0' ";
        $OrderBy = 'LastEditedDate DESC';
        $OpenedList = dbGetList('ResultInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
        $OpenedList = testSetResultListMultiInfo($OpenedList);

        $Where = "AssignedTo = '{$UCTestUserName}'";
        $Where .= " AND IsDroped = '0' ";
        $AssignedList = dbGetList('ResultInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
        $AssignedList = testSetResultListMultiInfo($AssignedList);

        foreach($AssignedList as $Item)
        {
          $AssignedStr .= "{$Item[ResultID]}&nbsp;<a href=\"Result.php?ResultID={$Item[ResultID]}\" title=\"{$Item[ResultTitle]}\" target=\"_blank\">{$Item[UCTitle]}</a><br />";
        }

        foreach($OpenedList as $Item)
        {
          $OpenedStr .= "{$Item[ResultID]}&nbsp;<a href=\"Result.php?ResultID={$Item[ResultID]}\" title=\"{$Item[ResultTitle]}\" target=\"_blank\">{$Item[UCTitle]}</a><br />";
        }
    }

    $Where = "UserName = '{$UCTestUserName}' AND QueryType = '{$TestMode}'";
    $OrderBy = 'QueryTitle ASC';
    $QueryList = dbGetList('TestUserQuery','', $Where, '', $OrderBy);

    foreach($QueryList as $Item)
    {
        $QueryStr .= "<div>";
        $QueryStr .= "<a href='?DelQueryID={$Item[QueryID]}' title='{$_LANG[Delete]}' onclick='return confirm(\"{$_LANG[ConfirmDelQuery]}\");' target='_self' style='color:#CC0000;text-decoration:none;font-size:11px;font-weight:bold;margin-right:0px;padding:0 5px;'><img src='Image/delete.gif'/></a>";
        $QueryStr .= "<a href='{$TestMode}List.php?QueryID={$Item[QueryID]}' title='{$Item[QueryTitle]}'>{$Item[QueryTitle]}</a>";
        $QueryStr .= "</div>";
    }

    $objResponse->addAssign('UCDiv0', 'innerHTML', $AssignedStr);
    $objResponse->addAssign('UCDiv1', 'innerHTML', $OpenedStr);
    $objResponse->addAssign('UCDiv2', 'innerHTML', $QueryStr);

    return $objResponse;
}

sysXajaxRegister("xUpdateUserControl");

$LimitNum = '10';

$TestMode = $_SESSION['TestMode'];
$UCTestUserName = mysql_real_escape_string($_SESSION['TestUserName']);

if($_GET['DelQueryID'] != '')
{
    dbDeleteRow('TestUserQuery', "QueryID='{$_GET[DelQueryID]}' AND UserName='{$UCTestUserName}'");
    jsGoto('UserControl.php');
    exit;
}

if($TestMode == 'Bug')
{
    $Columns = 'BugID,BugTitle';
    $Where = "OpenedBy = '{$UCTestUserName}' AND BugStatus <> 'Closed'";
    $Where .= " AND IsDroped = '0' ";
    $OrderBy = 'LastEditedDate DESC';
    $OpenedList = dbGetList('BugInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
    $OpenedList = testSetBugListMultiInfo($OpenedList);

    $Where = "AssignedTo = '{$UCTestUserName}' AND BugStatus <> 'Closed'";
    $Where .= " AND IsDroped = '0' ";
    $AssignedList = dbGetList('BugInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
    $AssignedList = testSetBugListMultiInfo($AssignedList);
}
elseif($TestMode == 'Case')
{
    $Columns = 'CaseID,CaseTitle';
    $Where = "OpenedBy = '{$UCTestUserName}'";
    $Where .= " AND IsDroped = '0' ";
    $OrderBy = 'LastEditedDate DESC';
    $OpenedList = dbGetList('CaseInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
    $OpenedList = testSetCaseListMultiInfo($OpenedList);

    $Where = "AssignedTo = '{$UCTestUserName}'";
    $Where .= " AND IsDroped = '0' ";
    $AssignedList = dbGetList('CaseInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
    $AssignedList = testSetCaseListMultiInfo($AssignedList);
}
elseif($TestMode == 'Result')
{
    $Columns = 'ResultID,ResultTitle';
    $Where = "OpenedBy = '{$UCTestUserName}'";
    $Where .= " AND IsDroped = '0' ";
    $OrderBy = 'LastEditedDate DESC';
    $OpenedList = dbGetList('ResultInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
    $OpenedList = testSetResultListMultiInfo($OpenedList);

    $Where = "AssignedTo = '{$UCTestUserName}'";
    $Where .= " AND IsDroped = '0' ";
    $AssignedList = dbGetList('ResultInfo',$Columns, $Where, '', $OrderBy, $LimitNum);
    $AssignedList = testSetResultListMultiInfo($AssignedList);
}

$Where = "UserName = '{$UCTestUserName}' AND QueryType = '{$TestMode}'";
$OrderBy = 'QueryTitle ASC';
$QueryList = dbGetList('TestUserQuery','', $Where, '', $OrderBy);

$TPL->assign('AssignedList', $AssignedList);
$TPL->assign('OpenedList', $OpenedList);
$TPL->assign('QueryList', $QueryList);
$TPL->assign('TestMode', $_SESSION['TestMode']);

$TPL->display('UserControl.tpl');
?>
