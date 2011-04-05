
<script language="JavaScript1.1">
  /* Create the text arrays and value arrays used to create the select */
  var DefaultFieldsText  = new Array({$DefaultFieldsText});
  var DefaultFieldsValue = new Array({$DefaultFieldsValue});
  
  {literal}

  
function localize(ContainerDiv,xPoint,yPoint)
{
        var widthReg = /px/ig;
        var heightReg = /px/ig;
        inputObj=xajax.$(ContainerDiv);
        inputObjWidth = 400;
        if(widthReg.test(inputObj.style.width))
        {
            inputObjWidth = inputObj.style.width.replace(widthReg,'');
        }
        xajax.$(ContainerDiv).style.width = inputObjWidth+"px";
        xajax.$(ContainerDiv).style.left = xPoint + "px";
        xajax.$(ContainerDiv).style.top  = yPoint - 50 + "px";

        xajax.$(ContainerDiv).style.overflowX = "hidden";
        xajax.$(ContainerDiv).style.overflowY = "auto";
        xajax.$(ContainerDiv).style.border = "1px solid #000000";
        xajax.$(ContainerDiv).style.backgroundColor = "#F1F1F1";


}
function hiddenDivSaveQuerySetTable()
{   
  if(xajax.$('SaveQueryDivIsFocus').value == '0')
    {
        hiddenObj("DivSaveQuerySetTable");
        setTimeout("createEnvelopDiv('DivSaveQuerySetTable')",60);

     }
 }
function showSaveQuery(xPoint,yPoint)
{
/*It not work at Chrome&Safari*/
//document.getElementById('QueryTitle').innerText = "";
if(xajax.$('DivSaveQuerySetTable').style.display == 'block')
{
    xajax.$('SaveQueryDivIsFocus').value = '0';
    hiddenDivSaveQuerySetTable();
    return ;
}
if(xajax.$('SaveQueryDivIsFocus').value == "")
{
    xajax.$('SaveQueryDivIsFocus').value = '0';
    setTimeout("displayObj('"+"DivSaveQuerySetTable"+"')",50);
}
else if(xajax.$('SaveQueryDivIsFocus').value=='0')
{
    if(xajax.$('SaveQueryDivIsFocus').style.display != 'block')
    {
        xajax.$('SaveQueryDivIsFocus').value = '0';
        setTimeout("displayObj('"+"DivSaveQuerySetTable"+"')",50);
    }
    else
    {
        hiddenDivSaveQuerySetTable();
    }
}
else
{
        xajax.$('SaveQueryDivIsFocus').value = '0';
        setTimeout("displayObj('"+"DivSaveQuerySetTable"+"')",50);
}
localize('DivSaveQuerySetTable',xPoint,yPoint);
setTimeout("createEnvelopDiv('DivSaveQuerySetTable')",60);

}
  {/literal}
</script>




<div id="DivSaveQuerySetTable" style="display:none;position:absolute;width:400px; height=50px; background-color:#FFFFFF;border:1px solid #808080; z-index:10002" onmouseover="xajax.$('SaveQueryDivIsFocus').value=1; " onmouseout="xajax.$('SaveQueryDivIsFocus').value=0; ">
   
    <table width="100%" align="center" border=0 cellpadding="1" cellspacing="1" class="BgTable">
      <tr>
        <td colspan="4" align="center">
        </td>
        <td align="center" width="8%">
          <a href="javascript:void(0);" title="Close" onclick="xajax.$('SaveQueryDivIsFocus').value='0';hiddenDivSaveQuerySetTable();return false;"
            style="text-decoration:none;font-size:12px;width:100%;display:block;font-weight:bold;"/>x</a>
        </td>
      </tr>
      <tr>
        <td>{$Lang.QueryTitle}</td>
        <td><input id="QueryTitle" name="QueryTitle" value="" maxlength="150" /></td>
        <td><input type="submit" class="Btn" id="SubmitButton" name="SubmitButton" value="{$Lang.SaveQuery}" accesskey="S" onclick="xajax.$('SaveQueryDivIsFocus').value='0';hiddenDivSaveQuerySetTable();" /></td>
        <td></td>   
        </tr>
    </table>
    </div>

</div>
<input type="hidden" name="QueryType" value="{$QueryType}" />

<input type="hidden" name="SaveQueryDivIsFocus"  id="SaveQueryDivIsFocus" value="" />


