<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * list cases.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');
require('Include/FuncImportOutport.php');

@ini_set('memory_limit', -1);
if($_SESSION['TestCurrentProjectID'] - 0 > 0)
{
    //$_SESSION['CaseQueryCondition'] = "ProjectID = '{$_SESSION[TestCurrentProjectID]}'";
}
if($_GET['ProjectID'] - 0 > 0)
{
    $_SESSION['CaseQueryCondition'] = "ProjectID = '{$_GET[ProjectID]}'";
    testSetCurrentProject($_GET['ProjectID']);
}
if($_GET['ModuleID'] - 0 > 0)
{
    $_SESSION['CaseQueryCondition'] = "ModuleID IN ({$_GET['ChildModuleIDs']})";
}
$_SESSION['TestCurrentModuleID'] = $_GET['ModuleID'];

if($_POST['PostQuery'])
{
    $QueryStr = baseGetGroupQueryStr($_POST);
    $_SESSION['CaseQueryCondition'] = $QueryStr;
    $_SESSION['CaseQueryTitle'] = '';

    $_SESSION['CaseAndOrListCondition'] = baseGetAndOrListStr($_POST);
    $_SESSION['CaseFieldListCondition'] = baseGetFieldListStr($_POST);
    $_SESSION['CaseOperatorListCondition'] = baseGetOperatorListStr($_POST);
    $_SESSION['CaseValueListCondition'] = baseGetValueListStr($_POST);

}

if($_REQUEST['QueryID'])
{
    $QueryInfo = dbGetRow('TestUserQuery', '', "QueryID='{$_REQUEST[QueryID]}' AND QueryType='Case'");
    $_SESSION['CaseQueryCondition'] = $QueryInfo['QueryString'];
    $_SESSION['CaseQueryTitle'] = $QueryInfo['QueryTitle'];

    $_SESSION['CaseAndOrListCondition'] = unserialize($QueryInfo['AndOrList']);
    $_SESSION['CaseFieldListCondition'] = unserialize($QueryInfo['FieldList']);
    $_SESSION['CaseOperatorListCondition'] = unserialize($QueryInfo['OperatorList']);
    $_SESSION['CaseValueListCondition'] = unserialize($QueryInfo['ValueList']);

    $_SESSION['CaseFieldsToShow'] = $QueryInfo['FieldsToShow'];

}
else
{
    $_REQUEST['QueryID'] = '-1';	
}

$WHERE = array();
$URL = array();

$WHERE[] = $_SESSION['TestUserACLSQL'];

if($_SESSION['CaseQueryCondition'] != '')
{
    $WHERE[] = $_SESSION['CaseQueryCondition'];
}

if($_GET['OrderBy'])
{
    $OrderByList = explode('|', $_GET['OrderBy']);
    $OrderByColumn = $OrderByList[0];
    $OrderByType = $OrderByList[1];
    $OrderBy = join(' ', $OrderByList);
    $URL[] = 'OrderBy=' . $_GET['OrderBy'];
}
else
{
    $OrderBy = ' CaseID DESC';
    $OrderByColumn = 'CaseID';
    $OrderByType = 'DESC';
}

if($_GET['QueryMode'])
{
    $QueryModeList = explode('|', $_GET['QueryMode']);
    $QueryColumn = $QueryModeList[0];
    $QueryValue = $QueryModeList[1];
    $WHERE = array();
    $WHERE[] = $_SESSION['TestUserACLSQL'];
    $QueryCondition = "";
    if(preg_match('/date/i', $QueryColumn))
    {
        $QueryCondition =  $QueryColumn . ' ' . sysStrToDateSql($QueryValue);
    }
    else
    {
        $QueryCondition = "{$QueryColumn}='{$QueryValue}'";
    }
    $_SESSION['CaseQueryCondition'] = $QueryCondition;
    $WHERE[] = $QueryCondition;
    $URL[] = 'QueryMode=' . sysStripSlash($_GET['QueryMode']);
}
$Url = '?' . join('&', $URL);
$WHERE[] = "IsDroped = '0'";
$Where = join(' AND ', $WHERE);


$FieldsToShow = testSetCustomFields('Case');

/* Get pagination */
$Pagination = new Page('CaseInfo', '', '', '', 'WHERE ' . $Where . ' ORDER BY ' . $OrderBy, $Url, $MyDB);
$LimitNum = $Pagination->LimitNum();
$TPL->assign('PaginationDetailInfo', $Pagination->getDetailInfo());

