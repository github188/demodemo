<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * english language file.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
$_LANG['Charset'] = 'UTF-8';

/* The error message to display when a user not logged in. */
$_LANG['Message'] =  array
(
    'NotLogin'     => 'You have not logged in the system. \\nPlease log in BugFree first!',
    'ErrorLogin'   => 'Incorrect user name or password. Please input again.',
    'NoPriv'       => "You don't have permission to access any project, <br />Please contact administrators.",
    "SucceedLogin" => "Logged in successfully!"
);

$_LANG['SysErrorMsg'] = 'Required URL not found or permission denied.';

/* Settting for DB */
$_LANG['DBErrorConnection']    = 'Connecting the database failed!';
$_LANG['DBSureExists']         = 'Please confirm the database exists';
$_LANG['DBSureUserNameAndPwd'] = 'Please confirm the database user name and password are correct.';
$_LANG['DBSureHost']           = 'Please confirm the database name is correct.';
$_LANG['DBSureRunning']        = 'Please confirm the database is running.';

/* BugFree team info. Don't change. */
$_LANG['AboutBF']    = 'About';
$_LANG['BFHome']     = 'Official Site';
$_LANG['BFHomePage'] = 'http://www.bugfree.org.cn';
$_LANG['ProductName']= 'BugFree';
$_LANG['Version']    = '2.1.1';

/* Define css style list. */
$_LANG['StyleList'] = array
(
  'Default' => 'Default',
);

$_LANG['DefaultStyle'] = 'Default';

$_LANG['UCAssignedToMe'] = 'Assigned to me';
$_LANG['UCOpenedByMe']   = 'Opened by me';
$_LANG['UCMyQuery']      = 'My Queries';

/* Define the length of the title to show in result list window and user control window. */
$_LANG['QueryTitleLength']   = 75;    // Used in QueryBug.php
$_LANG['ControlTitleLength'] = 36;    // Used in UserControl.php

/* Define overlib's value */
$_LANG['OLWIDTH']   = '300';
$_LANG['OLBGCOLOR'] = '#75736E';
$_LANG['OLFGCOLOR'] = '#F6F6F6';

/* And Or list. */
$_LANG['AndOr'] = array('And' => 'And', 'Or'  => 'Or');

$_LANG['FieldTypeOperationName'] = array
(
    'Project'=> 'Equal to,Not Equal to,Contain,Not Contain',
    'Number' => 'Equal to,Not Equal to,Greater than,Less than,Greater than and Equal to,Less than and Equal to,Contain',
    'Date'   => 'Equal to,Not Equal to,Greater than,Less than,Greater than and Equal to,Less than and Equal to,',
    'String' => 'Contain,Not Contain',
    'People' => 'Equal to,Not Equal to,Contain',
    'MutilPeople' => 'Equal to',
    'Option' => 'Equal to,Not Equal to',
    'Path'   => 'Contain,Not Contain,Under'
);

$_LANG['QueryCondition'] = 'Query Builder';
$_LANG['QueryTitle']     = 'Query Title';
$_LANG['PostQuery']      = 'Post Query';
$_LANG['SaveQuery']      = 'Save Query';
$_LANG['ResetQuery']     = 'Reset Query';
$_LANG['NoQueryTitle']   = 'Not input Query Title';
$_LANG['DuplicateQueryTitle'] = 'Are you sure to update Query ';
$_LANG['AddBuild'] = 'Add Build';

$_LANG['ConfirmDeleteTestFile']  = 'Are you sure to delete the file?';
$_LANG['ErrorExceedSize']        = 'Max file size exceeded';
$_LANG['ErrorDangerousFileType'] = 'Unsafe file type';

/*-BASIC SETTING FOR BUG RELATED FEATURES -*/
/* The fields of BugInfo table. */
$_LANG['BugFields'] = array
(
    'BugID'          => 'Bug ID',
    'ProjectID'      => 'Project ID',
    'ProjectName'    => 'Project Name',
    'ModuleID'       => 'Module ID',
    'ModulePath'     => 'Module Path',
    'BugTitle'       => 'Bug Title',
    'BugSeverity'    => 'Severity',
    'BugPriority'    => 'Priority',
    'BugType'        => 'Bug Type',
    'BugOS'          => 'OS',
    'BugBrowser'     => 'Browser',
    'BugMachine'     => 'Machine',
    'HowFound'       => 'How Found',
    'ReproSteps'     => 'Repro Steps',
    'BugStatus'      => 'Bug Status',
    'BugSubStatus'   => 'SubStatus',
    'LinkID'         => 'Related Bug',
    'DuplicateID'    => 'Duplicated Bug',
    'MailTo'         => 'Mail To',
    'OpenedBy'       => 'Opened By',
    'OpenedDate'     => 'Open Date',
    'OpenedBuild'    => 'Open Build',
    'AssignedTo'     => 'Assigned To',
    'AssignedDate'   => 'Assign Date',
    'ResolvedBy'     => 'Resolved By',
    'Resolution'     => 'Resolution',
    'ResolvedBuild'  => 'Resolve Build',
    'ResolvedDate'   => 'Resolve Date',
    'ModifiedBy'     => 'Ever Changed by',
    'ClosedBy'       => 'Closed By',
    'ClosedDate'     => 'Close Date',
    'LastEditedBy'   => 'Last Edited by',
    'LastEditedDate' => 'Last Edit Date',
    'BugKeyword'     => 'Keywords',
    'CaseID'         => 'Related Cases',
    'ResultID'       => 'Related Results',
);

/* The fields used to query in QueryBug.php.(Note: the field will be displayed in the order you defined here). */
$_LANG['BugQueryField'] = array
(
   'BugID'          => $_LANG['BugFields']['BugID'],
   'BugTitle'       => $_LANG['BugFields']['BugTitle'],
   'ProjectName'    => $_LANG['BugFields']['ProjectName'],
   'ModulePath'     => $_LANG['BugFields']['ModulePath'],
   'BugStatus'      => $_LANG['BugFields']['BugStatus'],
   'AssignedTo'     => $_LANG['BugFields']['AssignedTo'],
   'MailTo'         => $_LANG['BugFields']['MailTo'],
   'BugSeverity'    => $_LANG['BugFields']['BugSeverity'],
   'BugPriority'    => $_LANG['BugFields']['BugPriority'],
   'BugType'        => $_LANG['BugFields']['BugType'],
   'HowFound'       => $_LANG['BugFields']['HowFound'],
   'BugOS'          => $_LANG['BugFields']['BugOS'],
   'BugBrowser'     => $_LANG['BugFields']['BugBrowser'],
   'LastEditedBy'   => $_LANG['BugFields']['LastEditedBy'],
   'LastEditedDate' => $_LANG['BugFields']['LastEditedDate'],
   'OpenedBy'       => $_LANG['BugFields']['OpenedBy'],
   'OpenedDate'     => $_LANG['BugFields']['OpenedDate'],
   'OpenedBuild'    => $_LANG['BugFields']['OpenedBuild'],
   'ResolvedBy'     => $_LANG['BugFields']['ResolvedBy'],
   'ResolvedDate'   => $_LANG['BugFields']['ResolvedDate'],
   'ResolvedBuild'  => $_LANG['BugFields']['ResolvedBuild'],
   'Resolution'     => $_LANG['BugFields']['Resolution'],
   'DuplicateID'    => $_LANG['BugFields']['DuplicateID'],
   'ClosedBy'       => $_LANG['BugFields']['ClosedBy'],
   'ClosedDate'     => $_LANG['BugFields']['ClosedDate'],
   'BugSubStatus'   => $_LANG['BugFields']['BugSubStatus'],
   'BugMachine'     => $_LANG['BugFields']['BugMachine'],
   'BugKeyword'     => $_LANG['BugFields']['BugKeyword'],
   'LinkID'         => $_LANG['BugFields']['LinkID'],
   'CaseID'         => $_LANG['BugFields']['CaseID'],
   'ResultID'       => $_LANG['BugFields']['ResultID'],
   'ModifiedBy'     => $_LANG['BugFields']['ModifiedBy'],
);

