<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * view, new, edit, resolve, close, activate a bug.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

sysXajaxRegister("xProjectSetSlaveModule,xSetModuleOwner,xCreateSelectDiv,xProjectSetAssignedUser,xDeleteTestFile");

$ModuleType = 'Bug';
$BugID = $_REQUEST['BugID'];
$ActionType = $_REQUEST['ActionType'];
$Columns = '*';
$BugInfo = dbGetRow('BugInfo',$Columns,"BugID = '{$BugID}' AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
#$BugInfo['OpenedBuild'] = ($BugInfo['OpenedBuild'] || '0');

if(!$BugInfo['BugID'] && $ActionType != 'OpenBug')
{
    sysErrorMsg();
}

if($BugID > 0 && $ActionType == 'OpenBug')
{
    $ProjectID = $BugInfo['ProjectID'];
    $ModuleID = $BugInfo['ModuleID'];
    unset($BugInfo['LastEditedBy']);
    unset($BugInfo['LastEditedDate']);
    unset($BugInfo['ResolvedBy']);
    unset($BugInfo['Resolution']);
    unset($BugInfo['ResolvedBuild']);
    unset($BugInfo['ResolvedDate']);
    unset($BugInfo['ClosedBy']);
    unset($BugInfo['ClosedDate']);
    unset($BugInfo['ResultID']);
}

if($ActionType == 'OpenBug')
{
    /* Create PrjectList And ModuleList */
    $ProjectID = $ProjectID != '' ? $ProjectID : $_SESSION['TestCurrentProjectID'];
    $ModuleID = $ModuleID != '' ? $ModuleID : $_SESSION['TestCurrentModuleID'];
    $ModuleInfo = dbGetRow('TestModule','*',"ModuleID = '{$ModuleID}'");
    $BugInfo['BugStatus'] = 'Active';
    $BugInfo['BugSeverity'] = '1';
    $BugInfo['BugPriority'] = '1';
    $BugInfo['BugType'] = 'CodeError';
    $BugInfo['HowFound'] = 'FuncTest';
    $BugInfo['OpenedBuild'] = '0';
    $BugInfo['BugStatusName'] = $_LANG['BugStatus'][$BugInfo['BugStatus']];

    $PreBugID = 0;
    $NextBugID = 0;

    /* Fill the default repro steps */
    $DefaultBugValue = array();
    $DefaultBugValue = $_LANG['DefaultReproSteps'];
    $DefaultBugValue['ExpectResult'] = '';
    $ModuleSelected = $ModuleID;

    if($_GET['ResultID'] != '')
    {
        $ResultID = $_GET['ResultID'];
        $ResultInfo = dbGetRow('ResultInfo',$Columns,"ResultID = '{$ResultID}' AND {$_SESSION[TestUserACLSQL]}");
        if(!empty($ResultInfo))
        {
            $ProjectID = $ResultInfo['ProjectID'];
            $ResultModuleID = $ResultInfo['ModuleID'];
            $ModuleInfo = dbGetRow('TestModule','ModuleName',"ModuleID = '{$ResultModuleID}'");
            $ModuleSelected = $ModuleInfo['ModuleName'];
            $BugInfo['BugTitle'] = $ResultInfo['ResultTitle'];
            $BugInfo['ProjectID'] = $ResultInfo['ProjectID'];
            $BugInfo['BugOS'] = $ResultInfo['ResultOS'];
            $BugInfo['BugBrowser'] = $ResultInfo['ResultBrowser'];
            $BugInfo['BugMachine'] = $ResultInfo['ResultMachine'];
            $BugInfo['OpenedBuild'] = $ResultInfo['ResultBuild'];
            $BugInfo['ReproSteps'] = $ResultInfo['ResultSteps'];
            $BugInfo['ResultID'] = $ResultID;
        }
    }
}
else
{
    /* Create PrjectList And ModuleList */
    $ProjectID = $BugInfo['ProjectID'];
    $ModuleID = $BugInfo['ModuleID'];
    $ModuleSelected = $ModuleID;

    $PreBugInfo = dbGetList('BugInfo','BugID',"BugID < '{$BugID}' AND {$_SESSION[TestUserACLSQL]} AND {$_SESSION[BugQueryCondition]}",'','BugID DESC','1');
    $NextBugInfo = dbGetList('BugInfo','BugID',"BugID > '{$BugID}' AND {$_SESSION[TestUserACLSQL]} AND {$_SESSION[BugQueryCondition]}",'','BugID ASC','1');
    if(empty($PreBugInfo))
    {
        $PreBugID = 0;
    }
    else
    {
        $PreBugID = $PreBugInfo[0]['BugID'];
    }
    if(empty($NextBugInfo))
    {
        $NextBugID = 0;
    }
    else
    {
        $NextBugID = $NextBugInfo[0]['BugID'];
    }
    
    $UserList = testGetProjectUserNameList($BugInfo['ProjectID']);
    $BugInfo = testSetBugMultiInfo($BugInfo, $UserList);

    $BugActionList = testGetActionAndFileList('Bug', $BugInfo['BugID']);
    $LastActionID = testGetLastActionID('Bug',$BugInfo['BugID']);
    $BugInfo['ActionList'] = $BugActionList['ActionList'];
    $BugInfo['FileList'] = $BugActionList['FileList'];
    $BugInfo['DuplicateIDList'] = explode(',',$BugInfo['DuplicateID']);
    $BugInfo['LinkIDList'] = explode(',',$BugInfo['LinkID']);
    $BugInfo['CaseIDList'] = explode(',',$BugInfo['CaseID']);
}

if($ActionType == 'Activated' && $BugInfo['BugStatus'] == 'Active')
{
    jsGoto('Bug.php?BugID=' . $BugID);
}
if($ActionType == 'Resolved' && $BugInfo['BugStatus'] == 'Resolved')
{
    jsGoto('Bug.php?BugID=' . $BugID);
}
if($ActionType == 'Closed' && $BugInfo['BugStatus'] == 'Closed')
{
    jsGoto('Bug.php?BugID=' . $BugID);
}

$OnChangeStr = 'onchange="';
$OnChangeStr .= 'xajax_xProjectSetSlaveModule(this.value, \'SlaveModuleList\', \'ModuleID\', \'Bug\');';
$OnChangeStr .= 'xajax_xProjectSetAssignedUser(this.value);';
$OnChangeStr .= '"';
$OnChangeStr .= ' class="MyInput RequiredField"';
$ProjectListSelect = testGetValidProjectSelectList('ProjectID', $ProjectID, $OnChangeStr);

$OnChangeStr = '';
if($ActionType == 'OpenBug')
{
    $OnChangeStr = 'onchange="';
    $OnChangeStr .= 'xajax_xSetModuleOwner(this.value);';
    $OnChangeStr .= '"';
}
$OnChangeStr .= ' class="MyInput RequiredField"';
$ModuleSelectList = testGetSelectModuleList($ProjectID, 'ModuleID', $ModuleSelected, $OnChangeStr, $ModuleType);

$ProjectUserList = testGetProjectUserList($ProjectID, true);
$ProjectUserList['Closed'] = 'Closed';

if($ActionType == 'OpenBug')
{
    $ActionTypeName = $_LANG['OpenBug'];
    $AssignedTo = $ModuleInfo['ModuleOwner'] != '' ? $ModuleInfo['ModuleOwner'] : $BugInfo['AssignedTo'];
    $SelectAssignUserList = htmlSelect($ProjectUserList, 'AssignedTo', '',$AssignedTo, 'class="NormalSelect MyInput RequiredField"');
}
elseif($ActionType == '')
{
    $ActionTypeName = $_LANG['ViewBug'];
}
elseif($ActionType == 'Edited')
{
    $ActionTypeName = $_LANG['EditBug'];
    $SelectAssignUserList = htmlSelect($ProjectUserList, 'AssignedTo', '',$BugInfo['AssignedTo'], 'class="NormalSelect MyInput RequiredField"');
}
elseif($ActionType == 'Activated')
{
    $ActionTypeName = $_LANG['ActiveBug'];
    $SelectAssignUserList = htmlSelect($ProjectUserList, 'AssignedTo', '',$BugInfo['ResolvedBy'], 'class="NormalSelect MyInput RequiredField"');
}
elseif($ActionType == 'Resolved')
{
    $ActionTypeName = $_LANG['ResolveBug'];
    $SelectAssignUserList = htmlSelect($ProjectUserList, 'AssignedTo', '',$BugInfo['OpenedBy'], 'class="NormalSelect MyInput RequiredField"');
}
elseif($ActionType == 'Closed')
{
    $ActionTypeName = $_LANG['CloseBug'];
    $SelectAssignUserList = htmlSelect($ProjectUserList, 'AssignedTo', '','Closed', 'class="NormalSelect MyInput RequiredField"');
}

$BugSeveritySelectList = htmlSelect($_LANG['BugSeveritys'], 'BugSeverity', '', $BugInfo['BugSeverity'], 'class="MyInput RequiredField"');
$BugPrioritySelectList = htmlSelect($_LANG['BugPriorities'], 'BugPriority','', $BugInfo['BugPriority']);
$BugTypeSelectList = htmlSelect($_LANG['BugTypes'], 'BugType', '', $BugInfo['BugType'], 'class="MyInput RequiredField"');
$BugHowFoundList = htmlSelect($_LANG['BugHowFound'], 'HowFound', '', $BugInfo['HowFound'], 'class="MyInput RequiredField"');
$BugOSSelectList = htmlSelect($_LANG['BugOS'], 'BugOS', '', $BugInfo['BugOS']);
$BugSubStatusList = htmlSelect($_LANG['BugSubStatus'], 'BugSubStatus', '', $BugInfo['BugSubStatus']);
$BugBrowserList = htmlSelect($_LANG['BugBrowser'], 'BugBrowser', '', $BugInfo['BugBrowser']);
$BugMachineList = htmlSelect($_LANG['BugMachine'], 'BugMachine', '', $BugInfo['BugMachine']);

//$OpenedBuildList = testGetBugBuildList($ProjectID);
//$OpenedBuildSelectList = htmlSelect($OpenedBuildList, 'OpenedBuild', '', $BugInfo['OpenedBuild'], '', 'OpenedBuild,OpenedBuild');
$OnChangeStr = 'onchange="';
$OnChangeStr .= 'if(this.value==\'Duplicate\'){displayObj(\'DuplicateID\');xajax.$(\'DuplicateID\').focus();}else{xajax.$(\'DuplicateID\').value=\'\';hiddenObj(\'DuplicateID\');}';
$OnChangeStr .= '"';
$ResolutionSelectList = htmlSelect($_LANG['BugResolutions'], 'Resolution','',$BugInfo['Resolution'], 'class="MyInput RequiredField" ' . $OnChangeStr);
//$ResolvedBuildList = $OpenedBuildList;
//$ResolvedBuildSelectList = htmlSelect($ResolvedBuildList, 'ResolvedBuild', '', $BugInfo['ResolvedBuild'], '', 'OpenedBuild,OpenedBuild');



if($ActionType == 'OpenBug')
{
    $TPL->assign('HeaderTitle', $_LANG['OpenBug']);
}
else
{
    $TPL->assign('HeaderTitle', 'Bug #' . $BugInfo['BugID'] . ' ' . $BugInfo['BugTitle']);
}

$ConfirmArray = array('ReplyNote'=>'ReplyNote') + $_LANG['BugFields'];
$TPL->assign('ConfirmIds', jsArray($ConfirmArray, 'Key'));

$TPL->assign('BugInfo', $BugInfo);
$TPL->assign('PreBugID', $PreBugID);
$TPL->assign('NextBugID', $NextBugID);

$TPL->assign('ProjectID', $ProjectID);
$TPL->assign('ModuleID', $ModuleID);
$TPL->assign('ProjectList', $ProjectListSelect);
$TPL->assign('ModuleList', $ModuleSelectList);
$TPL->assign('BugTypeList', $BugTypeSelectList);
$TPL->assign('BugOSList', $BugOSSelectList);
$TPL->assign('BugSeverityList', $BugSeveritySelectList);
$TPL->assign('BugPriorityList', $BugPrioritySelectList);
$TPL->assign('BugBrowserList', $BugBrowserList);
$TPL->assign('BugSubStatusList', $BugSubStatusList);
$TPL->assign('BugHowFoundList', $BugHowFoundList);
$TPL->assign('ProjectUserList', $SelectUserList);
$TPL->assign('AssignedToUserList', $SelectAssignUserList);
$TPL->assign('OpenedByUserList', $SelectOpenUserList);
$TPL->assign('ResolutionList', $ResolutionSelectList);
$TPL->assign('ActionType', $ActionType);
$TPL->assign('ActionTypeName', $ActionTypeName);
$TPL->assign('ActionRealName', $_SESSION['TestRealName']);
$TPL->assign('ActionDate', date('Y-m-d'));
$TPL->assign('DefaultBugValue', $DefaultBugValue);
$TPL->assign('LastActionID',$LastActionID);

$TPL->display('Bug.tpl');
?>
