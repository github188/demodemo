<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * db sql.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
$CharSetCollate = '';
if(version_compare(@mysql_get_server_info(), '4.1.0', '>=') && $_CFG['DBCharset'] != '')
{
    $CharSetCollate = "DEFAULT CHARSET=" . $_CFG['DBCharset'];
}

$BugFreeInstallSQL = <<<EOT
CREATE TABLE `BugInfo` (
  `BugID` mediumint(7) unsigned NOT NULL auto_increment,
  `ProjectID` smallint(5) unsigned NOT NULL default '0',
  `ProjectName` varchar(100) NOT NULL default '',
  `ModuleID` int(10) unsigned NOT NULL default '0',
  `ModulePath` varchar(240) NOT NULL default '',
  `BugTitle` varchar(150) default NULL,
  `BugSeverity` tinyint(4) NOT NULL default '0',
  `BugPriority` tinyint(4) default NULL,
  `BugType` varchar(20) NOT NULL default '',
  `BugOS` varchar(100) default NULL,
  `BugBrowser` varchar(100) default NULL,
  `BugMachine` varchar(255) default NULL,
  `HowFound` varchar(50) default NULL,
  `ReproSteps` text,
  `BugStatus` varchar(20) NOT NULL default '',
  `BugSubStatus` varchar(50) NOT NULL default '',
  `LinkID` varchar(255) default NULL,
  `CaseID` varchar(255) NOT NULL default '',
  `DuplicateID` varchar(255) default NULL,
  `MailTo` varchar(255) NOT NULL default '',
  `OpenedBy` varchar(30) NOT NULL default '',
  `OpenedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `OpenedBuild` varchar(255) NOT NULL default '',
  `AssignedTo` varchar(30) NOT NULL default '',
  `AssignedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ResolvedBy` varchar(30) NOT NULL default '',
  `Resolution` varchar(20) NOT NULL default '',
  `ResolvedBuild` varchar(255) NOT NULL default '',
  `ResolvedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ClosedBy` varchar(30) NOT NULL default '',
  `ClosedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy` varchar(30) NOT NULL default '',
  `LastEditedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ModifiedBy` text,
  `BugKeyword` varchar(255) default NULL,
  `ResultID` mediumint(8) unsigned default NULL,
  `CustomField1` varchar(255) default NULL,
  `CustomField2` varchar(255) default NULL,
  `CustomField3` varchar(255) default NULL,
  `IsDroped` enum('1','0') NOT NULL default '0',
  PRIMARY KEY  (`BugID`),
  KEY `ModuleID` (`ModuleID`),
  KEY `BugTitle` (`BugTitle`),
  KEY `BugSeverity` (`BugSeverity`),
  KEY `BugType` (`BugType`),
  KEY `BugStatus` (`BugStatus`),
  KEY `OpenedBy` (`OpenedBy`),
  KEY `AssignedTo` (`AssignedTo`),
  KEY `ResolvedBy` (`ResolvedBy`),
  KEY `Resolution` (`Resolution`),
  KEY `ClosedBy` (`ClosedBy`),
  KEY `LastEditedBy` (`LastEditedBy`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `CaseInfo` (
  `CaseID` mediumint(8) unsigned NOT NULL auto_increment,
  `ProjectID` smallint(5) unsigned NOT NULL default '0',
  `ProjectName` varchar(100) NOT NULL default '',
  `ModuleID` smallint(5) unsigned NOT NULL default '0',
  `ModulePath` varchar(255) NOT NULL default '',
  `CaseTitle` varchar(150) NOT NULL default '',
  `CasePriority` tinyint(3) unsigned default NULL,
  `CaseType` varchar(20) NOT NULL default '',
  `CaseStatus` varchar(20) NOT NULL default '',
  `CaseMethod` varchar(10) NOT NULL default '',
  `CasePlan` varchar(10) NOT NULL default '',
  `CaseSteps` text,
  `OpenedBy` varchar(30) NOT NULL default '',
  `OpenedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `AssignedTo` varchar(30) NOT NULL default '',
  `AssignedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy` varchar(30) NOT NULL default '',
  `LastEditedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ModifiedBy` text,
  `ScriptedBy` varchar(30) NOT NULL default '',
  `ScriptedDate` varchar(10) NOT NULL default '',
  `ScriptStatus` varchar(20) NOT NULL default '',
  `ScriptLocation` varchar(255) NOT NULL default '',
  `MailTo` varchar(255) default NULL,
  `MarkForDeletion` enum('0','1') NOT NULL default '0',
  `LinkID` varchar(240) NOT NULL default '',
  `BugID` varchar(255) NOT NULL default '',
  `ResultID` text,
  `CaseKeyword` varchar(255) NOT NULL default '',
  `CustomField1` varchar(255) NOT NULL default '',
  `CustomField2` varchar(255) NOT NULL default '',
  `CustomField3` varchar(255) NOT NULL default '',
  `IsDroped` enum('1','0') NOT NULL default '0',
  PRIMARY KEY  (`CaseID`),
  KEY `ProjectID` (`ProjectID`),
  KEY `ProjectName` (`ProjectName`),
  KEY `ModuleID` (`ModuleID`),
  KEY `ModulePath` (`ModulePath`),
  KEY `CaseTitle` (`CaseTitle`),
  KEY `CasePriority` (`CasePriority`),
  KEY `CaseType` (`CaseType`),
  KEY `OpenedBy` (`OpenedBy`),
  KEY `AssignedTo` (`AssignedTo`),
  KEY `CaseKeyword` (`CaseKeyword`),
  KEY `IsDroped` (`IsDroped`)
) TYPE=MyISAM $CharSetCollate;