$_LANG["DefaultBugQueryFields"] = array
(
   "BugID"        => $_LANG["BugFields"]["BugID"],
   "BugSeverity"  => $_LANG["BugFields"]["BugSeverity"],
   "BugPriority"  => $_LANG["BugFields"]["BugPriority"],
   "BugTitle"     => $_LANG["BugFields"]["BugTitle"],
   "OpenedBy"     => $_LANG["BugFields"]["OpenedBy"],
   "AssignedTo"   => $_LANG["BugFields"]["AssignedTo"],
   "ResolvedBy"   => $_LANG["BugFields"]["ResolvedBy"],
   "Resolution"   => $_LANG["BugFields"]["Resolution"],
   "LastEditedDate"   => $_LANG["BugFields"]["LastEditedDate"]
);

/* Bug Severity. */
$_LANG['BugSeveritys'] = array
(
   '' => '',
   1 => '1',
   2 => '2',
   3 => '3',
   4 => '4',
);

/* Bug Priority. */
$_LANG['BugPriorities'] = array
(
   '' => '',
   1 => '1',
   2 => '2',
   3 => '3',
   4 => '4',
);

/* Define the OS list. */
$_LANG['BugOS'] = array
(
    ''        => '',
    'All'     => 'All',
    'Win7'    => 'Windows 7',
    'WinVista'=> 'Windows Vista',
    'WinXP'   => 'Windows XP',
    'Win2000' => 'Windows 2000',
    'Linux'   => 'Linux',
    'FreeBSD' => 'FreeBSD',
    'Unix'    => 'Unix',
    'MacOS'   => 'Mac OS',
    'Others'  => 'Others',
);

/* Define the Browser list. */
$_LANG['BugBrowser'] = array
(
 ''           => '',
 'All'        => 'All',
 'IE8'        => 'IE 8.0',
 'IE7'        => 'IE 7.0',
 'IE6'        => 'IE 6.0',
 'FireFox3.0' => 'FireFox 3.0',
 'FireFox2.0' => 'FireFox 2.0',
 'Chrome'     => 'Chrome',
 'Safari'     => 'Safari',
 'Opera'      => 'Opera',
 'Others'     => 'Others',
);

/* Define the types. */
$_LANG['BugTypes'] = array
(
  ''             => '',
  'CodeError'    => 'Code Defect',
  'Interface'    => 'User Interface',
  'DesignChange' => 'Design Change',
  'NewFeature'   => 'New Feature',
  'DesignDefect' => 'Design Defect',
  'Config'       => 'Configuration',
  'Install'      => 'Installation',
  'Security'     => 'Security',
  'Performance'  => 'Performance',
  'Standard'     => 'Standard',
  'Automation'   => 'Automation Scripts',
  'TrackThings'  => 'Issue Tracking',
  'BadCase'      => 'Bad Case',
  'Others'       => 'Others'
);

/* Define the status. */
$_LANG['BugStatus'] = array
(
    ''         => '',
    'Active'   => 'Active',
    'Resolved' => 'Resolved',
    'Closed'   => 'Closed'
);

/* Define the substatus. */
$_LANG['BugSubStatus'] = array
(
    ''              => '',
    'Hold'          => 'Hold',
    'LocalFix'      => 'Local Fix',
    'CheckedIn'     => 'Checked In',
    'CannotRegress' => 'Can\'t Regress'
);

/* Define the Resolution. */
$_LANG['BugResolutions'] = array
(
    ''             => '',
    'By Design'    => 'By Design',
    'Duplicate'    => 'Duplicate',
    'External'     => 'External',
    'Fixed'        => 'Fixed',
    'Not Repro'    => 'Not Repro',
    'Postponed'    => 'Postponed',
    'Will not Fix' => "Won't Fix"
);

/* Define the HowFound. */
$_LANG['BugHowFound'] = array
(
    ''             => '',
    'FuncTest'     => 'Functional Test',
    'UnitTest'     => 'Unit Test',
    'BVT'          => 'BVT',
    'Integrate'    => 'Integration Test',
    'System'       => 'System Test',
    'SmokeTest'    => 'Smoke Test',
    'Acceptance'   => 'Acceptance Test',
    'BugBash'      => 'Bug Bash',
    'AdHoc'        => 'Ad Hoc',
    'Regression'   => 'Regression Test',
    'SpecReview'   => 'Spec Review',
    'CodeReview'   => 'Code Review',
    'PostRTW'      => 'Post RTW',
    'Customer'     => 'Customer',
    'Partner'      => 'Partner',
    'Other'        => 'Others',
);

/*-BASIC SETTING FOR BUG RELATED FEATURES -*/
/* The fields of BugInfo table. */
$_LANG['CaseFields'] = array
(
  'CaseID'            => 'Case ID',
  'ProjectID'         => 'Project ID',
  'ProjectName'       => 'Project Name',
  'ModuleID'          => 'Module ID',
  'ModulePath'        => 'Module Path',
  'CaseTitle'         => 'Case Title',
  'CasePriority'      => 'Priority',
  'CaseType'          => 'Type',
  'CaseStatus'        => 'Case Status',
  'CaseMethod'        => 'Case Method',
  'CasePlan'          => 'Test Plan',
  'CaseSteps'         => 'Steps',
  'DisplayOrder'      => 'Display Order',
  'OpenedBy'          => 'Opened By',
  'OpenedDate'        => 'Open Date',
  'AssignedTo'        => 'Assigned To',
  'AssignedDate'      => 'Assign Date',
  'LastEditedBy'      => 'Last Edited By',
  'LastEditedDate'    => 'Last Edit Date',
  'ModifiedBy'        => 'Ever Changed By',
  'ScriptStatus'      => 'Script Status',
  'ScriptedBy'        => 'Scripted By',
  'ScriptedDate'      => 'Write Date',
  'ScriptLocation'    => 'Script Location',
  'MailTo'            => 'Mail To',
  'CaseKeyword'       => 'Keywords',
  'LinkID'            => 'Related Cases',
  'BugID'             => 'Related Bugs',
  'ResultID'          => 'Related Results',
  'DisplayOrder'      => 'Display Order',
  'MarkForDeletion'   => 'Mark For Deletion',
);

/* The fields used to query in QueryCase.php.(Note: the field will be displayed in the order you defined here). */
$_LANG['CaseQueryField'] = array
(
   'CaseID'          => $_LANG['CaseFields']['CaseID'],
   'CaseTitle'       => $_LANG['CaseFields']['CaseTitle'],
   'ProjectName'     => $_LANG['CaseFields']['ProjectName'],
   'ModulePath'      => $_LANG['CaseFields']['ModulePath'],
   'CaseStatus'      => $_LANG['CaseFields']['CaseStatus'],
   'AssignedTo'      => $_LANG['CaseFields']['AssignedTo'],
   'MailTo'          => $_LANG['CaseFields']['MailTo'],
   'CasePriority'    => $_LANG['CaseFields']['CasePriority'],
   'CaseType'        => $_LANG['CaseFields']['CaseType'],
   'CaseMethod'      => $_LANG['CaseFields']['CaseMethod'],
   'CasePlan'        => $_LANG['CaseFields']['CasePlan'],
   'LastEditedBy'    => $_LANG['CaseFields']['LastEditedBy'],
   'LastEditedDate'  => $_LANG['CaseFields']['LastEditedDate'],
   'OpenedBy'        => $_LANG['CaseFields']['OpenedBy'],
   'OpenedDate'      => $_LANG['CaseFields']['OpenedDate'],
   'ScriptStatus'    => $_LANG['CaseFields']['ScriptStatus'],
   'ScriptedBy'      => $_LANG['CaseFields']['ScriptedBy'],
   'ScriptedDate'    => $_LANG['CaseFields']['ScriptedDate'],
   'ScriptLocation'  => $_LANG['CaseFields']['ScriptLocation'],
   'MarkForDeletion' => $_LANG['CaseFields']['MarkForDeletion'],
   'CaseKeyword'     => $_LANG['CaseFields']['CaseKeyword'],
   'BugID'           => $_LANG['CaseFields']['BugID'],
   'LinkID'          => $_LANG['CaseFields']['LinkID'],
   'ResultID'        => $_LANG['CaseFields']['ResultID'],
   'DisplayOrder'    => $_LANG['CaseFields']['DisplayOrder'],
   'ModifiedBy'      => $_LANG['CaseFields']['ModifiedBy'],
);


