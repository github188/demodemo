<?php
define('API_NEWLINE', chr(0x03));
define('API_DEVIDER', chr(0x04));
define('API_KEY', '');

$_CFG['API']['FieldsDefauleValue']['Bug'] = array(
    'ProjectID'   => '', 
    'ModuleID'    => '0', 
    'BugTitle'    => '', 
    'ReproSteps'  => ' ', 
    'AssignedTo'  => 'Active',
    'BugSeverity' => '3', 
    'BugType'     => 'CodeError',
    'HowFound'    => 'FuncTest', 
    'OpenedBuild' => 'N/A',
    'ReplyNote'   => '',
    'BugKeyword'  => '',
);
$_CFG['API']['FieldsDefauleValue']['Case'] = array(
    'ProjectID'   => '', 
    'ModuleID'    => '0', 
    'CaseTitle'   => '', 
    'CaseSteps'   => ' ', 
    'CaseStatus'  => 'Active',
    'CaseType'    => 'Functional',
    'AssignedTo'  => 'Active',
    'CasePriority'=> '3', 
    'CaseMethod'  => 'Automation',
    'ReplyNote'   => '',
    'ScriptedBy'  => '',
    'ScriptedDate'=> '',
    'ScriptStatus'=> '',
    'ScriptLocation' => '',
    'BugID' => '0',
    'LinkID' => '0',
);
$_CFG['API']['FieldsDefauleValue']['Result'] = array(
    'ResultValue' => '',
    'ResultBuild' => 'N/A', 
    'ResultStatus'=> 'Completed',
    'CaseType'    => 'Functional',
    'AssignedTo'  => 'Closed',
    'ReplyNote'   => '',
);


/********************************************************************************************/
class object 
{
    function object()
    {
        $args = func_get_args();
        if(method_exists($this, '__destruct'))
        {
            register_shutdown_function(array(&$this, '__destruct'));
        }
        call_user_func_array(array(&$this, '__construct'), $args);
    }
                                       
    function __construct() 
    {
    }
}

class bfPage extends object
{
    var $Param;
    var $SessionName;
    var $SessionID;
    var $CacheExpire;
    var $expiretimestamp;
    var $ErrorCode;
    var $ErrorMsg;
    var $Rand;

    function __construct()
    {
        foreach($_POST as $key => $value)
        {
            $this->Param[$key] = $value;
        }
        foreach($_GET as $key => $value)
        {
            $this->Param[$key] = $value;
        }
        
        $now = time();
        $this->SessionName = session_name();
        $this->SessionID = session_id();
        $this->CacheExpire = session_cache_expire();
        if($_SESSION['ApiRand'] == '')
        {
            $this->Rand = substr(md5(mt_rand()),1,5);
            $_SESSION['ApiRand'] = $this->Rand;
        }
        else
        {
            $this->Rand = $_SESSION['ApiRand'];
        }
        
        if($_SESSION['ExpireTimestamp'] - 0 <= 0)
        {
            $_SESSION['ExpireTimestamp'] = $now + $this->CacheExpire;
        }
 
        $this->ExpireTimestamp = $now + $this->CacheExpire;
        if($this->ExpireTimestamp < $now)
        {
            //$this->setErrorCode('API_SESS_EXPIRE');
        }
    }

    function getSname()
    {
        return $this->SessionName;
    }

    function getSid()
    {
       return $this->SessionID;
    }

    function setErrorCode($code)
    {
        $this->ErrorCode = $code;
        return;
    }

    function getErrorCode()
    {
        return $this->ErrorCode;
    }

    function errorMsg()
    {
        if($this->ErrorCode !='')
        {
            echo ('error=' . $this->ErrorCode);
        }
        return;
    }
}

class bfUser extends bfPage
{
    var $UserName;
    var $UserPassword;
    var $UserAuth;

    function __construct($UserName)
    {
        parent::__construct();

        $UserInfo = testGetUserInfoByName($UserName);
        $this->UserName = $UserInfo['UserName'];
        $this->UserPassword = $UserInfo['UserPassword'];
        $this->UserAuth = $this->encrypt();
    }

    function login()
    {
        if($this->Param[$this->SessionName] != $this->SessionID || $this->Param[$this->SessionName] == '')
        {
            $this->setErrorCode('API_ERROR_LOGIN_WRONGSESSION');
            return false;
        }

        if($_SESSION['TestUserName'] != '' && $_SESSION['TestUserName'] == $this->UserNmae)
        {
            return true;
        }

        if($this->Param['mode'] != 'login')
        {
            $this->setErrorCode('API_ERROR_LOGIN_MODE');
            return false;
        }
        if($this->UserName == '')
        {
            $this->setErrorCode('API_ERROR_LOGIN_NOUSERNAME');
            return false;
        }
        if(!sysCheckUserNameFormat($this->UserName))
        {
            $this->setErrorCode('API_ERROR_LOGIN_WRONGUSERNAME');
            return false;
        }

        if($this->Param['auth'] != $this->UserAuth)
        {
            $this->setErrorCode('API_ERROR_LOGIN_WRONGAUTH');
            return false;
        }

        $_SESSION['TestUserName'] = $this->UserName;
        $_SESSION['TestUserACL'] = baseGetUserACL($_SESSION['TestUserName']);
        $_SESSION['TestUserACLSQL'] = 'ProjectID' . dbCreateIN(join(',', array_keys($_SESSION['TestUserACL'])));

        return true;
    }

