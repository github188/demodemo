<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * Common functions library of BugFree system.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */

//------------------------- SYSTEM FUNCTIONS -----------------------------------//
/**
 * Close database connection
 *
 * @author                  Yupeng Lee <leeyupeng@gmail.com>
 */
function sysCloseDB()
{
    global $MyDB, $MyUserDB, $_CFG;
    $MyDB->Close();
    if(!empty($_CFG['UserDB']))
    {
        $MyUserDB->Close();
    }
}

/**
 * Check database exists
 *
 * @author                  Yupeng Lee <leeyupeng@gmail.com>
 * @param   string   $DatabaseName
 * @return  bool
 */
function sysCheckDBExists($DatabaseName)
{
    global $_CFG;
    $DBExists = false;

    $link = @mysql_connect($_CFG['DB']['Host'], $_CFG['DB']['User'], $_CFG['DB']['Password']);
    $db_list = @mysql_list_dbs($link);

    while ($row = @mysql_fetch_object($db_list)) {
        if($DatabaseName == $row->Database)
        {
            $DBExists = false;
            break;
        }
    }

    return $DBExists;
}

/**
 * Echo system error
 *
 * @author                   Yupeng Lee <leeyupeng@gmail.com>
 */
function sysErrorMsg()
{
    global $TPL;
    $TPL->display('SysErrorMsg.tpl');
    exit;
}

/**
 * Return part of a string(Enhance the function substr())
 *
 * @author                  Chunsheng Wang <wwccss@263.net>
 * @global array                 the bug config array.
 * @param  string  $String  the string to cut.
 * @param  int     $Length  the length of returned string.
 * @param  booble  $Append  whether append "...": false|true
 * @return string           the cutted string.
 */
function sysSubStr($String,$Length,$Append = false)
{
    global $_CFG;

    $I = 0;
    $Count = 0;
    if($_CFG["DefaultLang"]  == "ZH_CN_UTF-8")
    {
        while ($Count < $Length)
        {
            $StringTMP = substr($String,$I,1);
            if ( ord($StringTMP) >=224 )
            {
                $StringTMP = substr($String,$I,3);
                $I = $I + 3;
                $Count += 2;
            }
            elseif( ord($StringTMP) >=192 )
            {
                $StringTMP = substr($String,$I,2);
                $I = $I + 2;
                $Count ++;
            }
            else
            {
                $I = $I + 1;
                $Count ++;
            }
            $StringLast[] = $StringTMP;
        }
        if($Count == $Length)
        {
            array_pop($StringLast);
        }
        $StringLast = implode("",$StringLast);
        if($Append && $String != $StringLast)
        {
            $StringLast .= "...";
        }
        return $StringLast;
    }
    else
    {
        while ($Count < $Length)
        {
            $StringTMP = substr($String,$I,1);
            if( ord($StringTMP) >=128 )
            {
                $StringTMP = substr($String,$I,2);
                $I = $I + 2;
                $Count += 2;
            }
            else
            {
                $I = $I + 1;
                $Count ++;
            }
            $StringLast[] = $StringTMP;
        }
        if($Count == $Length)
        {
            array_pop($StringLast);
        }
        $StringLast = implode("",$StringLast);
        if($Append && $String != $StringLast)
        {
            $StringLast .= "...";
        }
        return $StringLast;
    }
}

/**
 * Return Pinyin of a string
 *
 * @author                  LeeYupeng <leeyupeng@gmail.com>
 * @param  string  $String  the string to pinyin.
 * @return string           the pinyin string.
 */
function sysPinYinStr($String)
{
    global $_CFG;
    $PinYinString = '';
    $I = 0;
    $Length = strlen($String);
    $StringLast = array();
    while ($I < $Length)
    {
        $StringTMP = substr($String,$I,1);
        if (ord($StringTMP) >=224 )
        {
            $StringTMP = substr($String,$I,3);
            $I = $I + 3;
        }
        elseif(ord($StringTMP) >=192 )
        {
            $StringTMP = substr($String,$I,2);
            $I = $I + 2;
        }
        else
        {
            $I = $I + 1;
        }
        if(!preg_match('/[\(\)A-za-z]/', $StringTMP))
        {
            $PinYinStringTMP = $_CFG['PinYin'][$StringTMP];
            if($PinYinStringTMP == '')
            {
                $PinYinStringTMP = $StringTMP;
            }
            $PinYinString .= $PinYinStringTMP;
        }
        //$StringLast[] = $StringTMP;
    }
    if($PinYinString == $String) return "";
    return $PinYinString;
}

/*
 *
 * @author                  Chunsheng Wang <wwccss@263.net>
 * @global array                 the bug config array.
 * @param  string  $String  the string to cut.
 * @param  int     $Length  the length of returned string.
 * @param  booble  $Append  whether append "...": false|true
 * @return string           the cutted string.
 */
function sysDbSubStr($String,$Length)
{
    global $_CFG;

    $I = 0;
    $Count = 0;
    if($_CFG["DefaultLang"]  == "ZH_CN_UTF-8")
    {
        while ($Count < $Length)
        {
            $StringTMP = substr($String,$I,1);
            if ( ord($StringTMP) >=224 )
            {
                $StringTMP = substr($String,$I,3);
                $I = $I + 3;
                $Count += 3;
            }
            elseif( ord($StringTMP) >=192 )
            {
                $StringTMP = substr($String,$I,2);
                $I = $I + 2;
                $Count +=2;
            }
            else
            {
                $I = $I + 1;
                $Count ++;
            }
            $StringLast[] = $StringTMP;
        }
        if($Count == $Length)
        {
            array_pop($StringLast);
        }
        $StringLast = implode("",$StringLast);
        if($Append && $String != $StringLast)
        {
            $StringLast .= "...";
        }
        return $StringLast;
    }
    else
    {
        while ($Count < $Length)
        {
            $StringTMP = substr($String,$I,1);
            if( ord($StringTMP) >=128 )
            {
                $StringTMP = substr($String,$I,2);
                $I = $I + 2;
                $Count += 2;
            }
            else
            {
                $I = $I + 1;
                $Count ++;
            }
            $StringLast[] = $StringTMP;
        }
        if($Count == $Length)
        {
            array_pop($StringLast);
        }
        $StringLast = implode("",$StringLast);
        if($Append && $String != $StringLast)
        {
            $StringLast .= "...";
        }
        return $StringLast;
    }
}

