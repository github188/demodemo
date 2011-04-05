{include file="Header.tpl"}
<body id="BGColor" onload="this.focus();initShowGotoBCR();">
  <div id="TopNavMain">
    <div id="TopNavLogo">
      <a href="./" target="_top"><img src="Image/logo.png" title={$Lang.ProductName} /></a>
    </div>
    <div id="TopNavAbout">
      <span>{$Lang.Welcome},</span>
      <span id="UserName">{$TestRealName}</span>
      <a id="EditMyInfo" href="EditMyInfo.php" target="_blank">{$Lang.EditPer}</a>{if $TestIsAdmin || $TestIsProjectAdmin}
      <a href="Admin/" target="_blank">{$Lang.Admin}</a>{/if}
      <a href="Logout.php?Logout=Yes">{$Lang.Logout}</a>
      <a href="{$Lang.BFHomePage}/help" target="_blank">{$Lang.Manual}</a>
      <a href="{$Lang.BFHomePage}" target="_blank">{$Lang.BFHome}</a>
    </div>
    <div id="ProjectList">{$TopNavProjectList}</div>
    <div id="TopNavButton">
    <ul>
      <li id="TopNavBugLi" class="Cram{if $TestMode eq 'Bug'} Active{/if}"><a href="index.php?TestMode=Bug" id="TopNavBug" target="_top"><span class="BugMenuButton">Bug</span></a></li>
      <li id="TopNavCaseLi" class="Cram{if $TestMode eq 'Case'} Active{/if}"><a href="index.php?TestMode=Case" id="TopNavCase" target="_top"><span class="BugMenuButton">Test Case</span></a></li>
      <li id="TopNavResultLi" class="Cram{if $TestMode eq 'Result'} Active{/if}"><a href="index.php?TestMode=Result" id="TopNavResult" target="_top"><span class="BugMenuButton">Test Result</span></a></li>
    </ul>
    </div>
    <div id="Open">
      <a href="Bug.php?ActionType=OpenBug" id="OpenBug" class="BigButton OpenBug" target="_blank" {if $TestMode neq 'Bug'}style="display:none;"{/if}>
        {$Lang.OpenBug}
      </a>
      <a href="Case.php?ActionType=OpenCase" id="OpenCase" class="BigButton OpenCase" target="_blank"{if $TestMode neq 'Case'} style="display:none;"{/if}>
        {$Lang.OpenCase}
      </a>
    </div>
  </div>
</body>
</html>