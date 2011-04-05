<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * login.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require("Include/Init.inc.php");

$CurrentDBVersion = dbGetRow('TestOptions', '', "OptionName = 'dbVersion'");
if($CurrentDBVersion['OptionValue'] < BF_DB_VERSION)
{
    jsGoTo($_CFG["BaseURL"] . "/install.php","top");
    exit;
}

sysXajaxRegister("xCheckUserLogin,xSelectLanguage");
baseJudgeUserLogin();

if($_SESSION['TestUserName'] != '')
{
    jsGoto($_CFG["BaseURL"] . "/index.php","parent.parent");
    exit;
}

$LanguageDir = $_CFG['RealRootPath'] . '/Lang/';

require_once($LanguageDir . 'Languages.php');
$LanguageList = $_CFG['Languages'];
$LanguageSelectList = htmlSelect($LanguageList, 'Language', '',$_CFG['UserLang'], 'class="NormalSelect MyInput select" onchange="xajax_xSelectLanguage(this.value);return false;"');

$TPL->assign('LanguageList', $LanguageSelectList);
$TPL->assign('RememberStatus', $_COOKIE['BFRememberStatus']);
$TPL->assign('TestUserName', sysStripSlash($_COOKIE['BFUserName']));

$TPL->display('Login.tpl');
?>