/**
 * Enhance the function addslashes())
 *
 * @author                  Chunsheng Wang <wwccss@263.net>
 * @param  mix     $Data    the variable to addslashes.
 * @return mix              formated variable.
 */
function sysAddSlash($Data)
{
    if(is_array($Data))
    {
        foreach($Data as $Key => $Value)
        {
            if(is_array($Value))
            {
                $Data[$Key] = sysAddSlash($Value);
            }
            else
            {
                $Data[$Key] = addslashes($Value);
            }
        }
    }
    else
    {
        $Data = addslashes($Data);
    }
    return $Data;
}

/**
 * Enhance the function stripslashes())
 *
 * @author                  Chunsheng Wang <wwccss@263.net>
 * @param  mix     $Data    the variable to stripslashes.
 * @return mix              formated variable.
 */
function sysStripSlash($Data)
{
    if(is_array($Data))
    {
        foreach($Data as $Key => $Value)
        {
            if(is_array($Value))
            {
                $Data[$Key] = sysStripSlash($Value);
            }
            else
            {
                $Data[$Key] = stripslashes($Value);
            }
        }
    }
    else
    {
        $Data = stripslashes($Data);
    }
    return $Data;
}
/**
 * Strip html tages, perserver spaces and <br /><B>, nl2br.
 *
 * @author                            Chunsheng Wang <wwccss@263.net>
 * @param   string      $Html
 * @return  string      $Html
 */
function sysHtml2Txt($Html)
{
    $Html = str_replace("<br />", "<br>", $Html);        // Change <br /> to <br>, because the space will be replaced by &nbsp.
    $Html = str_replace(" ", "&nbsp;", $Html);           // Change space to &nbsp to perserve the space.
    $Html = str_replace("<br>", "<br />", $Html);        // Change <br> to <br />��so there are only <br>, no <br />.
    $Html = strip_tags($Html, "<br><B>");                // Strip html tages except <br> and <B>.
    $Html = nl2br($Html);                                // Change newline to <br />.
    $Html = str_replace("<br /><br />","<br />", $Html); // Change two <br /> to one <br>.
    return $Html;
}
/**
 * Enhanced mail function.
 *
 * @author                            Chunsheng Wang <wwccss@263.net>
 * @param   string      $ToList       To address list.
 * @param   string      $CCList       CC address list.
 * @param   string      $Subject      Subject.
 * @param   string      $Message      Message.
 */
function sysMail($ToList,$CCList,$Subject,$Message)
{
    global $_CFG, $_LANG;

    if(!$_CFG['Mail']['On'])
    {
        return;
    }

    require_once($_CFG['RealRootPath'] . "/Include/Class/PHPMailer/class.phpmailer.php");

    // Create an object of PHPMailer class and set the send method
    $Mail = new PHPMailer();
    switch(strtoupper($_CFG["Mail"]["SendMethod"]))
    {
        case "SMTP":
            $Mail->isSMTP();
            $Mail->Host     = $_CFG["Mail"]["SendParam"]["Host"];
            $Mail->SMTPAuth = $_CFG["Mail"]["SendParam"]["SMTPAuth"];
            $Mail->Username = $_CFG["Mail"]["SendParam"]["Username"];
            $Mail->Password = $_CFG["Mail"]["SendParam"]["Password"];
            break;
        case "MAIL":
            $Mail->isMail();
            break;
        case "SENDMAIL":
            $Mail->isSendmail();
            break;
        case "QMAIL":
            $Mail->isQmail();
            break;
    }

    // Define From Address.
    $Mail->From     = $_CFG["Mail"]["FromAddress"];
    $Mail->FromName = $_CFG["Mail"]["FromName"];

    if(!is_array($ToList) && $ToList != '')
    {
        $ToList = explode(',', $ToList);
    }
    if(empty($ToList) && is_array($CCList) && !empty($CCList))
    {
        $ToList[] = array_pop($CCList);
    }
    if(empty($ToList))
    {
        return false;
    }
    foreach($ToList as $To)
    {
        $Mail->addAddress($To);
    }
    // Add To Address.
    if(is_array($CCList))
    {
        $CCList = array_diff($CCList, $ToList);
        $CCList = array_unique($CCList);
        foreach($CCList as $CC)
        {
            $Mail->addCC($CC);
        }
    }

    // Add Subject.
    $Mail->Subject  =  stripslashes($Subject);

    // Set Body.
    $Mail->IsHTML(true);
    $Mail->CharSet = $_LANG["Charset"];
    $Mail->Body    = $Message;
    if(!$Mail->Send())
    {
       //jsAlert($Mail->ErrorInfo);
    }
}

/**
 * Sort an two-dimension array by some level two items use array_multisort() function.
 *
 * sysSortArray($Array,"Key1","SORT_ASC","SORT_RETULAR","Key2"����)
 * @author                      Chunsheng Wang <wwccss@263.net>
 * @param  array   $ArrayData   the array to sort.
 * @param  string  $KeyName1    the first item to sort by.
 * @param  string  $SortOrder1  the order to sort by("SORT_ASC"|"SORT_DESC")
 * @param  string  $SortType1   the sort type("SORT_REGULAR"|"SORT_NUMERIC"|"SORT_STRING")
 * @return array                sorted array.
 */
function sysSortArray($ArrayData,$KeyName1,$SortOrder1 = "SORT_ASC",$SortType1 = "SORT_REGULAR")
{
    if(!is_array($ArrayData))
    {
        return $ArrayData;
    }

    // Get args number.
    $ArgCount = func_num_args();

    // Get keys to sort by and put them to SortRule array.
    for($I = 1;$I < $ArgCount;$I ++)
    {
        $Arg = func_get_arg($I);
        if(!preg_match("/SORT/i",$Arg))
        {
            $KeyNameList[] = $Arg;
            $SortRule[]    = '$'.$Arg;
        }
        else
        {
            $SortRule[]    = $Arg;
        }
    }

    // Get the values according to the keys and put them to array.
    foreach($ArrayData AS $Key => $Info)
    {
        foreach($KeyNameList AS $KeyName)
        {
            ${$KeyName}[$Key] = $Info[$KeyName];
        }
    }

    // Create the eval string and eval it.
    $EvalString = 'array_multisort('.join(",",$SortRule).',$ArrayData);';
    eval ($EvalString);
    return $ArrayData;
}

