/**
 * add one option of a select to another select.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function addItem(ItemList,Target)
{
    for(var x = 0; x < ItemList.length; x++)
    {
        var opt = ItemList.options[x];
        if (opt.selected)
        {
            flag = true;
            for (var y=0;y<Target.length;y++)
            {
                var myopt = Target.options[y];
                if (myopt.value == opt.value)
                {
                    flag = false;
                }
            }
            if(flag)
            {
                Target.options[Target.options.length] = new Option(opt.text, opt.value, 0, 0);
            }
        }
    }
}

function addOptionByTextValue(Target,Text,Value)
{
    RawLength = Target.length;
    NewLength = Target.length+Value.length;
    for(I=RawLength;I<NewLength;I++)
    {
        Target.options[I] = new Option(Text[I-RawLength], Value[I-RawLength], 0, 0);
    }
}


/**
 * move one selected option from a select.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function delItem(ItemList)
{
    for(var x=ItemList.length-1;x>=0;x--)
    {
        var opt = ItemList.options[x];
        if (opt.selected)
        {
            ItemList.options[x] = null;
        }
    }
}

function delAllItem(ItemList)
{   for(var x=ItemList.length-1;x>=0;x--)
    {
        var opt = ItemList.options[x];
        ItemList.options[x] = null;
    }
}

/**
 * join items of an select with ",".
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function joinItem(ItemList)
{
    var OptionList = new Array();
    for (var x=0; x<ItemList.length;x++)
    {
        OptionList[x] = ItemList.options[x].value;
    }
    return OptionList.join(",");
}

/**
 * select all items of a select
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function selectItem(ItemList)
{
    for(var x=ItemList.length-1;x>=0;x--)
    {
        var opt = ItemList.options[x];
        opt.selected = true;
    }
}

/**
 * unselect all items of a select
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function selectNoItem(ItemList)
{
    for(var x=ItemList.length-1;x>=0;x--)
    {
        var opt = ItemList.options[x];
        opt.selected = false;
    }
}

/**
 * select one item of a select
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function selectOneItem(ItemList,ItemValue)
{
    for(I = 0;I < ItemList.options.length; I++)
    {
        if(ItemValue == ItemList.options[I].value)
        {
           ItemList.options[I].selected = true;
           return;
        }
    }
}

/**
 * move one selected option up from a select.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function upItem(ItemList)
{
    for(var x=1;x<ItemList.length;x++)
    {
        var opt = ItemList.options[x];
        if(opt.selected)
        {
            tmpUpValue = ItemList.options[x-1].value;
            tmpUpText  = ItemList.options[x-1].text;
            ItemList.options[x-1].value = opt.value;
            ItemList.options[x-1].text  = opt.text;
            ItemList.options[x].value = tmpUpValue;
            ItemList.options[x].text  = tmpUpText;
            ItemList.options[x-1].selected = true;
            ItemList.options[x].selected = false;
            break;
        }
    }
}

/**
 * move one selected option down from a select.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function downItem(ItemList)
{
    for(var x=0;x<ItemList.length-1;x++)
    {
        var opt = ItemList.options[x];
        if(opt.selected)
        {
            tmpUpValue = ItemList.options[x+1].value;
            tmpUpText  = ItemList.options[x+1].text;
            ItemList.options[x+1].value = opt.value;
            ItemList.options[x+1].text  = opt.text;
            ItemList.options[x].value = tmpUpValue;
            ItemList.options[x].text  = tmpUpText;
            ItemList.options[x+1].selected = true;
            ItemList.options[x].selected = false;
            break;
        }
    }
}

function getItemTextByNum(ItemList, i)
{
    if(ItemList.options[i].innerText)
    {
        return ItemList.options[i].innerText;
    }
    else if(ItemList.options[i].textContent)
    {
        return ItemList.options[i].textContent;
    }
    else
    {
        return '';
    }
}

function getItemTextByValue(ItemList, ItemValue)
{
    for(I = 0;I < ItemList.options.length; I++)
    {
        if(ItemValue == ItemList.options[I].value)
        {
           return getItemTextByNum(ItemList, I);
        }
    }
}

/**
 * return option's text which is selected
 */

function getItemSelectText(ItemList)
{
    for(I = 0;I < ItemList.options.length; I++)
    {
        if(ItemList.options[I].selected)
        {
           return getItemTextByNum(ItemList, I);
        }
    }
    return getItemText(ItemList, 0);
}

function displaceSelect(DispalceID, HiddenInputID, FromID)
{
    if(typeof(FromID) != "undefined")
    {
        addItem(xajax.$(FromID), xajax.$(DispalceID));
    }
    else
    {
        delItem(xajax.$(DispalceID));
    }
    xajax.$(HiddenInputID).value=joinItem(xajax.$(DispalceID));
}


