<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * Stat functions library of BugFree system.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
function reportPerDBColoumn($TableName, $ColumnName, $ReportCondition = '1', $ColumnLang = '', $OrderBy = 'SetValue DESC', $SetEmpty = false)
{
    $Columns = "{$ColumnName} AS SetName, COUNT(*) AS SetValue";
    $DataSetList = dbGetList($TableName,$Columns, $ReportCondition, 'SetName', $OrderBy, '', 'SetName');
    if(is_array($ColumnLang))
    {
        $DataSetList = reportSetFullTypeInfo($DataSetList, $ColumnLang, $SetEmpty);
    }
    return $DataSetList;
}

function reportPerProject($ReportMode,$ReportCondition = '1')
{
    $Columns = "ProjectName AS SetName, COUNT(*) AS SetValue";
    $DataSetList = dbGetList("{$ReportMode}Info",$Columns, $ReportCondition, 'ProjectID', 'SetValue DESC', '', 'SetName');
    $DataSetList = reportInterceptDataList($DataSetList, 'Percent', '10');
    return $DataSetList;
}

function reportPerModule($ReportMode, $ReportCondition = '1')
{
    $Columns = "ProjectID, ModulePath AS SetName, COUNT(*) AS SetValue";
    $DataSetList = dbGetList("{$ReportMode}Info",$Columns, $ReportCondition, 'ModulePath', 'SetValue DESC', '50', 'SetName');
    $DataSetList = reportInterceptDataList($DataSetList, 'Percent', 0);
    return $DataSetList;
}

function reportPerUser($TableName, $ActionColumn, $ReportCondition = '1', $Limit = 15)
{
    $TestUserList = testGetOneDimUserList();
    $Columns = "{$ActionColumn} AS SetName, COUNT(*) AS SetValue";
    $DataSetList = dbGetList($TableName,$Columns, $ReportCondition, 'SetName', 'SetValue DESC', '', 'SetName');
    $DataSetList = reportSetFullTypeInfo($DataSetList, $TestUserList, false);
    $DataSetList = reportInterceptDataList($DataSetList, 'Limit', $Limit);
    return $DataSetList;
}

