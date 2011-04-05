{include file="Header.tpl"}
{literal}
<style>
#TopNavLogo {
font-size: 45px;
}
table.CommonTable td {font-size:18px;padding:20px;}
</style>
{/literal}
<body class="PaddingBoy BugMode">
<div style="position:absolute;margin:-182px 0px 0px -290px;top:45%;left:45%;z-index:100;">
  <div id="TopNavMain" style="width:700px;height:80px">
    <span id="TopNavLogo">
{$Lang.InstallBugFree}
    </span>
  </div>
  <br />
{if $Step eq 2}
  {if $ErrorMsg}
    <table class="CommonTable ListTable" style="width:700px">
      <tr>
        <td><strong><i>Errror</i></strong></td>
      </tr>
      {foreach from=$ErrorMsg item=Msg}
      <tr>
        <td>{$Msg}</td>
      </tr>
      {/foreach}
      <tr>
        <td align="center"><input type="button" value=" Back " onclick="history.go(-1);" /></td>
      </tr>
    </table>
  <br />
  {else}
    <table class="CommonTable ListTable" style="width:700px">
      <tr>
        <td>
        {if $InstallOrUpgrade eq 'Install'}
        {$Lang.InstallSuccessed}
        {else}
        {$Lang.UpgradeSuccessed}{/if}</td>
      </tr>
    </table>
  {/if}
{else}
<table class="CommonTable ListTable" style="width:700px;font-size:18px;">
 {if $CurrentDBVersion eq '1'}
  <tr>
    <td><a href="?Step=2&UpgradeFrom=1" target="_self">{$Lang.UpgradeBugFreeFrom1}</a></td>
  </tr>
  <tr>
    {if $BugFree2Exists}
    <td><a href="?Step=2" target="_self">{$Lang.UpgradeBugFreeFrom2}</a></td>
    {else}
    <td><a href="?Step=2&NewInstall=1" target="_self">{$Lang.InstallNewBugFree}</a></td>
    {/if}
  </tr>
  {else}
  <tr>
    {if $BugFree2Exists}
    <td><a href="?Step=2" target="_self">{$Lang.UpgradeBugFreeFrom2}</a></td>
    {else}
    <td><a href="?Step=2&NewInstall=1" target="_self">{$Lang.InstallNewBugFree}</a></td>
    {/if}
  </tr>
  {/if}
  </tr>
</table>
{/if}
<br />
<center><a href="{$Lang.BFHomePage}" target="_blank" style="font-size:14px">{$Lang.ProductName} {$Lang.BFHome}</a></center>
</div>
</body>
</html>