function expandTop(Mode)
{
    try{
    var RightFrame = parent.document.getElementById('RightFrame');
    //var Control    = parent.document.getElementById("ControlTop");
    if(Mode == 'Open')
    {
        RightFrame.rows = "135,*";
        //Control.src     = "Image/ArrowUp.gif";
    }
    else
    {
        RightFrame.rows = "0,*";
        //Control.src     = "Image/ArrowDown.gif";
    }
    }catch(e){};
}

function createEnvelopDiv(DivId)
{
    Div = document.getElementById(DivId);
    var gap = 0;
    if(navigator&&navigator.userAgent.toLowerCase().indexOf("msie") == -1)
    {
        //return ;
        gap = 2;
    }

    //Div.onresize = function(){createEnvelopDiv(DivId)};

    var displayDiv = Div;
    if(typeof(displayDiv.envelopDiv) == "undefined")
    {
        // create a div to envelop
        var tEnvelopDiv = document.createElement("DIV");

        // define the envelopDiv's style
        tEnvelopDiv.id = DivId+"EvelopDiv";
        tEnvelopDiv.style.position = "absolute";
        tEnvelopDiv.style.left   = displayDiv.offsetLeft;
        tEnvelopDiv.style.top    = displayDiv.offsetTop;
        tEnvelopDiv.style.width  = displayDiv.offsetWidth-2*gap+"px";
        tEnvelopDiv.style.height = displayDiv.offsetHeight-2*gap+"px";
        tEnvelopDiv.style.zIndex = 10000;

        // create a iframe
        var tEnvelopIframe = document.createElement("IFRAME");
        var tDate = new Date();
        var tEnvelopIframeId = "envelopiframe" + tDate.getTime();
        tEnvelopIframe.id = tEnvelopIframeId;
        tEnvelopIframe.name = tEnvelopIframeId;

        // define the iframe's style
        tEnvelopIframe.style.position = "absolute";
        tEnvelopIframe.style.left     = "0";
        tEnvelopIframe.style.top      = "0";
        tEnvelopIframe.style.zIndex   = tEnvelopDiv.style.zIndex-1;
        tEnvelopIframe.frameBorder    = "0";
        //tEnvelopIframe.src            = "javascript:false;";
        tEnvelopIframe.src            = "";
        tEnvelopDiv.appendChild(tEnvelopIframe);

        displayDiv.envelopDiv = tEnvelopDiv;
        displayDiv.envelopDiv.EnvelopIframeId = tEnvelopIframeId;

        displayDiv.parentNode.insertBefore(displayDiv.envelopDiv,displayDiv);
    }

    // get the iframe
    tEnvelopDivIframeId = displayDiv.envelopDiv.EnvelopIframeId;
    if(frames&&(frames[tEnvelopDivIframeId]&&frames[tEnvelopDivIframeId].frameElement))
    {
        tIframe=frames[tEnvelopDivIframeId].frameElement;
    }
    else
    {
        tIframe=document.getElementById(tEnvelopDivIframeId);
    }
    //tIframe.style.left   = displayDiv.offsetLeft;
    //tIframe.style.top    = displayDiv.offsetTop;
    tIframe.style.width  = displayDiv.offsetWidth-2*gap+"px";
    tIframe.style.height = displayDiv.offsetHeight-2*gap+"px";
    tIframe.style.display = displayDiv.style.display;
    tIframe.style.visibility = displayDiv.style.visibility;
    displayDiv.style.zIndex = displayDiv.envelopDiv.style.zIndex+1;
    return false;
}

/**
 * get obj by elementid
 */
function idObj(ElementId)
{
    return document.getElementById(ElementId);
}

/*========================================for shadow==========================================*/
var HexArray = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
var HexStr = "0123456789ABCDEF";

var RgbStep = "";
var RgbStepNum=0;
var RgbLastBg = "";

function shadow(ElementId,ClassName)
{
    RgbStepNum=0;
    idObj(ElementId).style.backgroundColor="";
    BackGroundColor = getCurrentBgColor(ElementId);
    BgColorNumHex = BackGroundColor.replace("#", "");
    RgbLastBg = BgColorNumHex;
    idObj(ElementId).style.backgroundColor="#FFFFFF";
    R = "0x"+BgColorNumHex.charAt(0)+BgColorNumHex.charAt(1);
    G = "0x"+BgColorNumHex.charAt(2)+BgColorNumHex.charAt(3);
    B = "0x"+BgColorNumHex.charAt(4)+BgColorNumHex.charAt(5);

    rstep = toRgbHex(Math.floor((parseInt(0xFF)-parseInt(R))/40));
    gstep = toRgbHex(Math.floor((parseInt(0xFF)-parseInt(G))/40));
    bstep = toRgbHex(Math.floor((parseInt(0xFF)-parseInt(B))/40));
    RgbStep  = "0x"+rstep+gstep+bstep;

    shadowObj(ElementId);
}

