<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * install.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
@set_time_limit(0);
@ini_set('memory_limit', -1);

require_once('Include/Init.inc.php');
require_once('Schema.php');
$_CFG['DebugMode'] = false;

@session_destroy();
@setcookie("BFUserPWD", '', time()-1209600,BF_COOKIE_PATH);

if(!function_exists('mysql_connect'))
{
    $ErrorMsg[] = $_LANG["InstallErrorNoMysqlModule"];
    $TPL->assign("ErrorMsg", join("<br />\n", $ErrorMsg));
    $TPL->assign("NoBack", true);
    $TPL->display('Install.tpl');
    exit;
}

if($_GET['action'] == 'CreateDB')
{
    if(!sysCheckDBExists($_CFG['DB']['Database']))
    {
        $MyDB = &ADONewConnection('mysql', 'pear');
        $DBResult = $MyDB->Connect($_CFG['DB']['Host'], $_CFG['DB']['User'], $_CFG['DB']['Password']);
        $MyDB->Query('CREATE DATABASE ' . $_CFG['DB']['Database']) or die($_LANG["InstallErrorCreateBugDB"] . "<br/>" . $MyDB->ErrorMsg());
        die($_LANG["CreateDBSuccessed"]);
    }
    else
    {
        die($_LANG["InstallErrorDBExists"]);
    }
}

