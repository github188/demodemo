<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type"     content="text/html; charset={$Lang.Charset}">
<meta http-equiv="Content-Language" content="{$Lang.Charset}">
<style type="text/css">
{$CssStyle}
</style>
<title>{$Lang.ProductName}</title>
</head>
<body style="margin:16px;">
 <table width="98%" align="center" class="CommonTable BugMode">
    <caption style="text-align:left">{$Lang.NotifyEmailPoject}:{$ProjectName}</caption>
    <tr>
      <th>{$Lang.NotifyEmailModule}</th>
      <th>{$Lang.NotifyEmailBugStatistics}</th>
      <th>{$Lang.BugStatus.Active}</th>
      <th>{$Lang.BugStatus.Resolved}</th>
      <th>{$Lang.BugStatus.Closed}</th>
      <th>{$Lang.TotalCount}</th>
    </tr>
    {foreach from=$StatisticsThisWeek key=Key item=StatInfo}
    <tr>
      <th align="center" rowspan='3'>{$StatisticsThisWeek[$Key].ModuleName}</th>
      <td align="center">{$StatisticsThisWeek[$Key].Title}</td>
      <td align="center">
        {if $StatisticsThisWeek[$Key].BugOfThisWeekAciveTotal eq 0}
        {else}
        <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsThisWeek[$Key].ModuleID}&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekAciveTotalLink}">
            {$StatisticsThisWeek[$Key].BugOfThisWeekAciveTotal}
        </a>
        {/if}
     </td>
      <td align="center">
        {if $StatisticsThisWeek[$Key].BugOfThisWeekResolvedTotal eq 0}
        {else}
        <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsThisWeek[$Key].ModuleID}&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekResolvedTotalLink}">
           {$StatisticsThisWeek[$Key].BugOfThisWeekResolvedTotal}
        </a>
        {/if}
      </td>
      <td align="center">
        {if $StatisticsThisWeek[$Key].BugOfThisWeekClosedTotal eq 0}
        {else}
         <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsThisWeek[$Key].ModuleID}&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekClosedTotalLink}">
            {$StatisticsThisWeek[$Key].BugOfThisWeekClosedTotal}
         </a>
        {/if}
      </td>
      <td align="center">
        {if $StatisticsThisWeek[$Key].BugOfThisWeekTotal eq 0}
        {else}
         <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsThisWeek[$Key].ModuleID}&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekTotalLink}">
            {$StatisticsThisWeek[$Key].BugOfThisWeekTotal}
         </a>
        {/if}
     </td>
    </tr>
    <tr>
      <td align="center" >{$StatisticsNotOperateOverWeek[$Key].Title}</td>
      <td align="center">
        {if $StatisticsNotOperateOverWeek[$Key].BugOfOverWeekAciveTotal eq 0}
        {else}
         <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsNotOperateOverWeek[$Key].ModuleID}&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekAciveTotalLink}">
              {$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekAciveTotal}
         </a>
        {/if}
      </td>
      <td align="center">
        {if $StatisticsNotOperateOverWeek[$Key].BugOfOverWeekResolvedTotal eq 0}
        {else}
         <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsNotOperateOverWeek[$Key].ModuleID}&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekResolvedTotalLink}">
              {$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekResolvedTotal}
         </a>
        {/if}
     </td>
      <td align="center">
        {if $StatisticsNotOperateOverWeek[$Key].BugOfOverWeekClosedTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsNotOperateOverWeek[$Key].ModuleID}&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekClosedTotalLink}">
               {$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekClosedTotal}
          </a>
        {/if}
       </td>
      <td align="center">
       {if $StatisticsNotOperateOverWeek[$Key].BugOfOverWeekTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsNotOperateOverWeek[$Key].ModuleID}&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekTotalLink}">
               {$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekTotal}
          </a>
         {/if}
     </td>
    </tr>
    <tr>
      <td align="center" >{$StatisticsTotal[$Key].Title}</td>
      <td align="center">
        {if $StatisticsTotal[$Key].BugOfAciveTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsTotal[$Key].ModuleID}&Condition={$StatisticsTotal[$Key].BugOfAciveTotalLink}">
                {$StatisticsTotal[$Key].BugOfAciveTotal}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $StatisticsTotal[$Key].BugOfResolvedTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsTotal[$Key].ModuleID}&Condition={$StatisticsTotal[$Key].BugOfResolvedTotalLink}">
                {$StatisticsTotal[$Key].BugOfResolvedTotal}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $StatisticsTotal[$Key].BugOfClosedTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsTotal[$Key].ModuleID}&Condition={$StatisticsTotal[$Key].BugOfClosedTotalLink}">
                {$StatisticsTotal[$Key].BugOfClosedTotal}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $StatisticsTotal[$Key].BugOfTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID={$StatisticsTotal[$Key].ModuleID}&Condition={$StatisticsTotal[$Key].BugOfTotalLink}">
                {$StatisticsTotal[$Key].BugOfTotal}
          </a>
        {/if}
      </td>
    </tr>
       {/foreach}
      <tr>
    <th align="center" rowspan='3'>{$Lang.TotalCount}</th>
      <td align="center" >{$StatisticsThisWeek[$Key].Title}</td>
      <td align="center">
        {if $ProjectThisWeekActive  eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekAciveTotalLink}">
                {$ProjectThisWeekActive}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectThisWeekResolved eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekResolvedTotalLink}">
               {$ProjectThisWeekResolved}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectThisWeekClosed eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekClosedTotalLink}">
                {$ProjectThisWeekClosed}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectThisWeekTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsThisWeek[$Key].BugOfThisWeekTotalLink}">
                {$ProjectThisWeekTotal}
          </a>
        {/if}
      </td>
    </tr>

 <tr>

      <td align="center" >{$StatisticsNotOperateOverWeek[$Key].Title}</td>
      <td align="center">
        {if $ProjectOverWeekActive  eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekAciveTotalLink}">
                {$ProjectOverWeekActive}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectOverWeekResolved eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekResolvedTotalLink}">
               {$ProjectOverWeekResolved}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectOverWeekClosed eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekClosedTotalLink}">
                {$ProjectOverWeekClosed}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectOverWeekTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsNotOperateOverWeek[$Key].BugOfOverWeekTotalLink}">
                {$ProjectOverWeekTotal}
          </a>
        {/if}
      </td>
    </tr>
 <tr>
      <td align="center" >{$StatisticsTotal[$Key].Title}</td>
      <td align="center">
        {if $ProjectTotalActive  eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsTotal[$Key].BugOfAciveTotalLink}">
                {$ProjectTotalActive}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectTotalResolved eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsTotal[$Key].BugOfResolvedTotalLink}">
               {$ProjectTotalResolved}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectTotalClosed eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsTotal[$Key].BugOfClosedTotalLink}">
                {$ProjectTotalClosed}
          </a>
        {/if}
      </td>
      <td align="center">
        {if $ProjectTotal eq 0}
        {else}
          <a href="{$BaseUrl}/BugList.php?ActionType=FromNotifyEmail&ProjectID={$ProjectID}&TopModuleID=-1&Condition={$StatisticsTotal[$Key].BugOfTotalLink}">
                {$ProjectTotal}
          </a>
        {/if}
      </td>
    </tr>
   
  
  </table>
<br>
</body>
</html>