/**
 * Computes the difference of arrays with a lot of additional indexes check
 *
 * @author                  Yupeng Lee <leeyupeng@gmail.com>
 * @param   array  $Array1
 * @param   array  $Array2
 * @param   string $Assoc   indexes split by ,
 * @return  array           an array containing all the values from array1  that are not present in any of the other arrays
 */
function sysArrayDiffAssoc($Array1, $Array2, $Assoc)
{
    $ResultArray = array();
    $DiffArray = array_diff_assoc($Array1, $Array2);
	$AssocList = explode(',', $Assoc);
    if(!empty($DiffArray))
    {
        foreach($DiffArray as $Key => $Value)
        {
            if(in_array($Key, $AssocList))
            {
                $ResultArray[$Key] = $Value;
            }
        }
    }
    return $ResultArray;
}

/**
 * Computes the intersection of arrays with a lot of additional indexes check
 *
 * @author                  Yupeng Lee <leeyupeng@gmail.com>
 * @param   array  $Array1
 * @param   array  $Array2
 * @param   string $Assoc   indexes split by ,
 * @return  array           associative array containing all the values in array1  that are present in all of the arguments.
 */
function sysArrayIntersectAssoc($Array1, $Array2, $Assoc)
{
    $ResultArray = array();
    $IntersectArray = array_intersect_assoc($Array1, $Array2);
	$AssocList = explode(',', $Assoc);
    if(!$IntersectArray)
    {
        foreach($InersectArray as $Key => $Value)
        {
        	  $Assoc = trim($Assoc);
            if(in_array($Key, $AssocList))
            {
                $ResultArray[$Key] = $Value;
            }
        }
    }
    return $ResultArray;
}

/**
 * register function and assign to template
 *
 * @author                    Yupeng Lee<leeyupeng@gmail.com>
 * @param  string $Functions  the functions Separate with comma
 */
function sysXajaxRegister($Functions='')
{
    global $TPL, $Xajax, $_CFG;

    if($Functions)
    {
    	  $FunctionList = explode(",", $Functions);
    	
    	  foreach($FunctionList as $Function)
    	  {
    	      $Xajax->registerFunction($Function);
    	  }
    	  $Xajax->processRequests();
  	}
  	
    $TPL->assign("xajax_javascript", $Xajax->getJavascript($_CFG["BasePath"], "JS/xajax.js"));
}

//------------------------- DATABASE FUNCTIONS -----------------------------------//
/**
 * Create string like "IN('a','b')"  from "a,b"
 *
 * @author                     Chunsheng Wang <wwccss@263.net>
 * @param  string $ItemList    item list like "a,b,c".
 * @return string $ItemList    string like "IN('a','b','c')".
 */
function dbCreateIN($ItemList)
{
        $ItemList = explode(",",$ItemList);
        foreach($ItemList AS $Item)
        {
            $ItemListTMP[] = "'$Item'";
        }
        return " IN (".join(",",$ItemListTMP).") ";
}

/**
 * Merge $SourceSQL to $TargetSQL.
 *
 * @author                     Chunsheng Wang <wwccss@263.net>
 * @param  string $TargetSQL
 * @param  string $SourceSQL
 * @return string $MergedSQL
 */
function dbMergeSQL($TargetSQL, $SourceSQL)
{
    if(preg_match("/WHERE/i", $TargetSQL))
    {
        $TargetSQL = preg_replace("/where/i", "WHERE", $TargetSQL);
        $TargetSQL = explode("WHERE", $TargetSQL);
        $MergedSQL = " WHERE $SourceSQL AND" . $TargetSQL[1];
    }
    else
    {
        $MergedSQL = " WHERE $SourceSQL" . $TargetSQL;
    }
    return $MergedSQL;
}

/**
 * Get info of one filed.
 *
 * @author                     Chunsheng Wang <wwccss@263.net>
 * @global object              Object of ADO class.
 * @param  string $TableName
 * @param  string $FieldName
 * @return array  $FieldInfo
 */
function dbGetFieldInfo($TableName, $FieldName)
{
    global $MyDB;
    $ResultID = $MyDB->query("DESCRIBE $TableName $FieldName");
    if($ResultID)
    {
        $FieldInfo           = $ResultID->fetchRow();
        $SubStart            = strpos($FieldInfo["Type"], "(") +1;
        $SubEnd              = strpos($FieldInfo["Type"], ")");
        $SubDelta            = $SubEnd - $SubStart;
        $FieldInfo["Length"] = substr($FieldInfo["Type"], $SubStart, $SubDelta);
        return $FieldInfo;
    }
    else
    {
        die($MyDB->errorMsg());
    }
}

/**
 * Create select sql .
 *
 * @author                       Yupeng Lee <leeyupeng@gmail.com>
 * @param    string  $TableNames Table names split by ,
 * @param    string  $Columns    Columns split by ,
 * @param    string  $Where      SQL condition
 * @param    string  $GroupBy    SQL groupby
 * @param    string  $OrderBy    SQL order by
 * @param    string  $Limit      SQL limit m,n
 * @retrun   string              SQL
 */
function dbGetListSql($TableNames, $Columns = "", $Where = "", $GroupBy = "", $OrderBy = "", $Limit = "")
{
    $Sql = 'SELECT ';
    if(empty($Columns))
    {
        $Column = array();
        $TableList = explode(',', $TableNames);
        foreach($TableList as $TableName)
        {
            $Column[] = $TableName . '.*';
        }
        $Columns = join(',', $Column);
    }

    $Sql .= $Columns . ' FROM ' . $TableNames;

    if(!empty($Where))
    {
        $Sql .= ' WHERE ' . $Where;
    }

    if(!empty($GroupBy))
    {
        $Sql .= ' GROUP BY ' . $GroupBy;
    }

    if(!empty($OrderBy))
    {
        $Sql .= ' ORDER BY ' . $OrderBy;
    }

    if(!empty($Limit))
    {
        $Sql .= ' LIMIT ' . $Limit;
    }
    return $Sql;
}

/**
 *
 * Get table names with prefix
 *
 * @author                      Yupeng Lee<leeyupeng@gmail.com>
 * @param   string  $TableNames TableNames split by ,
 * @return  string              TableNames whith prefix split by ,
 */