$_LANG['DefaultCaseQueryFields'] = array
(
   'CaseID'          => $_LANG['CaseFields']['CaseID'],
   'CasePriority'    => $_LANG['CaseFields']['CasePriority'],
   'CaseTitle'       => $_LANG['CaseFields']['CaseTitle'],
   'CaseStatus'      => $_LANG['CaseFields']['CaseStatus'],
   'OpenedBy'        => $_LANG['CaseFields']['OpenedBy'],
   'AssignedTo'      => $_LANG['CaseFields']['AssignedTo'],
   'LastEditedDate'  => $_LANG['CaseFields']['LastEditedDate'],
   'DisplayOrder'    => $_LANG['CaseFields']['DisplayOrder'],
);


/* Define the CasePriority */
$_LANG['CaseStatuses'] = array
(
 'Active'      => 'Active',
 'Blocked'     => 'Blocked',
 'Investigate' => 'Investigating',
 'Reviewed'    => 'Reviewed',
);

$_LANG['CasePriorities'] = array
(
 ''=>'',
 '1'=>'1',
 '2'=>'2',
 '3'=>'3',
 '4'=>'4',
);

$_LANG['CaseTypes'] = array
(
 ''              => '',
 'Functional'    => 'Function',
 'Configuration' => 'Configuration',
 'Setup'         => 'Setup',
 'Security'      => 'Security',
 'Performance'   => 'Performance',
 'Other'         => 'Others',
);

$_LANG['CaseMethods'] = array
(
 ''           => '',
 'Manual'     => 'Manual',
 'Automation' => 'Automation'
);

$_LANG['CasePlans'] = array
(
 ''           => '',
 'Function'   => 'Functional Test',
 'UnitTest'   => 'Unit Test',
 'BVT'        => 'BVT',
 'Intergrate' => 'Intergration Test',
 'System'     => 'System Test',
 'Smoke'      => 'Smoke Test',
 'Acceptance' => 'Acceptance Test',
);

$_LANG['ScriptStatuses'] = array
(
 ''              => '',
 'NotPlanned'    => 'Not Planned',
 'Planning'      => 'Planning',
 'Blocked'       => 'Blocked',
 'Coding'        => 'Coding',
 'CodingDone'    => 'Coding Done',
 'Reviewed'      => 'Reviewed',
);


$_LANG['MarkForDeletions'] = array
(
 '0' => 'No',
 '1' => 'Yes'
);

/* The fields of ResultInfo table. */
$_LANG['ResultFields'] = array
(
  'ResultID'      => 'Result ID',
  'ProjectID'     => 'Project ID',
  'ProjectName'   => 'Project Name',
  'ModuleID'      => 'Module ID',
  'ModulePath'    => 'Module Path',
  'CaseID'        => 'Related Case',
  'ResultTitle'   => 'Result Title',
  'ResultValue'   => 'Result',
  'ResultStatus'  => 'Status',
  'ResultSteps'   => 'Steps',
  'OpenedBy'      => 'Opened By',
  'OpenedDate'    => 'Open Date',
  'AssignedTo'    => 'Assigned To',
  'MailTo'        => 'Mail To',
  'AssignedDate'  => 'Assign Date',
  'LastEditedBy'  => 'Last Edited By',
  'LastEditedDate'=> 'Last Edit Date',
  'ModifiedBy'    => 'Ever Changed By',
  'ResultOS'      => 'OS',
  'ResultBrowser' => 'Browser',
  'ResultMachine' => 'Machine',
  'ResultBuild'   => 'Build',
  'ResultKeyword' => 'Keywords',
  'BugID'         => 'Related Bugs'
);

/* The fields used to query in QueryCase.php.(Note: the field will be displayed in the order you defined here). */
$_LANG['ResultQueryField'] = array
(
   'ResultID'       => $_LANG['ResultFields']['ResultID'],
   'ResultTitle'    => $_LANG['ResultFields']['ResultTitle'],
   'ProjectName'    => $_LANG['ResultFields']['ProjectName'],
   'ModulePath'     => $_LANG['ResultFields']['ModulePath'],
   'ResultValue'    => $_LANG['ResultFields']['ResultValue'],
   'ResultStatus'   => $_LANG['ResultFields']['ResultStatus'],
   'AssignedTo'     => $_LANG['ResultFields']['AssignedTo'],
   'MailTo'         => $_LANG['ResultFields']['MailTo'],
   'LastEditedBy'   => $_LANG['ResultFields']['LastEditedBy'],
   'LastEditedDate' => $_LANG['ResultFields']['LastEditedDate'],
   'OpenedBy'       => $_LANG['ResultFields']['OpenedBy'],
   'OpenedDate'     => $_LANG['ResultFields']['OpenedDate'],
   'ResultBuild'    => $_LANG['ResultFields']['ResultBuild'],
   'ResultOS'       => $_LANG['ResultFields']['ResultOS'],
   'ResultBrowser'  => $_LANG['ResultFields']['ResultBrowser'],
   'ResultMachine'  => $_LANG['ResultFields']['ResultMachine'],
   'ResultKeyword'  => $_LANG['ResultFields']['ResultKeyword'],
   'CaseID'         => $_LANG['ResultFields']['CaseID'],
   'BugID'          => $_LANG['ResultFields']['BugID'],
   'ModifiedBy'     => $_LANG['ResultFields']['ModifiedBy'],
);

$_LANG['DefaultResultQueryFields'] = array
(
   'ResultID'       => $_LANG['ResultFields']['ResultID'],
   'ResultTitle'    => $_LANG['ResultFields']['ResultTitle'],
   'OpenedBy'       => $_LANG['ResultFields']['OpenedBy'],
   'AssignedTo'     => $_LANG['ResultFields']['AssignedTo'],
   'CaseID'         => $_LANG['ResultFields']['CaseID'],
   'ResultValue'    => $_LANG['ResultFields']['ResultValue'],
   'LastEditedDate' => $_LANG['ResultFields']['LastEditedDate'],
);


$_LANG['ResultStatuses'] = array
(
 'Completed'   => 'Completed',
 'Investigate' => 'Investigating',
 'Resolved'    => 'Resolved',
);

$_LANG['ResultOS']      = $_LANG['BugOS'];
$_LANG['ResultBrowser'] = $_LANG['BugBrowser'];
$_LANG['ResultMachine'] = $_LANG['BugMachine'];

$_LANG['ResultValues'] = array
(
 ''     => '',
 'Pass' => 'Passed',
 'Fail' => 'Failed',
);

/* nomal value. */
$_LANG['Confirm'] = 'Confirm';
$_LANG['Cancle']  = 'Cancel';

$_LANG['Edit']    = 'Edit';
$_LANG['Save']    = 'Save';
$_LANG['Delete']  = 'Delete';

$_LANG['AddTime']  = 'Added Time';
$_LANG['AddedBy']  = 'Added By';
$_LANG['LastTime'] = 'Last Modified Time';
$_LANG['LastModifiedBy'] = 'Modified By';

