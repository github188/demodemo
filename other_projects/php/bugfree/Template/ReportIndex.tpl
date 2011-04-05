{include file="ReportHeader.tpl"}
<frameset rows="34,*" name="IndexFrame" id="IndexFrame" frameborder="no" border="0" framespacing="0">
<frame src="ReportTop.php?ReportMode={$ReportMode}" name="NavFrame" id="NavFrame" scrolling="no">
<frameset cols="26%,*" name="MainFrame" id="MainFrame" frameborder="no" border="0" framespacing="0">
    <frame src="ReportLeft.php?ReportMode={$ReportMode}"  name="LeftFrame"    id="LeftFrame"    scrolling="auto" noresize>
    <frame src="ReportRight.php?ReportMode={$ReportMode}" name="RightFrame" id="RightFrame" scrolling="auto" noresize>
  </frameset>
</frameset>
</frameset>
<noframes>
  <body>
    {$TplConfig.Index.NotSupportFrame}
  </body>
</noframes>
</html>
