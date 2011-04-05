<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * page class.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */

class Page
{
    /**
     * MySQL连接操作符
     * @var resource
     */
     var $_MyLink;

    /**
     * 记录总数。
     * @var int
     */
     var $_RecTotal;

    /**
     * 每页显示的记录数。
     * @var int
     */
     var $_RecPerPage;

    /**
     * 分页总数。
     * @var int
     */
     var $_PageTotal;

    /**
     * 当前页面编号。
     * @var int
     */
     var $_PageID;

    /**
     * 链接地址，此地址包含分页信息，比如PageID，PageTotal等。
     * @var string
     */
     var $_URL;

    /**
     * 用于前台跳转用的URL地址，此地址里面不包含分页信息。
     * @var string
     */
     var $_PartURL;

    /**
     * 表名。
     * @var string
     */
     var $_TableName;

    /**
     * 查询条件。
     * @var string
     */
     var $_Where;

    /**
     * 初始化分页类。
     *
     * @param  string   $TableName   要查询的表名，如果是两个表，可以使用逗号隔开。但在where条件里面必须指定连接条件。
     * @param  int      $RecTotal    记录总数。
     * @param  int      $RecPerPage  每页显示的记录数。
     * @param  int      $PageID      当前分页编号。
     * @param  string   $Where       查询条件。
     * @param  string   $URL         链接条件。
     * @param  resource $MyLink      连接操作符。
     */
    function page($TableName, $RecTotal = "", $RecPerPage = 20, $PageID = 0, $Where = "", $URL="", $MyLink = '')
    {
        $TableName = dbGetPrefixTableNames($TableName);

        /* 设定数据库参数。*/
        $this->_MyLink     = $MyLink;
        $this->_TableName  = !empty($TableName) ? $TableName : die("请指定表名");
        $this->_Where      = $Where;

        /* 设定记录总数、每页显示数、分页总数和当前分页ID。*/
        $this->_RecTotal   = ($RecTotal   > 0) ? $RecTotal   : $this->getRecTotal();
        $RecPerPage = $RecPerPage > 0 ? $RecPerPage : $_GET['RecPerPage'];
        $this->_RecPerPage = ($RecPerPage > 0) ? $RecPerPage : ($_COOKIE['RecPerPageNum']>0 ? $_COOKIE['RecPerPageNum'] : 20);
        setcookie('RecPerPageNum', $this->_RecPerPage, time()+2592000);
        $this->setPageTotal();
        $this->setPageID($PageID);

        /* 设定url参数：_URL 和_PartURL。*/
        $this->setURL($URL);

        /* 传递记录总数。*/
        if(!preg_match("/\?/", $this->_URL))
        {
            $this->_URL     .= "?RecTotal=" . $this->_RecTotal . "&RecPerPage=$this->_RecPerPage&";
            $this->_PartURL .= "?RecTotal=" . $this->_RecTotal . "&";
        }
        else
        {
            $this->_URL     .= "&RecTotal=" . $this->_RecTotal . "&RecPerPage=$this->_RecPerPage&";
            $this->_PartURL .= "&RecTotal=" . $this->_RecTotal . "&";
        }
    }

    /* 取得某一条件查询结果的记录总数。*/
    function getRecTotal()
    {
        $SQL      = "SELECT COUNT(*) AS RecTotal FROM $this->_TableName $this->_Where";
        $Data = $this->_MyLink->GetRow($SQL);
        $this->_RecTotal = $Data['RecTotal'];
        return $this->_RecTotal;
    }

    /* 设定当前记录编号。*/
    function setPageID($PageID)
    {
        $PageID = $PageID > 0 ? $PageID : $_GET['PageID'];
        if($PageID > 0 and $PageID <= $this->_PageTotal)
        {
            $this->_PageID = $PageID;
        }
        else
        {
            $this->_PageID = 1;
        }
    }

    /* 获得分页总数。*/
    function setPageTotal()
    {
        $this->_PageTotal = ceil($this->_RecTotal / $this->_RecPerPage);
        return $this->_PageTotal;
    }

    /**
     * 设定URL地址。
     *
     * 如果没有指定$URL，则从$_SERVER["QUERY_STRING"]里面将要传递的参数解析出来，去掉PageID和PageTotal项，再拼成完成的URL地址。
     *
     * @bug                 如果传递的参数中含有数组，即var[]=xx&var[2]=xx，会有问题。:)
     * @param  string  $URL 调用此分页类时可以手工指定URL地址。
     * @return int          记录分页总数。
     */
     function setURL($URL = "")
     {
         if(!empty($URL))
         {
             $this->_URL = $URL;
             $this->_PartURL = $this->_URL;
             return;
         }

         /* 取当前程序的地址作为URL的开始部分。*/
         $this->_URL = $_SERVER["PHP_SELF"];

         /* 分析QUERY_STRING，去掉PageID, RecTotal和空值的变量。*/
         parse_str($_SERVER["QUERY_STRING"], $QueryList);
         foreach($QueryList as $QueryName => $QueryValue)
         {
             if(preg_match("/(PageID|RecTotal|RecPerPage)/i", $QueryName) or empty($QueryValue))
             {
                 unset($QueryList[$QueryName]);
             }
         }

         /* 拼凑成完成的URL地址。*/
         if(!empty($QueryList))
         {
             foreach($QueryList as $QueryName => $QueryValue)
             {
                 if(preg_match("/\?/", $this->_URL))
                 {
                     $this->_URL .= "&" . $QueryName . "=" . $QueryValue;
                 }
                 else
                 {
                     $this->_URL .= "?" . $QueryName . "=" . $QueryValue;
                 }
             }
         }

         $this->_PartURL = $this->_URL;
     }

