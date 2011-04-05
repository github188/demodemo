<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * search bug form.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

$FieldName = 'Field';
$OperatorName = 'Operator';
$ValueName = 'Value';
$AndOrName = 'AndOr';

$FieldList = array();
$OperatorList = array();
$ValueList = array();
$AndOrList = array();
$FieldCount = $_CFG['QueryFieldNumber'];
$Attrib = 'class="FullSelect"';

$And = 'checked';
$Or = '';

$FieldListSelectItem = array(0=>'ProjectName',1=>'OpenedBy',2=>'ModulePath',3=>'AssignedTo',4=>'BugID',5=>'BugTitle');
$OperatorListSelectItem = array(0=>'=', 2=>'LIKE', 5=>'LIKE');


if($_REQUEST['reset'])
{
    $_SESSION['BugFieldListCondition']='';
    $_SESSION['BugOperatorListCondition']='';
    $_SESSION['BugAndOrListCondition']='';
    $_SESSION['BugValueListCondition']='';
    $_SESSION['BugQueryCondition'] = '';
    $_SESSION['BugQueryTitle'] = '';
    $_SESSION['BugFieldsToShow'] = '';

}
if($_SESSION['BugFieldListCondition']!='')
  $FieldListSelectItem = $_SESSION['BugFieldListCondition'];
if($_SESSION['BugOperatorListCondition']!='')
  $OperatorListSelectItem = $_SESSION['BugOperatorListCondition'];
if($_SESSION['BugAndOrListCondition']!='')
  $SelectItem = $_SESSION['BugAndOrListCondition'];
if($_SESSION['BugValueListCondition']!='')
  $ValueListSelectItem = $_SESSION['BugValueListCondition'];

if(strpos($_SESSION['BugQueryCondition'],') OR (')!== FALSE)
{
   $Or = 'checked';
   $And = '';
}

if($_GET['QueryID'])
{
   $QueryInfo = dbGetRow('TestUserQuery', '', "QueryID='{$_REQUEST[QueryID]}' AND QueryType='Bug'");
   $AndOr = $QueryInfo['QueryString'];
   
   if(strpos($QueryInfo['QueryString'],') OR (')!== FALSE)
   {
      $Or = 'checked';
      $And = '';
   }
   if(strpos($QueryInfo['QueryString'],') AND (')!== FALSE)
   {
      $Or = '';
      $And = 'checked';
   }


   
   if($QueryInfo['FieldList']!='')
       $FieldListSelectItem = unserialize($QueryInfo['FieldList']);
   if($QueryInfo['OperatorList']!='')
       $OperatorListSelectItem = unserialize($QueryInfo['OperatorList']); 
   if($QueryInfo['AndOrList']!='')
       $SelectItem =  unserialize($QueryInfo['AndOrList']);  
   if($QueryInfo['ValueList']!='')
       $ValueListSelectItem = unserialize($QueryInfo['ValueList']); 
}

if($_REQUEST['UpdateQueryID'])
{
   $QueryStr = addslashes($_SESSION['BugQueryCondition']);
   $AndOrListCondition = serialize($_SESSION['BugAndOrListCondition']);
   $OperatorListCondition = serialize($_SESSION['BugOperatorListCondition']);
   $ValueListCondition = mysql_real_escape_string(serialize($_SESSION['BugValueListCondition']));
   $FieldListCondition = serialize($_SESSION['BugFieldListCondition']);
   $FieldsToShow = implode(",",array_keys(testSetCustomFields('Bug')));
   
   if(strpos($QueryStr, ') OR (')!== FALSE)
   {
      $Or = 'checked';
      $And = '';
   }
   if(strpos($QueryStr,') AND (')!== FALSE)
   {
      $Or = '';
      $And = 'checked';
   }
   
   dbUpdateRow('TestUserQuery', 'QueryString', "'{$QueryStr}'", 'AndOrList', "'{$AndOrListCondition}'", 'OperatorList', 
                    "'$OperatorListCondition'", 'ValueList',"'$ValueListCondition'", 'FieldList', "'$FieldListCondition'",  'FieldsToShow', "'$FieldsToShow'","QueryID={$_REQUEST['UpdateQueryID']}");
   
   jsGoTo('BugList.php',"parent.RightBottomFrame");
  
}