/* Define the Case Action  */
$_LANG['OpenCase'] = 'New Case';

/* Define the language about admin management */
$_LANG['ManageProject']    = 'Project Management';
$_LANG['ManageModule']     = 'Module Management';
$_LANG['ManageBugModule']  = 'Bug Modules';
$_LANG['ManageCaseModule'] = 'Case Modules';
$_LANG['ManageUser']       = 'User Management';
$_LANG['ManageGroup']      = 'Group Management';
$_LANG['UserLog']          = 'User Log';

/* Project */
$_LANG['ProjectList']      = 'Project List';
$_LANG['BackToProjectList'] = 'Back To Project List';
$_LANG['ProjectID']        = 'Project ID';
$_LANG['ProjectName']      = 'Project Name';
$_LANG['AllProject']       = 'All Project(s)';
$_LANG['AddProject']       = 'Add Project';
$_LANG['GoOnAddPro']       = 'Add More Project';
$_LANG['SaveProject']      = 'Save Project';
$_LANG['EditProject']      = 'Edit Project';
$_LANG['DropProject']      = 'Drop';
$_LANG['ConfirmDropProject'] = 'Are you sure you want to drop the project?';
$_LANG['ActiveProject']    = 'Activate';
$_LANG['ConfirmActiveProject'] = 'Are you sure you want to activate the project?';
$_LANG['ProjectExist']     = 'The project name already exists, please use another name.';
$_LANG['NoProjectName']    = 'No project name found';
$_LANG['NoEditPro']        = 'No change';
$_LANG['SucceedEditPro']   = 'Editing project succeeded.' . htmlLink($_LANG['BackToProjectList'], 'AdminProjectList.php');
$_LANG['SucceedAddPro']    = 'Adding project succeeded.';
$_LANG['ProjectGroup']     = 'Project Groups';
$_LANG['ProjectManager']   = 'Project Managers';
$_LANG['MergeProject']     = 'Merge Project';
$_LANG['MergeTo']          = 'Merge to';
$_LANG['MergeName']        = 'Name after merging';
$_LANG['MergeSelfDenied']  = 'Cannot merge self';
$_LANG['SucceedMergeProject'] = 'Merging project succeeded ';

/* Module */
$_LANG['ModuleList']       = 'Module List';
$_LANG['AddModule']        = 'Add Module';
$_LANG['EditModule']       = 'Edit Module';
$_LANG['DeleteModule']     = 'Delete Module';
$_LANG['ConfirmDeleteModule'] = 'Are you sure you want to delete the module?';
$_LANG['ModuleName']       = 'Module Name';
$_LANG['OwnProject']       = 'Under the Project';
$_LANG['ParentModule']     = 'Parent Module';
$_LANG['DisplayOrder']     = 'Display Order';
$_LANG['DisplayOrderNote'] = 'Please input an integer between 0~255';
$_LANG['ModuleOwner']      = 'Module Owner';
$_LANG['SaveModule']       = 'Save Module';
$_LANG['NoModuleName']     = 'Not input module name';
$_LANG['ModuleExist']      = 'The module name already exists, please use another name';
$_LANG['ModuleNotBeRecursive'] = 'The module cannot be the child of oneself';
$_LANG['ModuleHasChildModule'] = 'The module has child module, cannot be deleted';
$_LANG['SucceedAddModule'] = 'Adding module succeeded.';
$_LANG['SucceedEditModule'] = 'Editing module succeeded.';
$_LANG['SucceedDeleteModule'] = 'Deleting module succeeded.';


/* User */
$_LANG['UserList']         = 'User List';
$_LANG['BackToUserList']   = 'Back to User List';
$_LANG['UserID']           = 'User ID';
$_LANG['UserName']         = 'User Name';
$_LANG['RealName']         = 'Real Name';
$_LANG['Email']            = 'Email';
$_LANG['RawUserPassword']  = 'Old Password';
$_LANG['UserPassword']     = 'New Password';
$_LANG['RepeatUserPassword'] = 'Repeat Password';
$_LANG['ReceiveEmail']     = 'Accept to receive email notification';
$_LANG['RealNameNote']     = 'Please input a display name assigned to';
$_LANG['RawUserPasswordNote'] = 'Please input a default password';
$_LANG['EditUserPasswordNote'] = 'Password no change if empty';
$_LANG['EmailNote']        = 'Please input an email address to receive email notification';
$_LANG['WrongRawUserPassword'] = 'Old password error';
$_LANG['PasswordNotEqual'] = 'Password mismatched';
$_LANG['AddUser']          = 'Add User';
$_LANG['GoOnAddUser']      = 'Add More User';
$_LANG['SaveUser']         = 'Save User';
$_LANG['EditUser']         = 'Edit User';
$_LANG['DropUser']         = 'Drop';
$_LANG['AlreadyDropedUser'] = 'The user has already been dropped';
$_LANG['ActiveUser']       = 'Activate';
$_LANG['UserExist']        = 'User name already exists';
$_LANG['NoUserName']       = 'Not input user name';
$_LANG['NoRealName']       = 'Not input real name';
$_LANG['NoEmail']          = 'Not input email';
$_LANG['InvalidEmail']     = 'Invalid email address';
$_LANG['InvalidUserName']  = 'UserName can not have quotation marks and &';
$_LANG['NoPassword']       = 'Not input password';
$_LANG['NoEditUser']       = 'No changes';
$_LANG['SucceedEditUser']  = 'Editing user succeeded.' . htmlLink($_LANG['BackToUserList'], 'AdminUserList.php');
$_LANG['SuccessEditMyInfo'] = 'Editing my information succeeded.';
$_LANG['SucceedAddUser']   = 'Adding user succeeded';
$_LANG['ConfirmDelUser']   = 'Are you sure to drop the user?';
$_LANG['ConfirmActiveUser'] = 'Are you sure to activate the user?';
$_LANG['AuthModeName']     = 'Authentication';
$_LANG['AuthMode']['DB']   = 'Internal Account';
$_LANG['AuthMode']['LDAP'] = 'Domain Account';
$_LANG['InvalidLDAPUserNameFormat'] = 'Incorrect domain account format, using <strong>domain\username</strong> format';
$_LANG['LDAPUserNotFound'] = 'Domain Account not found';

/* Group */
$_LANG['GroupList']        = 'Group List';
$_LANG['BackToGroupList']  = 'Back to Group List';
$_LANG['GroupID']          = 'Group ID';
$_LANG['GroupName']        = 'Group Name';
$_LANG['GroupUser']        = 'Group User';
$_LANG['GroupManager']     = 'Group Manager';
$_LANG['GroupACL']         = 'Group Permission';
$_LANG['AddGroup']         = 'Add Group';
$_LANG['GoOnAddGroup']     = 'Add More Group';
$_LANG['SaveGroup']        = 'Save Group';
$_LANG['EditGroup']        = 'Edit Group';
$_LANG['DropGroup']        = 'Drop Group';
$_LANG['GroupNameExist']   = 'Group name already exists!';
$_LANG['GroupExist']       = 'Group already exists!';
$_LANG['NoGroupName']      = 'Not input group name';
$_LANG['NoEditGroup']      = 'No Changes';
$_LANG['SucceedEditGroup'] = 'Editing group succeeded.' . htmlLink($_LANG['BackToGroupList'], 'AdminGroupList.php');
$_LANG['SucceedAddGroup']  = 'Adding group succeeded.';
$_LANG['ConfirmDelGroup']  = 'Are you sure you want to delete the group?';
$_LANG['AllUserGroupName'] = '[All Users]';

/* User log */
$_LANG['UserLogList']      = 'User Log list';
$_LANG['LogID']            = 'Log ID';
$_LANG['LoginIP']          = 'Login IP';
$_LANG['LoginTime']        = 'Login Time';

