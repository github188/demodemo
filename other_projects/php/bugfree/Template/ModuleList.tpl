{include file="Header.tpl"}
  {literal}
  <script>
  function resizeList()
  {
       var h1 = document.body.clientHeight;
       var h2 = document.documentElement.clientHeight;
       var h3 = (h2>h1)?h2:h1;
       var hsub = 8;
       xajax.$('ModuleListFE').style.height = h2-hsub + 'px';
       xajax.$('ModuleTree').style.height = h2-hsub + 'px';
  }
  </script>
  {/literal}
<body class="LMode"  onResize="resizeList();" onload="initShowGotoBCR();resizeList();">
  <link href="Css/TreeMenu.css" rel="stylesheet" type="text/css">
  <script language="JavaScript" src="JS/TreeMenu.js" type="text/javascript"></script>
  <div id="ModuleListFE">
    <div id="ModuleTree">
    {$ModuleTree}
    </div>
  </div>
  <script type="text/javascript">createTreeMenu("ModuleListFE");</script>
</body>
</html>