    /**
     * 显示查询结果的分页链接。
     *
     * @param  string $Align  对其方式，默认为居中对齐。left|center|right
     * @return string         返回导航条文字。
     */
    function show($Align = "left", $Css = '')
    {
        global $_LANG;
        $PageLang = $_LANG['Pagination'];

        /* 定义链接符号。*/
        $FirstPage = $PageLang['FirstPage'];
        $EndPage   = $PageLang['EndPage'];
        $PrePage   = $PageLang['PrePage'];
        $NextPage  = $PageLang['NextPage'];

        /* 如果页码总数为零，则直接退出。*/
        if($this->_RecTotal == 0)
        {
            return $PageLang['NoResult'];
        }

        /* 跳转页面的js函数。*/
        $PageURL = "'" . $this->_PartURL . "PageID=' + PageID + '&RecPerPage=' + RecPerPage";
        $LinkString = <<<EOT
        <script language='Javascript'>
        function submitPage(mode)
        {
            PageTotal     = parseInt(document.getElementById('_PageTotal').value);
            if(mode == 'changePageID')
            {
                PageID        =  document.getElementById('_PageID').value;
                if(PageID > PageTotal)
                {
                    PageID = PageTotal;
                }
                else if(PageID < 1)
                {
                    PageID = 1;
                }
            }
            else if(mode == 'changeRecPerPage')
            {
                PageID = 0;
            }
            RecPerPage    = document.getElementById('_RecPerPage').value;
            location.href = $PageURL;
        }
        </script>
EOT;

        /* 手工指定每页显示数。*/
        $RecPerPageRange[10] = 10;
        $RecPerPageRange[20] = 20;
        $RecPerPageRange[35] = 35;
        $RecPerPageRange[50] = 50;
        $RecPerPageRange[100] = 100;

        $RecPerPageList = htmlSelect($RecPerPageRange, "_RecPerPage", "Reverse", $this->_RecPerPage, "onchange='submitPage(\"changeRecPerPage\");'");

        /* 显示记录总数。*/
        $LinkString .= "{$PageLang['TotalRecord']}<strong>$this->_RecTotal</strong>&nbsp;";
        $LinkString .= "$RecPerPageList{$PageLang['NumPerPage']}&nbsp;";
        $LinkString .= "{$PageLang['PageNum']}<strong>$this->_PageID</strong>/<strong>$this->_PageTotal</strong>&nbsp;";

         /* 生成第一页的链接。 */
        if($this->_PageID == 1)
        {
            $LinkString .= $FirstPage;
        }
        else
        {
            $LinkString .= $this->link($FirstPage, $this->_URL . "PageID=1");
        }

        /* 生成前页的链接。*/
        if($this->_PageID == 1)
        {
            $LinkString .= $PrePage;
        }
        else
        {
            $LinkString .= $this->link($PrePage, $this->_URL . "PageID=" . ($this->_PageID - 1));
        }

        /* 生成后页的链接。*/
        if($this->_PageID == $this->_PageTotal)
        {
             $LinkString .= $NextPage;
        }
        else
        {
            $LinkString .= $this->link($NextPage, $this->_URL . "PageID=" . ($this->_PageID + 1));
        }

        /* 生成最后一页的链接。*/
        if($this->_PageID == $this->_PageTotal)
        {
            $LinkString .= $EndPage;
        }
        else
        {
            $LinkString .= $this->link($EndPage, $this->_URL . "PageID=" . $this->_PageTotal);
        }

        /* 生成手工跳转文本框。*/
        $LinkString .= "<input type='hidden' id='_PageTotal' value='$this->_PageTotal' />
                        <input type='text'   id='_PageID'    value='$this->_PageID' style='text-align:center;width:30px;height:15px;font-size:11px;' />
                        <input type='button' id='goto'       value='Go' onclick='submitPage(\"changePageID\");' style='font-size:11px;'/>";

        return "<div id='Pagination' style='float:$Align; clear:none; $Css'>$LinkString</div>";
    }

    /**
     * 生成SQL查询语句的Limit部分
     *
     * @return string  Limit语句。
     */
    function Limit()
    {
        return " LIMIT " . $this->LimitNum();
    }