/* Setting the header menu */
$_LANG['Welcome'] = 'Welcome';
$_LANG['EditPer'] = 'Edit My Info';
$_LANG['Admin']   = 'Administation';
$_LANG['Logout']  = 'Log Out';
$_LANG['Manual']  = 'Help';

/* Setting the upload file */
$_LANG['Accessories']  = 'Attachment';
$_LANG['AttachFile']   = 'Attach File';
$_LANG['FileShowName'] = 'Display Name';


/*========================================Setting for single page=========================*/
/* Setting of Login.php */
$_LANG['LoginTitle']     = 'Welcome To BugFree';
$_LANG['TestUserName']   = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;User:';
$_LANG['TestUserPWD']    = 'Password:';
$_LANG['ButtonLogin']    = 'Login';
$_LANG['SelectLanguage'] = 'Language:';
$_LANG['SelectStyle']    = 'Select Style:';
$_LANG['RememberMe']     = 'Remember me on this computer.';

/* Setting of UserControl.php */
$_LANG['ConfirmDelQuery'] = 'Are you sure you want to delete the query?';


/*====================================Setting for bug, case, result=========================*/
$_LANG['PreButton']  = 'Previous';
$_LANG['NextButton'] = 'Next';
$_LANG['SaveButton'] = 'Save';
$_LANG['Comments']   = 'Comment';

/*=====================================Setting for open, edit, fix bug====================*/
$_LANG['OpenBug']    = 'New Bug';
$_LANG['ViewBug']    = 'Bug';
$_LANG['EditBug']    = 'Edit Bug';
$_LANG['ResolveBug'] = 'Resolve Bug';
$_LANG['CloseBug']   = 'Close Bug';
$_LANG['ActiveBug']  = 'Activate Bug';
$_LANG['Project']    = 'Project';
$_LANG['Module']     = 'Module';
$_LANG['BugTitle']   = 'Bug Title';
$_LANG['ReproSteps'] = 'Repro Steps';
$_LANG['DefaultReproSteps']['Step'] = '[Steps]';
$_LANG['DefaultReproSteps']['StepInfo'] = <<<EOT
1.
2.

EOT;
$_LANG['DefaultReproSteps']['Result']       = '[Result]';
$_LANG['DefaultReproSteps']['ExpectResult'] = '[Expected]';
$_LANG['DefaultReproSteps']['Note']         = '[Note]';

$_LANG['BugID'] = 'Bug ID';

$_LANG['NoBugTitle']        = 'Bug Title cannot be empty';
$_LANG['NoBugRepro']        = 'Repro Steps cannot be empty';
$_LANG['NoOpenBuild']       = 'Open Build cannot be empty';
$_LANG['NoBugSeverity']     = 'Severity cannot be empty';
$_LANG['NoBugType']         = 'Bug Type cannot be empty';
$_LANG['NoHowFound']        = 'How Found cannot be empty';
$_LANG['NoDuplicateID']     = 'Please input the duplicated bug ID';
$_LANG['NoResolvedBuild']   = 'Resolve Build cannot be empty';
$_LANG['NoResolution']      = 'Resolution cannot be empty';
$_LANG['AlreadyResolved']   = 'The bug has already been resolved';
$_LANG['AlreadyClosed']     = 'The bug has already been closed';
$_LANG['AlreadyActive']     = 'The bug has already been activated';
$_LANG['BugAlreadyChanged'] = 'The bug has already been modified, please reopen and edit again.';

$_LANG['OpenBugButton']     = 'New';
$_LANG['EditBugButton']     = 'Edit';
$_LANG['CopyBugButton']     = 'Copy';
$_LANG['ResolveBugButton']  = 'Resolve';
$_LANG['CloseBugButton']    = 'Close';
$_LANG['ActiveBugButton']   = 'Activate';
$_LANG['BugStatusInfo']     = 'Bug Status';
$_LANG['BugOpenedInfo']     = 'Open';
$_LANG['BugResolvedInfo']   = 'Resolve';
$_LANG['BugClosedInfo']     = 'Close';
$_LANG['BugConditionInfo']  = 'Bug Related';
$_LANG['BugOtherInfo']      = 'Other Info';
$_LANG['BugHistoryInfo']    = 'History';
$_LANG['BugReproInfo']      = 'Repro Steps';
$_LANG['BugFiles']          = 'Attachments';
$_LANG['MoreInfo']          = 'More Info';

/*=====================================Setting for add, edit case====================*/
$_LANG['AddCase']           = 'New Case';
$_LANG['NoCaseTitle']       = 'Case Title cannot be empty';
$_LANG['NoCaseStep']        = 'Case Steps cannot be empty';
$_LANG['NoCasePriority']    = 'Case Priority cannot be empty';
$_LANG['NoCaseType']        = 'Case Type cannot be empty';
$_LANG['NoCaseMethod']      = 'Case Method cannot be empty';
$_LANG['NoDisplayOrder']    = 'Case display order cannot be empty';
$_LANG['IllegalDisplayOrder']    = 'Case display order must be an integer between 0 and 255';
$_LANG['BadScriptedDate']   = 'Invalid date or date format. For exmple 2008-08-08';
$_LANG['CaseAlreadyChanged']= 'The case has already been modified, please reopen and edit again.';

$_LANG['CaseStatusInfo']    = 'Case Status';

$_LANG['CaseOpenedInfo']    = 'New';
$_LANG['CaseReviewInfo']    = 'Review';

$_LANG['AddCaseButton']     = 'New Case';
$_LANG['EditCaseButton']    = 'Edit';
$_LANG['CopyCaseButton']    = 'Copy';
$_LANG['RunCaseButton']     = 'Run';
$_LANG['RunCaseLink']       = 'Run Case';

$_LANG['CaseFiles']         = 'Attachment';
$_LANG['CaseMainInfo']      = 'Main Info';
$_LANG['CaseActionInfo']    = 'Action Info';
$_LANG['CaseStepsInfo']     = 'Steps';
$_LANG['CaseConditionInfo'] = 'Case Related';
$_LANG['CaseOtherInfo']     = 'Other Info';
$_LANG['CaseDefaultStepInfo'] = <<<EOT
[Steps]
1.
2.

[Verify]

[Note]

EOT;

$_LANG['CaseAutomationInfo'] = 'Automation Script';
/*=====================================Setting for add, edit result====================*/
$_LANG['OpenResult']         = 'New Result';
$_LANG['RunCaseStep']        = 'Run Case';
$_LANG['EditResult']         = 'Edit Result';
$_LANG['EditResultButton']   = 'Edit';
$_LANG['OpenBugButton']      = 'New Bug';
$_LANG['ResultActionInfo']   = 'Action Info';
$_LANG['ResultFiles']        = 'Attachments';

$_LANG['NoResultValue']      = 'Result Value cannot be empty';
$_LANG['NoResultSteps']      = 'Result Steps cannot be empty';
$_LANG['NoResultBuild']      = 'Result Build cannot be empty';
$_LANG['ResultAlreadyChanged'] = 'The result has already been modified, please reopen and edit again.';
$_LANG['ResultStatusInfo']   = 'Result Status';
$_LANG['ResultEnvInfo']      = 'Environment';
$_LANG['ResultOpenedInfo']   = 'New';
$_LANG['ResultMainInfo']     = 'Majar Info';
$_LANG['ResultActionInfo']   = 'Action Info';
$_LANG['ResultStepsInfo']    = 'Steps';
$_LANG['ResultConditionInfo'] = 'Result Related';
$_LANG['ResultOtherInfo']    = 'Other Info';


$_LANG['NotSaveMod'] = 'If you leave this page, all changes will be lost.';