if(!$DBResult)
{
    if(!sysCheckDBExists($_CFG['DB']['Database']))
    {
        $CreateDBUrl = "<a href='install.php?action=CreateDB'>{$_LANG['InstallCreateDB']}>></a>";
    }
    die("
    <h1>$_LANG[DBErrorConnection]</h1>
    <ul>
    	<li>$_LANG[DBSureExists] <strong><i>{$_CFG['DB']['Database']}</i> $CreateDBUrl</strong></li>
    	<li>$_LANG[DBSureUserNameAndPwd]</li>
    	<li>$_LANG[DBSureHost]</li>
    	<li>$_LANG[DBSureRunning]</li>
    </ul>
    ");
}

$ErrorMsg = array();

if(!isset($_CFG['DB']['TablePrefix']))$_CFG['DB']['TablePrefix'] = '';
$TablePrefix = $_CFG['DB']['TablePrefix'];

$BugFree1TableList = array(
"BugFile",
"BugGroup",
"BugHistory",
"BugInfo",
"BugModule",
"BugProject",
"BugQuery",
"BugUser",
);

$BugFree2TableList = array(
$TablePrefix."BugInfo",
$TablePrefix."CaseInfo",
$TablePrefix."ResultInfo",
$TablePrefix."TestAction",
$TablePrefix."TestFile",
$TablePrefix."TestHistory",
$TablePrefix."TestModule",
$TablePrefix."TestProject",
$TablePrefix."TestUser",
$TablePrefix."TestUserLog",
$TablePrefix."TestUserQuery",
$TablePrefix."TestOptions",
);

$CurrentDBVersion = '0';

$TempCurrentDBVersion = BF_DB_VERSION;

// dbversion 3
$TableInfo = $MyDB->GetAll("DESC " . $TablePrefix . "TestOptions");
if(!$TableInfo)
{
    $TempCurrentDBVersion = 2;
}

// dbversion >=3
$OptionInfo = dbGetRow('TestOptions', '', "OptionName = 'dbVersion'");
if($OptionInfo['OptionValue'] >= 3)
{
    $TempCurrentDBVersion = $OptionInfo['OptionValue'];
}

// dbversion 2
foreach($BugFree2TableList as $TableName)
{
    if($TableName != $TablePrefix."TestOptions")
    {
        $TableInfo = $MyDB->GetAll("DESC $TableName");
        if(!$TableInfo)
        {
            $TempCurrentDBVersion = 1;
            break;
        }
    }
}
if($TempCurrentDBVersion >= 2) $BugFree2Exists = true;
$TPL->assign('BugFree2Exists', $BugFree2Exists);

// dbversion 1
if($TempCurrentDBVersion == 1)
{
    foreach($BugFree1TableList as $TableName)
    {
        $TableInfo = $MyDB->GetAll("DESC $TableName");
        if(!$TableInfo)
        {
            $TempCurrentDBVersion = 0;
            break;
        }
    }

}

$CurrentDBVersion = $TempCurrentDBVersion;

if($_GET['Step'] == 2)
{
    $InstallOrUpgrade = 'Upgrade';
    $UpgradeTo = BF_DB_VERSION - 1;
    if($_CFG['DB']['TablePrefix'] == '')
    {
        $ErrorMsg[] = $_LANG["InstallErrorPrefixNeeded"];
    }
    elseif($CurrentDBVersion == BF_DB_VERSION)
    {
        $ErrorMsg[] = $_LANG["InstallErrorAlreadyNewest"];
    }
    elseif($CurrentDBVersion == 0)
    {
        $InstallOrUpgrade = 'Install';
        installNewBugFree();
    }
    elseif($CurrentDBVersion >= 1)
    {
        
        if($_GET['NewInstall'] == 1)
        {
            $InstallOrUpgrade = 'Install';
            installNewBugFree();
        }
        else
        {
            $InstallOrUpgrade = 'Upgrade';
            upgrade_list($CurrentDBVersion, $UpgradeTo);
        }
    }
}

$TPL->assign('Step', $_GET['Step']);
$TPL->assign('BF_DB_VERSION', BF_DB_VERSION);
$TPL->assign('CurrentDBVersion', $CurrentDBVersion);
$TPL->assign('ErrorMsg', $ErrorMsg);
$TPL->assign('InstallOrUpgrade', $InstallOrUpgrade);
$TPL->clear_compiled_tpl();
$TPL->display('Install.tpl');


function upgrade_list($From, $To = '')
{
    if($To == '')
    {
        $To = $From;
    }
    for($I=$From;$I<=$To;$I++)
    {
        $UpgradeFunctionName = "upgrade_$I";
        $UpgradeFunctionName();
    }
}

function upgrade_1()
{
    global $_CFG, $TablePrefix, $MyDB, $BugFreeUpgrade1Sql, $ErrorMsg;
    $TableInfo = $MyDB->GetAll("show create table BugInfo");
    if(!preg_match('/CHARSET=utf8/i', $TableInfo[0]['Create Table']) || preg_match('/COLLATE=utf8/i', $TableInfo[0]['Create Table']))
    {
        $MyDB = &ADONewConnection('mysql', 'pear');
        $DBResult = $MyDB->Connect($_CFG['DB']['Host'], $_CFG['DB']['User'], $_CFG['DB']['Password'], $_CFG['DB']['Database']);
        $ADODB_FETCH_MODE = ADODB_FETCH_ASSOC;
        $MyDB->debug_console = true;
    }

    sysExecuteSql($BugFreeUpgrade1Sql, $TablePrefix);
    if(empty($ErrorMsg))
    {

        if(DB_CHARSET != '')
        {
            $MyDB->Query("SET NAMES " . DB_CHARSET);
        }

        $MyDB->Query("ALTER TABLE `BugHistory` CHANGE `BugID` `BugID` MEDIUMINT( 7 )");
        $MyDB->Query("ALTER TABLE `BugFile` CHANGE `BugID` `BugID` MEDIUMINT( 7 )");
        $MyDB->Query("CREATE TABLE {$TablePrefix}TempBugFile SELECT * FROM BugFile");
        $MyDB->Query("CREATE TABLE {$TablePrefix}TempBugHistory SELECT * FROM BugHistory");

        $TotalBugFileList = dbGetList('TempBugFile');
        foreach($TotalBugFileList as $FileInfo)
        {
           $BugFileList[$FileInfo['BugID']][$FileInfo['FileID']] = $FileInfo;
        }

        $HistoryList = dbGetList('TempBugHistory');
        $BugOpenActionID = array();
        //print_r($HistoryList);

        foreach($HistoryList as $HistoryInfo)
        {
            $ActionID = $HistoryInfo['HistoryID'];
            $ActionTarget = 'Bug';
            $BugID = $HistoryInfo['BugID'];
            $ActionUser = $HistoryInfo['UserName'];
            $ActionDate = $HistoryInfo['ActionDate'];

            $ActionField = array();
            $OldValue = array();
            $NewValue = array();
            $NewFileList = array();


            $ActionType = $HistoryInfo['Action'];
            if(preg_match('/Resolved/i', $ActionType))
            {
                $ActionType = 'Resolved';
                $Resolution = str_replace('"', '',preg_replace("/Resolved AS \"()/", "\\1", $HistoryInfo['Action']));
                $ActionField[] = 'Resolution';
                $OldValue[] = '';
                $NewValue[] = $Resolution;
            }
            elseif(preg_match('/Assigned/i', $ActionType))
            {
                $ActionType = 'Edited';
                $AssignedTo = str_replace('"', '',preg_replace("/Assigned To \"()/", "\\1", $HistoryInfo['Action']));
                $ActionField[] = 'AssignedTo';
                $OldValue[] = 'Active';
                $NewValue[] = $AssignedTo;
            }

            if($ActionType == 'Opened')
            {
                $ActionNote = '';
                $ReproSteps = addslashes($HistoryInfo['FullInfo']);
                $BugOpenActionID[$BugID] = $ActionID;
                dbUpdateRow('BugInfo','ReproSteps', "'{$ReproSteps}'", "BugID={$BugID}");
            }
            elseif($ActionType != 'Edited')
            {
                $ActionNote = $HistoryInfo['FullInfo'];
            }
            else
            {
                $ActionNote = $HistoryInfo['FullInfo'];
                if(preg_match_all("/Change <B>(.*)<\/B> FROM <B>\"(.*)\"<\/B> TO <B>\"(.*)\"<\/B>/", $HistoryInfo['FullInfo'], $Matches))
                {
                    foreach($Matches[0] as $Key => $Value)
                    {
                        $ActionNote = str_replace($Value . "\n", '', $ActionNote);
                        $ActionNote = str_replace($Value, '', $ActionNote);

                        $ActionField[] = $Matches[1][$Key];
                        $OldValue[] = $Matches[2][$Key];
                        $NewValue[] = $Matches[3][$Key];
                    }
                }
                if(preg_match_all("/Changed <B>(.*)<\/B> from <B>\"(.*)\"<\/B> to <B>\"(.*)\"<\/B>/", $HistoryInfo['FullInfo'], $Matches))
                {
                    foreach($Matches[0] as $Key => $Value)
                    {
                        $ActionNote = str_replace($Value . "\n", '', $ActionNote);
                        $ActionNote = str_replace($Value, '', $ActionNote);

                        $ActionField[] = $Matches[1][$Key];
                        $OldValue[] = $Matches[2][$Key];
                        $NewValue[] = $Matches[3][$Key];
                    }
                }
                if(preg_match("/<B>Add File<\/B> \"(.*)\"/", $ActionNote, $FileMatches))
                {
                    $ActionNote = str_replace($FileMatches[0] . "\n", '', $ActionNote);
                    $ActionNote = str_replace($FileMatches[0], '', $ActionNote);
                    $FileList = explode(',', $FileMatches[1]);
                    foreach($FileList as $FileTitle)
                    {
                        $Where = "BugID = {$HistoryInfo[BugID]} AND FileTitle = '{$FileTitle}' AND AddUser='{$ActionUser}' AND AddDate='{$ActionDate}'";
                        $FileInfo = dbGetRow('TempBugFile','*', $Where);
                        $NewFileList[$FileInfo['FileID']] = $FileInfo;
                        unset($BugFileList[$BugID][$FileInfo['FileID']]);
                    }
                }
                if(preg_match("/<B>Added File<\/B> \"(.*)\"/", $ActionNote, $FileMatches))
                {
                    $ActionNote = str_replace($FileMatches[0] . "\n", '', $ActionNote);
                    $ActionNote = str_replace($FileMatches[0], '', $ActionNote);
                    $FileList = explode(',', $FileMatches[1]);
                    foreach($FileList as $FileTitle)
                    {
                        $Where = "BugID = {$HistoryInfo[BugID]} AND FileTitle = '{$FileTitle}' AND AddUser='{$ActionUser}' AND AddDate='{$ActionDate}'";
                        $FileInfo = dbGetRow('TempBugFile','*', $Where);
                        $NewFileList[$FileInfo['FileID']] = $FileInfo;
                        unset($BugFileList[$BugID][$FileInfo['FileID']]);
                    }
                }

            }


            $ActionNote = addslashes($ActionNote);
            //    echo $ActionNote. "\n----------------------------------------\n";

            $ActionID = dbInsertRow('TestAction', "$ActionID,'Bug',$BugID,'{$ActionUser}','{$ActionType}','{$ActionDate}','{$ActionNote}'",
                                    'ActionID,ActionTarget,IdValue,ActionUser,ActionType,ActionDate,ActionNote');

            // insert histroy
            foreach($ActionField as $Key => $Value)
            {
                testAddHistory($ActionID, $Value, addslashes($OldValue[$Key]), addslashes($NewValue[$Key]));
            }

            foreach($NewFileList as $FileInfo)
            {
                $FileID = sysInsertFileDB($ActionID, addslashes($FileInfo['FileTitle']), addslashes($FileInfo['FileName']), $FileInfo['FileType'],$FileInfo['FileSize']);
            }
        }
        foreach($BugFileList as $BugID => $BugFile)
        {
            foreach($BugFile as $FileInfo)
            {
                $FileID = sysInsertFileDB($BugOpenActionID[$BugID], addslashes($FileInfo['FileTitle']), addslashes($FileInfo['FileName']), $FileInfo['FileType'],$FileInfo['FileSize']);
            }
        }

        $MyDB->Query("DROP TABLE {$TablePrefix}TempBugFile");
        $MyDB->Query("DROP TABLE {$TablePrefix}TempBugHistory");
    }

}

function upgrade_2()
{
    global $TablePrefix, $MyDB, $BugFreeUpgrade2Sql;

    $ActionTargetList = array('Bug','Case','Result');
    foreach($ActionTargetList as $ActionTarget)
    {
        dbAddColumn("{$TablePrefix}{$ActionTarget}Info", 'ModifiedBy', 'text AFTER LastEditedDate');

        $ActionList = dbGetList('TestAction', '',"ActionTarget = '{$ActionTarget}'",'ActionID ASC');
        $ModifiedList =array();
        foreach($ActionList as $ActionInfo)
        {
            $ModifiedList[$ActionInfo['IdValue']][] = $ActionInfo['ActionUser'];
        }
        foreach($ModifiedList as $Id => $ActionUserList)
        {
            $ActionUsers = ',' . join(',', $ActionUserList) . ',';
            dbUpdateRow("{$ActionTarget}Info", 'ModifiedBy', "'{$ActionUsers}'", "{$ActionTarget}Id=$Id");
        }
    }

    dbAddColumn("{$TablePrefix}TestProject", 'LastEditedBy', "varchar(30) after AddDate");
    dbAddColumn("{$TablePrefix}TestUser", 'AddDate', "DATETIME  Default '0000-00-00 00:00:00' after Email");
    dbAddColumn("{$TablePrefix}TestUser", 'LastEditedBy', "varchar(30) after AddDate");
    dbAddColumn("{$TablePrefix}TestUser", 'LastDate', "DATETIME  Default '0000-00-00 00:00:00' after LastEditedBy");
    dbAddColumn("{$TablePrefix}TestUserGroup",'LastEditedBy', "varchar(30) after AddDate");

    $TableInfo = $MyDB->GetAll("DESC " . $TablePrefix . "TestOptions");
    if(!$TableInfo)
    {
        sysExecuteSql($BugFreeUpgrade2Sql, $TablePrefix);
    }
    sysUpdateOptions('dbVersion', '3');
}

function upgrade_3()
{
    global $TablePrefix;
    dbAddColumn("{$TablePrefix}BugInfo", 'BugSubStatus', "varchar(50) NOT NULL default '' AFTER BugStatus");
    dbAddColumn("{$TablePrefix}BugInfo", 'CaseID', "varchar(255) NOT NULL default '' AFTER LinkID");
    dbAddColumn("{$TablePrefix}CaseInfo", 'ScriptLocation', "varchar(255) NOT NULL default '' AFTER ScriptedDate");
    dbAddColumn("{$TablePrefix}CaseInfo", 'BugID', "varchar(255) NOT NULL default '' AFTER LinkID");
    sysUpdateOptions('dbVersion', '4');
}

function upgrade_4()
{
    global $TablePrefix, $MyDB, $BugFree2AddGroupSql, $_LANG;

    $TableInfo = $MyDB->GetAll("DESC " . $TablePrefix . "TestGroup");
    if(!$TableInfo)
    {
        sysExecuteSql($BugFree2AddGroupSql, $TablePrefix);
    }
    else
    {
        die($TablePrefix . "TestGroup already exists!");
    }

    dbAddColumn("{$TablePrefix}TestProject", 'ProjectManagers', "TEXT AFTER `ProjectName`");
    dbAddColumn("{$TablePrefix}TestProject", 'ProjectGroupIDs', "TEXT AFTER `ProjectManagers`");

    $UserGroupList = dbGetAllColumns('TestUserGroup');
    $ProjectGroupList = array();
    $GroupID = dbInsertRow('TestGroup', "'{$_LANG["AllUserGroupName"]}','','','',now(),'',now()"
        ,'GroupName,GroupManagers,GroupUser,AddedBy,AddDate,LastEditedBy,LastDate');
    $OldGroupIDNewGroupIDList = array();
    foreach($UserGroupList as $GroupInfo)
    {
        $GroupID = dbInsertRow('TestGroup', "'{$GroupInfo['GroupName']}','','{$GroupInfo['GroupUser']}','',now(),'',now()"
            ,'GroupName,GroupManagers,GroupUser,AddedBy,AddDate,LastEditedBy,LastDate');
        $OldGroupIDNewGroupIDList[$GroupInfo['GroupID']] = $GroupID;
    }
    foreach($UserGroupList as $GroupInfo)
    {
        $GroupACLList = unserialize($GroupInfo['GroupACL']);
        foreach($GroupACLList as $ProjectID => $ACL)
        {
            $ProjectGroupList[$ProjectID][$GroupInfo['GroupID']] = $OldGroupIDNewGroupIDList[$GroupInfo['GroupID']];
        }
    
    }
    
    foreach($ProjectGroupList as $ProjectID => $ProjectGroupIDList)
    {
        $GroupIDs = join(',', $ProjectGroupIDList);
        if($GroupIDs != '')
        {
            $GroupIDs = ',' . $GroupIDs . ',';
        }
        dbUpdateRow('TestProject','ProjectGroupIDs',"'{$GroupIDs}'", "ProjectID={$ProjectID}");
    }

    dbAddColumn("{$TablePrefix}TestProject", 'AddedBy', "varchar(30) default NULL AFTER DisplayOrder");
    dbAddColumn("{$TablePrefix}TestUser", 'AddedBy', "varchar(30) default NULL AFTER Email");
    sysUpdateOptions('dbVersion', '5');
}

function upgrade_5()
{
    global $TablePrefix;
    
    dbAddColumn("{$TablePrefix}CaseInfo", 'DisplayOrder', "tinyint(3) unsigned NOT NULL default 0");
    
    sysUpdateOptions('dbVersion', '6');
    
}

function upgrade_6()
{
    global $TablePrefix;
    
    dbAddColumn("{$TablePrefix}TestUserQuery", 'AndOrList', "text");
    dbAddColumn("{$TablePrefix}TestUserQuery", 'FieldList', "text");
    dbAddColumn("{$TablePrefix}TestUserQuery", 'OperatorList', "text");
    dbAddColumn("{$TablePrefix}TestUserQuery", 'ValueList', "text");

    sysUpdateOptions('dbVersion', '7');

}

function upgrade_7()
{
    global $TablePrefix;

    dbAddColumn("{$TablePrefix}TestProject", 'NotifyEmail', "text");
  

    sysUpdateOptions('dbVersion', '8');

}

function upgrade_8()
{
    global $TablePrefix, $MyDB;
    $MyDB->Query("ALTER TABLE `{$TablePrefix}TestUser` CHANGE `UserName` `UserName` VARCHAR(30) NOT NULL DEFAULT ''");
    sysUpdateOptions('dbVersion', '9');
}

function upgrade_9()
{
    global $TablePrefix;
    dbAddColumn("{$TablePrefix}TestUser", 'AuthMode', "VARCHAR(30) NOT NULL DEFAULT 'DB'");
    dbUpdateRow('TestUser', 'AuthMode', "'DB'");
    sysUpdateOptions('dbVersion', '10');
}

function upgrade_10()
{
    global $TablePrefix, $MyDB;
    $MyDB->Query("ALTER TABLE `{$TablePrefix}BugInfo` CHANGE `OpenedBuild` `OpenedBuild` VARCHAR(255) NOT NULL DEFAULT ''");
    $MyDB->Query("ALTER TABLE `{$TablePrefix}BugInfo` CHANGE `ResolvedBuild` `ResolvedBuild` VARCHAR(255) NOT NULL DEFAULT ''");
    sysUpdateOptions('dbVersion', '11');
}

function upgrade_11()
{
    global $TablePrefix, $MyDB;
    dbAddColumn("{$TablePrefix}TestUserQuery", 'FieldsToShow', "text");
    sysUpdateOptions('dbVersion', '12');
}

function upgrade_12()
{
    global $TablePrefix, $MyDB;
    dbAddColumn("{$TablePrefix}TestUser", 'Wangwang', "VARCHAR(20) NOT NULL DEFAULT '' AFTER `Email`");
    dbAddColumn("{$TablePrefix}TestUser", 'NoticeFlag', "TINYINT NOT NULL DEFAULT 2 AFTER `Wangwang`");
    sysUpdateOptions('dbVersion', '13');
}

function dbAddColumn($TableName, $FieldName, $FieldDesc)
{
    global $MyDB;
    $FieldExists = false;
    $FieldInfo = dbGetFieldInfo($TableName, $FieldName);
    if($FieldInfo['Field']) $FieldExists = true;
    if(!$FieldExists)
    {
        $MyDB->query("ALTER TABLE `{$TableName}` ADD {$FieldName} {$FieldDesc};");
    }
}

function installNewBugFree()
{
    global $TablePrefix, $ErrorMsg, $BugFreeInstallSQL, $_LANG;

    sysExecuteSql($BugFreeInstallSQL, $TablePrefix);

    if(!empty($ErrorMsg)) return;

    $Password = baseEncryptUserPWD('123456');
    $UserID = dbInsertRow('TestUser', "'admin','admin','{$Password}', '', now(), 'admin', now(), '0'"
        , "UserName, RealName, UserPassword, Email, AddDate, LastEditedBy, LastDate, IsDroped");

    $ProjectID = dbInsertRow('TestProject', "'Sample','0', '','', now(), 'admin', now()"
        , "ProjectName, DisplayOrder, ProjectDoc, ProjectPlan, AddDate, LastEditedBy, LastDate");

    $GroupACL[$ProjectID] = 'All';
    $GroupACL = serialize($GroupACL);
    $GroupID = dbInsertRow('TestUserGroup', "'Sample',',admin,','{$GroupACL}', now(), 'admin', now()"
        , "GroupName, GroupUser, GroupACL, AddDate, LastEditedBy, LastDate");

    $ValueSql .= "'{$ProjectID}','Sample','0','/',";
    $ValueSql .= "'{$_LANG["SampleBugInfo"]["BugTitle"]}','4',4,'Others','{$BugOS}',";
    $ValueSql .= "'{$BugBrowser}','{$BugMachine}','Other','{$_LANG["SampleBugInfo"]["ReproSteps"]}','Active',";
    $ValueSql .= "'{$LinkID}','{$DuplicateID}',NULL,";
    $ValueSql .= "'{$MailTo}','admin',now(),'N/A','admin',now(),'admin',now(),',admin,',";
    $ValueSql .= "'{$BugKeyWord}'";
    $BugID = dbInsertRow('BugInfo',$ValueSql,"ProjectID,ProjectName,ModuleID,ModulePath,BugTitle,BugSeverity,BugPriority,BugType,BugOS,BugBrowser,BugMachine,HowFound,ReproSteps,BugStatus,LinkID,DuplicateID,ResultID,MailTo,OpenedBy,OpenedDate,OpenedBuild,AssignedTo,AssignedDate,LastEditedBy,LastEditedDate,ModifiedBy,BugKeyword");

    $GroupID = dbInsertRow('TestGroup', "'{$_LANG["AllUserGroupName"]}','','','',now(),'',now()"
        ,'GroupName,GroupManagers,GroupUser,AddedBy,AddDate,LastEditedBy,LastDate');
    sysUpdateOptions('dbVersion', BF_DB_VERSION);
}

function sysUpdateOptions($OptionName, $OptionValue)
{
    $OptionInfo = dbGetRow('TestOptions', '', "OptionName = '{$OptionName}'");
    if(empty($OptionInfo))
    {
        dbInsertRow('TestOptions', "'{$OptionName}', '{$OptionValue}'", "OptionName,OptionValue");
    }
    else
    {
        dbUpdateRow('TestOptions', 'OptionValue', "'{$OptionValue}'", "OptionName = '{$OptionName}'");
    }
}

function sysExecuteSql($SQL, $TablePrefix = '')
{
    global $MyDB;
    global $ErrorMsg;

    // Read the table structure definition sql.
    $sql = addslashes($SQL);
    $sql = trim($sql);
    $sql = preg_replace("/#[^\n]*\n/", "", $sql);
    $sql = preg_replace("/--[^\n]*\n/", "", $sql);
    $sql = preg_replace("/CREATE TABLE `([a-z]{1,})`/i", "CREATE TABLE `{$TablePrefix}\\1`", $sql);
    $sql = preg_replace("/ALTER TABLE `([a-z]{1,})`/i", "ALTER TABLE `{$TablePrefix}\\1`", $sql);
    $sql = preg_replace("/UPDATE `([a-z]{1,})`/i", "UPDATE `{$TablePrefix}\\1`", $sql);
    $buffer = array();
    $ret    = array();
    $in_string = false;
    for($i=0; $i<strlen($sql)-1; $i++)
    {
        if($sql[$i] == ";" && !$in_string)
        {
            $ret[] = substr($sql, 0, $i);
            $sql = substr($sql, $i + 1);
            $i = 0;
        }

        if($in_string && ($sql[$i] == $in_string) && $buffer[0] != "\\")
        {
            $in_string = false;
        }
        elseif(!$in_string && ($sql[$i] == "\"" || $sql[$i] == "'") && (!isset($buffer[0]) || $buffer[0] != "\\"))
        {
            $in_string = $sql[$i];
        }
        if(isset($buffer[1]))
        {
            $buffer[0] = $buffer[1];
        }
        $buffer[1] = $sql[$i];
    }
    if(!empty($sql))
    {
        $ret[] = $sql;
    }

    // Excute the sql.
    $DBErrorMsg = array();
    for ($i=0; $i<count($ret); $i++)
    {
        $ret[$i] = stripslashes(trim($ret[$i]));
        if(!empty($ret[$i]) && $ret[$i] != "#")
        {
            $MyDB->Execute($ret[$i]) or $DBErrorMsg[] = $MyDB->ErrorMsg();
            if(count($DBErrorMsg)>0)
            {
                $ErrorMsg = $DBErrorMsg;
                return;
            }
        }
    }
}
?>