    function isLogin()
    {
        if($_SESSION['TestUserName'] != '')
        {
            return true;
        }
        $this->setErrorCode('API_ERROR_LOGIN_MODE');
        return false;
    }

    function encrypt()
    {
        return md5(md5($this->UserName . $this->UserPassword) . API_KEY . $this->Rand);
    }
}

function bfIconv($Charset1 = '', $Charset2 = '', $String = '')
{
    if($Charset1 == '' || $Charset2 == '')
    {
        return $String;
    }
    if(strtolower($Charset1) == strtolower($Charset2))
    {
        return $String;
    }
    else
    {
        if(function_exists('iconv'))
        {
            return iconv($Charset1, $Charset2, $String);
        }
        elseif(function_exists('mb_convert_encoding'))
        {
            return mb_convert_encoding($String, $Charset2, $Charset1);
        }
        else
        {
            return $String;
        }
    }
}

function createSuccessMsg($MessageInfo)
{ 
    global $Charset;    
    $message = "success" . API_NEWLINE;
    $titleList = array();
    $valueList = array();
    foreach($MessageInfo as $Key => $Value)
    {
        $titleList[] = $Key;
        $valueList[] = bfIconv('UTF-8',$Charset,$Value);
    }

    $message .= join(API_DEVIDER, $titleList) . API_NEWLINE;
    $message .= join(API_DEVIDER, $valueList);
    $message .= API_NEWLINE . md5($message);
    return $message;
}

function apiEditItem($ItemType, $FieldsValue, $RawItemInfo)
{
    global $Charset;    
    global $bfUser;
    $LastActionID = testGetLastActionID($ItemType,$RawItemInfo[$ItemType . 'ID']);
    foreach($FieldsValue as $FieldName => $DefaultValue)
    {
        if(in_array($FieldName, array('BugTitle', 'ReproSteps', 'CaseTitle', 'CaseSteps', 'ResultSteps', 'AssignedTo','ScriptedBy')))
        {
            $RawItemInfo[$FieldName] = html_entity_decode($RawItemInfo[$FieldName]);
            $RawItemInfo[$FieldName] = addslashes($RawItemInfo[$FieldName]);
        }
        if(!isset($bfUser->Param[$FieldName])) continue;
        if(in_array($FieldName, array('BugTitle', 'ReproSteps', 'CaseTitle', 'CaseSteps', 'ResultSteps', 'AssignedTo','ScriptedBy')))
        {
            $bfUser->Param[$FieldName] = addslashes($bfUser->Param[$FieldName]);
        }
        if($FieldName == 'ScriptLocation')
        { 
            $bfUser->Param[$Fieldname] = urldecode($bfUser->Param[$FieldName]);
        }
        $ParamValue = bfIconv($Charset, 'UTF-8', $bfUser->Param[$FieldName]);
        $RawItemInfo[$FieldName] = $ParamValue;
    }
    if($RawItemInfo['BugStatus'] == 'Active')
    {
        unset($RawItemInfo['ResolvedBuild']);
        unset($RawItemInfo['Resolution']);
    }
    if(isset($bfUser->Param['ActionType']))
    {
        $RawItemInfo['ActionType'] = $bfUser->Param['ActionType'];
    }
 
    $RawItemInfo['TestUserName'] = mysql_real_escape_string($_SESSION['TestUserName']);
    $RawItemInfo['LastActionID'] = $LastActionID;
    $EditFunction  = 'testEdit' . $ItemType;
    $ActionInfo = $EditFunction($RawItemInfo);
    //print_r($ActionInfo);
    return $ActionInfo['Bingle'];
}

function apiAddItem($ItemType, $FieldsValue)
{
    global $Charset;    
    global $bfUser;
    $ItemInfo = array();
    if($ItemType == 'Result')
    {
        $CaseInfo = dbGetRow('CaseInfo',$Coumns,"CaseID = '{$_REQUEST[CaseID]}'  AND {$_SESSION[TestUserACLSQL]} AND IsDroped = '0'");
        if(!empty($CaseInfo))
        {
            $ItemInfo['ResultTitle'] = $CaseInfo['CaseTitle'];
            $ItemInfo['ProjectID'] = $CaseInfo['ProjectID'];
            $ItemInfo['ModuleID'] = $CaseInfo['ModuleID'];
            $ItemInfo['ResultSteps'] = $CaseInfo['CaseSteps'];
            $ItemInfo['CaseID'] = $CaseInfo['CaseID'];
        }
    }

    foreach($FieldsValue as $FieldName => $DefaultValue)
    {
        if(in_array($FieldName, array('BugTitle', 'ReproSteps', 'CaseTitle', 'CaseSteps', 'ResultSteps','AssignedTo','ScriptedBy')))
        {
            $bfUser->Param[$FieldName] = addslashes($bfUser->Param[$FieldName]);
        }
        if($FieldName == 'ScriptLocation')
        { 
            $bfUser->Param[$Fieldname] = urldecode($bfUser->Param[$FieldName]);
        }
        $ParamValue = bfIconv($Charset, 'UTF-8', $bfUser->Param[$FieldName]);
        $ItemInfo[$FieldName] = $ParamValue != '' ? $ParamValue : $DefaultValue;
    }
    $ItemInfo['TestUserName'] = mysql_real_escape_string($_SESSION['TestUserName']);
    
    $AddFunction  = 'testOpen' . $ItemType;
    $ActionInfo = $AddFunction($ItemInfo);
    $ID = $ActionInfo[$ItemType . 'ID'];
    //print_r($ActionInfo);
    return $ID;
}
?>