function shadowObj(ElementId)
{
	if ( RgbStepNum <= 39 )
	{
        BackGroundColor = getCurrentBgColor(ElementId);
        BgColorNumHex = BackGroundColor.replace("#", "");
        BgColorNumDec = parseInt("0x"+BgColorNumHex)-parseInt(RgbStep);
        if(BgColorNumDec < parseInt("0x" + RgbLastBg))
        {
            BgColorNumDec = parseInt("0x" + RgbLastBg);
            BgColorNumHex = toHex(BgColorNumDec);
        }
        BgColorNumHex = toHex(BgColorNumDec);
        idObj(ElementId).style.backgroundColor = "#" + BgColorNumHex;

		RgbStepNum++;
	    setTimeout("shadowObj('"+ElementId+"')", 12);
	}
}

/**
 * get current background color
 */
function getCurrentBgColor(ElementID)
{
    obj = idObj(ElementID);
    return rgbStrToHex(GetCurrentStyle(obj,"backgroundColor"));
}

/**
 * get elements property value
 */
function GetCurrentStyle (Element, PropertyName)
{
	if (Element.currentStyle)
    {
		return Element.currentStyle[PropertyName];
	}
	else if (window.getComputedStyle)
    {
		PropertyName = PropertyName.replace (/([A-Z])/g, "-$1");
		PropertyName = PropertyName.toUpperCase ();
		return window.getComputedStyle (Element, "").getPropertyValue(PropertyName);
	}
	return null;
}

/**
 * convert the rgb string like rgb(0,0,0) to #000000
 */
function rgbStrToHex(RGBStr)
{
    RGBStr = RGBStr + "";
    RGBStrTemp = RGBStr.replace(/rgb\((\d+),(\s?)(\d+),(\s?)(\d+)\)/g, "$1,$3,$5");
    if(RGBStrTemp == RGBStr)
    {
        return RGBStr;
    }
    RGB = RGBStrTemp.split(",");
    RGBStr = "#";
    for(var i=0; i <=2; i++)
    {
        RGBStr += toRgbHex(RGB[i]);
    }
    return RGBStr;
}


function toRgbHex(N)
{
  var High, Low;
  var N = Math.round(N);
  Low = N % 16;
  High = Math.floor((N / 16)) % 16;
  return (HexArray[High] + HexArray[Low]);
}


function toHex(HexNum)
{
    var t=0;
    var DecNum='';
    do
    {
        t = HexNum % 16;
        DecNum = HexArray[t].toString() +DecNum;
        HexNum = parseInt(HexNum/16);
    }while (parseInt(HexNum)>0);
    for(var i=0; i<6-DecNum.length; i++)
    {
        DecNum = "0" + DecNum;
    }
    return DecNum;
}

function toRgbDec(Hex)
{
    for(var i=0; i<6-Hex.length; i++)
    {
        Hex = "0" + Hex;
    }
    Hex = Hex.toUpperCase();
    dec = HexStr.indexOf(Hex.charAt(0))*16*16*16*16*16 + HexStr.indexOf(Hex.charAt(1))*16*16*16*16+
          HexStr.indexOf(Hex.charAt(2))*16*16*16 + HexStr.indexOf(Hex.charAt(3))*16*16+
          HexStr.indexOf(Hex.charAt(4))*16 + HexStr.indexOf(Hex.charAt(5));
  return dec;
}


/*=======================================Div select=================================*/
function getAbsPoint(obj)
{
  var x = obj.offsetLeft;
  var y = obj.offsetTop;
  while(obj = obj.offsetParent)
  {
    x += obj.offsetLeft;
    y += obj.offsetTop;
  }
  return  {"x": x, "y": y};
}

