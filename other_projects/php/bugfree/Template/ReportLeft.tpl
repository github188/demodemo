{include file="ReportHeader.tpl"}
<body>
<link href="Css/TreeMenu.css" rel="stylesheet" type="text/css">
<script language="JavaScript" src="JS/TreeMenu.js" type="text/javascript"></script>
<form method="post" action="ReportRight.php?ReportMode={$ReportMode}" target="RightFrame">
<div id="ReportTypeDiv" class="CommonDiv" style="text-align:left;margin-top:3px;width:260px;">
{foreach from=$ReportTypeList key=ReportType item=ReportTypeName}
<input type="checkbox" id="{$ReportType}" name="ReportTypeList[]" value="{$ReportType}" {if $ReportTypeListChecked[$ReportType] == '0' || $ReportTypeListChecked[$ReportType] != ''}checked="checked"{/if}><label for="{$ReportType}">{$ReportTypeName}</label><br />
{/foreach}
<br />
<center>
<input type="Submit" class="Btn" name="ReportSubmit" value="{$Lang.ViewReportBTN}" />
<input type="button" class="Btn" value="{$Lang.SelectAllBTN}" onclick="selectAllCheckbox('ReportTypeDiv');" />
<input type="button" class="Btn" value="{$Lang.SelectNoneBTN}" onclick="selectAllCheckbox('ReportTypeDiv',false);" />
</center>
</div>
</form>
<script type="text/javascript">
{literal}
function selectAllCheckbox(objid, check)
{
    var obj=xajax.$(objid);
    var checkobjs = obj.getElementsByTagName("input");
    for(var i=0; i<checkobjs.length; i++)
    {
        if(checkobjs[i].type.toLowerCase() == "checkbox" )
        {
            if(typeof(check) == "undefined" || check == true)
            {
                checkobjs[i].checked = true;
            }
            else if(check == false)
            {
                checkobjs[i].checked = false;
            }
        }
    }

}
{/literal}
</script>
</body>
</html>
