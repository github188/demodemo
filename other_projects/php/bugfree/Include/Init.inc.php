<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * init file.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */

/* Get the BugFree real root path. */
$_CFG['RealRootPath'] = dirname(dirname(__FILE__));
define('REAL_ROOT_PATH', $_CFG['RealRootPath']);
define("BF_DB_VERSION", '13');

if(!file_exists($_CFG['RealRootPath'] . '/Include/Config.inc.php'))
{
    die("
        <h1>File 'Include/Config.inc.php' cannot be found. </h1>
        <pre>
        You can manually copy a new Include/Config.inc.php from Include/Config.inc.Sample.php.
        </pre>
");
}

@ini_set("include_path", "." . PATH_SEPARATOR . $_CFG['RealRootPath'] . PATH_SEPARATOR . $_CFG["RealRootPath"] . "/Include/" . PATH_SEPARATOR . $_CFG["RealRootPath"] . "/Include/Class/" . PATH_SEPARATOR . $_CFG["RealRootPath"] . "/Include/Class/PhpMailer");

require(REAL_ROOT_PATH . '/Include/Config.inc.php');                   // Config file.

/* Start session. */
//session_cache_limiter('private, must-revalidate');
session_start();
require(REAL_ROOT_PATH . '/Include/Session.inc.php');                  // Session file.
header('Cache-control: private');

/* Create the BaseURL of BugFree system.
 * If the url of BugFree is wrong, you can specify it by hand, for example: http://www.test.com/BugFree.
 * Note: No "/" at the end of the url.
 */
if(strtolower($_SERVER["HTTPS"]) == "on")
{
    $ServerProtocol = "https";
    $ServerPort = "";
}
else
{
    $ServerProtocol = "http";
    $ServerPort = $_SERVER["SERVER_PORT"] == "80" ? "":":" . $_SERVER["SERVER_PORT"];
}
$_CFG["BasePath"] = preg_replace("/[\/\\\]{1}$/i" , "", dirname($_SERVER["SCRIPT_NAME"]));
$_CFG["BasePath"] = preg_replace("/Admin[\/\\\]{0,1}$/i" , "", $_CFG["BasePath"]);
$_CFG["BaseURL"] = $ServerProtocol . "://" . $_SERVER["SERVER_NAME"] . $ServerPort . $_CFG["BasePath"];
define(BF_COOKIE_PATH, $_CFG['BasePath'] . '/');

/* Require config, functions and class files. */
require(REAL_ROOT_PATH . '/Include/FuncMain.inc.php');                      // Functions file.
require(REAL_ROOT_PATH . '/Include/FuncModel.inc.php');                     // Functions file.
require(REAL_ROOT_PATH . '/Include/Class/Page.class.php');                  // Page class.
require(REAL_ROOT_PATH . '/Include/Class/ADOLite/adodb.inc.php');           // ADO class.
require(REAL_ROOT_PATH . '/Include/Class/TemplateLite/class.template.php'); // Smarty class.

/* Set the language. */
if(isset($_COOKIE['BFUserLang']) && $_COOKIE['BFUserLang'] != '')  $_CFG['UserLang']  = $_COOKIE['BFUserLang'];
if(isset($_SESSION['InstallLanguage']) && $_SESSION['InstallLanguage'] != '')  $_CFG['UserLang']  = $_SESSION['InstallLanguage'];
if(isset($_COOKIE['BFUserStyle'])) $_CFG['UserStyle'] = $_COOKIE['BFUserStyle'];
if(empty($_CFG['UserLang']))       $_CFG['UserLang']  = $_CFG['DefaultLang'];
if(empty($_CFG['UserStyle']))      $_CFG['UserStyle'] = $_CFG['DefaultStyle'];

/* Include the Language file, send heard info. */
if($_CFG['UserLang'] == '')$_CFG['UserLang'] = $_CFG['DefaultLang'];
$LangCommon = $_CFG['RealRootPath'] . '/Lang/' . $_CFG['UserLang'] . '/_COMMON.php';
if(file_exists($LangCommon)) require($LangCommon);
$LangPinYin = $_CFG['RealRootPath'] . '/Lang/' . $_CFG['UserLang'] . '/PinYin.php';
if(file_exists($LangPinYin)) require($LangPinYin);
define ('XAJAX_DEFAULT_CHAR_ENCODING', $_LANG['Charset']);
@header("Content-Type: text/html; charset=".$_LANG["Charset"]);

require(REAL_ROOT_PATH . "/Include/Class/Xajax/xajax.inc.php");       // xajax class.

/* Connect to BugFree database server and return the global DB handler -- $MyDB which is used anywhere and set the FETCH_MODE to ASSOC. */
$MyDB = &ADONewConnection('mysql', 'pear');
$DBResult = $MyDB->Connect($_CFG['DB']['Host'], $_CFG['DB']['User'], $_CFG['DB']['Password'], $_CFG['DB']['Database']);
if(!$DBResult)
{
    if(!preg_match('/install/i', $_SERVER['PHP_SELF']))
    {
        jsGoTo($_CFG["BaseURL"] . "/install.php","top");
        exit;
    }
}

$ADODB_FETCH_MODE = ADODB_FETCH_ASSOC;
$MyDB->debug_console = true;
if(version_compare(@mysql_get_server_info(), '4.1.0', '>=')){$_CFG['Mysql4.1.0Plus'] = true;}
if($_CFG['DBCharset'] != '' && $_CFG['Mysql4.1.0Plus'])
{
    $MyDB->Query("SET NAMES {$_CFG['DBCharset']}");
}

/* Connect to validating database if it's different from BugFree database and return the global DB handler --$MyUserDB. */
if(!empty($_CFG['UserDB']))
{
    $MyUserDB = &ADONewConnection('mysql', 'pear');
    $MyUserDB->NConnect($_CFG['UserDB']['Host'], $_CFG['UserDB']['User'], $_CFG['UserDB']['Password'], $_CFG['UserDB']['Database']);
    if($_CFG['DBCharset'] == 'UTF8')
    {
        $MyUserDB->Query("SET NAMES UTF8");
    }
}
register_shutdown_function('sysCloseDB');

/* Turn off the runtime magic_quotes feature. */
ini_set('magic_quotes_runtime', 0);

/* Init template class. */
$TPL = new Template_Lite;
$TPL->template_dir = $_CFG['TemplateDir'];
$TPL->compile_dir  = $_CFG['TPLCompileDir'];
//$TPL->debugging = true;

/* Add slashes to REQUEST, GET, POST, COOKIE, FILES vars. */
if(!get_magic_quotes_gpc())
{
    $_REQUEST = sysAddSlash($_REQUEST);
    $_GET     = sysAddSlash($_GET);
    $_POST    = sysAddSlash($_POST);
    $_COOKIE  = sysAddSlash($_COOKIE);
    $_FILES   = sysAddSlash($_FILES);
}

if($_SERVER["SERVER_NAME"] != '')
{
    $_CFG['BrowserMode'] = true;

    /* Set xajax */
    $Xajax = new xajax();
    //$Xajax->debugOn();
    if(strtolower($_LANG['Charset']) != 'utf-8')
    {
        $Xajax->decodeUTF8InputOn();
    }
    sysXajaxRegister();
}
//$Xajax->debugOn();

/* Set default testmode */
if($_SESSION['TestMode'] == '')
{
    $_SESSION['TestMode'] = 'Bug';
}

if($_SESSION['BugQueryCondition'] == '')
{
    $_SESSION['BugQueryCondition'] = '1';
}
if($_SESSION['CaseQueryCondition'] == '')
{
    $_SESSION['CaseQueryCondition'] = '1';
}
if($_SESSION['ResultQueryCondition'] == '')
{
    $_SESSION['ResultQueryCondition'] = '1';
}

/* */
$OrderTypeReverseArray = array('ASC'=>'DESC', 'DESC'=>'ASC');
$TPL->assign('OrderTypeReverseArray', $OrderTypeReverseArray);
$TPL->assign('OrderTypeArrowArray',array('ASC'=>'&uarr;', 'DESC'=>'&darr;'));

/* Assign $Config defined in the Lang file to the template. */
$TPL->assign('CFG',  $_CFG);
$TPL->assign('Lang', $_LANG);
$TPL->assign('TestMode', $_SESSION['TestMode']);

/* Judge whether the user is a valid user. */
if(!preg_match('/(Login.php|install|api|Logout)/i', $_SERVER['PHP_SELF']) &&  $_CFG['BrowserMode'])
{
    baseJudgeUserLogin();
    if($_SESSION['TestUserName'] == '')
    {
        if(!preg_match('/(TopNav.php|UserControl.php|ModuleList.php|Search|ControlFrame|Login)/i', $_SERVER['PHP_SELF']))
        {
            $_SESSION['LoginJumpURI'] = $_SERVER['REQUEST_URI'];
        }
        jsGoTo($_CFG["BaseURL"] . "/Login.php","top");
        exit;
    }
}
if(preg_match('/Login.php|Logout/i', $_SERVER['PHP_SELF']) && $_SESSION['LoginJumpURI'] == '')
{
    $_SESSION['LoginJumpURI'] = 'index.php';
}

?>
