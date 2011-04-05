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
        inputObjWidth = 600;
        if(widthReg.test(inputObj.style.width))
        {
            inputObjWidth = inputObj.style.width.replace(widthReg,'');
        }
        xajax.$(ContainerDiv).style.width = inputObjWidth+"px";

        xajax.$(ContainerDiv).style.left = xPoint-430+"px";
        xajax.$(ContainerDiv).style.top  = yPoint+10+"px";
}
function hiddenDivUploadSetTable()
{
    if(xajax.$('UploadDivIsFocus').value == '0')
    {
        hiddenObj("DivUploadSetTable");
    }
}
function showUploadFile(xPoint,yPoint)
{
if(xajax.$('DivUploadSetTable').style.display == 'block')
{
    xajax.$('UploadDivIsFocus').value = '0';
    hiddenDivUploadSetTable();
    return;
}
if(xajax.$('UploadDivIsFocus').value == "")
{
    xajax.$('UploadDivIsFocus').value = '0';
    setTimeout("displayObj('"+"DivUploadSetTable"+"')",200);
}
else if(xajax.$('UploadDivIsFocus').value=='0')
{
    if(xajax.$('UploadDivIsFocus').style.display != 'block')
    {
        xajax.$('UploadDivIsFocus').value = '0';
        setTimeout("displayObj('"+"DivUploadSetTable"+"')",200);
    }
    else
    {
        hiddenDivUploadSetTable();
    }
}
else
{
        xajax.$('UploadDivIsFocus').value = '0';
        setTimeout("displayObj('"+"DivUploadSetTable"+"')",200);
}
localize('DivUploadSetTable',xPoint,yPoint);
}
  {/literal}
</script>
<form method="post" action="" enctype="multipart/form-data">

 <div id="DivUploadSetTable" style="display:none;position:absolute;width:370px;background-color:#FFFFFF;border:1px solid #808080" onmouseover="xajax.$('UploadDivIsFocus').value=1; " onmouseout="xajax.$('UploadDivIsFocus').value=0; ">
    <table width="100%" align="center" border=0 id="CustomSetTable" cellpadding="1" cellspacing="1" class="BgTable">
      <tr>
        <td colspan="4" align="left" style="padding-left:40px;padding-top:10px">
         {$Lang.ImportLable}
        </td>
        <td align="center" style="padding-right:10px;" >
          <a href="#" title="Close" onclick="xajax.$('UploadDivIsFocus').value='0';hiddenDivUploadSetTable();" style="text-decoration:none;font-size:12px;width:100%;display:block;font-weight:bold;"/>x</a>
        </td>
      </tr>
      <tr>
        <td><input name="MAX_FILE_SIZE" type="hidden" id="MAX_FILE_SIZE" value="2000000"></td>
        <td><input name="file" type="file"  value="浏览" > <input type="submit" name="Submit" value="上传"></td>
        <input name="action" type="hidden" id="action" value="uppic">
      </tr>
      <tr>
        <td></td>
        <td align="left" style="padding-left:40px; padding-bottom:10px">{$Lang.ImportNotes}</td>
      </tr>

    </table>
    </div>

</div>
<input type="hidden" name="UploadDivIsFocus"  id="UploadDivIsFocus" value="" />

</form>