$ColumnArray = @array_keys($FieldsToShow);
$Columns = 'CaseID,' . join(',',$ColumnArray);

$OrderColumnList = $ColumnArray;
$OrderByTypeList = array();
foreach($OrderColumnList as $OrderColumn)
{
    if($OrderColumn == $OrderByColumn)
    {
        $OrderByTypeList[$OrderColumn] = $OrderTypeReverseArray[$OrderByType];
    }
    else
    {
        $OrderByTypeList[$OrderColumn] = $OrderByType;
    }
}

if($_FILES['file']['error']==2){
      sysObFlushJs("alert('{$_LANG['ImportFileExceed']}');");
}
elseif($_FILES['file']['error']==0)
{
        if($_FILES['file']['tmp_name'])   // 批量导入
        {
	 
	  $findtype=strtolower(strrchr($_FILES['file']['name'],"."));
         
          if($findtype!='.xml'){
            sysObFlushJs("alert('{$_LANG['ImportFileSuffixError']}');");
          }
          else {
		  $file=fopen($_FILES['file']['tmp_name'],"r");      
		  $FileContents = fread($file, filesize($_FILES['file']['tmp_name'])); 
		  $FileContents = preg_replace('/<Font.*>|<\/Font>/Us', '', $FileContents);
                  
                 // $FileContents = preg_replace("/'/Us", "\'", $FileContents);

                  $Dom =  XML_unserialize($FileContents);                   
                  $DomRowArray = $Dom[Workbook][Worksheet][Table][Row];
                
                  ParseImportXML($DomRowArray,$_LANG);
        }
    }
}

if($_GET['Export'] == 'HtmlTable')
{
    $Columns = '';
    $LimitNum = '';
}

$CaseList = dbGetList('CaseInfo',$Columns, $Where, '', $OrderBy, $LimitNum);

if($_GET['Export'] == 'XMLFile')  //批量导出
{   
   $CaseExportList = dbGetList('CaseInfo','', $Where, '', $OrderBy); //�?��要导出的Case列表
   if(sizeof($CaseExportList)>5000){
      sysObFlushJs("alert('{$_LANG['ExportCountExceed']}');");

   }  
   else { 
      $CaseExportColumnMust = array('CaseID' , 'CaseTitle', 'ProjectName', 'ModulePath', 'CaseSteps'); //必须包含的字�?
      
      $CaseExportColumnArray = $ColumnArray; 
      foreach($CaseExportColumnMust as $Item)
      { //如果定制的列没有包含必须包含的字段，则将该字段加入到导出字段�?
         if(!in_array($Item, $CaseExportColumnArray))
            $CaseExportColumnArray[] = $Item;

      }
      
      $Content = ExportXML($CaseExportList,$CaseExportColumnArray,$_LANG['CaseFields']);


      header('Content-type: text/xml; charset=utf-8');
      header('Content-Disposition: attachment; filename=caselist.xml');
	  header("Expires: 0");
	  header("Cache-Control: must-revalidate, post-check=0,pre-check=0");
	  header("Pragma: public");
      
      echo $Content;
      exit;
   }
}

$UserNameList = testGetOneDimUserList();
$CaseList = testSetCaseListMultiInfo($CaseList, $UserNameList);

$TPL->assign('CaseListNumber', sizeof($CaseList));
$TPL->assign('CaseList', $CaseList);
$TPL->assign('OrderByColumn', $OrderByColumn);
$TPL->assign('OrderByType', $OrderByType);
$TPL->assign('QueryMode', $_GET['QueryMode']);
$TPL->assign('QueryColumn', $QueryColumn);
$TPL->assign('OrderByTypeList', $OrderByTypeList);
$TPL->assign('BaseTarget', '_self');
$TPL->assign('TestMode', 'Case');

if($_GET['Export'] == 'HtmlTable')
{
    $TPL->assign('DataList', $CaseList);
    $TPL->assign('FieldsToShow', $_LANG["CaseFields"]);
    $TPL->display('ExportList.tpl');
    exit;
}
$TPL->assign('QueryID', $_REQUEST['QueryID']);
$TPL->assign('QueryTitle', $_SESSION['CaseQueryTitle'] );

$TPL->display('CaseList.tpl');




?>