function dbGetPrefixTableNames($TableNames)
{
    global $_CFG;

    if($_CFG['UserDB']['User'] != '' && $TableNames == $_CFG['UserTable']['TableName'])
    {
        return $TableNames;
    }
    $TableList = explode(',', $TableNames);
    $PrefixTableNameList = array();
    foreach($TableList as $TableName)
    {
        $TableName = trim($TableName);
        $PrefixTableNameList[] = $_CFG['DB']['TablePrefix'] . $TableName;
    }
    $PrefixTableNames = join(',', $PrefixTableNameList);

    return $PrefixTableNames;
}

/**
 * Get list info from db
 *
 * @author                       Yupeng Lee <leeyupeng@gmail.com>
 * @param    string  $TableNames Table names split by ,
 * @param    string  $Columns    Columns split by ,
 * @param    string  $Where      SQL condition
 * @param    string  $GroupBy    SQL groupby
 * @param    string  $OrderBy    SQL order by
 * @param    string  $Limit      SQL limit m,n
 * @param    string  $ListKey    return array whith index $ListKey
 * @param    string  $DB         Database name
 * @retrun   array               List array
 */
function dbGetList($TableNames, $Columns = "", $Where = "", $GroupBy = "", $OrderBy = "", $Limit = "", $ListKey = "", $DB = "")
{
    if(empty($DB))
    {
        global $MyDB;
        $DB = $MyDB;
    }

    $TableNames = dbGetPrefixTableNames($TableNames);

    $DataList = array();
    $Sql =  dbGetListSql($TableNames, $Columns, $Where, $GroupBy, $OrderBy, $Limit);
    
    if($ListKey == '')
    {
        $DataList = $DB->GetAll($Sql);
    }
    else
    {
        $rs = $DB->Execute($Sql);
        if($rs)
        {
            while(!$rs->EOF)
            {
                $Row = $rs->fields;
                $DataList[$Row[$ListKey]] = $Row;
                $rs->MoveNext();
            }
        }
    }
    if(!is_array($DataList)) return array();
    return $DataList;
}

/**
 * Get all column list info from db
 *
 * @author                       Yupeng Lee <leeyupeng@gmail.com>
 * @param    string  $TableNames Table names split by ,
 * @param    string  $Where      SQL condition
 * @param    string  $GroupBy    SQL groupby
 * @param    string  $OrderBy    SQL order by
 * @param    string  $Limit      SQL limit m,n
 * @param    string  $ListKey    return array whith index $ListKey
 * @param    string  $DB         Database name
 * @retrun   array               List array
 */
function dbGetAllColumns($TableNames, $Where = "", $GroupBy = "", $OrderBy = "", $Limit = "", $ListKey = "", $DB = "")
{
    return  dbGetList($TableNames, "", $Where, $GroupBy, $OrderBy, $Limit, $ListKey, $DB);
}

/**
 * Get row info from db
 *
 * @author                       Yupeng Lee <leeyupeng@gmail.com>
 * @param    string  $TableNames Table names split by ,
 * @param    string  $Columns    Columns split by ,
 * @param    string  $Where      SQL condition
 * @param    string  $DB         Database name
 * @retrun   array               List array
 */
function dbGetRow($TableName, $Columns = "", $Where = "", $DB = "")
{
    if(empty($DB))
    {
        global $MyDB;
        $DB = $MyDB;
    }

    $TableName = dbGetPrefixTableNames($TableName);

    $Sql = dbGetListSql($TableName, $Columns, $Where);
    $Data = $DB->GetRow($Sql);
    return $Data;
}

/**
 * Checks if the given value exists in the database table's column
 *
 * @author                       Yupeng Lee <leeyupeng@gmail.com>
 * @param    string  $ValueList
 * @param    string  $TableName
 * @param    string  $ColumnName
 * @retrun   string  $Values
 */
function dbGetValidValueList($ValueList, $TableName, $ColumnName)
{
    $ValueList = trim($ValueList);
    if($ValueList == '')
    {
        return '';
    }
    $Values = dbGetList($TableName, $ColumnName, $ColumnName . dbCreateIN($ValueList), '','','',$ColumnName);
    
    $Values = join(',', array_keys($Values));
    return $Values;
}

/**
 * Insert row into table
 *
 * @author              Yupeng Lee<leeyupeng@gmail.com>
 * @param  string $TableName table name
 * @param  string $Values    value
 * @param  string $Columns   columns
 * @return int               Insert ID
 */
function dbInsertRow($TableName, $Values, $Columns = '')
{
    global $MyDB;
    $DB = $MyDB;

    $TableName = dbGetPrefixTableNames($TableName);

    $Sql = 'INSERT INTO ' . $TableName;

    if(!empty($Columns))
    {
        $Sql .= ' (' . $Columns . ') ';
    }

    $Sql .= ' VALUES (' . $Values . ')';

    $DB->Execute($Sql);

    return $MyDB->Insert_ID();
}

/**
 * Update row
 *
 * @author                   Yupeng Lee<leeyupeng@gmail.com>
 * @param  mix
 */
function dbUpdateRow()
{
    global $MyDB;
    $DB = $MyDB;

    $ArgNum = func_num_args();
    if($ArgNum < 3)
    {
        return false;
    }

    $ArgList = func_get_args();

    $TableName = $ArgList[0];
    $TableName = dbGetPrefixTableNames($TableName);

    if($ArgNum % 2 == 0)
    {
        $Where = array_pop($ArgList);
    }

    $Sql = 'UPDATE ' . $TableName . ' SET ';
    $SetSqlList = array();
    for($I=1; $I < $ArgNum -1; $I=$I+2)
    {
        if(isset($ArgList[$I]) && isset($ArgList[$I+1]))
        {
            $SetSqlList[] = "{$ArgList[$I]} = {$ArgList[$I+1]}";
        }
    }

    $SetSql = join(',', $SetSqlList);

    $Sql .= $SetSql;

    if(!empty($Where))
    {
        $Sql .= ' WHERE ' . $Where;
    }

    $DB->Execute($Sql);
}

/**
 * delete a row from db
 *
 * @author                Yupeng Lee<leeyupeng@gmail.com>
 *
 * @param  string $TableName table name
 * @param  string $Where     SQL condition
 */
function dbDeleteRow($TableName, $Where = "")
{
    global $MyDB;
    $DB = $MyDB;

    $TableName = dbGetPrefixTableNames($TableName);

    $Sql = 'DELETE FROM ' . $TableName;
    if(!empty($Where))
    {
        $Sql .= ' WHERE ' . $Where;
    }

    $DB->Execute($Sql);
}

