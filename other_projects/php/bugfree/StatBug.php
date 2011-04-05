<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * bug stat info.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("Include/Init.inc.php");

set_time_limit(0);

$Today         = date("Y-m-d", time());
$Tomorrow         = date("Y-m-d", time() + 24 * 3600 );
$OneWeekBefore = date("Y-m-d",time() - 24 * 3600 * 6);


$Columns = 'ProjectID, ProjectName, NotifyEmail';
$ProjectList = dbGetList('TestProject', $Columns); //所有项目
$TestProjectEmailList = dbGetList('TestProject', 'NotifyEmail');
$NotifyEmailArray = array();
foreach($TestProjectEmailList as $Item)
{
    array_push($NotifyEmailArray, $Item['NotifyEmail']);
}
$NotifyEmailArray = array_unique(explode(',',implode(',',$NotifyEmailArray))); //保存了所有的NotifyEmail，且去重


foreach($NotifyEmailArray as $NotifyEmailItem)
{//对项目对应的邮件循环
    $StatContent = '';
    foreach($ProjectList as $Item) //项目循环
    {
        if($NotifyEmailItem && in_array($NotifyEmailItem, explode(',',$Item['NotifyEmail'])))
        {      
        $ProjectThisWeekActive   = 0;
        $ProjectThisWeekResolved = 0;
        $ProjectThisWeekClosed = 0;
        $ProjectThisWeekTotal = 0;

        $ProjectOverWeekActive = 0;
        $ProjectOverWeekResolved = 0;
        $ProjectOverWeekClosed = 0;
        $ProjectOverWeekTotal = 0;

        $ProjectTotalActive = 0;
        $ProjectTotalResolved = 0;
        $ProjectTotalClosed = 0;
        $ProjectTotal = 0;

            $ProjectID = $Item['ProjectID'];
            $Columns = 'ModuleID, ModuleName';
            $TopModuleList = dbGetList('TestModule', $Columns, "ProjectID = {$ProjectID} AND ParentID=0 And ModuleType='Bug'"); //获取一级Module
            $ChildModuleList = testGetProjectModuleList($ProjectID, 'Bug');//获取所有Module，以及父子关系

            $ModuleIDsArray = array(); //该模块ID => 该模块的所有子模块ID
            $ModuleIDsArray[0] = array('ChildModuleIDs' => 0, 'ModuleName' => '/'); //增加‘/'模块
            foreach($TopModuleList as $TopMudule)
               $ModuleIDsArray[$TopMudule['ModuleID']] = array('ChildModuleIDs' => $ChildModuleList[$TopMudule['ModuleID']]['ChildIDs'], 'ModuleName' => $TopMudule['ModuleName']);


            //统计七天内新建的bug-------------------------------------------------------------------------------------------------
            $StatisticsThisWeekArray = array(); //上周所有模块的统计数组
            $StatisticsThisWeek = array(); //上周单个模块的统计
            foreach($ModuleIDsArray as $Key => $Value)
            {
               $BugOfThisWeekAciveTotal=0;
               $BugOfThisWeekResolvedTotal=0;
               $BugOfThisWeekClosedTotal=0;
               $BugOfThisWeekTotal=0;
               $BugOfThisWeek = dbGetList('BugInfo', 'BugStatus', "OpenedDate < '{$Tomorrow}' AND OpenedDate >= '{$OneWeekBefore}' AND ModuleID in ({$Value[ChildModuleIDs]}) AND ProjectID={$ProjectID}");
               foreach($BugOfThisWeek as $BugItem)
               {
                  switch($BugItem['BugStatus'])
                  {
                      case 'Active':
                            $BugOfThisWeekAciveTotal += 1;
                            $BugOfThisWeekTotal += 1;
                           
                            break;
                      case 'Resolved':
                            $BugOfThisWeekResolvedTotal += 1;
                            $BugOfThisWeekTotal += 1;
                            break;
                      case 'Closed':
                            $BugOfThisWeekClosedTotal += 1;
                            $BugOfThisWeekTotal += 1;
                            break;
                   }
               }
               
               $StatisticsThisWeek['Title'] = $_LANG['StatisticsThisWeek'] . '(' .$OneWeekBefore .' - '. $Today .')';
               $StatisticsThisWeek['BugOfThisWeekAciveTotal'] = $BugOfThisWeekAciveTotal;
               $StatisticsThisWeek['BugOfThisWeekAciveTotalLink'] = "OpenedDate<'{$Tomorrow}' AND OpenedDate>='{$OneWeekBefore}' AND BugStatus='Active'";
               $StatisticsThisWeek['BugOfThisWeekResolvedTotal'] = $BugOfThisWeekResolvedTotal;
               $StatisticsThisWeek['BugOfThisWeekResolvedTotalLink'] = "OpenedDate<'{$Tomorrow}' AND OpenedDate>='{$OneWeekBefore}' AND BugStatus='Resolved'";
               $StatisticsThisWeek['BugOfThisWeekClosedTotal'] = $BugOfThisWeekClosedTotal;
               $StatisticsThisWeek['BugOfThisWeekClosedTotalLink'] = "OpenedDate<'{$Tomorrow}' AND OpenedDate>='{$OneWeekBefore}' AND BugStatus='Closed'";
               $StatisticsThisWeek['BugOfThisWeekTotal'] = $BugOfThisWeekTotal;
               $StatisticsThisWeek['BugOfThisWeekTotalLink'] = "OpenedDate<'{$Tomorrow}' AND OpenedDate>='{$OneWeekBefore}'";
               $StatisticsThisWeek['ModuleName'] =$Value['ModuleName'];
               $StatisticsThisWeek['ModuleID'] =$Key;
               $ProjectThisWeekActive +=   $StatisticsThisWeek['BugOfThisWeekAciveTotal'];
                $ProjectThisWeekResolved +=  $StatisticsThisWeek['BugOfThisWeekResolvedTotal'];
                $ProjectThisWeekClosed += $StatisticsThisWeek['BugOfThisWeekClosedTotal'];
                $ProjectThisWeekTotal += $StatisticsThisWeek['BugOfThisWeekTotal'];
               

               array_push($StatisticsThisWeekArray,$StatisticsThisWeek);
               unset($StatisticsThisWeek);
            }
            //统计超过七天未处理bug-------------------------------------------------------------------------------------------------
            $StatisticsNotOperateOverWeekArray = array(); //超过一周单个模块的统计
            $StatisticsNotOperateOverWeek = array(); //超过一周所有模块的统计数组
            foreach($ModuleIDsArray as $Key => $Value)
            {
                $BugOfOverWeekAciveTotal=0;
                $BugOfOverWeekResolvedTotal=0;
                $BugOfOverWeekClosedTotal=0;
                $BugOfOverWeekTotal=0;
                $BugOverWeekTotal = dbGetList('BugInfo', 'BugStatus', "LastEditedDate < '{$OneWeekBefore}' AND BugStatus != 'Closed' AND IsDroped <> 1
                                    AND ModuleID in ({$Value[ChildModuleIDs]}) AND ProjectID={$ProjectID}");
                foreach($BugOverWeekTotal as $BugItem)
                {
                    switch($BugItem['BugStatus'])
                    {
                       case 'Active':
                             $BugOfOverWeekAciveTotal += 1;
                             $BugOfOverWeekTotal += 1;
                             break;
                       case 'Resolved':
                             $BugOfOverWeekResolvedTotal += 1;
                             $BugOfOverWeekTotal += 1;
                             break;
                       case 'Closed':
                             $BugOfOverWeekClosedTotal += 1;
                             $BugOfOverWeekTotal += 1;
                             break;
                     }
                 }
                 $StatisticsNotOperateOverWeek['Title'] = $_LANG['StatisticsNotOperateOverWeek'] . '(' .' < '. $OneWeekBefore .')';
                 $StatisticsNotOperateOverWeek['BugOfOverWeekAciveTotal'] = $BugOfOverWeekAciveTotal;
                 $StatisticsNotOperateOverWeek['BugOfOverWeekAciveTotalLink'] = "LastEditedDate<'{$OneWeekBefore}' AND BugStatus!='Closed' AND IsDroped<>1 AND BugStatus='Active'";
                 $StatisticsNotOperateOverWeek['BugOfOverWeekResolvedTotal'] = $BugOfOverWeekResolvedTotal;
                 $StatisticsNotOperateOverWeek['BugOfOverWeekResolvedTotalLink'] = "LastEditedDate<'{$OneWeekBefore}' AND BugStatus!='Closed' AND IsDroped<>1 AND BugStatus='Resolved'";
                 $StatisticsNotOperateOverWeek['BugOfOverWeekClosedTotal'] = $BugOfOverWeekClosedTotal;
                 $StatisticsNotOperateOverWeek['BugOfOverWeekClosedTotalLink'] = "LastEditedDate<'{$OneWeekBefore}' AND BugStatus!='Closed' AND IsDroped<>1 AND BugStatus='Closed'";
                 $StatisticsNotOperateOverWeek['BugOfOverWeekTotal'] = $BugOfOverWeekTotal;
                 $StatisticsNotOperateOverWeek['BugOfOverWeekTotalLink'] = "LastEditedDate<'{$OneWeekBefore}' AND BugStatus!='Closed' AND IsDroped<>1";
                 $StatisticsNotOperateOverWeek['ModuleName'] =$Value['ModuleName'];
                 $StatisticsNotOperateOverWeek['ModuleID'] =$Key;
                 $ProjectOverWeekActive +=   $StatisticsNotOperateOverWeek['BugOfOverWeekAciveTotal'];
                 $ProjectOverWeekResolved +=  $StatisticsNotOperateOverWeek['BugOfOverWeekResolvedTotal'];
                 $ProjectOverWeekClosed += $StatisticsNotOperateOverWeek['BugOfOverWeekClosedTotal'];
                 $ProjectOverWeekTotal += $StatisticsNotOperateOverWeek['BugOfOverWeekTotal'];

               

                 array_push($StatisticsNotOperateOverWeekArray,$StatisticsNotOperateOverWeek);
                 unset($StatisticsNotOperateOverWeek);
            }

           
            //统计所有-------------------------------------------------------------------------------------------------
            $StatisticsTotalArray = array();//所有模块的统计数组
            $StatisticsTotal = array();//单个模块的统计
            foreach($ModuleIDsArray as $Key => $Value)
            {
               $BugOfAciveTotal=0;
               $BugOfResolvedTotal=0;
               $BugOfClosedTotal=0;
               $BugOfTotal=0;
               $BugTotal = dbGetList('BugInfo', 'BugStatus', " ModuleID in ({$Value[ChildModuleIDs]}) AND ProjectID={$ProjectID}");
              
               foreach($BugTotal as $BugItem)
               {
                  switch($BugItem['BugStatus'])
                  {
                     case 'Active':
                          $BugOfAciveTotal += 1;
                          $BugOfTotal += 1;
                          break;
                     case 'Resolved':
                          $BugOfResolvedTotal += 1;
                          $BugOfTotal += 1;
                          break;
                     case 'Closed':
                          $BugOfClosedTotal += 1;
                          $BugOfTotal += 1;
                          break;
                     }
                }

                
                $StatisticsTotal['Title'] = $_LANG['StatisticsTotal'];
                $StatisticsTotal['BugOfAciveTotal'] = $BugOfAciveTotal;
                $StatisticsTotal['BugOfAciveTotalLink'] = "BugStatus='Active'";
                $StatisticsTotal['BugOfResolvedTotal'] = $BugOfResolvedTotal;
                $StatisticsTotal['BugOfResolvedTotalLink'] = "BugStatus='Resolved'";
                $StatisticsTotal['BugOfClosedTotal'] = $BugOfClosedTotal;
                $StatisticsTotal['BugOfClosedTotalLink'] = "BugStatus='Closed'";
                $StatisticsTotal['BugOfTotal'] = $BugOfTotal;
                $StatisticsTotal['BugOfTotalLink'] = "1 ";
                $StatisticsTotal['ModuleName'] =$Value['ModuleName'];
                $StatisticsTotal['ModuleID'] =$Key;
                $ProjectTotalActive +=   $StatisticsTotal['BugOfAciveTotal'];
                $ProjectTotalResolved +=  $StatisticsTotal['BugOfResolvedTotal'];
                $ProjectTotalClosed += $StatisticsTotal['BugOfClosedTotal'];
                $ProjectTotal += $StatisticsTotal['BugOfTotal'];

               
                array_push($StatisticsTotalArray,$StatisticsTotal);
                unset($StatisticsTotal);
            }
            
            $CssStyle = join("",file($_CFG['RealRootPath'] . "/Css/Mail.css"));
            $TPL->assign('CssStyle',$CssStyle);
            $TPL->assign('ProjectName',$Item['ProjectName']);
            $TPL->assign('ProjectID',$Item['ProjectID']);
            $TPL->assign('StatisticsThisWeek',$StatisticsThisWeekArray);
            $TPL->assign('StatisticsTotal',$StatisticsTotalArray);
            $TPL->assign('StatisticsNotOperateOverWeek',$StatisticsNotOperateOverWeekArray);
            
            $TPL->assign('ModuleCount',count($ModuleIDsArray));

            
            $TPL->assign('ProjectTotalActive',$ProjectTotalActive);
            $TPL->assign('ProjectTotalResolved',$ProjectTotalResolved);
            $TPL->assign('ProjectTotalClosed',$ProjectTotalClosed);
            $TPL->assign('ProjectTotal',$ProjectTotal);
            $TPL->assign('ProjectOverWeekActive',$ProjectOverWeekActive);
            $TPL->assign('ProjectOverWeekResolved',$ProjectOverWeekResolved);
            $TPL->assign('ProjectOverWeekClosed',$ProjectOverWeekClosed);
            $TPL->assign('ProjectOverWeekTotal',$ProjectOverWeekTotal);
            $TPL->assign('ProjectThisWeekActive',$ProjectThisWeekActive);
            $TPL->assign('ProjectThisWeekResolved',$ProjectThisWeekResolved);
            $TPL->assign('ProjectThisWeekClosed',$ProjectThisWeekClosed);
            $TPL->assign('ProjectThisWeekTotal',$ProjectThisWeekTotal);



            if($_CFG['BrowserMode'])
            {
                $BaseUrl = $_CFG["BaseURL"];
            }
            else
            {
                $BaseUrl = $argv[1];
                $CssStyle = `pwd` .  "/Css/Mail.css";
            }
            if($BaseUrl == '')
            {
                die('Wrong Baseurl');
            }
            $TPL->assign("BaseUrl", $BaseUrl);
            $StatContent .= $TPL->fetch('StatBug.tpl');            
            unset($StatisticsTotalArray);
            unset($StatisticsThisWeekArray);
            unset($StatisticsNotOperateOverWeekArray);
        }
    }//end 项目循环
   


 // echo $StatContent;
    @sysMail($NotifyEmailItem,'','Bug ' . $_LANG['ReportForms'], $StatContent);

}//end NotifyEmail循环
?>
