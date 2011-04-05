<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * view, new, edit a result.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

sysXajaxRegister("xProjectSetSlaveModule,xSetModuleOwner,xCreateSelectDiv,xProjectSetAssignedUser,xDeleteTestFile");

$ModuleType = 'Case';

$ResultID = $_REQUEST['ResultID'];
$ActionType = $_REQUEST['ActionType'];

if($ActionType == 'OpenResult')
{
    /* Create PrjectList And ModuleList */
    $CaseID = $_GET['CaseID'];
    $Coumns = '*';
    $CaseInfo = dbGetRow('CaseInfo',$Coumns,"CaseID = '{$CaseID}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
    $ProjectID = $CaseInfo['ProjectID'];
    $ModuleID = $CaseInfo['ModuleID'];
    if(!$CaseInfo['CaseID'])
    {
        sysErrorMsg();
    }
}
elseif($ResultID != '')
{
    $Coumns = '*';
    $ResultInfo = dbGetRow('ResultInfo',$Coumns,"ResultID = '{$ResultID}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
    if(!$ResultInfo['ResultID'])
    {
        sysErrorMsg();
    }

    $ProjectID = $ResultInfo['ProjectID'];
    $ModuleID = $ResultInfo['ModuleID'];

    $PreResultInfo = dbGetList('ResultInfo','ResultID',"ResultID < '{$ResultID}' AND {$_SESSION[TestUserACLSQL]} AND {$_SESSION[ResultQueryCondition]}",'','ResultID DESC','1');
    $NextResultInfo = dbGetList('ResultInfo','ResultID',"ResultID > '{$ResultID}' AND {$_SESSION[TestUserACLSQL]} AND {$_SESSION[ResultQueryCondition]}",'','ResultID ASC','1');
    if(empty($PreResultInfo))
    {
        $PreResultID = 0;
    }
    else
    {
        $PreResultID = $PreResultInfo[0]['ResultID'];
    }
    if(empty($NextResultInfo))
    {
        $NextResultID = 0;
    }
    else
    {
        $NextResultID = $NextResultInfo[0]['ResultID'];
    }

    $UserList = testGetProjectUserNameList($ResultInfo['ProjectID']);
    $ResultInfo = testSetResultMultiInfo($ResultInfo, $UserList);

    $ResultActionList = testGetActionAndFileList('Result', $ResultInfo['ResultID']);
    $ResultInfo['ActionList'] = $ResultActionList['ActionList'];
    $ResultInfo['FileList'] = $ResultActionList['FileList'];
    $ResultInfo['DuplicateIDList'] = explode(',',$ResultInfo['DuplicateID']);
    $ResultInfo['LinkIDList'] = explode(',',$ResultInfo['LinkID']);
    $LastActionID = testGetLastActionID('Result',$ResultID);

    $TPL->assign('ResultInfo', $ResultInfo);
}
else
{
    if($ActionType != 'OpenResults')
    sysErrorMsg();
}

if($ActionType == 'OpenResult' || $ActionType == 'OpenResults')
{
    $TPL->assign('HeaderTitle', $_LANG['OpenResult']);
}
else
{
    $TPL->assign('HeaderTitle', 'Result #' . $ResultInfo['ResultID'] . ' ' . $ResultInfo['ResultTitle']);
}

$ACLAttrib = 'class="NormalSelect MyInput RequiredField"';
$ResultValueSelectList = htmlSelect($_LANG['ResultValues'], 'ResultValue', '', $ResultInfo['ResultValue'],$ACLAttrib);
$ACLAttrib = 'class="NormalSelect MyInput"';
$ResultStatusSelectList = htmlSelect($_LANG['ResultStatuses'], 'ResultStatus', '', $ResultInfo['ResultStatus'],$ACLAttrib);
$ResultOSSelectList = htmlSelect($_LANG['ResultOS'], 'ResultOS', '', $ResultInfo['ResultOS']);
$ResultBrowserList = htmlSelect($_LANG['ResultBrowser'], 'ResultBrowser', '', $ResultInfo['ResultBrowser']);
$ResultMachineList = htmlSelect($_LANG['ResultMachine'], 'ResultMachine', '', $ResultBuild['ResultMachine']);

$ResultMarkForDeletionSelectList = htmlSelect($_LANG['MarkForDeletions'], 'MarkForDeletion','',$ResultInfo['MarkForDeletion']);
$ProjectUserList = testGetProjectUserList($ProjectID, 'true');
$ACLAttrib = 'class="NormalSelect MyInput RequiredField"';
unset($ProjectUserList['Active']);
$ProjectUserList = array('Closed'=>'Closed') + $ProjectUserList;
$AssignUserSelectList = htmlSelect($ProjectUserList, 'AssignedTo', '', $ResultInfo['AssignedTo'], $ACLAttrib);
$ProjectUserList = array(''=>'')+$ProjectUserList;
$ACLAttrib = 'class="NormalSelect MyInput"';
$ScriptedBySelectList = htmlSelect($ProjectUserList, 'ScriptedBy', '', $ResultInfo['ScriptedBy'], $ACLAttrib);

$ConfirmArray = array('ReplyNote'=>'ReplyNote') + $_LANG['ResultFields'];
$TPL->assign('ConfirmIds', jsArray($ConfirmArray, 'Key'));

if($ActionType == 'OpenResults')
{

    $WHERE[] = $_SESSION['CaseQueryCondition'];
    $Where = join(' AND ', $WHERE);
    $Where .= " AND IsDroped = '0'";
    $CaseListCreateResults = dbGetList('CaseInfo','CaseID', $Where, '');

    $TPL->assign('CaseListCreateResultsLen', count($CaseListCreateResults));
}

$TPL->assign('CaseInfo', $CaseInfo);
$TPL->assign('PreResultID', $PreResultID);
$TPL->assign('NextResultID', $NextResultID);
$TPL->assign('ProjectID', $ProjectID);
$TPL->assign('ProjectList', $ProjectListSelect);
$TPL->assign('ModuleList', $ModuleSelectList);
$TPL->assign('ModuleID', $ModuleID);
$TPL->assign('ResultValueSelectList', $ResultValueSelectList);
$TPL->assign('ResultStatusSelectList', $ResultStatusSelectList);
$TPL->assign('ResultOSSelectList', $ResultOSSelectList);
$TPL->assign('ResultBrowserSelectList', $ResultBrowserList);
$TPL->assign('ResultMachineSelectList', $ResultMachineList);
$TPL->assign('AssignedToUserList', $AssignUserSelectList);
$TPL->assign('ActionType', $ActionType);
$TPL->assign('LastActionID',$LastActionID);

$TPL->display('Result.tpl');
?>