/*============================================HTMl FUNCTIONS=====================================*/
/**
 * create tags like <a href="">text</a>
 *
 * @author                      wwccss<wwccss@263.net>
 * @param string $LinkText      the link text.
 * @param string $LinkURL       the link url.
 * @param string $LinkTarget    the target window
 * @param booble $Echo          show directly or false.
 */
function htmlLink($LinkText, $LinkURL, $LinkTarget = "_self", $Echo = false)
{
    $LinkString = "<a href='$LinkURL' target='$LinkTarget'>$LinkText</a>";
    if($Echo)
    {
       echo $LinkString;
    }
    return $LinkString;
}

/**
 * create tags like "<select><option></option></select>"
 *
 * @author                     wwccss<wwccss@263.net>
 * @param array  $ArrayData    the array to create select tag from.
 * @param string $SelectName   the name of the select tag.
 * @param string $Mode         Normal|Reverse,if normal, show the key of the array in the select box, else show the value of the array in the select box.
 * @param string $SelectItem   the item(s) to be selected, can like item1,item2.
 * @param string $Attrib       other params such as multiple, size and style.
 * @param booble $Echo         show directly or false.
 */
function htmlSelect($ArrayData, $SelectName, $Mode = "Reverse", $SelectItem = "", $Attrib = "", $PairIndex = "", $Echo = false)
{
    if(!is_array($ArrayData))
    {
        $ArrayData = array();
    }

    $Mode = $Mode != 'Normal' ? 'Reverse' : 'Normal';

    /* The begin. */
    $SelectString = "\n  <select name='$SelectName' ";

    /* Set the id. */
    if(preg_match("/\[/i",$SelectName))
    {
        $SelectName    = explode("[",$SelectName);
        $SelectString .= "id='$SelectName[0]' ";
    }
    else
    {
       $SelectString  .= "id='$SelectName'";
    }

    /* The param. */
    $SelectString .= " $Attrib >\n";

    /* Explode the SelectItems. */
    $SelectItem = explode(",", $SelectItem);

    /* The option. */
    if($PairIndex != "")
    {
        $PairIndex = explode(',', $PairIndex);
        $KeyIndex = trim($PairIndex[0]);
        $ValueIndex = trim($PairIndex[1]);
    }

    if(count($ArrayData) == 0 && !preg_match('/multiple/', strtolower($Attrib)))
    {
        $SelectString .='<option selected label="null" value="" style="color:#AAAAAA;">--------</option>';
    }
    else
    {
        foreach ($ArrayData as $Key => $Value)
        {
            if(is_array($Value))
            {
                $Key = $Value[$KeyIndex];
                $Value = $Value[$ValueIndex];
            }
            if($Mode == "Normal")
            {
                if(in_array($Value, $SelectItem) && $SelectItem != '')
                {
                    $SelectString .= "<option value='$Value' selected='selected'>$Key</option>";
                }
                else
                {
                    $SelectString .= "<option value='$Value'>$Key</option>";
                }
            }
            elseif($Mode == "Reverse")
            {
                if(in_array($Key, $SelectItem) && $SelectItem != '')
                {
                    $SelectString .= "<option value='$Key' selected='selected'>$Value</option>";
                }
                else
                {
                    $SelectString .= "<option value='$Key'>$Value</option>";
                }
            }
            $SelectString .= "\n";
        }
    }
    /* The end. */
    $SelectString .= "</select>\n";

    if($Echo)
    {
        echo $SelectString;
    }
    return $SelectString;
}

/**
 * Create tags like "<input type='radio' />"
 *
 * @author                        wwccss<wwccss@263.net>
 * @param array  $ArrayData       the array to create radio tag from.
 * @param string $RadioName       the name of the radio tag.
 * @param string $Mode            Normal|Reverse,if normal, show the key of the array, else show the value of the array.
 * @param string $DefaultChecked  the value to checked by default.
 * @param string $Attrib          other attribs.
 * @param booble $Echo            show directly or false.
 */
function htmlRadio($ArrayData, $RadioName, $Mode = "Reverse", $DefaultChecked = "", $Attrib = "", $Echo = false)
{
    if(!is_array($ArrayData))
    {
       return false;
    }
    foreach($ArrayData as $Key => $Value)
    {
       if($Mode == "Reverse")
       {
           $SwitchTMP = $Value;
           $Value     = $Key;
           $Key       = $SwitchTMP;
       }
       $RadioString .= "<input type='radio' name='$RadioName' value='$Value' ";
       $RadioString .= ($Value == $DefaultChecked) ? " checked ='checked'" : "";
       $RadioString .= $Attrib;
       $RadioString .= " />$Key\n";
    }
    return $RadioString;
}

/**
 * create tags like "<input type='checkbox' />"
 *
 * @author                        wwccss<wwccss@263.net>
 * @param array  $ArrayData       the array to create checkbox tag from.
 * @param string $CheckBoxName    the name of the checkbox tag.
 * @param string $Mode            Normal|Reverse,if normal, show the key of the array, else show the value of the array.
 * @param string $DefaultChecked  the value to checked by default.
 * @param string $Attrib          other attribs.
 * @param booble $Echo            show directly or false.
 */
function htmlCheckBox($ArrayData, $CheckBoxName, $Mode = "Reverse", $DefaultChecked = "", $Attrib = "", $Echo = false)
{
    if(!is_array($ArrayData))
    {
       return false;
    }
    foreach($ArrayData as $Key => $Value)
    {
       if($Mode == "Reverse")
       {
           $SwitchTMP = $Value;
           $Value     = $Key;
           $Key       = $SwitchTMP;
       }
       $CheckBoxString .= "<input type='checkbox' name='$CheckBoxName" . "[]' value='$Value' ";
       $CheckBoxString .= ($Value == $DefaultChecked) ? " checked ='checked'" : "";
       $CheckBoxString .= $Attrib;
       $CheckBoxString .= " />$Key\n";
    }
    return $CheckBoxString;
}

/**
 * create tags like "<input type='text' />"
 *
 * @author                        wwccss<wwccss@263.net>
 * @param string $TextName        the name of the text input tag.
 * @param string $DefaultValue    the default value.
 * @param string $Attrib          other attribs.
 * @param booble $Echo            show directly or false.
 */
