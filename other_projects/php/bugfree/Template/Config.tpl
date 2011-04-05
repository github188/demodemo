/**
 * Config file of BugFree system.
 *
 * BugFree is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * BugFree is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BugFree; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 * @version     2.0
 * @created     {$CreateDate}
 */

/* Report all errors except E_NOTICE. */
error_reporting(E_ALL ^ E_NOTICE);

/* 1. Define surported language list and default language. Note: you can use only one charset Chinese lanuage now. */
$_CFG['LangList']['EN_UTF-8']    = 'English UTF-8';
$_CFG['LangList']['ZH_CN_UTF-8'] = 'ZH_CN UTF-8';
$_CFG['DefaultLang']             = '{$Language}';{if $InstallMode}
$_CFG['DBCharset']               = 'UTF8';{/if}

/* 2. Define admin user list. Like this: array('wangcs','liuzf') */
$_CFG['AdminUser'] = array('{$Config.AdminUserName}');

/* Define report user list. Like this: array('admin', 'someone@example.com');*/
$_CFG['MailReportUser'] = array('');

/* 3. Define the username and password of the BugFree database. */
$_CFG['DB']['User']     = '{$Config.DBUser}';
$_CFG['DB']['Password'] = '{$Config.DBPassword}';
$_CFG['DB']['Host']     = '{$Config.DBHost}';
$_CFG['DB']['Database'] = '{$Config.DBDatabase}';
$_CFG['DB']['TablePrefix'] = '{$Config.TablePrefix}';

{if $Config.UseAnotherUserDB == 'on'}
/*
 * 4. Define the username and password of the user validating database.
 * If the user validating database is different from the BugFree database, uncomment lines below.
 */

$_CFG['UserDB']['User']     = '{$Config.UserDBUser}';
$_CFG['UserDB']['Password'] = '{$Config.UserDBPassword}';
$_CFG['UserDB']['Host']     = '{$Config.UserDBHost}';
$_CFG['UserDB']['Database'] = '{$Config.UserDBDatabase}';

/* 5. Define validating table and it's fields. If you don't use item 4, don't change them. */
$_CFG['UserTable']['TableName']     = '{$Config.TableName}';
$_CFG['UserTable']['UserName']      = '{$Config.UserName}';
$_CFG['UserTable']['RealName']      = '{$Config.RealName}';
$_CFG['UserTable']['UserPassword']  = '{$Config.UserPassword}';
$_CFG['UserTable']['Email']         = '{$Config.Email}';
$_CFG['UserTable']['EncryptType']   = '{$Config.EncryptType}';    // md5|text|mysqlpassword
{else}
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
$_CFG['UserTable']['EncryptType']   = 'md5';    // md5|text|mysqlpassword|discuzuc
{/if}

/* 6. Query Setting. */
$_CFG['QueryFieldNumber'] = 6;      // The fields number to query in Search*.php
$_CFG['ShowQuery']        = false;  // Showing query condition or not(QueryBug.php).
$_CFG['RecordPerPage']    = 20;     // Record count per page(QueryBug.php).

/* 7. File Setting. */
$_CFG['File']['BugFileName']       = 'BugFileName'; // Bug file name needed in AddBugForm.tpl and function bugAddFile() in FunctionsMain.inc.php.
$_CFG['File']['UploadDirectory']   = $_CFG['RealRootPath'] . '/{$Config.UploadDirectory}';     // The directory to store uploaded files which must be under the BugFree directory and can be writed by others. */
$_CFG['File']['MaxFileSize']       = {$Config.MaxFileSize};    // The max file size(Byte).
$_CFG['File']['DangerousTypeList'] = array('php','php3','php4','cgi','pl','py','asp','jsp','exe');  // Dangerous file types list, will changed to .txt

/* 8. Mail setting. */
$_CFG['Mail']['On']          = true;
$_CFG['Mail']['FromAddress'] = "{$Config.FromAddress}";
$_CFG['Mail']['FromName']    = '{$Config.FromName}';
$_CFG['Mail']['ReportTo']    = array();  // Where bug statistics message sened to. If empty, to all users.
$_CFG['Mail']['SendMethod']  = '{$Config.SendMethod}';   // MAIL|SENDMAIL|SMTP|QMAIL

/* 9. SMTP param setting. */
$_CFG['Mail']['SendParam']['Host']     = '{$Config.SmtpHost}';       // The server to connect. Default is localhost
$_CFG['Mail']['SendParam']['SMTPAuth'] = {$Config.SmtpAuth};    // Whether or not to use SMTP authentication. Default is FALSE
$_CFG['Mail']['SendParam']['Username'] = '{$Config.SmtpUserName}';       // The username to use for SMTP authentication.eg:bugfree@bugfree.org.cn.
$_CFG['Mail']['SendParam']['Password'] = '{$Config.SmtpPassword}';       // The password to use for SMTP authentication.

/* 10. Auto update. We recommend you to set this to true, thus you can keep update with the latest version. */
$_CFG['Version']    = '2.0';    // Don't change.
$_CFG['AutoUpdate'] = true;     // true|false.

/* 11. Config the template's dir and its compile dir. */
$_CFG['TemplateDir']    = $_CFG['RealRootPath'] . '/Template';
$_CFG['TPLCompileDir']  = $_CFG['RealRootPath'] . '/Data/TplCompile';
