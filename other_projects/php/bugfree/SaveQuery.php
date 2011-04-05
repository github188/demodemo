<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * save query condition.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');

$QueryType = $_POST['QueryType'];


if($_POST['SaveQuery'])
{
    $QueryStr = baseGetGroupQueryStr($_POST);
    $_SESSION[$QueryType . 'QueryCondition'] = $QueryStr;

    
    
    $TPL->assign('QueryType', $QueryType);
    $TPL->display('SaveQuery.tpl');
}

if(isset($_POST['QueryTitle']))
{
    $QueryStr = baseGetGroupQueryStr($_POST);
    $_SESSION[$QueryType . 'QueryCondition'] = $QueryStr;

    $_SESSION[$QueryType . 'AndOrListCondition'] = baseGetAndOrListStr($_POST);
    $_SESSION[$QueryType . 'FieldListCondition'] = baseGetFieldListStr($_POST);
    $_SESSION[$QueryType . 'OperatorListCondition'] = baseGetOperatorListStr($_POST);
    $_SESSION[$QueryType . 'ValueListCondition'] = baseGetValueListStr($_POST);
    
    $_POST['QueryTitle'] = trim($_POST['QueryTitle']);


    if($_POST['QueryTitle'] == '')
    {
        jsAlert($_LANG['NoQueryTitle']);
        jsGoTo('back');
        exit;
    }

    $UserName = mysql_real_escape_string($_SESSION['TestUserName']);
    $QueryTitle = trim($_POST['QueryTitle']);

          // check the querytitle
    $TempQuery = dbGetRow('TestUserQuery', "", "QueryTitle = '{$QueryTitle}' AND QueryType = '{$QueryType}' AND UserName = '{$UserName}'");
    if(!empty($TempQuery))
    {
          //jsAlert($_LANG['DuplicateQueryTitle']);
          //jsGoTo('back');
          jsConfirm($_LANG['DuplicateQueryTitle'] . '\"' .$QueryTitle .'\"?' ,  'Search' . $QueryType . '.php?UpdateQueryID=' . $TempQuery['QueryID'], 'back' , 'parent.RightTopFrame');
    }
    else
    {
        $_SESSION[$QueryType . 'QueryTitle'] = $QueryTitle;
        $QueryStr = addslashes($_SESSION[$QueryType . 'QueryCondition']);
        $AndOrListCondition = serialize($_SESSION[$QueryType . 'AndOrListCondition']);
        $OperatorListCondition = serialize($_SESSION[$QueryType . 'OperatorListCondition']);
        $ValueListCondition = mysql_real_escape_string(serialize($_SESSION[$QueryType . 'ValueListCondition']));
        $FieldListCondition = serialize($_SESSION[$QueryType . 'FieldListCondition']);
        $FieldsToShow = implode(",",array_keys(testSetCustomFields($QueryType)));
        
        $QueryID = dbInsertRow('TestUserQuery',"'{$UserName}','{$QueryType}','{$QueryTitle}','{$QueryStr}',now(),'{$AndOrListCondition}',
             '{$OperatorListCondition}', '{$ValueListCondition}','{$FieldListCondition}','{$FieldsToShow}'", 'UserName,QueryType,QueryTitle,QueryString,AddDate,
               AndOrList, OperatorList, ValueList, FieldList,FieldsToShow');
        
        jsGoTo("UserControl.php","parent.LeftBottomFrame");
        jsGoTo($QueryType . 'List.php');
    }
    
}
?>