function htmlText($TextName, $DefaultValue = "", $Attrib = "")
{
    $TextString = "<input type='text' name='$TextName' id='$TextName' value='$DefaultValue' $Attrib />\n";
    if($Echo)
    {
        echo $TextString;
    }
    return $TextString;
}

/**
 * create tags like "<input type='password' />"
 *
 * @author                        wwccss<wwccss@263.net>
 * @param string $PasswordName    the name of the password input tag.
 * @param string $Attrib          other attribs.
 * @param booble $Echo            show directly or false.
 */
function htmlPassword($PasswordName, $Attrib = "", $Echo = false)
{
    $PasswordName2   = $PasswordName . '2';
    $PasswordString  = "<input type='password' name='$PasswordName'  id='$PasswordName'  $Attrib />\n";
    $PasswordString .= "<input type='password' name='$PasswordName2' id='$PasswordName2' $Attrib />\n";
    if($Echo)
    {
        echo $PasswordString;
    }
    return $PasswordString;
}

/**
 * create tags like "<textarea></textarea>"
 *
 * @author                        wwccss<wwccss@263.net>
 * @param string $TextAreaName    the name of the textarea tag.
 * @param string $DefaultValue    the default value of the textarea tag.
 * @param string $Attrib          other attribs.
 * @param booble $Echo            show directly or false.
 */
function htmlTextarea($TextAreaName, $DefaultValue = "", $Attrib = "")
{
    $TextAreaString = "<textarea name='$TextAreaName' id='$TextAreaName' $Attrib>$DefaultValue</textarea>\n";
    if($Echo)
    {
        echo $TextAreaString;
    }
    return $TextAreaString;
}

/**
 * create tags like "<input type='file' />".
 *
 * @author                        wwccss<wwccss@263.net>
 * @param string $FileName        the name of the file name.
 * @param string $Attrib          other attribs.
 * @param booble $Echo            show directly or false.
 */
function htmlFile($FileName, $Attrib = "", $Echo = false)
{
    $FileString = "<input type='file' name='$FileName' id='$FileName' $Attrib />\n";
    if($Echo)
    {
        echo $FileString;
    }
    return $FileString;
}

/*============================================JAVASCRIPT FUNCTIONS=====================================*/
/**
 * The start of javascript.
 *
 * @param   booble  $Echo   echo the javascript string or not.
 * @return  string          the start tag of javascript scripts.
 */