/**
 * Creat data of BugsPerProject report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerProject($ReportCondition = '1')
{
    return reportPerProject('Bug',$ReportCondition);
}

/**
 * Creat data of BugsPerModule  report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerModule($ReportCondition = '1')
{
    return reportPerModule('Bug',$ReportCondition);
}


/**
 * Creat data of BugsPerSeverity report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerSeverity($ReportCondition = '1')
{
    global $_LANG;
    unset($_LANG['BugSeveritys']['']);
    return reportPerDBColoumn('BugInfo', 'BugSeverity', $ReportCondition, $_LANG['BugSeveritys']);
}

/**
 * Creat data of BugsPerPriority report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerPriority($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('BugInfo', 'BugPriority', $ReportCondition, $_LANG['BugPriorities']);
}

/**
 * Creat data of BugsPerResolution report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerResolution($ReportCondition = '1')
{
    global $_LANG;
    unset($_LANG['BugResolutions']['']);
    return reportPerDBColoumn('BugInfo', 'Resolution', $ReportCondition . " AND Resolution <> ''", $_LANG['BugResolutions']);
}


/**
 * Creat data of BugsPerStatus report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerStatus($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('BugInfo', 'BugStatus', $ReportCondition, $_LANG['BugStatus']);
}

/**
 * Creat data of BugsPerType report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerType($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('BugInfo', 'BugType', $ReportCondition, $_LANG['BugTypes']);
}

/**
 * Creat data of BugsPerHowFound report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerHowFound($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('BugInfo', 'HowFound', $ReportCondition, $_LANG['BugHowFound']);
}

/**
 * Creat data of BugsPerOS report
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerOS($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('BugInfo', 'BugOS', $ReportCondition, $_LANG['BugOS']);
}

/**
 * Creat data of BugsPerBrowser report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugsPerBrowser($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('BugInfo', 'BugBrowser', $ReportCondition, $_LANG['BugBrowser']);
}

/**
 * Creat data of OpenedBugsPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedBugsPerUser($ReportCondition = '1', $Limit = 15)
{
    return reportPerUser('BugInfo', 'OpenedBy', $ReportCondition, $Limit);
}

/**
 * Creat data of OpenedBugsPerDay report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedBugsPerDay($ReportCondition = '1')
{
    return reportPerDBColoumn('BugInfo', "DATE_FORMAT(OpenedDate, '%y-%m-%d')", $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of OpenedBugsPerWeek report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedBugsPerWeek($ReportCondition = '1')
{
    $WeekExp = "Date_FORMAT(Date_SUB(OpenedDate, INTERVAL (if(DATE_FORMAT(OpenedDate,'%w') = 0,7,DATE_FORMAT(OpenedDate, '%w')))-1 DAY), '%y-%m-%d')";
    return reportPerDBColoumn('BugInfo', $WeekExp, $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of OpenedBugsPerMonth report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedBugsPerMonth($ReportCondition = '1')
{
    return reportPerDBColoumn('BugInfo', "DATE_FORMAT(OpenedDate, '%y-%m')", $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of ResolvedBugsPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResolvedBugsPerUser($ReportCondition = '1', $Limit = 15)
{
    $ReportCondition .= " AND ResolvedBy <> ''";
    return reportPerUser('BugInfo', 'ResolvedBy', $ReportCondition, $Limit);
}

/**
 * Creat data of ResolvedBugsPerDay report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResolvedBugsPerDay($ReportCondition = '1')
{
    $ReportCondition .= " AND Resolution <> ''";
    return reportPerDBColoumn('BugInfo', "DATE_FORMAT(ResolvedDate, '%y-%m-%d')", $ReportCondition, '', 'ResolvedDate ASC');
}


/**
 * Creat data of ResolvedBugsPerWeek report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResolvedBugsPerWeek($ReportCondition = '1')
{
    $ReportCondition .= " AND Resolution <> ''";
    $WeekExp = "Date_FORMAT(Date_SUB(ResolvedDate, INTERVAL (if(DATE_FORMAT(ResolvedDate,'%w') = 0,7,DATE_FORMAT(ResolvedDate, '%w')))-1 DAY), '%y-%m-%d')";
    return reportPerDBColoumn('BugInfo', $WeekExp, $ReportCondition, '', 'ResolvedDate ASC');
}

/**
 * Creat data of ResolvedBugsPerMonth report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResolvedBugsPerMonth($ReportCondition = '1')
{
    $ReportCondition .= " AND Resolution <> ''";
    return reportPerDBColoumn('BugInfo', "DATE_FORMAT(ResolvedDate, '%y-%m')", $ReportCondition, '', 'ResolvedDate ASC');
}



/**
 * Creat data of ClosedBugsPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportClosedBugsPerUser($ReportCondition = '1', $Limit = 15)
{
    $ReportCondition .= " AND ClosedBy <> ''";
    return reportPerUser('BugInfo', 'ClosedBy', $ReportCondition, $Limit);
}

/**
 * Creat data of ClosedBugsPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportClosedBugsPerDay($ReportCondition = '1')
{
    $ReportCondition .= " AND BugStatus = 'Closed'";
    return reportPerDBColoumn('BugInfo', "DATE_FORMAT(ClosedDate, '%y-%m-%d')", $ReportCondition, '', 'ClosedDate ASC');
}


/**
 * Creat data of ClosedBugsPerWeek report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportClosedBugsPerWeek($ReportCondition = '1')
{
    $ReportCondition .= " AND BugStatus = 'Closed'";
    $WeekExp = "Date_FORMAT(Date_SUB(ClosedDate, INTERVAL (if(DATE_FORMAT(ClosedDate,'%w') = 0,7,DATE_FORMAT(ClosedDate, '%w')))-1 DAY), '%y-%m-%d')";
    return reportPerDBColoumn('BugInfo', $WeekExp, $ReportCondition, '', 'ClosedDate ASC');
}

/**
 * Creat data of ClosedBugsPerMonth report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportClosedBugsPerMonth($ReportCondition = '1')
{
    $ReportCondition .= " AND BugStatus = 'Closed'";
    return reportPerDBColoumn('BugInfo', "DATE_FORMAT(ClosedDate, '%y-%m')", $ReportCondition, '', 'ClosedDate ASC');
}

/**
 * Creat data of AssignedBugsPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportAssignedBugsPerUser($ReportCondition = '1', $Limit = 15)
{
    $ReportCondition .= " AND AssignedTo <> 'Active' AND AssignedTo <> 'Closed' AND BugStatus <> 'Closed'";
    return reportPerUser('BugInfo', 'AssignedTo', $ReportCondition, $Limit);
}

/**
 * Creat data of BugLiveDays report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugLiveDays($ReportCondition = '1')
{
    $ReportCondition .= " AND BugStatus = 'Closed'";
    $Columns = "(TO_DAYS(ClosedDate) - TO_DAYS(OpenedDate)) AS SetName, COUNT(*) AS SetValue";
    $DataSetList = dbGetList('BugInfo',$Columns, $ReportCondition, 'SetName', 'SetValue ASC', '', 'SetName');
    $DataSets = array(0=>array("SetName"=>"0", "SetValue"=>"0"),
                      1=>array("SetName"=>"1", "SetValue"=>"0"),
                      2=>array("SetName"=>"2", "SetValue"=>"0"),
                      3=>array("SetName"=>"3", "SetValue"=>"0"),
                      4=>array("SetName"=>"4", "SetValue"=>"0"),
                      5=>array("SetName"=>"5", "SetValue"=>"0"),
                      6=>array("SetName"=>"6", "SetValue"=>"0"),
                      7=>array("SetName"=>"7", "SetValue"=>"0"),
                      14=>array("SetName"=>"1-2 weeks", "SetValue"=>"0"),
                      28=>array("SetName"=>"2-4 weeks", "SetValue"=>"0"),
                      90=>array("SetName"=>"1-3 months", "SetValue"=>"0"),
                      180=>array("SetName"=>"3-6 months", "SetValue"=>"0"),
                      "FarOff"=>array("SetName"=>"6+ months", "SetValue"=>"0"));
    $AllIsZero = true;
    foreach($DataSetList as $Key => $Value)
    {
        $TempCountValue = intval($Value["SetName"]);
        if($TempCountValue > 0)
        {
            $AllIsZero = false;
        }
        if($TempCountValue <= 7)
        {
            $DataSets[$TempCountValue]["SetValue"] += $Value["SetValue"];
        }
        elseif($TempCountValue <= 14)
        {
            $DataSets[14]["SetValue"] += $Value["SetValue"];
        }
        elseif($TempCountValue <= 28)
        {
            $DataSets[28]["SetValue"] += $Value["SetValue"];
        }
        elseif($TempCountValue <= 90)
        {
            $DataSets[90]["SetValue"] += $Value["SetValue"];
        }
        elseif($TempCountValue <= 180)
        {
            $DataSets[180]["SetValue"] += $Value["SetValue"];
        }
        elseif($TempCountValue > 180)
        {
            $DataSets["FarOff"]["SetValue"] += $Value["SetValue"];
        }
    }
    if($AllIsZero)
    {
        return array();
    }
    else
    {
        return $DataSets;
    }
}

/**
 * Creat data of BugHistorys report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportBugHistorys($ReportCondition = '1')
{
    global $_CFG;
    $TablePrefix = $_CFG['DB']['TablePrefix'];

    $ReportCondition .= " AND {$TablePrefix}BugInfo.BugID = {$TablePrefix}TestAction.IdValue AND {$TablePrefix}TestAction.ActionTarget='Bug' AND {$TablePrefix}BugInfo.BugStatus = 'Closed'";
    $Columns = "BugID,COUNT(*) AS SetName";
    $DataSetList = dbGetList('BugInfo,TestAction',$Columns, $ReportCondition, "{$TablePrefix}TestAction.IdValue", 'SetName ASC', '', 'BugID');

    $DataSets = array();
    foreach($DataSetList as $Data)
    {
        $DataSets[$Data["SetName"]]["SetName"] = $Data["SetName"];
        $DataSets[$Data["SetName"]]["SetValue"] ++;
    }
    $DataSets = reportInterceptDataList($DataSets, 'Limit', '15');

    return $DataSets;
}

/**
 * Creat data of ActivatedBugsPerDay report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportActivatedBugsPerDay($ReportCondition = '1')
{
    global $_CFG;
    $TablePrefix = $_CFG['DB']['TablePrefix'];

    $ReportCondition .= " AND {$TablePrefix}BugInfo.BugID = {$TablePrefix}TestAction.IdValue AND {$TablePrefix}TestAction.ActionTarget='Bug' AND ActionType='Activated'";
    return reportPerDBColoumn('BugInfo,TestAction', "DATE_FORMAT(ActionDate, '%y-%m-%d')", $ReportCondition, '', 'ActionDate ASC');
}

/**
 * Creat data of ActivatedBugsPerWeek report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportActivatedBugsPerWeek($ReportCondition = '1')
{
    global $_CFG;
    $TablePrefix = $_CFG['DB']['TablePrefix'];

    $WeekExp = "Date_FORMAT(Date_SUB(ActionDate, INTERVAL (if(DATE_FORMAT(ActionDate,'%w') = 0,7,DATE_FORMAT(ActionDate, '%w')))-1 DAY), '%y-%m-%d')";
    $ReportCondition .= " AND {$TablePrefix}BugInfo.BugID = {$TablePrefix}TestAction.IdValue AND {$TablePrefix}TestAction.ActionTarget='Bug' AND ActionType='Activated'";
    return reportPerDBColoumn('BugInfo,TestAction', $WeekExp, $ReportCondition, '', 'ActionDate ASC');
}

/**
 * Creat data of ActivatedBugsPerDay report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportActivatedBugsPerMonth($ReportCondition = '1')
{
    global $_CFG;
    $TablePrefix = $_CFG['DB']['TablePrefix'];

    $ReportCondition .= " AND {$TablePrefix}BugInfo.BugID = {$TablePrefix}TestAction.IdValue AND {$TablePrefix}TestAction.ActionTarget='Bug' AND ActionType='Activated'";
    return reportPerDBColoumn('BugInfo,TestAction', "DATE_FORMAT(ActionDate, '%y-%m')", $ReportCondition, '', 'ActionDate ASC');
}


/**********************************************Case Report****************************************/
/**
 * Creat data of CasesPerProject report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerProject($ReportCondition = '1')
{
    return reportPerProject('Case',$ReportCondition);
}

/**
 * Creat data of CasesPerModule  report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerModule($ReportCondition = '1')
{
    return reportPerModule('Case',$ReportCondition);
}

/**
 * Creat data of CasesPerStatus report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Case config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerStatus($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('CaseInfo', 'CaseStatus', $ReportCondition, $_LANG['CaseStatuses']);
}

/**
 * Creat data of CasesPerPriority report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Case config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerPriority($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('CaseInfo', 'CasePriority', $ReportCondition, $_LANG['CasePriorities']);
}


function reportCasesPerType($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('CaseInfo', 'CaseType', $ReportCondition, $_LANG['CaseTypes']);
}

/**
 * Creat data of CasesPerMethod report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Case config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerMethod($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('CaseInfo', 'CaseMethod', $ReportCondition, $_LANG['CaseMethods']);
}

/**
 * Creat data of CasesPerPlan report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Case config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerPlan($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('CaseInfo', 'CasePlan', $ReportCondition, $_LANG['CasePlans']);
}

/**
 * Creat data of OpenedCasesPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Case config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedCasesPerUser($ReportCondition = '1', $Limit = 15)
{
    return reportPerUser('CaseInfo', 'OpenedBy', $ReportCondition, $Limit);
}


/**
 * Creat data of OpenedCasesPerDay report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedCasesPerDay($ReportCondition = '1')
{
    return reportPerDBColoumn('CaseInfo', "DATE_FORMAT(OpenedDate, '%y-%m-%d')", $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of OpenedCasesPerWeek report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedCasesPerWeek($ReportCondition = '1')
{
    $WeekExp = "Date_FORMAT(Date_SUB(OpenedDate, INTERVAL (if(DATE_FORMAT(OpenedDate,'%w') = 0,7,DATE_FORMAT(OpenedDate, '%w')))-1 DAY), '%y-%m-%d')";
    return reportPerDBColoumn('CaseInfo', $WeekExp, $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of OpenedCasesPerMonth report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedCasesPerMonth($ReportCondition = '1')
{
    return reportPerDBColoumn('CaseInfo', "DATE_FORMAT(OpenedDate, '%y-%m')", $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of CasePerScriptStatus report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Case config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasePerScriptStatus($ReportCondition = '1')
{
    global $_LANG;
    unset($_LANG['ScriptStatuses']['']);
    return reportPerDBColoumn('CaseInfo', 'ScriptStatus', $ReportCondition . " AND ScriptStatus <> ''", $_LANG['ScriptStatuses']);
}

/**
 * Creat data of CasesPerScriptedBy report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the bug config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportCasesPerScriptedBy($ReportCondition = '1', $Limit = 15)
{
    return reportPerUser('CaseInfo', 'ScriptedBy', $ReportCondition . " AND ScriptedBy <> ''", $Limit);
}

/**
 * Creat data of ScriptedDatePerDay report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportScriptedDatePerDay($ReportCondition = '1')
{
    return reportPerDBColoumn('CaseInfo', "DATE_FORMAT(ScriptedDate, '%y-%m-%d')", $ReportCondition . " AND ScriptedDate <> ''", '', 'ScriptedDate ASC');
}

/**
 * Creat data of ScriptedDatePerWeek report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportScriptedDatePerWeek($ReportCondition = '1')
{
    $WeekExp = "Date_FORMAT(Date_SUB(ScriptedDate, INTERVAL (if(DATE_FORMAT(ScriptedDate,'%w') = 0,7,DATE_FORMAT(ScriptedDate, '%w')))-1 DAY), '%y-%m-%d')";
    return reportPerDBColoumn('CaseInfo', $WeekExp, $ReportCondition . " AND ScriptedDate <> ''", '', 'ScriptedDate ASC');
}

/**
 * Creat data of ScriptedDatePerMonth report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportScriptedDatePerMonth($ReportCondition = '1')
{
    return reportPerDBColoumn('CaseInfo', "DATE_FORMAT(ScriptedDate, '%y-%m')", $ReportCondition . " AND ScriptedDate <> ''", '', 'ScriptedDate ASC');
}

/**********************************************Result Report****************************************/
/**
 * Creat data of ResultsPerProject report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResultsPerProject($ReportCondition = '1')
{
    return reportPerProject('Result',$ReportCondition);
}

/**
 * Creat data of ResultsPerModule  report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResultsPerModule($ReportCondition = '1')
{
    return reportPerModule('Result',$ReportCondition);
}


/**
 * Creat data of ResultsPerValue report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Result config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResultsPerValue($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('ResultInfo', 'ResultValue', $ReportCondition, $_LANG['ResultValuees']);
}

/**
 * Creat data of ResultsPerStatus report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Result config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResultsPerStatus($ReportCondition = '1')
{
    global $_LANG;
    unset($_LANG['ResultStatuses']['']);
    return reportPerDBColoumn('ResultInfo', 'ResultStatus', $ReportCondition . " AND ResultStatus <> ''", $_LANG['ResultStatuses']);
}
/**
 * Creat data of OpenedResultsPerUser report.
 *
 * @author                              leeyupeng<leeyupeng@gmail.com>
 * @global array                        the Result config array.
 * @global object                       the object of adodb class.
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedResultsPerUser($ReportCondition = '1', $Limit = 15)
{
    return reportPerUser('ResultInfo', 'OpenedBy', $ReportCondition, $Limit);
}

/**
 * Creat data of OpenedResultsPerDay report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedResultsPerDay($ReportCondition = '1')
{
    return reportPerDBColoumn('ResultInfo', "DATE_FORMAT(OpenedDate, '%y-%m-%d')", $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of OpenedResultsPerWeek report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedResultsPerWeek($ReportCondition = '1')
{
    $WeekExp = "Date_FORMAT(Date_SUB(OpenedDate, INTERVAL (if(DATE_FORMAT(OpenedDate,'%w') = 0,7,DATE_FORMAT(OpenedDate, '%w')))-1 DAY), '%y-%m-%d')";
    return reportPerDBColoumn('ResultInfo', $WeekExp, $ReportCondition, '', 'OpenedDate ASC');
}

/**
 * Creat data of OpenedResultsPerMonth report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportOpenedResultsPerMonth($ReportCondition = '1')
{
    return reportPerDBColoumn('ResultInfo', "DATE_FORMAT(OpenedDate, '%y-%m')", $ReportCondition, '', 'OpenedDate ASC');
}


/**
 * Creat data of ResultsPerOS report
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResultsPerOS($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('ResultInfo', 'ResultOS', $ReportCondition, $_LANG['ResultOS']);
}

/**
 * Creat data of ResultsPerBrowser report.
 *
 * @author                              Yupeng Lee <leeyupeng@gmail.com>
 * @param  string   $ReportCondition    the query condition of report.
 * @return array                        data set.
 */