ALTER TABLE `CaseInfo` ADD `DisplayOrder` tinyint(3) unsigned NOT NULL default 0;

CREATE TABLE `ResultInfo` (
  `ResultID` mediumint(8) unsigned NOT NULL auto_increment,
  `ProjectID` smallint(5) unsigned NOT NULL default '0',
  `ProjectName` varchar(100) NOT NULL default '',
  `ModuleID` smallint(5) unsigned NOT NULL default '0',
  `ModulePath` varchar(255) NOT NULL default '',
  `CaseID` mediumint(8) unsigned NOT NULL default '0',
  `ResultTitle` varchar(150) NOT NULL default '',
  `ResultValue` enum('Pass','Fail') NOT NULL default 'Fail',
  `ResultStatus` varchar(30) NOT NULL default '',
  `MailTo` varchar(255) NOT NULL default '',
  `OpenedBy` varchar(30) NOT NULL default '',
  `OpenedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ResultSteps` text,
  `AssignedTo` varchar(30) NOT NULL default '',
  `AssignedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy` varchar(30) NOT NULL default '',
  `LastEditedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ModifiedBy` text,
  `ResultOS` varchar(100) NOT NULL default '',
  `ResultBrowser` varchar(100) NOT NULL default '',
  `ResultMachine` varchar(255) NOT NULL default '',
  `ResultBuild` varchar(255) NOT NULL default '',
  `ResultKeyword` varchar(255) NOT NULL default '',
  `BugID` varchar(255) NOT NULL default '',
  `CustomField1` varchar(255) NOT NULL default '',
  `CustomField2` varchar(255) NOT NULL default '',
  `CustomField3` varchar(255) NOT NULL default '',
  `IsDroped` enum('1','0') NOT NULL default '0',
  PRIMARY KEY  (`ResultID`),
  KEY `ProjectID` (`ProjectID`),
  KEY `ProjectName` (`ProjectName`),
  KEY `ModuleID` (`ModuleID`),
  KEY `ModulePath` (`ModulePath`),
  KEY `CaseID` (`CaseID`),
  KEY `ResultValue` (`ResultValue`),
  KEY `OpenedBy` (`OpenedBy`),
  KEY `AssignedTo` (`AssignedTo`),
  KEY `IsDroped` (`IsDroped`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestAction` (
  `ActionID` mediumint(8) unsigned NOT NULL auto_increment,
  `ActionTarget` enum('Bug','Case','Result') NOT NULL default 'Bug',
  `IdValue` mediumint(8) unsigned NOT NULL default '0',
  `ActionUser` varchar(30) NOT NULL default '',
  `ActionType` varchar(30) NOT NULL default '',
  `ActionDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ActionNote` text,
  PRIMARY KEY  (`ActionID`),
  KEY `Action` (`ActionTarget`,`IdValue`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestFile` (
  `FileID` int(10) unsigned NOT NULL auto_increment,
  `ActionID` mediumint(8) unsigned NOT NULL default '0',
  `FileTitle` varchar(100) NOT NULL default '',
  `FileName` varchar(50) NOT NULL default '',
  `FileType` varchar(10) NOT NULL default '',
  `FileSize` varchar(20) NOT NULL default '',
  `IsDroped` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`FileID`),
  KEY `ActionID` (`ActionID`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestHistory` (
  `HistoryID` mediumint(8) unsigned NOT NULL auto_increment,
  `ActionID` mediumint(8) unsigned NOT NULL default '0',
  `ActionField` varchar(30) NOT NULL default '',
  `OldValue` text,
  `NewValue` text,
  PRIMARY KEY  (`HistoryID`),
  KEY `ActionID` (`ActionID`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestModule` (
  `ModuleID` int(10) unsigned NOT NULL auto_increment,
  `ModuleType` enum('Bug','Case') default 'Bug',
  `ProjectID` smallint(10) unsigned NOT NULL default '0',
  `ModuleName` varchar(100) NOT NULL default '',
  `ModuleGrade` tinyint(4) NOT NULL default '0',
  `ParentID` int(10) unsigned NOT NULL default '0',
  `ModuleOwner` varchar(30) default NULL,
  `DisplayOrder` tinyint(3) unsigned NOT NULL default '0',
  `AddDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `IsDroped` enum('1','0') NOT NULL default '0',
  PRIMARY KEY  (`ModuleID`),
  KEY `ProjectID` (`ProjectID`),
  KEY `ModuleName` (`ModuleName`),
  KEY `ModuleGrade` (`ModuleGrade`),
  KEY `ParentID` (`ParentID`),
  KEY `DisplayOrder` (`DisplayOrder`),
  KEY `IsDroped` (`IsDroped`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestProject` (
  `ProjectID` smallint(5) unsigned NOT NULL auto_increment,
  `ProjectName` varchar(100) NOT NULL default '',
  `ProjectManagers` TEXT,
  `ProjectGroupIDs` TEXT,
  `ProjectDoc` varchar(255) NOT NULL default '',
  `ProjectPlan` varchar(255) NOT NULL default '',
  `DisplayOrder` tinyint(3) unsigned NOT NULL default '0',
  `AddedBy`  varchar(30) NOT NULL default '',
  `AddDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy`  varchar(30) NOT NULL default '',
  `LastDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `IsDroped` enum('1','0') NOT NULL default '0',
   NotifyEmail TEXT,

  PRIMARY KEY  (`ProjectID`),
  KEY `ProjectName` (`ProjectName`),
  KEY `DisplayOrder` (`DisplayOrder`),
  KEY `IsDroped` (`IsDroped`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestUser` (
  `UserID` smallint(4) NOT NULL auto_increment,
  `UserName` varchar(30) NOT NULL default '',
  `UserPassword` varchar(40) NOT NULL default '',
  `RealName` varchar(20) NOT NULL default '',
  `Email` varchar(50) NOT NULL default '',
  `Wangwang` VARCHAR(20) NOT NULL default '',
  `NoticeFlag` TINYINT NOT NULL DEFAULT 2,
  `AddedBy`  varchar(30) NOT NULL default '',
  `AddDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy`  varchar(30) NOT NULL default '',
  `LastDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `IsDroped` enum('1','0') NOT NULL default '0',
  `AuthMode` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`UserID`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestUserLog` (
  `LogID` mediumint(8) unsigned NOT NULL auto_increment,
  `UserName` varchar(30) NOT NULL default '',
  `LoginIP` varchar(15) NOT NULL default '',
  `LoginTime` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`LogID`),
  KEY `UserName` (`UserName`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestUserQuery` (
  `QueryID` int(10) unsigned NOT NULL auto_increment,
  `UserName` varchar(30) NOT NULL default '',
  `QueryType` enum('Bug','Case','Result') default 'Bug',
  `QueryTitle` varchar(100) NOT NULL default '',
  `QueryString` text,
   `AndOrList`  text,
   `OperatorList` text,
   `ValueList` text,
   `FieldList` text,
   `FieldsToShow` text,
  `AddDate` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`QueryID`),
  KEY `UserName` (`UserName`),
  KEY `QueryType` (`QueryType`)
) TYPE=MyISAM $CharSetCollate;
CREATE TABLE `TestOptions` (
  `OptionId` bigint(20) NOT NULL auto_increment,
  `OptionName` varchar(64) NOT NULL default '',
  `OptionValue` longtext NOT NULL,
  PRIMARY KEY  (`OptionID`,`OptionName`),
  KEY `OptionName` (`OptionName`)
) Type=MyISAM $CharSetCollate;
CREATE TABLE `TestGroup` ( 
  `GroupID` smallint(5) unsigned NOT NULL auto_increment,
  `GroupName` varchar(60) NOT NULL default '', 
  `GroupManagers` TEXT, 
  `GroupUser` longtext, 
  `AddedBy` varchar(30) default NULL, 
  `AddDate` datetime NOT NULL default '0000-00-00 00:00:00', 
  `LastEditedBy` varchar(30) default NULL, 
  `LastDate` datetime NOT NULL default '0000-00-00 00:00:00', 
  PRIMARY KEY  (`GroupID`)
) Type=MyISAM $CharSetCollate;
EOT;

$BugFreeUpgrade1Sql = <<<EOT
#BugInfo
CREATE TABLE `BugInfo` SELECT * FROM `BugInfo`;

ALTER TABLE `BugInfo` ADD PRIMARY KEY ( `BugID` );
ALTER TABLE `BugInfo`
  CHANGE `BugTitle` `BugTitle` VARCHAR( 150 ),
  CHANGE `BugOS`    `BugOS`    VARCHAR( 100 ),
  CHANGE `LinkID`   `LinkID`   VARCHAR( 255 ),
  CHANGE `BugID` `BugID` MEDIUMINT( 7 ) UNSIGNED NOT NULL AUTO_INCREMENT,
  ADD `BugPriority` TINYINT NULL AFTER `BugSeverity`,
  ADD `BugBrowser` VARCHAR(100) AFTER BugOS,
  ADD `BugMachine` VARCHAR(255) AFTER BugBrowser,
  ADD `HowFound`   VARCHAR(50)  AFTER BugMachine,
  ADD `ReproSteps` Text AFTER HowFound,
  ADD `DuplicateID` VARCHAR(255) AFTER LinkID,
  ADD `BugKeyword` VARCHAR(255),
  ADD `ResultID`   MEDIUMINT UNSIGNED,
  ADD `CustomField1` VARCHAR(255),
  ADD `CustomField2` VARCHAR(255),
  ADD `CustomField3` VARCHAR(255),
  ADD `IsDroped` ENUM( '1', '0' ) DEFAULT '0' NOT NULL ;

ALTER TABLE `BugInfo` ADD INDEX (`ModuleID`);
ALTER TABLE `BugInfo` ADD INDEX (`BugTitle`);
ALTER TABLE `BugInfo` ADD INDEX (`BugSeverity`);
ALTER TABLE `BugInfo` ADD INDEX (`BugType`);
ALTER TABLE `BugInfo` ADD INDEX (`BugStatus`);
ALTER TABLE `BugInfo` ADD INDEX (`OpenedBy`);
ALTER TABLE `BugInfo` ADD INDEX (`AssignedTo`);
ALTER TABLE `BugInfo` ADD INDEX (`ResolvedBy`);
ALTER TABLE `BugInfo` ADD INDEX (`Resolution`);
ALTER TABLE `BugInfo` ADD INDEX (`ClosedBy`);
ALTER TABLE `BugInfo` ADD INDEX (`LastEditedBy`);


# BugProject to TestProject
CREATE TABLE `TestProject` SELECT * FROM `BugProject`;

ALTER TABLE `TestProject` ADD PRIMARY KEY ( `ProjectID` );
ALTER TABLE `TestProject` CHANGE `ProjectID` `ProjectID` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT;
ALTER TABLE `TestProject` ADD `DisplayOrder` TINYINT UNSIGNED NOT NULL DEFAULT '0' AFTER ProjectPlan,
  ADD `LastDate` DATETIME NOT NULL  DEFAULT '0000-00-00 00:00:00',
  ADD `IsDroped` ENUM( '1', '0' ) DEFAULT '0' NOT NULL ;

ALTER TABLE `TestProject` ADD INDEX (`ProjectName`);
ALTER TABLE `TestProject` ADD INDEX ( `DisplayOrder` ) ;
ALTER TABLE `TestProject` ADD INDEX ( `IsDroped` );

#BugModule to TestModule
CREATE TABLE `TestModule` SELECT * FROM `BugModule`;

ALTER TABLE `TestModule`
  ADD `ModuleType` ENUM('Bug', 'Case') DEFAULT 'Bug' AFTER ModuleID,
  ADD `ModuleOwner` VARCHAR(30) AFTER ParentID,
  ADD `DisplayOrder` TINYINT UNSIGNED NOT NULL DEFAULT '0' AFTER ModuleOwner,
  ADD `LastDate` DATETIME NOT NULL DEFAULT '0000-00-00 00:00:00',
  ADD `IsDroped` ENUM( '1', '0' ) DEFAULT '0' NOT NULL ;

ALTER TABLE `TestModule` ADD PRIMARY KEY ( `ModuleID` );
ALTER TABLE `TestModule` CHANGE `ModuleID` `ModuleID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT;
ALTER TABLE `TestModule` ADD INDEX (`ProjectID`);
ALTER TABLE `TestModule` ADD INDEX (`ModuleName`);
ALTER TABLE `TestModule` ADD INDEX (`ModuleGrade`);
ALTER TABLE `TestModule` ADD INDEX (`ParentID`);
ALTER TABLE `TestModule` ADD INDEX ( `DisplayOrder` ) ;
ALTER TABLE `TestModule` ADD INDEX ( `IsDroped` );


# Change BugUser to TestUser
CREATE TABLE `TestUser` SELECT * FROM `BugUser`;

ALTER TABLE `TestUser` ADD `IsDroped` ENUM( '1', '0' ) DEFAULT '0' NOT NULL ;

ALTER TABLE `TestUser` ADD PRIMARY KEY ( `UserID` );
ALTER TABLE `TestUser` CHANGE `UserID` `UserID` SMALLINT(4) UNSIGNED NOT NULL AUTO_INCREMENT;


# Change BugGroup to TestUserGroup
CREATE TABLE `TestUserGroup` SELECT * FROM `BugGroup`;

ALTER TABLE `TestUserGroup` ADD `AddDate` DATETIME NOT NULL DEFAULT '0000-00-00 00:00:00' AFTER GroupACL;

ALTER TABLE `TestUserGroup` ADD PRIMARY KEY ( `GroupID` );
ALTER TABLE `TestUserGroup` CHANGE `GroupID` `GroupID` SMALLINT(5) UNSIGNED NOT NULL AUTO_INCREMENT;


# Change BugQuery to TestUserQuery
CREATE TABLE `TestUserQuery` SELECT * FROM `BugQuery`;

ALTER TABLE `TestUserQuery` ADD `QueryType` ENUM('Bug','Case','Result') DEFAULT 'Bug' AFTER UserName;

ALTER TABLE `TestUserQuery` ADD PRIMARY KEY ( `QueryID` );
ALTER TABLE `TestUserQuery` CHANGE `QueryID` `QueryID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT;
ALTER TABLE `TestUserQuery` ADD INDEX ( `UserName` );
ALTER TABLE `TestUserQuery` ADD INDEX ( `QueryType` );
UPDATE `TestUserQuery` SET QueryString=INSERT(QueryString,1,6,'');

CREATE TABLE `CaseInfo` (
  `CaseID` mediumint(8) unsigned NOT NULL auto_increment,
  `ProjectID` smallint(5) unsigned NOT NULL default '0',
  `ProjectName` varchar(100) NOT NULL default '',
  `ModuleID` smallint(5) unsigned NOT NULL default '0',
  `ModulePath` varchar(255) NOT NULL default '',
  `CaseTitle` varchar(150) NOT NULL default '',
  `CasePriority` tinyint(3) unsigned default NULL,
  `CaseType` varchar(20) NOT NULL default '',
  `CaseStatus` varchar(20) NOT NULL default '',
  `CaseMethod` varchar(10) NOT NULL default '',
  `CasePlan` varchar(10) NOT NULL default '',
  `CaseSteps` text NOT NULL default '',
  `OpenedBy` varchar(30) NOT NULL default '',
  `OpenedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `AssignedTo` varchar(30) NOT NULL default '',
  `AssignedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy` varchar(30) NOT NULL default '',
  `LastEditedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ScriptedBy` varchar(30) NOT NULL default '',
  `ScriptedDate` varchar(10) NOT NULL default '',
  `ScriptStatus` varchar(20) NOT NULL default '',
  `MailTo` varchar(255) default NULL,
  `MarkForDeletion` enum('0','1') NOT NULL default '0',
  `LinkID` varchar(240) NOT NULL default '',
  `ResultID` text NOT NULL default '',
  `CaseKeyword` varchar(255) NOT NULL default '',
  `CustomField1` varchar(255) NOT NULL default '',
  `CustomField2` varchar(255) NOT NULL default '',
  `CustomField3` varchar(255) NOT NULL default '',
  `IsDroped` enum('1','0') NOT NULL default '0',
  PRIMARY KEY  (`CaseID`),
  KEY `ProjectID` (`ProjectID`),
  KEY `ProjectName` (`ProjectName`),
  KEY `ModuleID` (`ModuleID`),
  KEY `ModulePath` (`ModulePath`),
  KEY `CaseTitle` (`CaseTitle`),
  KEY `CasePriority` (`CasePriority`),
  KEY `CaseType` (`CaseType`),
  KEY `OpenedBy` (`OpenedBy`),
  KEY `AssignedTo` (`AssignedTo`),
  KEY `CaseKeyword` (`CaseKeyword`),
  KEY `IsDroped` (`IsDroped`)
) TYPE=MyISAM $CharSetCollate;

CREATE TABLE `ResultInfo` (
  `ResultID` mediumint(8) unsigned NOT NULL auto_increment,
  `ProjectID` smallint(5) unsigned NOT NULL default '0',
  `ProjectName` varchar(100) NOT NULL default '',
  `ModuleID` smallint(5) unsigned NOT NULL default '0',
  `ModulePath` varchar(255) NOT NULL default '',
  `CaseID` mediumint(8) unsigned NOT NULL default '0',
  `ResultTitle` varchar(150) NOT NULL default '',
  `ResultValue` enum('Pass','Fail') NOT NULL default 'Fail',
  `ResultStatus` varchar(30) NOT NULL default '',
  `MailTo` varchar(255) NOT NULL default '',
  `OpenedBy` varchar(30) NOT NULL default '',
  `OpenedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ResultSteps` text NOT NULL default '',
  `AssignedTo` varchar(30) NOT NULL default '',
  `AssignedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `LastEditedBy` varchar(30) NOT NULL default '',
  `LastEditedDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ResultOS` varchar(100) NOT NULL default '',
  `ResultBrowser` varchar(100) NOT NULL default '',
  `ResultMachine` varchar(255) NOT NULL default '',
  `ResultBuild` varchar(255) NOT NULL default '',
  `ResultKeyword` varchar(255) NOT NULL default '',
  `BugID` varchar(255) NOT NULL default '',
  `CustomField1` varchar(255) NOT NULL default '',
  `CustomField2` varchar(255) NOT NULL default '',
  `CustomField3` varchar(255) NOT NULL default '',
  `IsDroped` enum('1','0') NOT NULL default '0',
  PRIMARY KEY  (`ResultID`),
  KEY `ProjectID` (`ProjectID`),
  KEY `ProjectName` (`ProjectName`),
  KEY `ModuleID` (`ModuleID`),
  KEY `ModulePath` (`ModulePath`),
  KEY `CaseID` (`CaseID`),
  KEY `ResultValue` (`ResultValue`),
  KEY `OpenedBy` (`OpenedBy`),
  KEY `AssignedTo` (`AssignedTo`),
  KEY `IsDroped` (`IsDroped`)
) TYPE=MyISAM $CharSetCollate;

CREATE TABLE `TestAction` (
  `ActionID` mediumint(8) unsigned NOT NULL auto_increment,
  `ActionTarget` enum('Bug','Case','Result') NOT NULL default 'Bug',
  `IdValue` mediumint(8) unsigned NOT NULL default '0',
  `ActionUser` varchar(30) NOT NULL default '',
  `ActionType` varchar(30) NOT NULL default '',
  `ActionDate` datetime NOT NULL default '0000-00-00 00:00:00',
  `ActionNote` text NOT NULL default '',
  PRIMARY KEY  (`ActionID`),
  KEY `Action` (`ActionTarget`,`IdValue`)
) TYPE=MyISAM $CharSetCollate;

CREATE TABLE `TestFile` (
  `FileID` int(10) unsigned NOT NULL auto_increment,
  `ActionID` mediumint(8) unsigned NOT NULL default '0',
  `FileTitle` varchar(100) NOT NULL default '',
  `FileName` varchar(50) NOT NULL default '',
  `FileType` varchar(10) NOT NULL default '',
  `FileSize` varchar(20) NOT NULL default '',
  `IsDroped` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`FileID`),
  KEY `ActionID` (`ActionID`)
) TYPE=MyISAM $CharSetCollate;

CREATE TABLE `TestHistory` (
  `HistoryID` mediumint(8) unsigned NOT NULL auto_increment,
  `ActionID` mediumint(8) unsigned NOT NULL default '0',
  `ActionField` varchar(30) NOT NULL default '',
  `OldValue` text NOT NULL default '',
  `NewValue` text NOT NULL default '',
  PRIMARY KEY  (`HistoryID`),
  KEY `ActionID` (`ActionID`)
) TYPE=MyISAM $CharSetCollate;

CREATE TABLE `TestUserLog` (
  `LogID` mediumint(8) unsigned NOT NULL auto_increment,
  `UserName` varchar(30) NOT NULL default '',
  `LoginIP` varchar(15) NOT NULL default '',
  `LoginTime` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`LogID`),
  KEY `UserName` (`UserName`)
) TYPE=MyISAM $CharSetCollate;
EOT;

$BugFreeUpgrade2Sql = <<<EOT
CREATE TABLE `TestOptions` (
  `OptionId` bigint(20) NOT NULL auto_increment,
  `OptionName` varchar(64) NOT NULL default '',
  `OptionValue` longtext NOT NULL,
  PRIMARY KEY  (`OptionID`,`OptionName`),
  KEY `OptionName` (`OptionName`)
) Type=MyISAM $CharSetCollate;
EOT;

$BugFree2AddGroupSql = <<<EOT
CREATE TABLE `TestGroup` ( 
  `GroupID` smallint(5) unsigned NOT NULL auto_increment,
  `GroupName` varchar(60) NOT NULL default '', 
  `GroupManagers` TEXT, 
  `GroupUser` longtext, 
  `AddedBy` varchar(30) default NULL, 
  `AddDate` datetime NOT NULL default '0000-00-00 00:00:00', 
  `LastEditedBy` varchar(30) default NULL, 
  `LastDate` datetime NOT NULL default '0000-00-00 00:00:00', 
  PRIMARY KEY  (`GroupID`)
) Type=MyISAM $CharSetCollate;
EOT;
?>