function selectDiv(inputObj)
{
    this.inputObj = inputObj;
    this.id = inputObj.id+'SelectDiv';

    this.createContainerDiv = function()
    {
        if(xajax.$(this.id))
        {
            xajax.$(this.id).innerHTML = "";
        }
        else
        {
            xajax.insertAfter(inputObj.id,'div',this.id);
        }
        this.ContainerDiv = xajax.$(this.id);
        this.ContainerDiv.style.position = "absolute";
        this.ContainerDiv.style.display = "inline";
        var widthReg = /px/ig;
        var heightReg = /px/ig;
        inputObjWidth = 190;
        inputObjHeight = 20;
        if(widthReg.test(inputObj.style.width))
        {
            inputObjWidth = inputObj.style.width.replace(widthReg,'');
        }
        if(heightReg.test(inputObj.style.height))
        {
            inputObjHeight = inputObj.style.height.replace(heightReg,'');
        }
        var absPoint = getAbsPoint(inputObj);
        this.ContainerDiv.style.width = inputObjWidth+"px";
        this.ContainerDiv.style.left = absPoint['x']+"px";
        this.ContainerDiv.style.top  = absPoint['y']+inputObjHeight+"px";
        this.ContainerDiv.style.overflowX = "hidden";
        this.ContainerDiv.style.overflowY = "auto";
        this.ContainerDiv.style.border = "1px solid #000000";
        this.ContainerDiv.style.backgroundColor = "#F1F1F1";
        this.ContainerDiv.className = 'selectDiv';
    }

    this.getSelectOption = function()
    {
        var tesarray = new Array('eee','4342','dfas','dfasfds');
        this.selectOptionKeyList = tesarray;
        this.selectOptionValueList = tesarray;
    }

    this.createOptionDiv = function(optionKeyList,optionValueList)
    {
        if(optionKeyList == "")
        {
            this.hidden();
        }
        else
        {
            var temp_key_value;
            for(i=0; i< optionKeyList.length; i++)
            {
                var optionId = this.getOptionIdByNum(i);
                if(!xajax.$(optionId))
                {
                    xajax.create(this.id,'div',optionId);
                }
                xajax.$(optionId).innerHTML = optionValueList[i];
                xajax.$(optionId).style.textAlign = 'left';
                xajax.$(optionId).style.padding = '2px 2px 0';
                xajax.$(optionId).key = optionKeyList[i];
                xajax.$(optionId).onclick = function()
                {
                    temp_key_value = (this.key+'').replace(/([\\"'])/g, "\\$1").replace(/\u0000/g, "\\0");
                    eval(inputObj.setValueFunction+"('"+inputObj.id+"','"+temp_key_value+"');");
                }
                xajax.$(optionId).onmouseover = function()
                {
                    unselectAllOption(xajax.$(this.id).parentNode.id);
                    selectDivSelectOption(this.id);
                }
                xajax.$(optionId).onmouseout = function()
                {
                    unselectDivSelectOption(this.id);
                }

            }

            this.slelectOptionByNum(0);
            this.selectOptionCount = optionKeyList.length;
            this.selectedOptionNum = 0;
            this.selectedValue = optionValueList[0];
            this.dispaly();
        }

    }

    this.slelectOption = function(optionId)
    {
        selectDivSelectOption(optionId);
    }

    this.unselectOption = function(optionId)
    {
        unselectDivSelectOption(optionId);
    }

    this.slelectOptionByNum = function(num)
    {
        var optionId = this.getOptionIdByNum(num);
        this.slelectOption(optionId);
    }

    this.unselectOptionByNum = function(num)
    {
        var optionId = this.getOptionIdByNum(num);
        this.unselectOption(optionId);
    }

    this.getOptionIdByNum = function(num)
    {
        return this.id+'Sub_'+num;
    }

    this.moveUp = function()
    {
        if(this.selectedOptionNum == 0)
        {
            this.unselectOptionByNum(this.selectedOptionNum);
            this.selectedOptionNum = this.selectOptionCount - 1;
            this.slelectOptionByNum(this.selectedOptionNum);
        }
        else
        {
            this.unselectOptionByNum(this.selectedOptionNum);
            this.selectedOptionNum --;
            this.slelectOptionByNum(this.selectedOptionNum);
        }
    }

    this.moveDown = function()
    {
        if(this.selectedOptionNum == this.selectOptionCount - 1)
        {
            this.unselectOptionByNum(this.selectedOptionNum);
            this.selectedOptionNum = 0;
            this.slelectOptionByNum(this.selectedOptionNum);
        }
        else
        {
            this.unselectOptionByNum(this.selectedOptionNum);
            this.selectedOptionNum ++;
            if(this.selectedOptionNum < this.selectOptionCount)
            {
                this.slelectOptionByNum(this.selectedOptionNum);
            }
        }

    }

    this.getSelectedKey = function()
    {
        return xajax.$(this.id+'Sub_'+this.selectedOptionNum).key;
    }

    this.getSelectedValue = function()
    {
        return xajax.$(this.id+'Sub_'+this.selectedOptionNum).innerHTML.toString();
    }

    this.dispaly = function()
    {
        xajax.$(this.id).style.display = "block";
        createEnvelopDiv(this.id);
    }

    this.hidden = function()
    {
        xajax.$(this.id).style.display = "none";
        createEnvelopDiv(this.id);
    }
    //this.create
}

function unselectAllOption(objId)
{
    var options = xajax.$(objId).getElementsByTagName('div');
    for(var i=0;i<options.length;i++)
    {
        if(options[i].selected)
        {
            unselectDivSelectOption(options[i].id);
            return;
        }
    }
}

function selectDivSelectOption(optionId)
{
    try{
    xajax.$(optionId).style.color = '#FFFFFF';
    //xajax.$(optionId).style.fontWeight = 'bold';
    xajax.$(optionId).style.backgroundColor = '#37428A';
    xajax.$(optionId).selected = 1;
    }catch(e){};
}

function unselectDivSelectOption(optionId)
{
    try{
    xajax.$(optionId).style.color = '#000000';
    //xajax.$(optionId).style.fontWeight = 'normal';
    xajax.$(optionId).style.backgroundColor = '#F1F1F1';
    xajax.$(optionId).selected = 0;
    }catch(e){};
}


function showDivList(evt)
{
    evt = (evt) ? evt : ((window.event) ? window.event : "");
    keyCode = evt.keyCode ? evt.keyCode : (evt.which ? evt.which :evt.charCode);
    if(keyCode == 9) return true;
    eventElement = evt.srcElement ? evt.srcElement : (evt.target ? evt.target : "");
    eId = eventElement.id;
    if(evt.altKey || evt.shiftKey)
    {
        return true;
    }
    if(evt)
    {
        if(keyCode == '13')
        {
            eventElement.value = eventElement.OldValue;
        }
        if(eventElement.OldValue == eventElement.value)
        {
            if(eventElement.selectDiv)
            {
                if(keyCode == '38')
                {
                    eventElement.selectDiv.moveUp();
                }
                else if(keyCode == '40')
                {
                    eventElement.selectDiv.moveDown();
                }
                else if(keyCode == '13')
                {
                    if(eventElement.selectDiv)
                    {
                        var replaceValue = eventElement.selectDiv.getSelectedKey();
                        replaceValue = (replaceValue+'').replace(/([\\"'])/g, "\\$1").replace(/\u0000/g, "\\0");
                        eval(eventElement.setValueFunction+"('"+eventElement.id+"','"+replaceValue+"');");
                        return false;
                    }
                }
            }
        }
        else
        {
            eventElement.OldValue = eventElement.value;
            eval("var newSearchValue = "+eventElement.getSearchValueFunction+"('"+eId+"');");
            if(newSearchValue == null)
            {
                return;
            }
            if(eventElement.reLoadData != 1 && typeof(eventElement.selectDiv) != "undefined" && typeof(eventElement.selectDiv.optionKeyList) != "undefined" && eventElement.selectDiv.optionKeyList != "")
            {
                if(newSearchValue.indexOf(eventElement.searchValue) == 0 || eventElement.searchValue.indexOf(newSearchValue) == 0)
                {
                    if(newSearchValue.length>eventElement.searchValue)
                    {
                        eventElement.searchValue = newSearchValue;
                    }

                    var keyArrayList = new Array();
                    var valueArrayList = new Array();
                    for(var i=0;i<eventElement.selectDiv.optionKeyList.length;i++)
                    {
                        if(eventElement.selectDiv.optionKeyList[i].indexOf(newSearchValue.trim()) >= 0 && newSearchValue.trim() != "")
                        {
                            keyArrayList.push(eventElement.selectDiv.optionKeyList[i]);
                            valueArrayList.push(eventElement.selectDiv.optionValueList[i]);
                        }
                    }
                    if(keyArrayList)
                    {
                        eventElement.selectDiv.createContainerDiv();
                        eventElement.selectDiv.createOptionDiv(keyArrayList,valueArrayList);
                    }
                    else
                    {
                        eventElement.selectDiv.optionKeyList = '';
                        eventElement.selectDiv.optionValueList = '';
                    }
                }
                else
                {
                    eventElement.selectDiv = new selectDiv(eventElement);
                    eventElement.selectDiv.createContainerDiv();
                    createEnvelopDiv(eventElement.selectDiv.ContainerDiv.id);
                    eventElement.searchValue = newSearchValue;
                    xajax_xCreateSelectDiv(eId,eventElement.searchValue, eventElement.loadFunction, xajax.$('CurrentProjectID').value);
                }
            }
            else
            {
                eventElement.selectDiv = new selectDiv(eventElement);
                eventElement.selectDiv.createContainerDiv();
                createEnvelopDiv(eventElement.selectDiv.ContainerDiv.id);
                eventElement.searchValue = newSearchValue;
                xajax_xCreateSelectDiv(eId,eventElement.searchValue, eventElement.loadFunction, xajax.$('CurrentProjectID').value);
            }
        }
    }
}


String.prototype.trim  =  function()
{
    return  this.replace(/(^\s*)|(\s*$)/g,  "");
}

String.prototype.ltrim  =  function()
{
    return  this.replace(/(^\s*)/g,  "");
}
String.prototype.rtrim  =  function()
{
    return  this.replace(/(\s*$)/g,  "");
}


function getCursPos(objId)
{
    var obj = xajax.$(objId);
    if(obj.setSelectionRange)
    {
        startPos = obj.selectionStart;
        endPos = obj.selectionEnd;
        i= startPos;
        obj.currentCursPos = i;
    }
    else
    {
        obj.focus();
        var r = document.selection.createRange();
        var i;
        var ivalue = "%$#234@@#%$%^%&%$%&adssfa3#2xixi3fdaf9^^adff&$%^**&*&";
        r.text = ivalue;
        i = obj.value.indexOf(ivalue);
        r.moveStart("character", -ivalue.length);
        r.text = "";
        obj.currentCursPos = i;
    }
    return obj.currentCursPos;
}

function getInputSearchValueByComma(objId)
{
    var obj = xajax.$(objId);
    var splitStr = ',';
    var lastCommaPos = obj.value.indexOf(splitStr);
    var cursPos = getCursPos(objId);
    if(lastCommaPos == -1)
    {
        newStartPos = -1;
    }
    else
    {
        tempLastCommaPos = lastCommaPos;
        while(lastCommaPos < cursPos && lastCommaPos != -1)
        {
            tempLastCommaPos = lastCommaPos;
            lastCommaPos = obj.value.indexOf(splitStr,lastCommaPos+1)
        }
        lastCommaPos = tempLastCommaPos;
        newStartPos = lastCommaPos+1;
    }
    newEndPos = cursPos;
    if(typeof(obj.value[newEndPos]) != "undefined" && typeof(obj.value[newEndPos] != splitStr))
    {
        return null;
    }
    return obj.value.substring(newStartPos,newEndPos);
}

function setValueByComma(objId,value)
{
    var obj = xajax.$(objId);
    obj.selectDiv.hidden();
    obj.selectDiv.optionKeyList = '';
    var splitStr = ',';
    var lastCommaPos = obj.value.indexOf(splitStr);
    var cursPos = obj.currentCursPos;
    if(lastCommaPos == -1)
    {
        newStartPos = -1;
    }
    else
    {
        tempLastCommaPos = lastCommaPos;
        while(lastCommaPos < cursPos && lastCommaPos != -1)
        {
            tempLastCommaPos = lastCommaPos;
            lastCommaPos = obj.value.indexOf(splitStr,lastCommaPos+1);
        }
        lastCommaPos = tempLastCommaPos;
        newStartPos = lastCommaPos+1;
    }
    newEndPos = cursPos+1;
    replaceInputValueByPos(objId,newStartPos,newEndPos,value+splitStr);
}

function initSelectDiv(objId,loadFunction,getSearchValueFunction,setValueFunction,reLoadData)
{
    if(typeof(reLoadData) == "undefined")
    {
        reLoadData = 0;
    }
    obj = xajax.$(objId);
    obj.reLoadData = reLoadData;
    obj.loadFunction = loadFunction;
    obj.getSearchValueFunction = getSearchValueFunction;
    obj.setValueFunction = setValueFunction;
    xajax.addHandler(objId,'onkeyup','showDivList');
    xajax.$(objId).onblur = function(){
        try{
            setTimeout("hiddenObj('"+this.selectDiv.id+"')",300);
            setTimeout("createEnvelopDiv('"+this.selectDiv.id+"')",300);
        }catch(e){}
    };
}

function hiddenObj(objId)
{
    var objIdList=objId.split(',');
    for(i=0;i<objIdList.length;i++)
    {
        xajax.$(objIdList[i]).style.display = "none";
    }
}

function displayObj(objId)
{
    var objIdList=objId.split(',');
    for(i=0;i<objIdList.length;i++)
    {
        xajax.$(objIdList[i]).style.display = "block";
    }
}

function disableObj(objId)
{
    var objIdList=objId.split(',');
    for(i=0;i<objIdList.length;i++)
    {
        xajax.$(objIdList[i]).style.disabled = "disabled";
    }
}


function replaceInputValueByPos(objId, startPos, endPos, replaceValue)
{
    obj = xajax.$(objId);
    if(startPos < 0)
    {
        startPos = 0;
    }
    if(endPos < 0)
    {
        endPos = 0;
    }
    if(obj.createTextRange)
    {
        var range = obj.createTextRange();
        range.collapse(true);
        range.moveEnd('character',endPos);
        range.moveStart('character',startPos);
        range.select();
        range.text = replaceValue;
    }
    else if(window.getSelection)
    {
        var oldValue = obj.value;
        var oldLength = obj.value.length;
        if(oldLength > 0)
        {
            var firstPart = oldValue.substring(0,startPos);
            var lastPart = oldValue.substring(endPos,oldLength);
            var middlePart = "";
            obj.value = firstPart + replaceValue + lastPart;
        }

    }
    focusInputPos(objId,startPos+replaceValue.length);
}

function focusInputRange(objId,startPos,endPos)
{
    obj = xajax.$(objId);
    if(obj.createTextRange)
    {
        var range = obj.createTextRange();
        range = obj.createTextRange();
        range.collapse(true);
        range.moveEnd('character',endPos);
        range.moveStart('character',startPos);
        range.select();
    }
    else if(obj.setSelectionRange)
    {
        obj.setSelectionRange(startPos,endPos);
        obj.focus();
    }
}

function focusInputPos(objId,pos)
{
    focusInputRange(objId,pos,pos);
}

function focusInputEndPos(objId)
{
    var obj = xajax.$(objId);
    focusInputRange(objId,obj.value.length,obj.value.length);
}

function submitForm(formId)
{
    eval("document."+formId+".submit();");
}


/**
 * create an select.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function createSelect(SelectName,TextArray,ValueArray,SelectedValue)
{
    SelectString  = '<select name="' + SelectName + '" id="' + SelectName + '" class="FullSelect">';
    for(I = 0; I <= TextArray.length - 1;I ++)
    {
        Selected = '';
        if(SelectedValue != '' && ValueArray[I] == SelectedValue)
        {
           Selected = ' selected="selected" ';
        }
        SelectString += '<option value="' + ValueArray[I] + '"'+ Selected +'>' + TextArray[I] + '</option>';
    }
    SelectString += '</select>';
    return SelectString;
}


/**
 * set the Operator list to select correct item according to the query field selected.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function setQueryOperator(I,Operator)
{
    var FieldName = xajax.$('Field'+I).value;
    var tmpStr = '';
    for(j=0;j<FieldType.length;j++)
    {
        tmpStr = FieldType[j];
        if(tmpStr.indexOf(FieldName) >= 0)
        {
            delAllItem(xajax.$("Operator"+I));
            addOptionByTextValue(xajax.$("Operator"+I), FieldOperationTypeText[j].split(','),FieldOperationTypeValue[j].split(','));
            break;
        }
    }

  
    if(Operator!='')
       selectOneItem(xajax.$("Operator" + I),Operator);
    
    return;

    //if (FieldName.match("Title|Path"))
    //{
    //    selectOneItem(xajax.$("Operator" + I),"LIKE");
    //}
    //else
    //{
    //    selectOneItem(xajax.$("Operator" + I),"=");
    //}
}

function setQueryValue(I, Reset,ValueListSelectItem)
{
    var tdObj = xajax.$('ValueTd'+I);
    var FieldName = xajax.$('Field'+I).value;
    var FieldValue = xajax.$('Value'+I).value;
    
    if(typeof(Reset) != "undefined" && Reset == 'reset')
    {
        FieldValue = '';
    }

    if(!xajax.$('AutoComplete').checked)
    {
        tdObj.innerHTML = createInput("Value" + I,FieldValue);
        return true;
    }
        
    if (FieldName.match("ProjectName") && (xajax.$('Operator'+I).value == '=' || xajax.$('Operator'+I).value == '!='))
    {
        tdObj.innerHTML = createSelect("Value" + I,ProjectNameText,ProjectNameValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("ScriptStatus"))
    {
        tdObj.innerHTML = createSelect("Value" + I,ScriptStatusText,ScriptStatusValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("ScriptedBy"))
    {
        tdObj.innerHTML = createSelect("Value" + I,ScriptedByText,ScriptedByValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("BugStatus|CaseStatus|ResultStatus"))
    {
        tdObj.innerHTML = createSelect("Value" + I,StatusText,StatusValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("Severity") && (xajax.$('Operator'+I).value != 'IN'))
    {
        tdObj.innerHTML = createSelect("Value" + I,SeverityText,SeverityValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("BugType|CaseType"))
    {
        tdObj.innerHTML = createSelect("Value" + I,TypeText,TypeValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("BugOS|ResultOS"))
    {
        tdObj.innerHTML = createSelect("Value" + I,OSText,OSValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("BugBrowser|ResultBrowser"))
    {
        tdObj.innerHTML = createSelect("Value" + I,BrowserText,BrowserValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("Resolution"))
    {

        tdObj.innerHTML = createSelect("Value" + I,ResolutionText,ResolutionValue,FieldValue);
        if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("HowFound"))
    {
        tdObj.innerHTML = createSelect("Value" + I,HowFoundText,HowFoundValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("SubStatus"))
    {
        tdObj.innerHTML = createSelect("Value" + I,SubStatusText,SubStatusValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("OpenedBy|ResolvedBy|ClosedBy|LastEditedBy|ModifiedBy|MailTo") && (xajax.$('Operator'+I).value != 'IN'))
    {
        tdObj.innerHTML = createSelect("Value" + I,UserText,UserValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("AssignedTo"))
    {
        tdObj.innerHTML = createSelect("Value" + I,ACUserText,ACUserValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
//    else if (FieldName.match("Date"))
//    {
//        if(FieldValue == '') FieldValue = getDateTime();
//        tdObj.innerHTML = createInput("Value" + I,FieldValue,getDateTime());
//
//
//    }
    else if (FieldName.match("Priority") && (xajax.$('Operator'+I).value != 'IN'))
    {
        tdObj.innerHTML = createSelect("Value" + I,PriorityText,PriorityValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("Method"))
    {
        tdObj.innerHTML = createSelect("Value" + I,MethodText,MethodValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("Plan"))
    {
        tdObj.innerHTML = createSelect("Value" + I,PlanText,PlanValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("MarkForDeletion"))
    {
        tdObj.innerHTML = createSelect("Value" + I,MarkForDeletionText,MarkForDeletionValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else if (FieldName.match("ResultValue"))
    {
        tdObj.innerHTML = createSelect("Value" + I,ResultValueText,ResultValueValue,FieldValue);
	if(ValueListSelectItem!='')
	{
            selectOneItem(xajax.$("Value" + I),ValueListSelectItem);
	}
    }
    else
    {
        tdObj.innerHTML = createInput("Value" + I,FieldValue);
        if(ValueListSelectItem!=undefined)
	xajax.$("Value" + I).value = ValueListSelectItem;
    }
}

/**
 * create an input.
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function createInput(InputName,InputValue, Title)
{
    if(typeof(Title) == 'undefined') Title = '';
    return '<input type="text" title="' + Title + '" name="' + InputName + '"  id="' + InputName + '" value="' + InputValue + '"' + ' size=5 style="width:95%">';
}

/**
 * Get current datetime
 *
 * @author  Chunsheng Wang <wwccss@263.net>
 */
function getDateTime()
{
    objDate = new Date();
    var tempMonth = objDate.getMonth()+1;
    var tempDay = objDate.getDate();
    if(tempMonth < 10) tempMonth = "0"+""+tempMonth;
    if(tempDay < 10) tempDay = "0"+""+tempDay;
    //return objDate.getFullYear() + "-" + tempMonth + "-" + tempDay + " " + objDate.getHours() + ":" + objDate.getMinutes();
    return objDate.getFullYear() + "-" + tempMonth + "-" + tempDay ;

}

function setQueryForm(I,Operator,ValueListSelectItem)
{

    setQueryOperator(I,Operator);
    setQueryValue(I,'reset',ValueListSelectItem);
}

function superAddObjValue(objID,addValue)
{
    xajax.$(objID).value += ',' + addValue;
}

function openWindow(Url,WindowName,Width,Height)
{
    if(typeof(WindowName) == 'undefined')
    {
        WindowName = 'newTestWindow';
    }
    if(typeof(Width) == 'undefined' || typeof(Height) == 'undefined')
    {
        var opened=window.open(Url,WindowName,'height='+screen.width+',width='+screen.width+', toolbar=yes, menubar=yes, scrollbars=yes, resizable=yes,location=yes, status=yes');
    }
    else
    {
        //var opened=window.open(Url,WindowName,'height='+Height+', width='+Width+', toolbar=no, menubar=no, scrollbars=yes, resizable=no,location=no, status=no');
        var opened=window.open(Url,WindowName,'height='+Height+', width='+Width);
    }
    opened.focus();

}

/*============================Cookie===========================================*/
var TestCookie =
{
    read: function (name)
    {
        var arrCookies = document.cookie.split ('; ');
        for (var i=0; i<arrCookies.length; i++)
        {
            var arrCookie = arrCookies[i].split ('=');

            if (arrCookie[0] == name)
            {
                return decodeURIComponent (arrCookie[1]);
            }
        }
        return false;
    },

    write: function (name, value, expires, path)
    {
        if (expires)
        {
            var date = new Date ();
            date.setTime (date.getTime () + (((((expires * 24) * 60) * 60) * 1000)));
            expires = '; expires=' + date.toGMTString ();
        }
        else expires = '';

        if (!path) path = '/';

        document.cookie = name+'='+encodeURIComponent (value)+expires+'; path='+path;
    },

    remove: function (name)
    {
        this.write (name, '', -1);
    }
}

function setConfirmExitArrays()
{
    for(var i = 0; i < ConfirmIds.length; i++)
    {
        var elem = xajax.$(ConfirmIds[i]);
        if (elem)
        {
            if(elem.type == 'checkbox' || elem.type == 'radio')
            {
                ConfirmValues[i] = elem.checked;
            }
            else
            {
                ConfirmValues[i] = elem.value;
            }
        }
    }
}

function confirmWhenExit()
{
    if(NeedToConfirm)
    {
        for (var i = 0; i < ConfirmValues.length; i++)
        {
            var elem = xajax.$(ConfirmIds[i]);
            if(elem)
            {
                if((elem.type == 'checkbox' || elem.type == 'radio') && ConfirmValues[i] != elem.checked) return ConfirmText;
                else if (!(elem.type == 'checkbox' || elem.type == 'radio') && elem.value != ConfirmValues[i]) return ConfirmText;
            }
        }
    }
}

function setNeedConfirm()
{
    var tempSetNeedConfirm=function(){NeedToConfirm=true;}
    setTimeout(tempSetNeedConfirm,300);
}


function getKeyCode(e)
{
  if (document.all) { return event.keyCode; }
  else if (document.getElementById) { return (e.keyCode != 0) ? e.keyCode : e.charCode; }
  // else if (document.getElementById) { return e.keyCode; }
  else if (document.layers) { return e.which; }
}

function showGotoBCR(e)
{
  e = (e) ? e : ((window.event) ? window.event : "");
  var keyCode =  getKeyCode(e);
  if(e.ctrlKey && keyCode == 71)
  {
    setTimeout('showGotoBCRPrompt()',200);
    return false;
  }
}

function showGotoBCRPrompt()
{
  var id = prompt('Enter the '+TestMode+' ID:', '');
  if(id !== null && /^[1-9][0-9]*$/.test(id))
  {
    openWindow(TestMode+'.php?'+TestMode+'ID='+id, TestMode+'ID'+id);
  }
  return true;
}

function initShowGotoBCR()
{
//document.body.onkeydown=showGotoBCR;
//document.documentElement.onkeydown=showGotoBCR;
document.onkeydown=showGotoBCR;
}
