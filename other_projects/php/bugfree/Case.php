<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * view, new, edit a case.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

sysXajaxRegister("xProjectSetSlaveModule,xSetModuleOwner,xCreateSelectDiv,xProjectSetAssignedUser,xProjectSetScriptedBy,xDeleteTestFile");

$ModuleType = 'Case';

$CaseID = intval($_REQUEST['CaseID']);
$ActionType = $_REQUEST['ActionType'];

if($CaseID > 0)
{
    $Coumns = '*';
    $CaseInfo = dbGetRow('CaseInfo',$Coumns,"CaseID = '{$CaseID}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
    if(!$CaseInfo['CaseID'])
    {
        sysErrorMsg();
    }
    $CaseInfo['BugID'] = trim($CaseInfo['BugID'],',');
    $ProjectID = $CaseInfo['ProjectID'];
    $ModuleID = $CaseInfo['ModuleID'];

    $UserList = testGetProjectUserNameList($CaseInfo['ProjectID']);
    $CaseInfo = testSetCaseMultiInfo($CaseInfo, $UserList);
    $CaseInfo['DuplicateIDList'] = explode(',',$CaseInfo['DuplicateID']);
    $CaseInfo['LinkIDList'] = explode(',',$CaseInfo['LinkID']);
    $CaseInfo['BugIDList'] = explode(',',$CaseInfo['BugID']);
}

if($ActionType == 'OpenCase' && $CaseID <= 0)
{
    /* Create PrjectList And ModuleList */
    $ProjectID = $_SESSION['TestCurrentProjectID'];
    $ModuleID = $_SESSION['TestCurrentModuleID'] == '' ? 0 : $_SESSION['TestCurrentModuleID'];
}
elseif($ActionType == 'OpenCase' && $CaseID > 0)
{
    $TPL->assign('CopyCaseID', $CaseID);
}
elseif($ActionType != 'OpenCase' && $CaseID > 0)
{
    $CaseActionList = testGetActionAndFileList('Case', $CaseInfo['CaseID']);
    $CaseInfo['ActionList'] = $CaseActionList['ActionList'];
    $CaseInfo['FileList'] = $CaseActionList['FileList'];

    $PreCaseInfo = dbGetList('CaseInfo','CaseID',"CaseID < '{$CaseID}' AND {$_SESSION[TestUserACLSQL]} AND {$_SESSION[CaseQueryCondition]}",'','CaseID DESC','1');
    $NextCaseInfo = dbGetList('CaseInfo','CaseID',"CaseID > '{$CaseID}' AND {$_SESSION[TestUserACLSQL]} AND {$_SESSION[CaseQueryCondition]}",'','CaseID ASC','1');
    if(empty($PreCaseInfo))
    {
        $PreCaseID = 0;
    }
    else
    {
        $PreCaseID = $PreCaseInfo[0]['CaseID'];
    }
    if(empty($NextCaseInfo))
    {
        $NextCaseID = 0;
    }
    else
    {
        $NextCaseID = $NextCaseInfo[0]['CaseID'];
    }
    $LastActionID = testGetLastActionID('Case',$CaseID);
}
else
{
    sysErrorMsg();
}

if($ActionType == 'OpenCase')
{
    $TPL->assign('HeaderTitle', $_LANG['OpenCase']);
}
else
{
    $TPL->assign('HeaderTitle', 'Case #' . $CaseInfo['CaseID'] . ' ' . $CaseInfo['CaseTitle']);
}

$OnChangeStr = 'onchange="';
$OnChangeStr .= 'xajax_xProjectSetSlaveModule(this.value, \'SlaveModuleList\', \'ModuleID\', \'Case\');';
$OnChangeStr .= 'xajax_xProjectSetAssignedUser(this.value);';
$OnChangeStr .= 'xajax_xProjectSetScriptedBy(this.value);';
$OnChangeStr .= '"';
$OnChangeStr .= ' class="MyInput RequiredField"';
$ProjectListSelect = testGetValidProjectSelectList('ProjectID', $ProjectID, $OnChangeStr);

$OnChangeStr = '';
if($ActionType == 'OpenCase')
{
    $OnChangeStr = 'onchange="';
    $OnChangeStr .= 'xajax_xSetModuleOwner(this.value);';
    $OnChangeStr .= '"';
}
$OnChangeStr .= ' class="MyInput RequiredField"';
$ModuleSelectList = testGetSelectModuleList($ProjectID, 'ModuleID', $ModuleID, $OnChangeStr, $ModuleType);

$CaseStatusSelectList = htmlSelect($_LANG['CaseStatuses'], 'CaseStatus','',$CaseInfo['CaseStatus']);
$CasePrioritySelectList = htmlSelect($_LANG['CasePriorities'], 'CasePriority','',$CaseInfo['CasePriority'], 'class="MyInput RequiredField"');
$CaseTypeSelectList = htmlSelect($_LANG['CaseTypes'], 'CaseType','',$CaseInfo['CaseType'], 'class="MyInput RequiredField"');
$CaseMethodSelectList = htmlSelect($_LANG['CaseMethods'], 'CaseMethod','',$CaseInfo['CaseMethod'], 'class="MyInput RequiredField"');
$CasePlanSelectList = htmlSelect($_LANG['CasePlans'], 'CasePlan','',$CaseInfo['CasePlan']);
$ScriptStatusSelectList = htmlSelect($_LANG['ScriptStatuses'], 'ScriptStatus','',$CaseInfo['ScriptStatus']);

$CaseMarkForDeletionSelectList = htmlSelect($_LANG['MarkForDeletions'], 'MarkForDeletion','',$CaseInfo['MarkForDeletion']);
$ProjectUserList = testGetProjectUserList($ProjectID, 'true');
$ACLAttrib = 'class="NormalSelect MyInput RequiredField"';
$AssignUserSelectList = htmlSelect($ProjectUserList, 'AssignedTo', '', $CaseInfo['AssignedTo'], $ACLAttrib);
unset($ProjectUserList['Active']);
$ProjectUserList = array(''=>'')+$ProjectUserList;
$ACLAttrib = 'class="NormalSelect MyInput"';
$ScriptedBySelectList = htmlSelect($ProjectUserList, 'ScriptedBy', '', $CaseInfo['ScriptedBy'], $ACLAttrib);

$ConfirmArray = array('ReplyNote'=>'ReplyNote') + $_LANG['CaseFields'];
$TPL->assign('ConfirmIds', jsArray($ConfirmArray, 'Key'));

$TPL->assign('CaseInfo', $CaseInfo);
$TPL->assign('PreCaseID', $PreCaseID);
$TPL->assign('NextCaseID', $NextCaseID);
$TPL->assign('ProjectID', $ProjectID);
$TPL->assign('ProjectList', $ProjectListSelect);
$TPL->assign('ModuleList', $ModuleSelectList);
$TPL->assign('ModuleID', $ModuleID);
$TPL->assign('CaseStatusSelectList', $CaseStatusSelectList);
$TPL->assign('CasePrioritySelectList', $CasePrioritySelectList);
$TPL->assign('CaseTypeSelectList', $CaseTypeSelectList);
$TPL->assign('CaseMethodSelectList', $CaseMethodSelectList);
$TPL->assign('CasePlanSelectList', $CasePlanSelectList);
$TPL->assign('ScriptStatusSelectList', $ScriptStatusSelectList);
$TPL->assign('ScriptedBySelectList', $ScriptedBySelectList);
$TPL->assign('CaseMarkForDeletionSelectList', $CaseMarkForDeletionSelectList);
$TPL->assign('AssignedToUserList', $AssignUserSelectList);
$TPL->assign('ActionType', $ActionType);
$TPL->assign('LastActionID',$LastActionID);

$TPL->display('Case.tpl');
?>
