function getObject(o)
{
    var o=document.getElementById(o)?document.getElementById(o):o;
    return o;
}
function changeClass(o,a,b)
{
    var o=getObject(o);
    o=o.parentNode;
    o.className=o.className==a?b:a;
}
function createTreeMenu(id)
{
    this.OpenClass = typeof(OpenClass) == "undefined" ? this.OpenClass = "OpenedNode":"";
    this.ClosedClass = typeof(ClosedClass) == "undefined" ? this.ClosedClass = "ClosedNode":"";
    this.ChildClass = typeof(ChildClass) == "undefined" ? this.ChildClass = "ChildNode":"";

    this.ULTreeMenu = getObject(id);
    this.ULTreeMenu.className += " bugTreeMenu";

    this.UlNode = getObject(id).getElementsByTagName("ul");
    if(this.UlNode.length == 0)
    {
        return;
    }
    this.FirstUlNode = this.UlNode[0];
    this.FirstUlNode.style.marginLeft = 0;

    this.AllNodes = getObject(id).getElementsByTagName("li");
    if(typeof(InAdministration) != "undefined" && InAdministration)
    {
        this.NodeImgUrl="../Image/TreeMenu/blank.gif";
    }
    else
    {
        this.NodeImgUrl="Image/TreeMenu/blank.gif";
    }
    this.LeafImgBlank ="<img src=\""+this.NodeImgUrl+"\" class=\"forClick\" />";
    this.RamoseImgBlank ="<img src=\""+this.NodeImgUrl+"\" class=\"forClick\" onclick=\"changeClass(this,'"+this.OpenClass+"','"+this.ClosedClass+"');\" />";
    for (i=0;i<this.AllNodes.length;i++ )
    {
        ChildCount = this.AllNodes[i].getElementsByTagName("li").length;
        this.AllNodes[i].innerHTML=(ChildCount == 0?this.LeafImgBlank:this.RamoseImgBlank)+this.AllNodes[i].innerHTML;
        if(this.AllNodes[i].className=="" && ChildCount == 0)
        {
            this.AllNodes[i].className=this.ChildClass;
        }
        else if(this.AllNodes[i].className=="" && ChildCount > 0)
        {
            this.AllNodes[i].className=this.ClosedClass;
        }
    }
}

function treeMenuChildAction(parentID,childId)
{
    var Elements = document.getElementById(parentID).getElementsByTagName("*");
    var i;
    var tempTagName;
    for(i in Elements)
    {
        tempTagName = Elements[i].tagName;
        if(typeof (tempTagName) != "undefined" && tempTagName.toLowerCase() == 'a')
        {
            if(Elements[i].id == childId)
            {
                Elements[i].style.fontWeight = 'bold';
            }
            else if(Elements[i].style.fontWeight == 'bold')
            {
                Elements[i].style.fontWeight = 'normal';
            }
        }
    }
}
