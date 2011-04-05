<input type="hidden" name="MAX_FILE_SIZE" value="{$CFG.File.MaxFileSize}" />
<div id="AttachFilesDiv" style="width:100%;text-align:left;clear:both;padding:1px;margin-bottom:5px;position:relative;">
  <a href="#" id="AttachFile" onclick="return false;" onfocus="this.blur();" style="clear:both;cursor:hand;position:absolute;display:block;font-weight:bold;width:120px;text-align:left;margin-left:-1px;padding:5px 0px;color:blue;text-decoration:none;">
    {$Lang.AttachFile}({math equation="(x/y)" x=$CFG.File.MaxFileSize y=1024}K)
   </a>
  <blockquote id="AttachFilesBlock" style="clear:both;margin:2px 0;padding:0;display:block;height:10px;"></blockquote>
  <ol style="list-style-position:inside;margin-left:5px;">
    <blockquote id="AttachFiles" style="margin:0;padding:0;"></blockquote>
  </ol>
</div>
{literal}
<script type="text/javascript">
//<![CDATA[
var uploadFileName = "TestFile";
var FileInputCount = 0;

createInputFile('AttachFilesDiv');
function createInputFile(parentId)
{
    parentObj = xajax.$(parentId);
    xajax.createInput(parentId,'file',uploadFileName+'[]',uploadFileName+FileInputCount);
    fileObj = xajax.$(uploadFileName+FileInputCount);
    fileObj.style.cursor='hand';
    fileObj.style.position="absolute";
    fileObj.style.float="left";
    fileObj.style.marginLeft="0";
    fileObj.style.marginTop="0";
    fileObj.style.left="-42px";
    fileObj.style.top=0;
    fileObj.style.filter="alpha(opacity=0);";
    fileObj.style.opacity="0";
    fileObj.size=1;
    fileObj.contentEditable=false;
    fileObj.onclick = function(){this.blur();}
    fileObj.onchange = function(){createFilePathSpan(this.id);}
}

function createFilePathSpan(objId)
{
    var obj = xajax.$(objId);
    var liId = objId+'PathSpan';

    FileInputCount ++;
    xajax.insert('AttachFiles','li', liId);
    fileName = getUploadFileName(obj.value);
    xajax.$(liId).innerHTML = '<input type="text" size="30" value="'+fileName+'" id="'+ objId +'ShowText" style="font-size:11px;border:0;"/>';
    xajax.$(liId).innerHTML += '&nbsp;&nbsp<a href="#" onclick="xajax.remove(\''+objId+'\');xajax.remove(\''+liId+'\');return false;" style="text-decoration:none;color:red;font-weight:bold;"><img src="Image/deletefile.gif" alt="remove" /></a>';
    xajax.$(objId).style.left = "-10000px";
    createInputFile('AttachFilesDiv');
    xajax.$(objId +'ShowText').readOnly = true;
}
function getUploadFileName(fileString)
{
    x=fileString.lastIndexOf("\\") > 0 ? fileString.lastIndexOf("\\") : fileString.lastIndexOf("/")
    if(x+0> 0)
    {
        x++;
        fileName=fileString.substr(x,fileString.length-1);
        return fileName;
    }
    return fileString;
}
//]]>
</script>
{/literal}