function jsStart($Echo = false)
{
    $JS = "<script language='Javascript'>";
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * The end of javascript.
 *
 * @param   booble  $Echo   echo the javascript string or not.
 * @return  string          the end tag of javascript scripts.
 */
function jsEnd($Echo = false)
{
    $JS = "</script>";
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * show a alert box.
 *
 * @param   array   $Text   the text to be showd in the alert box.
 * @param   booble  $Echo   echo the javascript string or not.
 * @return  string          the javascript script.
 */
function jsAlert($Text, $Echo = true)
{
    $JS  = jsStart();
    $JS .= <<<EOT
    alert('$Text');
EOT;
    $JS .= jsEnd();
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * show a alert box.
 *
 * @param   array   $TextArray   the text to be showd in the alert box.
 * @return  string               the javascript script.
 */
function jsAlertArray($TextArray)
{
    return jsAlert(join('\n',$TextArray), false);
}

/**
 * show a confirm box, press ok go to URL1, else go to URL2.
 *
 * @param string $ConfirmText  the text to be showed.
 * @param string $ConfirmURL   the url to go to when press 'ok'.
 * @param string $CancleURL    the url to go to when press 'cancle'.
 * @param booble $Echo         echo the javascript string or not.
 * @param string $Target       the name of the window to change location.
 */
function jsConfirm($ConfirmText, $ConfirmURL, $CancleURL = 'BACK', $ConfirmTarget = "self", $CancleTarget = "self", $Echo = true)
{
    $JS = jsStart();

    if(strtoupper($ConfirmURL) == "BACK")
    {
        $ConfirmAction = "history.back(-1);";
    }
    else
    {
        $ConfirmAction = "$ConfirmTarget.location = '$ConfirmURL';";
    }

    if(strtoupper($CancleURL) == "BACK")
    {
        $CancleAction = "history.back(-1);";
    }
    else
    {
        $CancleAction = "$CancleTarget.location = '$CancleURL';";
    }

    $JS .= <<<EOT
    if(confirm("$ConfirmText"))
    {
        $ConfirmAction
    }
    else
    {
        $CancleAction
    }
EOT;
    $JS .= jsEnd();
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * change the location of the $Target window to the $URL.
 *
 * @param   string $URL    the url will go to.
 * @param   string $Target the target of the url.
 * @param   booble $Echo   echo the javascript string or not.
 * @return  string         the javascript string.
 */
function jsGoTo($URL, $Target = "self", $Echo = true)
{
    $JS  = jsStart();
    if(strtolower($URL) == "back")
    {
        $JS .= "history.back(-1);";
    }
    else
    {
        $JS .= "$Target.location='$URL';";
    }
    $JS .= jsEnd();
    if($Echo)
    {
        echo($JS);
    }
    return $JS;
}

/**
 * select an item of a select box.
 *
 * @param string $ObjName        the object name of the select box.
 * @param string $ItemValue      the value of the item to be selected.
 * @param string $FunctionName   the name of the function to create. If empty, execute directly.
 * @param booble $Echo           show directly or false.
 */
function jsSelectOption($ObjName, $ItemValue, $FunctionName = "", $Echo = true)
{
    $JS = jsStart();
    if(!empty($FunctionName))
    {
        $JS .= "function $FunctionName(){";
    }

    $JS .= <<<EOT
    var Value='$ItemValue';
    for(I = 0; I < $ObjName.options.length; I++)
    {
        if(Value.indexOf(',') >= 0)
        {
            ValueList = Value.split(',');
            for(Y = 0; Y < ValueList.length; Y++)
            {
                if(ValueList[Y] == $ObjName.options[I].value)
                {
                    $ObjName.options[I].selected = true;
                }
            }
        }
        else if(Value == $ObjName.options[I].value)
        {
           $ObjName.options[I].selected = true;
        }
    };
EOT;

    if(!empty($FunctionName))
    {
        $JS .= "}";
    }

    $JS .= jsEnd();
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * select an item of a group of radios.
 *
 * @param string $ObjName   the object name of the radios.
 * @param string $ItemValue the value of the item to be selected.
 * @param booble $Echo      show or false.
 */
function jsSelectRadio($ObjName, $ItemValue, $Echo = false)
{
    $JS  = jsStart();
    $JS .= <<<EOT
    for(I=0; I< $ObjName.length; I++)
    {
        if($ObjName.[I].value == '$ItemValue')
        {
            $ObjName.[I].checked=true;
        }
    };
EOT;
    $JS .= jsEnd();
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * Close current window.
 *
 * @param   booble $Echo   echo the javascript string or not.
 * @return  string         the close window javascript.
 */
function jsCloseWindow()
{
    $JS = jsStart() . "window.close();" . jsEnd();
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * Goto a page after a timer.
 *
 * @param   string $URL    the url will go to.
 * @param   string $Target the target of the url.
 * @param   int    $Time   the timer, msec.
 * @param   booble $Echo   echo the javascript string or not.
 * @return  string         the javascript string.
 */
function jsRefresh($URL, $Target = "self", $Time = 3000, $Echo = false)
{
    $JS  = jsStart();
    $JS .= "setTimeout(\"$Target.location='$URL'\", $Time);";
    $JS .= jsEnd();
    if($Echo)
    {
        echo $JS;
    }
    return $JS;
}

/**
 * Create a javascript array whith array's key or value
 *
 * @author                   Yupeng Lee <leeyupeng@gmail.com>
 * @param   array  $Array
 * @param   string $KeyOrValue
 * @return  string $JsArrayStr
 */
function jsArray($Array, $KeyOrValue = 'Value')
{
    $JsArrayStr = 'new Array(';
    if(!empty($Array))
    {
        $NewArray = array();
        foreach($Array as $Key => $Value)
        {
            $NewArray[sysAddSlash($Key)] = sysAddSlash($Value);
        }

        if($KeyOrValue == 'Value')
        {
            $JsArrayStr .= '"' . join('","', $NewArray) . '"';
        }
        else
        {
            $JsArrayStr .= '"' . join('","', array_keys($NewArray)) . '"';
        }
    }
    $JsArrayStr .= ');';

    return $JsArrayStr;
}

/**
 * Flush (send) the javascript output buffer
 *
 * @author                     Yupeng Lee <leeyupeng@gmail.com>
 * @param   string  $JsStr
 */
function sysObFlushJs($JsStr)
{
    sysObFlush(jsStart() . $JsStr . jsEnd());
}

/**
 * Flush (send) the  output buffer
 *
 * @author                     Yupeng Lee <leeyupeng@gmail.com>
 * @param   string  $Str
 */
function sysObFlush($Str)
{
    echo $Str;
    ob_flush();
    flush();
}
/**
 * Check the UserName for validity
 *
 * @author                     Yupeng Lee <leeyupeng@gmail.com>
 * @param    string $UserNameStr
 * @return   bool              valid or invalid
 */
function sysCheckUserNameFormat($UserNameStr)
{
    $UserNameStr = trim($UserNameStr);
    if(preg_match("/".chr(38). "/", $UserNameStr) || preg_match('/"/', $UserNameStr) || preg_match("/'/", $UserNameStr))
    {
        return false;
    }
    return true;
}

/**
 * Check the password for validity
 *
 * @author                     Yupeng Lee <leeyupeng@gmail.com>
 * @param    string $EmailStr
 * @return   bool              valid or invalid
 */
function sysCheckEmailFormat($EmailStr)
{
    $EmailStr = trim($EmailStr);
    if(!preg_match("/^[_\.0-9a-z-]+@([0-9a-z][0-9a-z-]+\.)+[a-z]{2,4}$/i", $EmailStr))
    {
        return false;
    }
    return true;
}

/**
 * Check the date format for validity (just like 2008-01-01)
 *
 * @author                     Yupeng Lee <leeyupeng@gmail.com>
 * @param    string  $DateStr
 * @return   bool              valid or invalid
 */
function sysCheckDateFormat($DateStr)
{
    $DateStr = trim($DateStr);
    if(!preg_match("/^[0-9]{4}-[0-9]{2}-[0-9]{2}$/i", $DateStr))
    {
        return false;
    }
    else
    {
        $DateStrArray = explode('-', $DateStr);
        return checkdate($DateStrArray[1], $DateStrArray[2], $DateStrArray[0]);
    }
    return true;
}

/**
 * Create the date SQL
 *
 * @author                     Yupeng Lee <leeyupeng@gmail.com>
 * @param    string $DateStr
 * @return   string            SQL just like: BETWEEN "2008-01-01 00:00:00" AND "2008-01-01 23:59:59"
 */
function sysStrToDateSql($DateStr)
{
    $Pattern = "([0-9]{4})-([0-9]{1,2})-([0-9]{1,2}) *([0-9]{0,2}):*([0-9]{0,2}):*([0-9]{0,2})";
    $StartDateTime = "";
    $EndDateTime = "";
    if(preg_match("/$Pattern/", $DateStr, $Ereg))
    {
        $Ereg[2] = str_pad($Ereg[2], 2, '0', STR_PAD_LEFT);
        $Ereg[3] = str_pad($Ereg[3], 2, '0', STR_PAD_LEFT);
        $StartDateTime = $Ereg[1] . '-' . $Ereg[2] . '-' . $Ereg[3];
        $EndDateTime = $StartDateTime;

        if($Ereg[4] != '')
        {
            $Ereg[4] = str_pad($Ereg[4], 2, '0', STR_PAD_LEFT);
            $StartDateTime .= ' ' . $Ereg[4];
            $EndDateTime .= $StartDateTime;
        }
        else
        {
            $StartDateTime .= ' 00';
            $EndDateTime .= ' 23';
        }
        if($Ereg[5] != '')
        {
            $Ereg[5] = str_pad($Ereg[5], 2, '0', STR_PAD_LEFT);
            $StartDateTime .= ':' . $Ereg[5];
            $EndDateTime .= $StartDateTime;
        }
        else
        {
            $StartDateTime .= ':00';
            $EndDateTime .= ':59';
        }
        if($Ereg[6] != '')
        {
            $Ereg[6] = str_pad($Ereg[6], 2, '0', STR_PAD_LEFT);
            $StartDateTime .= ':' . $Ereg[6];
            $EndDateTime .= $StartDateTime;
        }
        else
        {
            $StartDateTime .= ':00';
            $EndDateTime .= ':59';
        }
    }
    return "BETWEEN '" . $StartDateTime . "' AND '" . $EndDateTime . "'";
}

/**
 * Check the upload files for validity
 *
 * @author                    Yupeng Lee<leeyupeng@gmail.com>
 */
function sysCheckUploadFile()
{
    global $_CFG, $_LANG;

    $ActionMsg = array('Bingle'=>true,'BingleMsg'=>array(), 'FailedMsg'=>array(), 'ValidFileCount'=>0);

    $FileList = $_FILES['TestFile'];
    $MaxFileSizeName = floor($_CFG['File']['MaxFileSize']/1024) . 'k';
    for($I=0; $I<count($FileList['name']); $I ++)
    {
        $FileName = $FileList['name'][$I];
        if($FileName == '')
        {
            continue;
        }
        $FileType = sysGetFileType($FileList['name'][$I]);
        $FileSize = $FileList['size'][$I];

        if(in_array($FileType,$_CFG['File']['DangerousTypeList']))
        {
            $ActionMsg['FailedMsg'][] = $FileName . $_LANG['ErrorDangerousFileType'];
            $ActionMsg['Bingle'] = false;
        }
        if($FileList['error'][$I] == '2')
        {
            $ActionMsg['FailedMsg'][] = $FileName . ' ' . $_LANG['ErrorExceedSize'];
            $ActionMsg['Bingle'] = false;
        }
        if(!in_array($FileType,$_CFG['File']['DangerousTypeList']) && $FileSize <= $_CFG['File']['MaxFileSize'])
        {
            $ActionMsg['ValidFileCount'] ++ ;
        }
    }

    return $ActionMsg;
}

/**
 * Upload file
 *
 * @author                     Yupeng Lee<leeyupeng@gmail.com>
 * @param   int  $ProjectID
 * @param   int  $ActionID
 */
function sysUploadFile($ProjectID,$ActionID)
{
    global $_CFG;
    $Path = $_CFG['File']['UploadDirectory'];

    $FileList = $_FILES['TestFile'];
    $TitleList = $_POST['TestFileTitle'];
    $ReturnFileList = array();
    if(!is_dir($Path) && !@mkdir($Path))
    {
        return false;
    }

    for($I=0; $I<count($FileList['name']); $I ++)
    {
        $FileName = $FileList['name'][$I];
        $FileType = sysGetFileType($FileList['name'][$I]);
        $FilePathName = sysCreateUploadFilePath($ProjectID, $I) . '.' . $FileType;
        $FileSize = $FileList['size'][$I];
        if($FileSize <= 1024 * 1024 )
        {
            $FileSizeName = round($FileSize / 1024,5) . "KB";
        }
        else
        {
            $FileSizeName = round($FileSize / (1024 * 1024),2) . "MB";
        }
        if($FilePathName != '')
        {
            // upload
            $FilePath = $Path . '/' . $FilePathName;
            if(!copy($FileList["tmp_name"][$I],$FilePath))
            {
                $ReturnFileList[$I]['Bingle'] = false;
            }
            else
            {
                $ReturnFileList[$I]['Bingle'] = true;
            }
        }
        else
        {
            $ReturnFileList[$I]['Bingle'] = false;
        }

        $ReturnFileList[$I]['FileName'] = $FilePathName;
        $ReturnFileList[$I]['FileSize'] = $FileSizeName;
        $ReturnFileList[$I]['FileType'] = $FileType;

        $FileTitle = $FileName;
        if(trim($TitleList[$I]) != '')
        {
            $FileTitle = trim($TitleList[$I]);
        }
        $ReturnFileList[$I]['FileTitle'] = $FileTitle;

        @unlink($FileList["tmp_name"][$I]);
    }

    foreach($ReturnFileList as $Key=>$FileInfo)
    {
        if($FileInfo['Bingle'])
        {
            $FileID = sysInsertFileDB($ActionID, $FileInfo['FileTitle'], $FileInfo['FileName'], $FileInfo['FileType'],$FileInfo['FileSize']);
            $ReturnFileList[$Key]['FileID'] = $FileID;
        }
    }

    return $ReturnFileList;
}

/**
 * Upload file
 *
 * @author                     Yupeng Lee<leeyupeng@gmail.com>
 * @param   int  $ProjectID
 * @param   int  $ActionID
 */
function sysInsertFileDB($ActionID,$FileTitle='',$FileName='',$FileType='',$FileSize='')
{
    $FileID = dbInsertRow('TestFile', "'{$ActionID}','{$FileTitle}','{$FileName}','{$FileType}','{$FileSize}'",
                            'ActionID,FileTitle,FileName,FileType,FileSize');
    return $FileID;
}

/**
 * Get file type
 *
 * @author                    Yupeng Lee<leeyupeng@gmail.com>
 * @param    string   $FileName
 */
function sysGetFileType($FileName)
{
    $FileType = explode(".",$FileName);
    $FileType = strtolower(array_pop($FileType));
    if(strlen($FileType) == strlen($FileName))
    {
        return '';
    }
    return $FileType;
}

/**
 * Create upload file path
 *
 * @author                     Yupeng Lee<leeyupeng@gmail.com>
 * @param   string  $ProjectID
 * @param   string  $FileName
 */
function sysCreateUploadFilePath($ProjectID, $FileNum)
{
    global $_CFG;

    // Create project dir
    $UploadPath = $_CFG['File']['UploadDirectory'];
    $Path .= 'Project' . $ProjectID;
    if(!is_dir($UploadPath . '/' . $Path))
    {
        if(!@mkdir($UploadPath . '/' .$Path))
        {
            return '';
        }
    }

    // Create current month dir
    $Path .= '/' . date("Ym");
    if(!is_dir($UploadPath . '/' .$Path))
    {
        if(!@mkdir($UploadPath . '/' .$Path))
        {
            return '';
        }
    }

    // Create file path
    $Path .= '/' . date("His") . rand(10,20) . $FileNum;

    return $Path;
}

function sysTrimExplode($Separator, $String)
{
    $array = explode($Separator, $String);
    if(current($array) == '')
    {
        array_shift($array);
    }
    if(end($array) == '')
    {
        array_pop($array);
    }
    return $array;
}
?>