function reportResultsPerBrowser($ReportCondition = '1')
{
    global $_LANG;
    return reportPerDBColoumn('ResultInfo', 'ResultBrowser',$ReportCondition, $_LANG['ResultBrowser']);
}


function reportSetFullTypeInfo($DataSetList, $FullTypeArray, $SetEmpty = false, $EmptyValue = "")
{
    global $_LANG;
    foreach($FullTypeArray as $Key => $Value)
    {
        $Value = str_replace("'", "%26apos;", $Value);
        $Value = str_replace(">", "%26gt;", $Value);
        $Value = str_replace("<", "%26lt;", $Value);
        $Value = str_replace("&", "%26", $Value);
        if($DataSetList[$Key]['SetValue'] > 0)
        {
            $DataSetList[$Key]["SetName"] = $Value;
        }
        else
        {
            if($SetEmpty)
            {
                $DataSetList[$Key]["SetName"] = $Value;
                $DataSetList[$Key]["SetValue"] = '';
            }
        }
    }
    if($DataSetList['']['SetValue'] > 0)
    {
        $DataSetList['']['SetName'] = $_LANG['Blank'];
    }
    return $DataSetList;
}

function reportCreateSingleXmlStr($DataSetList, $GraphOption=array(), $DisplayNullValue = true, $ShowByRange = true)
{
    global $FC_ColorCounter;

    if($ShowByRange)
    {
        $MaxRange = 26;
        $DataSetCount = count($DataSetList);
        $RangeValue = floor($DataSetCount/$MaxRange);
        if($MaxRange >= $DataSetCount)
        {
            $ShowByRange = false;
        }
    }

    $XmlStr = '<graph ';
    foreach($GraphOption as $Key => $Value)
    {
        $XmlStr .= "{$Key}='{$Value}' ";
    }
    if($ShowByRange)
    {
        $XmlStr .= "showValues = '0'";
    }
    $XmlStr .= '>';
    $Num = 0;
    foreach($DataSetList as $Name => $Value)
    {
        $Color = getFCColor();
        if(!empty($Value[SetValue])||$DisplayNullValue)
        {
            if($ShowByRange && $RangeValue > 0 && $Num%$RangeValue != 0)
            {
                $ShowVN = " showValue= '0' showName='0'";
            }
            else
            {
                $ShowVN = "";
            }
            $XmlStr .= "<set name='{$Value[SetName]}' value='{$Value[SetValue]}' color='{$Color}' {$ShowVN}/>";
        }
        $Num ++;
    }

    $XmlStr .= '</graph>';
    $FC_ColorCounter = 0;

    return $XmlStr;
}

