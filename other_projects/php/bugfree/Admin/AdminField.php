<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin module list.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

baseJudgeAdminUserLogin();
sysXajaxRegister("xAdminAddField,xAdminUpdateField");

$ProjectID = $_GET['ProjectID'];
$ModuleType = $_GET['ModuleType'];
$ModuleType == "Bug" ? $ModuleType = "Bug" : $ModuleType = "Case";
if($_SESSION['TestIsProjectAdmin'])
{
    $ProjectInfo = current(testGetProjectList("ProjectID = '{$_GET[ProjectID]}' AND ProjectManagers LIKE '%," . mysql_real_escape_string(mysql_real_escape_string($_SESSION['TestUserName'])) . ",%'"));
}
elseif($_SESSION['TestIsAdmin'])
{
    $ProjectInfo = current(testGetProjectList("ProjectID = '{$_GET[ProjectID]}'"));
}
if(empty($ProjectInfo))
{
    sysErrorMsg();
}
if(isset($_GET['Action']))
{
    if('AddField' == $_GET['Action'])
    {
        $TPL->assign('ModuleType', $ModuleType);
        $TPL->assign('ProjectInfo', $ProjectInfo);
    }
    if('UpdateField' == $_GET['Action'])
    {
        $TPL->assign('ModuleType', $ModuleType);
        $TPL->assign('ProjectInfo', $ProjectInfo);
        $xml = simplexml_load_string($ProjectInfo['FieldSet']);
        $fields = $xml->xpath('/fieldset/fields[@type="' . $ModuleType . '"]/field[@id="' . $_GET['FieldName'] . '"]');
        $Field = array();
        $Field['FieldName'] = $fields[0]->name;
        $Field['FieldText'] = $fields[0]->text;
        $Field['FieldType'] = $fields[0]->type;
        $Field['FieldStatus'] = $fields[0]->status;
        $Field['FieldOption'] = $fields[0]->option;
        $TableName = testGetFieldTable($ModuleType, $ProjectID);
        $FieldInfo = dbGetFieldInfo($TableName, $Field['FieldName']);
        $FieldValue = '';
        switch($Field['FieldType'])
        {
            case 'radio':
            case 'select':
            case 'checkbox': {
                $FieldValue = str_replace('\'', '', $FieldInfo['Length']);
                break;
            }
            default: {
                $FieldValue = $FieldInfo['Default'];
                break;
            }
        }
        $Field['FieldValue'] = $FieldValue;
        $TPL->assign('Field', $Field);
    }
    $TPL->assign('Action', $_GET['Action']);
}
$TPL->display('Admin/Field.tpl');