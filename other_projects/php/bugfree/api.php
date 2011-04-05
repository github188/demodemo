<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * BugFre api.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');
require('Include/Api.inc.php');

$Mode = $_REQUEST['mode'];
$Charset = $_REQUEST['charset'];
if($Charset == '') $Charset = 'UTF-8';
@header("Content-Type: text/html; charset=".$Charset);

$MessageInfo = array();
switch($Mode)
{
    // get session
    case 'getsid':
        $bfPage = new bfPage();
        $MessageInfo['sessionname'] = $bfPage->SessionName;
        $MessageInfo['sessionid'] = $bfPage->SessionID;
        $MessageInfo['rand'] = $bfPage->Rand;
        echo createSuccessMsg($MessageInfo);
        break;

    // user login
    case 'login':
        $bfUser = new bfUser($_REQUEST['username']);
        if(!$bfUser->login())
        {
            $bfUser->errorMsg();
        }
        else
        {
            $MessageInfo['timeout'] = $bfUser->ExpireTimestamp;
            echo createSuccessMsg($MessageInfo);
        }
        break;

    // get bug, case, result info
    case 'getbug': case 'getcase': case 'getresult':
        preg_match('/^get(bug|case|result)$/', $Mode, $regs);
        $lowerItem = $regs[1];
        $Item = ucfirst($lowerItem);
        $ID = $_REQUEST[$Item . 'ID'];

        $bfUser = new bfUser($_SESSION['TestUserName']);
        if(!$bfUser->isLogin())
        {
            $bfUser->errorMsg();
        }
        else
        {
            $MessageInfo = dbGetRow("{$Item}Info",$Coumns,"{$Item}ID = '{$ID}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
            if(empty($MessageInfo))
            {
                echo "error=" . 'API_ERROR_NOPERMISSION';
            }
            else
            {
                echo createSuccessMsg($MessageInfo);
            }
        }
        break;

    // add bug, case, result
    case 'addbug': case 'addcase': case 'addresult':
        preg_match('/^add(bug|case|result)$/', $Mode, $regs);
        $lowerItem = $regs[1];
        $Item = ucfirst($lowerItem);

        $bfUser = new bfUser($_SESSION['TestUserName']);
        if(!$bfUser->isLogin())
        {
            $bfUser->errorMsg();
        }
        else
        {
            $ProjectID = $bfUser->Param['ProjectID'];
            if($Mode == 'addresult')
            {
                $CaseInfo = dbGetRow('CaseInfo',$Coumns,"CaseID = '{$_REQUEST[CaseID]}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
                if(!empty($CaseInfo['ProjectID'])) $ProjectID = $CaseInfo['ProjectID'];
            }
            if($_SESSION['TestUserACL'][$ProjectID] != 'All')
            {
                echo "error=" . 'API_ERROR_NOPERMISSION';
            }
            else
            {
                // field's default value
                $FieldsValue = $_CFG['API']['FieldsDefauleValue'][$Item];
                $ID = apiAddItem($Item, $FieldsValue);
                if($ID > 0)
                {
                    $MessageInfo[$Item . 'ID'] = $ID;
                    echo createSuccessMsg($MessageInfo);
                }
                else
                {
                    echo 'error=ADD_' . $Item . '_FAILED';
                }        
            }
        }
        break;

    // update bug, case, result
    case 'updatebug': case 'updatecase': case 'updateresult':
        preg_match('/^update(bug|case|result)$/', $Mode, $regs);
        $lowerItem = $regs[1];
        $Item = ucfirst($lowerItem);
        $ID = $_REQUEST[$Item . 'ID'];

        $bfUser = new bfUser($_SESSION['TestUserName']);
        if(!$bfUser->isLogin())
        {
            $bfUser->errorMsg();
        }
        else
        {
            $ItemInfo = dbGetRow("{$Item}Info",$Coumns,"{$Item}ID = '{$ID}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
            if(empty($ItemInfo))
            {
                echo "error=" . 'API_ERROR_NOPERMISSION';
            }
            else
            {
                // field's default value
                $FieldsValue = $_CFG['API']['FieldsDefauleValue'][$Item];
                $Bingle = apiEditItem($Item, $FieldsValue, $ItemInfo);
                if($Bingle > 0)
                {
                    echo 'success';
                }
                else
                {
                    echo 'error=UPDATE_' . $Item . '_FAILED';
                }        
            }
        }
        break;
}