for($I=0; $I<$FieldCount; $I ++)
{
    $FieldListOnChange = ' onchange="setQueryForm('.$I.');"';
    $OperatorListOnChange = ' onchange="setQueryValue('.$I.');"';
    $FieldList[$I] = htmlSelect($_LANG['BugQueryField'], $FieldName . $I, $Mode, $FieldListSelectItem[$I], $Attrib.$FieldListOnChange);
    $OperatorList[$I] = htmlSelect($_LANG['Operators'], $OperatorName . $I, $Mode, $OperatorListSelectItem[$I], $Attrib.$OperatorListOnChange);
    $ValueList[$I] = '<input id="'.$ValueName.$I.'" name="' . $ValueName.$I .'" type="text" size="5" style="width:95%;"/>';
    $AndOrList[$I] = htmlSelect($_LANG['AndOr'], $AndOrName . $I, $Mode, $SelectItem[$I], $Attrib);
}

$SimpleProjectList = array(''=>'')+testGetValidSimpleProjectList();

$AutoTextValue['ProjectNameText']=jsArray($SimpleProjectList);
$AutoTextValue['ProjectNameValue']=jsArray($SimpleProjectList);
$AutoTextValue['SeverityText']=jsArray($_LANG['BugSeveritys']);
$AutoTextValue['SeverityValue']=jsArray($_LANG['BugSeveritys'], 'Key');
$AutoTextValue['PriorityText']=jsArray($_LANG['BugPriorities']);
$AutoTextValue['PriorityValue']=jsArray($_LANG['BugPriorities'], 'Key');
$AutoTextValue['TypeText']=jsArray($_LANG['BugTypes']);
$AutoTextValue['TypeValue']=jsArray($_LANG['BugTypes'], 'Key');
$AutoTextValue['StatusText']=jsArray($_LANG['BugStatus']);
$AutoTextValue['StatusValue']=jsArray($_LANG['BugStatus'], 'Key');
$AutoTextValue['OSText']=jsArray($_LANG['BugOS']);
$AutoTextValue['OSValue']=jsArray($_LANG['BugOS'], 'Key');
$AutoTextValue['BrowserText']=jsArray($_LANG['BugBrowser']);
$AutoTextValue['BrowserValue']=jsArray($_LANG['BugBrowser'], 'Key');
$AutoTextValue['MachineText']=jsArray($_LANG['BugMachine']);
$AutoTextValue['MachineValue']=jsArray($_LANG['BugMachine'], 'Key');
$AutoTextValue['ResolutionText']=jsArray($_LANG['BugResolutions']);
$AutoTextValue['ResolutionValue']=jsArray($_LANG['BugResolutions'], 'Key');
$AutoTextValue['HowFoundText']=jsArray($_LANG['BugHowFound']);
$AutoTextValue['HowFoundValue']=jsArray($_LANG['BugHowFound'], 'Key');
$AutoTextValue['SubStatusText']=jsArray($_LANG['BugSubStatus']);
$AutoTextValue['SubStatusValue']=jsArray($_LANG['BugSubStatus'], 'Key');

$AutoTextValue['FieldType'] = jsArray($_CFG['FieldType']);
$AutoTextValue['FieldOperationTypeValue'] = jsArray($_CFG['FieldTypeOperation']);
$AutoTextValue['FieldOperationTypeText'] = jsArray($_LANG['FieldTypeOperationName']);

$UserList = testGetCurrentUserNameList('PreAppend');
$ACUserList = array(''=>'', 'Active' => 'Active') + $UserList+array('Closed'=>'Closed');
$UserList = array(''=>'') + $UserList;
$AutoTextValue['ACUserText']=jsArray($ACUserList);
$AutoTextValue['ACUserValue']=jsArray($ACUserList, 'Key');
$AutoTextValue['UserText']=jsArray($UserList);
$AutoTextValue['UserValue']=jsArray($UserList, 'Key');

$TPL->assign('OperatorListSelectItem', $OperatorListSelectItem);
$TPL->assign('ValueListSelectItem', $ValueListSelectItem);
$TPL->assign('QueryTitle', $_REQUEST['QueryTitle']);


$TPL->assign('And', $And);
$TPL->assign('Or', $Or);

$TPL->assign('AutoTextValue', $AutoTextValue);
$TPL->assign("FieldList", $FieldList);
$TPL->assign("OperatorList", $OperatorList);
$TPL->assign("ValueList", $ValueList);
$TPL->assign("AndOrList", $AndOrList);
$TPL->assign("FieldCount",$FieldCount);

$TPL->display('Search.tpl');
?>