/*=====================================Setting for pagination===========================*/
$_LANG['Pagination']['FirstPage']   = 'First Page';
$_LANG['Pagination']['EndPage']     = 'Last Page';
$_LANG['Pagination']['PrePage']     = 'Previous Page';
$_LANG['Pagination']['NextPage']    = 'Next Page';
$_LANG['Pagination']['TotalRecord'] = 'Total Items:';
$_LANG['Pagination']['NumPerPage']  = 'items per page';
$_LANG['Pagination']['PageNum']     = 'Page:';
$_LANG['Pagination']['NoResult']    = 'No Result';
$_LANG['Pagination']['Result']      = 'Result';


/*=====================================Setting for list caption==========================*/
$_LANG['CustomDisplay']     = 'Custom Fields';
$_LANG['AllFieldsTitle']    = 'Available Fields';
$_LANG['FieldsToShowTitle'] = 'Display Fields';
$_LANG['FieldsToShowBTN']   = 'OK';
$_LANG['FieldsDefaultBTN']  = 'Default Fields';
$_LANG['ExportHtmlTable']   = 'Export';
$_LANG['ReportForms']       = 'Report';
$_LANG['ExportCases']       = 'Export';
$_LANG['ImportCases']       = 'Import'; 
$_LANG['OpenedBugsInLastWeek'] = 'Opened Bugs in Last Week';
$_LANG['StaleBugsForOneWeek']  = 'Stale Bugs for One Week';

/*=====================================Setting for edit my info=========================*/
$_LANG['EditMyInfoButton'] = 'Submit';


/*=====================================Setting for install===================*/
$_LANG['InstallBugFree']      = 'BugFree Installation';
$_LANG['InstallChoose']       = 'Please Select';
$_LANG['InstallNextButton']   = 'Next';
$_LANG['InstallButton']       = 'Start to install';
$_LANG['UpgradeButton']       = 'Start to upgrade';
$_LANG['InstallNewBugFree']   = 'Install a new BugFree 2';
$_LANG['UpgradeBugFreeFrom1'] = 'Upgrade from BugFree 1.1';
$_LANG['UpgradeBugFreeFrom2'] = 'Upgrade from BugFree 2';
$_LANG['UpgradeBugFree']      = 'Upgrade BugFree';
$_LANG['UpgradeNotice']       = 'Before upgrading, please backup BugFree 1.x\'s program files, images and data.';
$_LANG['UpgradeLanguage']     = 'Language of BugFree need to be upgraded';
$_LANG['InstallStep1']        = '1. Set DB params of BugFree';
$_LANG['InstallStep2']        = '2. Set DB params of other database used to manage users';
$_LANG['InstallStep3']        = '3. Set Email params';
$_LANG['InstallStep4']        = '4. Others';
$_LANG['InstallStep5']        = '5. To be an administrator';
$_LANG['InstallDBHost']       = 'Database server host';
$_LANG['InstallDBUser']       = 'Database user';
$_LANG['InstallDBPassword']   = 'Password';
$_LANG['InstallDBDatabase']   = 'Database server host';
$_LANG['InstallTablePrefix']  = 'Table prefix';
$_LANG['InstallCreateDB']     = 'Create it';
$_LANG['InstallUserDBHost']   = 'Database server host';
$_LANG['InstallUserDBUser']   = 'Database user';
$_LANG['InstallUserDBPassword'] = 'Password';
$_LANG['InstallUserDBDatabase'] = 'Database name';
$_LANG['InstallTableName']     = 'Table name(phpbb:phpbb_users)';
$_LANG['InstallUserName']      = 'Field name of \'UserName\' (phpbb:username)';
$_LANG['InstallRealName']      = 'Field name of \'RealName\' (phpbb:username)';
$_LANG['InstallUserPassword']  = 'Field name of \'UserPassword\'(phpbb:user_password)';
$_LANG['InstallEmail']         = 'Field name of \'Email\'(phpbb:user_email)';
$_LANG['InstallEncryptType']   = 'Encrypt type(phpbb:md5)';
$_LANG["InstallFromAddress"]   = "From address";
$_LANG["InstallFromName"]      = "From name";
$_LANG["InstallSendMethod"]    = "Send method";
$_LANG["InstallSmtpHost"]      = "SMTP server host address";
$_LANG["InstallSmtpAuth"]      = "Need authentication?";
$_LANG["InstallSmtpUserName"]  = "SMTP User name";
$_LANG["InstallSmtpPassword"]  = "SMTP Password";
$_LANG["InstallUploadDirectory"]   = "Where to upload the attached file(The location should be readable and writable)";
$_LANG["InstallMaxFileSize"]       = "Max size of uploaded file in bytes";
$_LANG["InstallAdminUserName"]     = "Administrator's user name";
$_LANG["InstallAdminRealName"]     = "Administrator's real name";
$_LANG["InstallAdminUserEmail"]    = "Administrator's EMail";
$_LANG["InstallAdminUserPassword"] = "Administrator's password";
$_LANG["SampleBugInfo"]["BugTitle"] = "Sample：Welcome to use BugFree!";
$_LANG["SampleBugInfo"]["ReproSteps"] = <<<EOT
This is a bug sample.
Please visit administration page [url]Admin[/url] first.
1. Add new users in User Management page;
2. Add new group in Group Management page, assign users to the group;
3. Add new project in Project Management page, assign groups to the project.

For more help on how to use BugFree, please visit [url]http://www.bugfree.org.cn/help[/url].
EOT;
$_LANG["InstallSuccessed"] = <<<EOT
Installation Successed!<br /><br />
Default administrator user name: admin<br />
Default administrator password: 123456<br /><br />
Please click <a href="Login.php" target="_self"><b>Here</b></a> to login. Please change the password after logging in.<br />
EOT;
$_LANG["UpgradeSuccessed"] = 'Upgrade Successed, please click <a href="Login.php" target="_self"><b>Here</b></a> to login.';
$_LANG["InstallErrorNoMysqlModule"]   = "Mysql module not found";
$_LANG["InstallErrorBugDB"]           = "Verifying BugFree database failed:";
$_LANG["InstallErrorPrefixNeeded"]    = "Database table prefix not found";
$_LANG["InstallErrorCreateBugDB"]     = "Creating BugFree database failed: ";
$_LANG["InstallErrorUserDB"]          = "Verifying user database failed:";
$_LANG["InstallErrorUserTable"]       = " User managing table: ";
$_LANG["InstallErrorWritable"]        = " Should be readable and writable (o=rwx) ";
$_LANG["InstallErrorSmtpAuth"]        = " SMTP need user name and password ";
$_LANG["InstallErrorAdminUserInfo"]   = " Please specify the username, email, password of the administrator ";
$_LANG["InstallErrorCreateAdminUser"] = "Creating the administrator acount failed";
$_LANG["InstallErrorDBExists"]        = "The database already exists";
$_LANG["InstallErrorAlreadyNewest"]   = "Already the lastest version of BugFree. <br />Click <a href='Login.php' target='_self'><b>Here</b></a> to login.";
$_LANG["CreateDBSuccessed"]           = 'Creating the Database Successed! Please click <a href="install.php" target="_self"><b>Continue</b></a>';

/*=====================================Setting for notice===============================*/
$_LANG['NoticeBugSubject'] = 'Bugs assigned to you by now:';

/*=====================================Setting for report================================*/
$_LANG['ViewReportBTN'] = 'View Report';
$_LANG['SelectAllBTN']  = 'Select All';
$_LANG['SelectNoneBTN'] = 'Unselect All';
$_LANG['Others']        = 'Others';
$_LANG['TotalCount']    = 'Total';
$_LANG['Blank']         = '[Blank]';
$_LANG['ReportTimeRange'] = array
(
    'All'       => 'All',
    'LastWeek'  => 'Last Week',
    'ThisMonth' => 'This Month',
    'ThisYear'  => 'This Year',
    'Custom'    => 'Custom',
);

