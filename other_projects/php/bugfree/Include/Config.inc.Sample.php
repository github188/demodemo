<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * config sample.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */

/* Report all errors except E_NOTICE. */
error_reporting(E_ALL ^ E_NOTICE);

/* 1. Define surported language list and default language. Note: you can use only one charset Chinese lanuage now. */
$_CFG['LangList']['EN_UTF-8']    = 'English UTF-8';
$_CFG['LangList']['ZH_CN_UTF-8'] = 'ZH_CN UTF-8';
$_CFG['DefaultLang']             = 'ZH_CN_UTF-8';

/* 2. Define admin user list. Like this: array('admin','yourloginname') */
$_CFG['AdminUser'] = array('admin');

/* Define report user list. Like this: array('admin', 'someone@example.com');*/
$_CFG['MailReportUser'] = array('');

/* 3. Define the username and password of the BugFree database. */
$_CFG['DB']['User']        = 'root';
$_CFG['DB']['Password']    = '';
$_CFG['DB']['Host']        = 'localhost';
$_CFG['DB']['Database']    = 'bugfree2';
$_CFG['DB']['TablePrefix'] = 'bf_';
$_CFG['DBCharset']         = 'UTF8';

/*
 * 4. Define the username and password of the user validating database.
 * If the user validating database is different from the BugFree database, uncomment lines below.
 */
/*
$_CFG['UserDB']['User']     = 'root';
$_CFG['UserDB']['Password'] = '';
$_CFG['UserDB']['Host']     = 'localhost';
$_CFG['UserDB']['Database'] = 'BugFree';
*/

/* 5. Define validating table and it's fields. If you don't use item 4, don't change them. */
$_CFG['UserTable']['TableName']     = 'TestUser';
$_CFG['UserTable']['UserName']      = 'UserName';
$_CFG['UserTable']['RealName']      = 'RealName';
$_CFG['UserTable']['UserPassword']  = 'UserPassword';
$_CFG['UserTable']['Email']         = 'Email';
$_CFG['UserTable']['NoticeFlag']    = 'NoticeFlag';
$_CFG['UserTable']['EncryptType']   = 'md5';    // md5|text|mysqlpassword

/* 6. Query Setting. */
$_CFG['QueryFieldNumber'] = 6;      // The fields number to query in QueryBugForm.php
$_CFG['ShowQuery']        = false;  // Showing query condition or not(QueryBug.php).
$_CFG['RecordPerPage']    = 20;     // Record count per page(QueryBug.php).

/* 7. File Setting. */
$_CFG['File']['BugFileName']       = 'BugFileName'; // Bug file name needed in AddBugForm.tpl and function bugAddFile() in FunctionsMain.inc.php.
$_CFG['File']['UploadDirectory']   = $_CFG['RealRootPath'] . '/BugFile';     // The directory to store uploaded files which must be under the BugFree directory and can be written by others. */
$_CFG['File']['MaxFileSize']       = 1024 * 1000;    // The max file size(Byte).
$_CFG['File']['DangerousTypeList'] = array('php','php3','php4','cgi','pl','py','asp','jsp');  // Dangerous file types list, will changed to .txt

/* 8. Mail setting. */
$_CFG['Mail']['On']          = true;
$_CFG['Mail']['FromAddress'] = "bugfree@{$_SERVER['SERVER_NAME']}";
$_CFG['Mail']['FromName']    = 'BugFree';
$_CFG['Mail']['ReportTo']    = array();  // Where bug statistics message sened to. If empty, to all users.
$_CFG['Mail']['SendMethod']  = 'SMTP';   // MAIL|SENDMAIL|SMTP|QMAIL

/* 9. SMTP param setting. */
$_CFG['Mail']['SendParam']['Host']     = '';       // The server to connect. Default is localhost
$_CFG['Mail']['SendParam']['SMTPAuth'] = false;    // Whether or not to use SMTP authentication. Default is FALSE
$_CFG['Mail']['SendParam']['Username'] = '';       // The username to use for SMTP authentication.
$_CFG['Mail']['SendParam']['Password'] = '';       // The password to use for SMTP authentication.

/* 10. Auto update. We recommend you to set this to true, thus you can keep update with the latest version. */
$_CFG['Version']    = '2.0';    // Don't change.
$_CFG['AutoUpdate'] = true;     // true|false.

/* 11. Define the template dir. */
$_CFG['TemplateDir']    = $_CFG['RealRootPath'] . '/Template';
$_CFG['TPLCompileDir']  = $_CFG['RealRootPath'] . '/Data/TplCompile';

/* 12. Define debug mode. */
$_CFG['DebugMode'] = false;
?>
