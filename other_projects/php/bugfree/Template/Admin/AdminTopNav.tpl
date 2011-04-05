  <div id="TopNavMain" class="AdminMode">
    <a id="TopNavLogo" href="../"><img src="../Image/logo.png" title={$Lang.ProductName} /></a>
    <div id="AdminTopNavButton">
    <ul>
      <li class="Cram{php}if(preg_match('/\/(AdminProject)/', $_SERVER['REQUEST_URI']) || preg_match('/\/(AdminModule)/', $_SERVER['REQUEST_URI'])) echo ' Active';{/php}"><a href="AdminProjectList.php" class="AdminButton"  target="_top"><span class="BugMenuButton">{$Lang.ManageProject}</span></a></li>
      <li class="Cram{php}if(preg_match('/\/(AdminUser)/', $_SERVER['REQUEST_URI']) && !preg_match('/\/(AdminUserLog)/', $_SERVER['REQUEST_URI'])) echo ' Active';{/php}"><a href="AdminUserList.php"  class="AdminButton" target="_top"><span class="BugMenuButton">{$Lang.ManageUser}</span></a></li>
      <li class="Cram{php}if(preg_match('/\/(AdminGroup)/', $_SERVER['REQUEST_URI'])) echo ' Active';{/php}"><a href="AdminGroupList.php"  class="AdminButton" target="_top"><span class="BugMenuButton">{$Lang.ManageGroup}</span></a></li>
      {if $templatelite.session.TestIsAdmin}<li class="Cram{php}if(preg_match('/\/(AdminUserLog)/', $_SERVER['REQUEST_URI'])) echo ' Active';{/php}"><a href="AdminUserLogList.php" class="AdminButton" target="_top"><span class="BugMenuButton">{$Lang.UserLog}</span></a></li>{/if}
    </ul>
    </div>
    <span id="TopNavAbout">
      <big>{$Lang.Admin}</big>
    </span>
  </div>