$_LANG['ReportBugCount'] = 'Bug Counts';
$_LANG["BugReportType"] =  array
(
    "BugsPerProject"     => "Bugs by Project",
    "BugsPerModule"      => "Bugs by Module",
    "BugsPerStatus"      => "Bugs by Status",
    "BugsPerSeverity"    => "Bugs by Severity",
    "BugsPerPriority"    => "Bugs by Priority",
    "BugsPerType"        => "Bugs by Type",
    "BugsPerHowFound"    => "Bugs by How Found",
    "BugsPerOS"          => "Bugs by OS",
    "BugsPerBrowser"     => "Bugs by Browser",
    "OpenedBugsPerUser"  => "Bugs by Opened by",
    "OpenedBugsPerDay"   => "Bugs by Open Date(per day)",
    "OpenedBugsPerWeek"  => "Bugs by Open Date(per week)",
    "OpenedBugsPerMonth" => "Bugs by Open Date(per month)",
    "ResolvedBugsPerUser"=> "Bugs by Resolved by",
    "BugsPerResolution"  => "Bugs by Resolution",
    "ResolvedBugsPerDay" => "Bugs by Resolve Date(per day)",
    "ResolvedBugsPerWeek"=> "Bugs by Resolve Date(per week)",
    "ResolvedBugsPerMonth"=> "Bugs by Resolve Date(per month)",
    "ClosedBugsPerUser"  => "Bugs by Closed by",
    "ClosedBugsPerDay"   => "Bugs by Close Date(per day)",
    "ClosedBugsPerWeek"  => "Bugs by Close Date(per week)",
    "ClosedBugsPerMonth" => "Bugs by Close Date(per month)",
    "ActivatedBugsPerDay" => "Bugs by Activate Date(per day)",
    "ActivatedBugsPerWeek" => "Bugs by Activate Date(per week)",
    "ActivatedBugsPerMonth" => "Bugs by Activate Date(per month)",
    "BugLiveDays"        => "Bugs by Resolved Duration",
    "AssignedBugsPerUser"=> "Bugs by Assigned to",
    "BugHistorys"        => "Bugs by History",
);


$_LANG['ReportCaseCount'] = 'Case Counts';
$_LANG["CaseReportType"] =  array
(
    "CasesPerProject"     => "Cases by Project",
    "CasesPerModule"      => "Cases by Module",
    "CasesPerStatus"      => "Cases by Status",
    "CasesPerPriority"    => "Cases by Priority",
    "CasesPerType"        => "Cases by Type",
    "CasesPerMethod"      => "Cases by Method",
    "CasesPerPlan"        => "Cases by Plan",
    "OpenedCasesPerUser"  => "Cases by Opened by",
    "OpenedCasesPerDay"   => "Cases by Open Date(per day)",
    "OpenedCasesPerWeek"  => "Cases by Open Date(per week)",
    "OpenedCasesPerMonth" => "Cases by Open Date(per month)",
    "CasePerScriptStatus" => "Cases by Script Status",
    "CasesPerScriptedBy"  => "Cases by Scripted by",
    "ScriptedDatePerDay"  => "Cases by Write Date(per day)",
    "ScriptedDatePerWeek" => "Cases by Write Date(per week)",
    "ScriptedDatePerMonth" => "Cases by Write Date(per month)",
);

$_LANG['ReportResultCount'] = 'Result Counts';
$_LANG["ResultReportType"] =  array
(
    "ResultsPerProject"     => "Results by Project",
    "ResultsPerModule"      => "Results by Module",
    "ResultsPerValue"       => "Results by Value",
    "ResultsPerStatus"      => "Results by Status",
    "OpenedResultsPerUser"  => "Results by Opened by",
    "OpenedResultsPerDay"   => "Results by Open Date(per day)",
    "OpenedResultsPerWeek"  => "Results by Open Date(per week)",
    "OpenedResultsPerMonth" => "Results by Open Date(per month)",
    "ResultsPerOS"          => "Results by OS",
    "ResultsPerBrowser"     => "Results by Browser",
);

$_CFG['ZeroTime'] = '0000-00-00 00:00:00';
$_CFG['NotForceDownloadFileType'] = array('jpg','jpeg','gif','png','bmp','html','htm');

$_CFG['FCFSWFType'] = array
(
   'Area2D'     => 'FCF_Area2D.swf',
   'Bar2D'      => 'FCF_Bar2D.swf',
   'Column2D'   => 'FCF_Column2D.swf',
   'Column3D'   => 'FCF_Column3D.swf',
   'Doughnut2D' => 'FCF_Doughnut2D.swf',
   'Line'       => 'FCF_Line.swf',
   'Pie3D'      => 'FCF_Pie3D.swf',
);

$_CFG['ReportTypeSWF'] =  array
(
    "BugsPerProject"     => 'Pie3D',
    "BugsPerModule"      => 'Pie3D',
    "BugsPerStatus"      => 'Pie3D',
    "BugsPerSeverity"    => 'Pie3D',
    "BugsPerPriority"    => 'Pie3D',
    "BugsPerHowFound"    => 'Pie3D',
    "BugsPerOS"          => 'Pie3D',
    "BugsPerBrowser"     => 'Pie3D',
    "OpenedBugsPerUser"  => 'Bar2D',
    "OpenedBugsPerDay"   => 'Column2D',
    "OpenedBugsPerWeek"  => 'Column2D',
    "OpenedBugsPerMonth" => 'Column2D',
    "ResolvedBugsPerDay" => 'Line',
    "ResolvedBugsPerWeek"  => 'Line',
    "ResolvedBugsPerMonth" => 'Line',
    "ClosedBugsPerUser"   => 'Bar2D',
    "ClosedBugsPerDay"    => 'Column3D',
    "ClosedBugsPerWeek"   => 'Column3D',
    "ClosedBugsPerMonth"  => 'Column3D',
    "ActivatedBugsPerDay" => 'Column2D',
    "ActivatedBugsPerWeek" => 'Column2D',
    "ActivatedBugsPerMonth" => 'Column2D',
    "AssignedBugsPerUser" => 'Bar2D',
    "ResolvedBugsPerUser" => 'Bar2D',
    "BugsPerResolution"  => 'Bar2D',
    "BugsPerType"        => 'Bar2D',
    "BugLiveDays"        => 'Column2D',
    "BugHistorys"        => 'Column2D',
    "CasesPerProject"    => 'Pie3D',
    "CasesPerModule"     => 'Pie3D',
    "CasesPerStatus"     => "Pie3D",
    "CasesPerPriority"   => "Pie3D",
    "CasesPerType"       => "Pie3D",
    "CasesPerMethod"     => "Pie3D",
    "CasesPerPlan"       => "Pie3D",
    "OpenedCasesPerUser" => "Bar2D",
    "OpenedCasesPerDay"  => "Column2D",
    "OpenedCasesPerWeek" => "Column2D",
    "OpenedCasesPerMonth" => "Column2D",
    "CasePerScriptStatus" => "Bar2D",
    "CasesPerScriptedBy"  => "Bar2D",
    "ScriptedDatePerDay"  => "Column3D",
    "ScriptedDatePerWeek" => "Column3D",
    "ScriptedDatePerMonth" => "Column3D",
    "ResultsPerProject"    => "Pie3D",
    "ResultsPerModule"     => "Pie3D",
    "ResultsPerValue"      => "Pie3D",
    "ResultsPerStatus"     => "Pie3D",
    "OpenedResultsPerUser"  => "Bar2D",
    "OpenedResultsPerDay"   => "Column3D",
    "OpenedResultsPerWeek"  => "Column3D",
    "OpenedResultsPerMonth" => "Column3D",
    "ResultsPerOS"          => "Column2D",
    "ResultsPerBrowser"     => "Column2D",
);

