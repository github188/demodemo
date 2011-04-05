<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * mail notice.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("Include/Init.inc.php");
@set_time_limit(0);
@ini_set('memory_limit', -1);

$Where = "AssignedTo != 'Closed' && AssignedTo != 'Active' AND IsDroped = '0' AND BugStatus != 'Closed'";
$OrderBy = 'LastEditedDate DESC';
$Columns = 'BugID,BugSeverity,BugPriority,BugTitle,BugStatus,OpenedBy,AssignedTo,ResolvedBy,Resolution,LastEditedDate';
$BugList = dbGetList('BugInfo',$Columns, $Where, '', $OrderBy);

$UserList = testGetUserList();
foreach($UserList as $UserInfo)
{
    $UserNameList[$UserInfo['UserName']] = $UserInfo['RealName'];
}

$BugList = testSetBugListMultiInfo($BugList, $UserNameList);

foreach($BugList as $BugInfo)
{
    $BugGroupedList[$BugInfo["AssignedTo"]][] = $BugInfo;
}
$UserNameList = @array_keys($BugGroupedList);
$InUserNames = dbCreateIN(sysAddSlash(join(',',$UserNameList)));
$UserList = testGetUserList("{$_CFG[UserTable][UserName]} {$InUserNames}");

$CssStyle = join("",file($_CFG['RealRootPath'] . "/Css/Mail.css"));
$TPL->assign("CssStyle",$CssStyle);

if($_CFG['BrowserMode'])
{
    $BaseUrl = $_CFG["BaseURL"];
}
else
{
    $BaseUrl = $argv[1];
}
if($BaseUrl == '')
{
    die('Wrong Baseurl');
}
$TPL->assign("BaseUrl", $BaseUrl);

foreach($BugGroupedList as $AssignedTo => $UserBugList)
{
    $TPL->assign("UserBugList",$UserBugList);

    // Get bug list message in html.
    $Message = $TPL->fetch("Notice.tpl");
    $Subject = $_LANG["NoticeBugSubject"] . " ". count($UserBugList);

    // Mail.
    if(!empty($UserList[$AssignedTo]['Email']))
    {
        $To = $UserList[$AssignedTo]['Email'];
        @sysMail($To,'',$Subject,$Message);
    }
}
?>
