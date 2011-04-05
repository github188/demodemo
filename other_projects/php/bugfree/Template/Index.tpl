{include file="Header.tpl"}
<frameset rows="52,*,10" name="IndexFrame" id="IndexFrame" frameborder="no" border="0" framespacing="0">
<frame src="TopNav.php" name="NavFrame" id="NavFrame" scrolling="no">
<frameset cols="{if $CloseLeftFrame}0{else}260{/if},15,*" name="MainFrame" id="MainFrame" frameborder="no" border="0" framespacing="0">
  <frameset rows="*,230"  name="LeftFrame" id="LeftFrame" frameborder="no" border="0" framespacing="0">
    <frame src="ModuleList.php"  name="LeftTopFrame"    id="LeftTopFrame"    scrolling="no" noresize>
    <frame src="UserControl.php?TestMode={$TestMode}" name="LeftBottomFrame" id="LeftBottomFrame" scrolling="no"   noresize>
  </frameset>
  <frame src="ControlFrame.php" name="ControlFrame" id="ControlFrame" scrolling="no" border="0" noresize>
  <frameset rows="145,*" cols="*"  name="RightFrame"       id="RightFrame"    framespacing="0" frameborder="no" border="0">
    <frame src="Search{$TestMode}.php"  name="RightTopFrame"    id="RightTopFrame" scrolling="no" noresize>
    <frame src="{$TestMode}List.php" name="RightBottomFrame" id="RightBottomFrame" scrolling="no" noresize>
  </frameset>
</frameset>
<frame src="Footer.php" scrolling="no" noresize>
</frameset>
<noframes>
  <body>
    {$TplConfig.Index.NotSupportFrame}
  </body>
</noframes>
</html>
