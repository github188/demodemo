<script language="JavaScript1.1">
  /* Create the text arrays and value arrays used to create the select */
  var DefaultFieldsText  = new Array({$DefaultFieldsText});
  var DefaultFieldsValue = new Array({$DefaultFieldsValue});

  {literal}
  /**
   * Set to the default fields to show .
   *
   * @author leeyupeng <leeyupeng@gmail.com>
   */
  function setDefaultFields(ItemList)
  {
      for(var x=ItemList.length-1; x>=0; x--)
      {
          var opt = ItemList.options[x];
          ItemList.options[x] = null;
      }

      for(var x=0; x<= DefaultFieldsText.length-1; x++)
      {
          ItemList.options[x]  = new Option(DefaultFieldsText[x], DefaultFieldsValue[x], 0, 0);
      }
  }
  function submitForm()
  {
      xajax.$("FieldsToShow").value = joinItem(xajax.$("FieldsToShowList"));
      if(xajax.$("FieldsToShow").value == '')
      {
          alert('Must select one field!');
          return false;
      }
      else
      {
          return true;
      }
  }
function localize(ContainerDiv,xPoint,yPoint)
{
        var widthReg = /px/ig;
        var heightReg = /px/ig;
        inputObj=xajax.$(ContainerDiv);
        inputObjWidth = 600;
        if(widthReg.test(inputObj.style.width))
        {
            inputObjWidth = inputObj.style.width.replace(widthReg,'');
        }
        xajax.$(ContainerDiv).style.width = inputObjWidth+"px";
        xajax.$(ContainerDiv).style.left = xPoint-430+"px";
        xajax.$(ContainerDiv).style.top  = yPoint+10+"px";
}
function hiddenDivCustomSetTable()
{
    if(xajax.$('CustomDivIsFocus').value == '0')
    {
        hiddenObj("DivCustomSetTable");
    }
}
function showDivCustomSetTable(xPoint,yPoint)
{
if(xajax.$('DivCustomSetTable').style.display == 'block')
{
    xajax.$('CustomDivIsFocus').value = '0';
    hiddenDivCustomSetTable();
    return;
}
if(xajax.$('CustomDivIsFocus').value == "")
{
    xajax.$('CustomDivIsFocus').value = '0';
    setTimeout("displayObj('"+"DivCustomSetTable"+"')",200);
}
else if(xajax.$('CustomDivIsFocus').value=='0')
{
    if(xajax.$('CustomDivIsFocus').style.display != 'block')
    {
        xajax.$('CustomDivIsFocus').value = '0';
        setTimeout("displayObj('"+"DivCustomSetTable"+"')",200);
    }
    else
    {
        hiddenDivCustomSetTable();
    }
}
else
{
        xajax.$('CustomDivIsFocus').value = '0';
        setTimeout("displayObj('"+"DivCustomSetTable"+"')",200);
}
localize('DivCustomSetTable',xPoint,yPoint);
}
  {/literal}
</script>
<form method="post" action="" target="_self" onsubmit="return submitForm();">
<div id="DivCustomSetTable" style="display:none;position:absolute;width:450px;background-color:#FFFFFF;border:1px solid #808080" onmouseover="xajax.$('CustomDivIsFocus').value=1;" onmouseout="xajax.$('CustomDivIsFocus').value=0;">
    <table width="100%" align="center" border=0 id="CustomSetTable" cellpadding="1" cellspacing="1" class="BgTable">
      <tr>
        <td colspan="4" align="center">
        </td>
        <td align="center">
          <a href="#" title="Close" onclick="xajax.$('CustomDivIsFocus').value='0';hiddenDivCustomSetTable();" style="text-decoration:none;font-size:12px;width:100%;display:block;font-weight:bold;"/>x</a>
        </td>
      </tr>
      <tr align="center" valign="middle" class="BgRow">
        <td width="5%" align="left">&nbsp;</td>
        <td width="20%">
          <fieldset style="border:0;font-size:12px;">
            <legend>{$Lang.AllFieldsTitle}</legend>
            {$FieldsToSelectList}
          </fieldset>
        </td>
        <td width="10%">
          <input type="button" value=">>" class="ActionButton" style="width:1.7cm" onclick="addItem(xajax.$('FieldsToSelectList'),xajax.$('FieldsToShowList'))" /><br />
          <input type="button" value="<<" class="ActionButton" style="width:1.7cm" onclick="delItem(xajax.$('FieldsToShowList'))" />
        </td>
        <td width="20%">
          <fieldset style="border:0;font-size:12px;">
          <legend>{$Lang.FieldsToShowTitle}</legend>
          {$FieldsToShowList}
          </fieldset>
        </td>
        <td width="8%" align="left">
          <input type="button" value="&uarr;" style="width:0.6cm" onclick="JavaScript:upItem(xajax.$('FieldsToShowList'))" /><br />
          <input type="button" value="&darr;" style="width:0.6cm" onclick="JavaScript:downItem(xajax.$('FieldsToShowList'))" />
        </td>
      </tr>
      <tr>
        <td colspan="5" align="center" style="padding:10px 0 6px">
          <input type="submit" class="Btn" value="{$Lang.FieldsToShowBTN}" class="ActionButton" accesskey="O">
          <input type="button" class="Btn" value="{$Lang.FieldsDefaultBTN}" class="ActionButton" onclick="setDefaultFields(xajax.$('FieldsToShowList'))" accesskey="D" /><br />
        </td>
      </tr>
    </table>
    </div>
    <input type="hidden" name="CustomDivIsFocus"  id="CustomDivIsFocus" value="" />
    <input type="hidden" name="FieldsToShow"  id="FieldsToShow" value="" />
</form>