function reportInterceptDataList($DataSetList, $InterceptType, $InterceptValue)
{
    global $_LANG;
    $LangOther = $_LANG['Others'];
    $DataSetListSum = 0;
    foreach($DataSetList as $Key => $Value)
    {
        $DataSetListSum += $Value['SetValue'];
    }
    $NewDataSetList = array();

    if($InterceptType == 'Limit' && count($DataSetList) > $InterceptValue)
    {
        foreach($DataSetList as $Key => $Value)
        {
            if($InterceptValue > 1)
            {
                $NewDataSetList[$Key] = $Value;
                $DataSetListSum -= $Value['SetValue'];
                $InterceptValue --;
            }
            else
            {
                $NewDataSetList['Others'] = array('SetName'=>$LangOther, 'SetValue' =>$DataSetListSum);;
                break;
            }
        }
    }
    elseif($InterceptType == 'Percent')
    {
        $CriticalValue = floor($DataSetListSum * $InterceptValue / 100);
        foreach($DataSetList as $Key => $Value)
        {
            if($DataSetListSum > $CriticalValue)
            {
                $NewDataSetList[$Key] = $Value;
                $DataSetListSum -= $Value['SetValue'];
                $InterceptValue --;
            }
            else
            {
                $NewDataSetList['Others'] = array('SetName'=>$LangOther, 'SetValue' =>$DataSetListSum);;
                break;
            }
        }
    }
    else
    {
        $NewDataSetList = $DataSetList;
    }
    return $NewDataSetList;
}

function reportCreatePieNoteStr($DataSetList, $GraphOption=array())
{
    global $FC_ColorCounter;

    $CycleColor = array('#EEEEEE','#F9F9F9');
    $NoteStr = '<center><table class="CommonTable ListTable BugMode" style="border:0;width:50%">';
    foreach($DataSetList as $Name => $Value)
    {
        if(empty($Value[SetValue]))
        {
            $Value[SetValue] = 0;
        }
        $Color = getFCColor();
        $Cycle = $FC_ColorCounter % 2;
        $NoteStr .= <<<EOT
<tr style='background-color:{$CycleColor[$Cycle]};'>
  <td valign="middle" align="center" width="15" height="10px">
  <div style="background-color:{$Color};width:8px;height:8px;font-size:8px;">&nbsp;</div>
  </td>
  <td valign="middle" align="center" width="5" height="10px">&nbsp;</td>
  <td>{$Value[SetName]}</td>
  <td align="right">{$Value[SetValue]}</td>
</tr>
EOT;
    }

    $NoteStr .= '</table></center><br />';
    $FC_ColorCounter = 0;

    return $NoteStr;
}
?>