$_CFG['ReportGraphOption']['Default'] = array
(
   'decimalPrecision'  => '0',
   'formatNumberScale' => '0',
   'rotateNames'       => '1',
   'basefontsize'      => '12',
   'formatNumber'      => '0',
);

$_CFG['ReportGraphOption']['Column2D'] = $_CFG['ReportGraphOption']['Default'] + array();
$_CFG['ReportGraphOption']['Column3D'] = $_CFG['ReportGraphOption']['Default'] + array();
$_CFG['ReportGraphOption']['Bar2D']    = $_CFG['ReportGraphOption']['Default'] + array();
$_CFG['ReportGraphOption']['Pie3D']    = $_CFG['ReportGraphOption']['Default'] + array
(
  'showPercentageValues'=>'0',
  'showNames'       =>'1',
  'showValues'      =>'0',
  'pieRadius'       =>"150",
  'pieYScale'       =>'60',
  'pieBorderAlpha'  =>'40',
  'pieFillAlpha'    =>'70',
  'pieSliceDepth'   =>'15',
  'chartLeftMargin' =>"0",
  'chartRightMargin'=>"0",
  'chartTopMargin'  =>"0"
);
$_CFG['ReportGraphOption']['Line'] = $_CFG['ReportGraphOption']['Default'] + array
(
  'showAlternateHGridColor' => '1',
  'AlternateHGridColor'     => 'ff5904',
  'divLineColor'            => 'ff5904',
  'divLineAlpha'            => '20',
  'alternateHGridAlpha'     => '5',
  'anchorRadius'            => '1',
);
$_CFG['ReportTypeGraphOption']['BugsPerProject'] = array
(
 'showNames' => 0,
);
$_CFG['ReportTypeGraphOption']['BugsPerModule'] = array
(
 'showNames' => 0,
);
$_CFG['ReportTypeGraphOption']['CasesPerProject'] = array
(
 'showNames' => 0,
);
$_CFG['ReportTypeGraphOption']['CasesPerModule'] = array
(
 'showNames' => 0,
);
$_CFG['ReportTypeGraphOption']['ResultsPerProject'] = array
(
 'showNames' => 0,
);
$_CFG['ReportTypeGraphOption']['ResultsPerModule'] = array
(
 'showNames' => 0,
);

/* Operators. */
$_CFG['FieldType'] = array
(
    'Project'=> 'ProjectName',
    'Number' => 'BugID,CaseID,ResultID,BugSeverity,BugPriority,CasePriority,DuplicateID,LinkID,DisplayOrder',
    'Date'   => 'LastEditedDate,OpenedDate,ResolvedDate,ClosedDate,ScriptedDate',
    'String' => 'BugTitle,OpenedBuild,ResolvedBuild,BugMachine,BugKeyword,CaseTitle,CaseKeyword,ResultTitle,ResultBuild,ResultMachine,ResultKeyword',
    'People' => 'AssignedTo,LastEditedBy,OpenedBy,ResolvedBy,ClosedBy,ScriptedBy',
    'MutilPeople' => 'MailTo,ModifiedBy',
    'Option' => 'BugStatus,BugSubStatus,BugType,HowFound,BugOS,BugBrowser,Resolution,CaseStatus,CaseType,CaseMethod,CasePlan,ScriptStatus,MarkForDeletion,ResultValue,ResultStatus,ResultOS,ResultBrowser',
    'Path'   => 'ModulePath'
);

$_CFG['FieldTypeOperation'] = array
(
    'Project'=> '=,!=,LIKE,NOT LIKE',
    'Number' => '=,!=,>,<,>=,<=,IN',
    'Date'   => '=,!=,>,<,>=,<=',
    'String' => 'LIKE,NOT LIKE',
    'People' => '=,!=,IN',
    'MutilPeople' => '=',
    'Option' => '=,!=',
    'Path'   => 'LIKE,NOT LIKE,UNDER'
);

$_LANG['ExportCountExceed'] = 'Export items can not exceed 5000.';
$_LANG['ImportLable'] = 'Please choose an import file:';
$_LANG['InputFileExceed'] = 'The max file size is 2M.';
$_LANG['InputFileSuffixError'] = 'The imported file\'s extension must be .xml.';
$_LANG['ImportColumnNotNull'] = 'can not be null.';
$_LANG['ImportFinished'] = 'Import finished! Total cases: {param0}, succeed cases: {param1}, failed cases: {param2}.';
$_LANG['ImportNotes'] = '<br><div  style="text-align:left;color:red"><b>All import fields can not be null</b></div>
                         <br><div  style="text-align:left"><b>Import file must contain the following Fields: </b></div><br> 
                          1. Case ID:  Add new case if case ID is 0, otherwise edit the existance case.<br>
                          2. Case Title, Project Name, Module Path, Steps  <br>                                                  
                          <br> 
                          <div  style="text-align:left"><b>Import file can include the following optional fields:</b></div><br> 
                          Assigned to, Mail to, Priority, Case Type, Case Method<br>
                          Write Date, Script Location, Mark For Deletion, Keywords<br>
                          Scripted By, Case Status, Test Plan, Script Status, Display Order';

$_LANG['RunCases'] = 'Batch Run';
$_LANG['OpenBatchCases'] = 'Batch Open Result';
$_LANG['MultipleCases'] = 'Mutiple items';
$_LANG['RunCasesNotNull'] = 'Please choose cases';
$_LANG['NotifyEmail'] = 'Notify Email';
$_LANG['NotifyEmailPoject'] = 'Project';
$_LANG['NotifyEmailModule'] = 'Module';
$_LANG['NotifyEmailBugStatistics'] = 'Bug Statistics ';
$_LANG['StatisticsThisWeek'] = 'Opened bugs in last seven days';
$_LANG['StatisticsNotOperateOverWeek'] = 'Stale bugs for seven days';
$_LANG['StatisticsTotal'] = 'history statistics';
$_LANG['RunCasesMaxValue'] = 'The count of cases can not be more than 100';
$_LANG['RunCasesConfirm'] = ' {param0} result(s) will be created，are you sure?';
$_LANG['DateInvalidAlert'] = 'Please use valid date format. For example, 2009-10-8 or -7.';
$_LANG['WrongEmailFormat'] = 'Invalid email format';
$_LANG['MutipleEmails'] = 'Format As Comma Separated';
$_LANG['CaseImport'] = array
(
  'Case ID'            => 'CaseID',
  'Project ID'           => 'ProjectID',
  'Project Name'          => 'ProjectName',
  'Module ID'           =>'ModuleID',
  'Module Path'          => 'ModulePath',
  'Case Title'         =>'CaseTitle',
  'Priority'            => 'CasePriority',
  'Type'         => 'CaseType',
  'Case Status'         => 'CaseStatus',
  'Case Method'          => 'CaseMethod',
  'Test Plan'          => 'CasePlan',
   'Steps'             => 'CaseSteps',
  'Display Order'          => 'DisplayOrder',
  'Opened By'            => 'OpenedBy',
   'Open Date'         => 'OpenedDate',
  'Assigned To'            => 'AssignedTo',
  'Assign Date'          => 'AssignedDate',
  'Last Edited by'        => 'LastEditedBy',
  'Last Edit Date'      =>'LastEditedDate',
  'Ever Changed by'        => 'ModifiedBy',
  'Script Status'          =>  'ScriptStatus',
  'Scripted By'            => 'ScriptedBy',
  'Write Date'          => 'ScriptedDate',
  'Script Location'          =>  'ScriptLocation',
  'Mail To'            => 'MailTo',
  'Keywords'            => 'CaseKeyword',
  'Related Cases'         => 'LinkID',
  'Related Bugs'          => 'BugID',
  'Related Results'       => 'ResultID',
  'Display Order'          => 'DisplayOrder',
  'Mark For Deletion'          => 'MarkForDeletion',
);
?>