    function LimitNum()
    {
        /* 保证最后一页查出的记录数为RecPerPage。加上不等于1的限制是为了防止出现limit -20,10这样的情况。*/
        if($this->_RecTotal < $this->_RecPerPage)
        {
	        $Limit = "0," . $this->_RecTotal;
        }
        //elseif($this->_PageID == $this->_PageTotal and $this->_PageTotal != 1)
        //{
        //    $Limit =  ($this->_RecTotal - $this->_RecPerPage) . "," . $this->_RecPerPage;
        //}
        else
        {
            $Limit =  $this->_RecPerPage * ($this->_PageID - 1) . "," . $this->_RecPerPage;
        }
        return $Limit;
    }

   /**
    * 生成<a href="">text</a>标记。
    *
    * @param string $Text	    链接文字。
    * @param string $URL	    链接地址。
    */
    function link($Text,$URL)
    {
        $LinkString = "<a href='".$URL."'>".$Text."</A>";
        return $LinkString;
    }

    function getDetailInfo()
    {

        global $_LANG;
        $PageLang = $_LANG['Pagination'];

        /* 定义链接符号。*/
        $PrePage   = $PageLang['PrePage'];
        $NextPage  = $PageLang['NextPage'];

        $DetailInfo = array();

        /* 跳转页面的js函数。*/
        $PageURL = "'" . $this->_PartURL . "PageID=' + PageID + '&RecPerPage=' + RecPerPage";
        $LinkString = <<<EOT
        <script language='Javascript'>
        function submitPage(mode)
        {
            PageTotal     = parseInt(document.getElementById('_PageTotal').value);
            if(mode == 'changePageID')
            {
                PageID        =  document.getElementById('_PageID').value;
                if(PageID > PageTotal)
                {
                    PageID = PageTotal;
                }
                else if(PageID < 1)
                {
                    PageID = 1;
                }
            }
            else if(mode == 'changeRecPerPage')
            {
                PageID = 0;
            }
            RecPerPage    = document.getElementById('_RecPerPage').value;
            location.href = $PageURL;
        }
        </script>
<input type='hidden' id='_PageTotal' value='$this->_PageTotal' />
EOT;

        /* 手工指定每页显示数。*/
        $RecPerPageRange[10] = 10;
        $RecPerPageRange[20] = 20;
        $RecPerPageRange[35] = 35;
        $RecPerPageRange[50] = 50;
        $RecPerPageRange[100] = 100;

        $RecPerPageList = htmlSelect($RecPerPageRange, "_RecPerPage", "Reverse", $this->_RecPerPage, "onchange='submitPage(\"changeRecPerPage\");'");

        $DetailInfo['RecPerPageList'] = $LinkString . $RecPerPageList;

        /* 生成前页的链接。*/
        if($this->_PageID == 1)
        {
            $DetailInfo['PrePageSymLink'] = '<-';
            $DetailInfo['PrePageSymImgLink'] = '<img src="Image/arrow_left_disabled.gif" />';
            $DetailInfo['PrePageLiteralLink']  = $PrePage;
        }
        else
        {
            $DetailInfo['PrePageSymLink'] = $this->link('<-', $this->_URL . "PageID=" . ($this->_PageID - 1));
            $DetailInfo['PrePageSymImgLink'] = $this->link('<img src="Image/arrow_left_enabled.gif" />', $this->_URL . "PageID=" . ($this->_PageID - 1));
            $DetailInfo['PrePageLiteralLink'] = $this->link($PrePage, $this->_URL . "PageID=" . ($this->_PageID - 1));
        }

        /* 生成后页的链接。*/
        if($this->_PageID == $this->_PageTotal || $this->_PageTotal == 0)
        {
             $DetailInfo['NextPageSymLink']  = '->';
             $DetailInfo['NextPageSymImgLink']  = '<img src="Image/arrow_right_disabled.gif" />';
             $DetailInfo['NextPageLiteralLink']  = $NextPage;
        }
        else
        {
            $DetailInfo['NextPageSymLink'] = $this->link('->', $this->_URL . "PageID=" . ($this->_PageID + 1));
            $DetailInfo['NextPageSymImgLink'] = $this->link('<img src="Image/arrow_right_enabled.gif" />', $this->_URL . "PageID=" . ($this->_PageID + 1));
            $DetailInfo['NextPageLiteralLink'] = $this->link($NextPage, $this->_URL . "PageID=" . ($this->_PageID + 1));
        }

        if($this->_RecTotal < $this->_RecPerPage)
        {
            if($this->_RecTotal == 0)
            {
                $DetailInfo['FromNum'] = 0;
            }
            else
            {
                $DetailInfo['FromNum'] = 1;
            }
            $DetailInfo['ToNum'] = $this->_RecTotal;
        }
        elseif($this->_PageID == $this->_PageTotal and $this->_PageTotal != 1)
        {
            $DetailInfo['FromNum'] = $this->_RecPerPage * ($this->_PageID - 1) + 1;
            $DetailInfo['ToNum'] = $this->_RecTotal;
        }
        else
        {
            $DetailInfo['FromNum'] = $this->_RecPerPage * ($this->_PageID - 1) + 1;
            $DetailInfo['ToNum'] = $DetailInfo['FromNum'] + $this->_RecPerPage - 1;
        }
        $DetailInfo['RecTotal'] = $this->_RecTotal;

        //$DetailInfo['NextLink'] =
        return $DetailInfo;
    }
}
?>
