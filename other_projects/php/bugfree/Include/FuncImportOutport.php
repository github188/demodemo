<?php
require('Include/Class/XmlParse.class.php');

function ExportXML($CaseExportList,$CaseExportColumnArray,$FieldsArray){
	$RowCount = count($CaseExportList)+1;
	$ColumnCount = count($CaseExportColumnArray);

	$Content = "<?xml version=\"1.0\" encoding=\"utf-8\"?>
                 <?mso-application progid=\"Excel.Sheet\"?>
                 <Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"
                 xmlns:o=\"urn:schemas-microsoft-com:office:office\"
                 xmlns:x=\"urn:schemas-microsoft-com:office:excel\"
                 xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"
                 xmlns:html=\"http://www.w3.org/TR/REC-html40\">
                 <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">
                  <Created>1996-12-17T01:32:42Z</Created>
                  <LastSaved>2009-11-21T14:55:15Z</LastSaved>
                  <Version>11.9999</Version>
                 </DocumentProperties>
                 <OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">
                  <RemovePersonalInformation/>
                 </OfficeDocumentSettings>
                 <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">
                  <WindowHeight>4530</WindowHeight>
                  <WindowWidth>8505</WindowWidth>
                  <WindowTopX>480</WindowTopX>
                  <WindowTopY>120</WindowTopY>
                  <AcceptLabelsInFormulas/>
                  <ProtectStructure>False</ProtectStructure>
                  <ProtectWindows>False</ProtectWindows>
                 </ExcelWorkbook>
                 <Styles>
                  <Style ss:ID=\"Default\" ss:Name=\"Normal\">
                   <Alignment ss:Vertical=\"Bottom\"/>
                   <Borders/>
                   <Font ss:FontName=\"\" x:CharSet=\"134\" ss:Size=\"12\"/>
                   <Interior/>
                   <NumberFormat/>
                   <Protection/>
                  </Style>
                  <Style ss:ID=\"s21\">
                   <Alignment ss:Vertical=\"Bottom\" ss:WrapText=\"1\"/>
                  </Style>
                 </Styles>
                 <Worksheet ss:Name=\"Sheet1\">
                  <Table ss:ExpandedColumnCount=\"".$ColumnCount ."\" ss:ExpandedRowCount=\"" . $RowCount ."\" x:FullColumns=\"1\"
                   x:FullRows=\"1\" ss:DefaultColumnWidth=\"54\" ss:DefaultRowHeight=\"14.25\">";

   	$TempStr = "\n<Row>";
   	foreach($CaseExportColumnArray as $ExportItem)
   	{ 
       		$TempStr .=" \n<Cell><Data ss:Type=\"String\">" . $FieldsArray[$ExportItem] . "</Data></Cell>\n";
   	}
   	$TempStr .= "</Row>\n";
   	$arr_search = array('<','>',"'",'&', '"',"\n");
   	$arr_replace = array('&lt;','&gt;','&apos;','&amp;','&quot;', '&#10;');
        $arr_search1 = array('&amp;lt;','&amp;gt;','&amp;quot;','&amp;amp;','&amp;apos;');
   	$arr_replace1 = array('&lt;','&gt;','&quot;', '&amp;','&apos;');
 
    
   	foreach($CaseExportList as $CaseItem)
   	{
       		$TempStr .= "\n<Row>";
       		foreach($CaseExportColumnArray as $Column)
       		{         
         	   $TempStr .=" \n<Cell><Data ss:Type=\"String\">" . str_ireplace($arr_search1,$arr_replace1,str_ireplace($arr_search,$arr_replace,$CaseItem[$Column])) . "</Data></Cell> \n";
       		}
       		$TempStr .= "</Row>\n";
   	}

       
	$Content .= $TempStr;
	$Content .= "</Table>
                  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">
                   <Selected/>
                   <Panes>
                    <Pane>
                     <Number>3</Number>
                     <ActiveRow>1</ActiveRow>
                     <ActiveCol>1</ActiveCol>
                    </Pane>
                   </Panes>
                   <ProtectObjects>False</ProtectObjects>
                   <ProtectScenarios>False</ProtectScenarios>
                  </WorksheetOptions>
                 </Worksheet>
		 </Workbook>";
	return $Content;
}

function ParseImportXML($DomRowArray,$_LANG){
     $ImportHeader = true;
     $CaseImportColumnsXML = array(); //xml�е�����ֶ�
     $CaseImportListItemXML = array();//xml�е�
     
     foreach($DomRowArray as $Key=>$Value){ //Parse Dom
        if(strpos($Key, 'attr')===false){
	  if($ImportHeader===true){ //Import Fields
             foreach($Value['Cell'] as $CellKey => $CellValue){
                if(strpos($CellKey, 'attr')===false){
                   if($CellValue['Data']!==NULL)
                      array_push($CaseImportColumnsXML,$CellValue['Data']);
                   if($CellValue['ss:Data']!==NULL)
                      array_push($CaseImportColumnsXML,$CellValue['ss:Data']);
                   $ImportHeader = false;
                }
             }
           }
          else{ //Import Datum
             $Temp = array();
             foreach($Value['Cell'] as $CellKey => $CellValue){
	        if(strpos($CellKey, 'attr')===false){
		   if($CellValue['Data']!==NULL)
		      array_push($Temp,$CellValue['Data']);
		   elseif( $CellValue['ss:Data']!==NULL)
		      array_push($Temp,$CellValue['ss:Data']);
	         }
	      }
              array_push($CaseImportListItemXML,$Temp);
           }
         }
      }// end for

      
      $CaseImportColumnMust = array('CaseID' , 'CaseTitle', 'ProjectName', 'ModulePath', 'CaseSteps'); //must import
      $CaseImportColumnSelected = array('CaseStatus' , 'AssignedTo', 'MailTo', 'CasePriority', 'CaseType',
                                         'CaseMethod', 'CasePlan', 'ScriptStatus', 'ScriptedBy', 'ScriptedDate',
                                         'ScriptLocation','MarkForDeletion', 'CaseKeyword', 'DisplayOrder'); //selected import
      
      $CaseIDColumn = 0;  // 初始化CaseID对应的列序号
      $CaseImportColumnsRealName = $CaseImportColumnsXML;
      $CaseImportColumns = array();// 导入字段数组，与数据库字段一致
      foreach($CaseImportColumnsRealName as $Item)
         array_push($CaseImportColumns,$_LANG['CaseImport'][$Item]);

      $Importable = true;

      foreach($CaseImportColumnMust as $ColumnItem){ //判断是否包含必须导入的字段
         if(!in_array($ColumnItem,$CaseImportColumns)){
            $Importable = false;
            sysObFlushJs("alert('{$ColumnItem}{$_LANG['ImportColumnNotNull']}');");
            break;
         }
      }

      $ImportColumnArray = array(); //最后要导入的字段（过滤掉不可导入的字段）
      if($Importable){//必须导入的字段都存在
	 if($CaseImportColumns){
           foreach($CaseImportColumns as $Key => $Value){
             if(trim($Value)=="CaseID")
                $CaseIDColumn = $Key; //Case ID的Key
             if(in_array(trim($Value),$CaseImportColumnMust) || in_array(trim($Value),$CaseImportColumnSelected))
                $ImportColumnArray[$Key] = $Value; 
           }
         $ImportColumnArray[] = 'ProjectID';
         $ImportColumnArray[] = 'ModuleID';
         }
         $ImportTotalItem = 0;
         $ImportSuccessItem = 0;
         $ImportFailItem = 0;

         foreach($CaseImportListItemXML as $CaseImportListItemXMLKey =>$CaseImportListItemXMLValue){
              
              $ImportTotalItem = $ImportTotalItem + 1;

              if($CaseImportListItemXMLValue&&count($CaseImportColumnsXML)==count($CaseImportListItemXMLValue)){
                 
                 $ValidateColumns = GetValidateColumns($CaseImportListItemXMLValue,$ImportColumnArray); //判断字段的合法性
                 if($ValidateColumns){ //必填字段合法
                      $ImportSuccessItem = $ImportSuccessItem + 1;
                      $CaseRow = dbGetRow('CaseInfo', 'CaseID', "CaseID='{$CaseImportListItemXMLValue[$CaseIDColumn]}'");
                      if($CaseRow){ //CaseID存在，则更新Case相关消息
                         $UpdateColumnArray = array();
                         foreach($ImportColumnArray as $Key => $Value){
                           $EditKeyValue = array();

                           $QueryInfo = dbGetRow('CaseInfo', '', "CaseID='{$CaseImportListItemXMLValue[$CaseIDColumn]}'"); //取出要修改的Case信息
                           $EditKeyValue['BugID'] = $QueryInfo['BugID'];
                           $EditKeyValue['ResultID'] = $QueryInfo['ResultID'];
                           $EditKeyValue['CasePriority'] = $QueryInfo['CasePriority'];
                           $EditKeyValue['DisplayOrder'] = $QueryInfo['DisplayOrder'];
                           $EditKeyValue['CaseKeyword'] = $QueryInfo['CaseKeyword'];
                           $EditKeyValue['LinkID'] = $QueryInfo['LinkID'];
                           $EditKeyValue['CaseMethod'] = $QueryInfo['CaseMethod'];
                           $EditKeyValue['CaseType'] = $QueryInfo['CaseType'];
                           $EditKeyValue['MarkForDeletion'] = $QueryInfo['MarkForDeletion'];

                           $Value = trim($Value);
                           $CaseImportListItemXMLValue[$Key] = trim($CaseImportListItemXMLValue[$Key]);
                            switch($Value){
                               case 'ProjectID':
                                    $UpdatedColumnArray[$Key] = 'ProjectID';
                                    $CaseImportListItemXMLValue[$Key] = $ValidateColumns['ProjectID'];
                                    break;
                               case 'ModuleID':
                                    $UpdatedColumnArray[$Key] = 'ModuleID';
                                    $CaseImportListItemXMLValue[$Key] =  $ValidateColumns['ModuleID'];
                                    break;
                               case 'AssignedTo':
                                    if(trim($CaseImportListItemXMLValue[$Key])=='Active')
                                       $UpdatedColumnArray[$Key] = $Value;
                                    elseif(dbGetRow('TestUser', 'UserName', "UserName='{$CaseImportListItemXMLValue[$Key]}'"))
                                       $UpdatedColumnArray[$Key] = $Value;        
                                    break;
                               case 'MarkForDeletion':
                                    if((trim($CaseImportListItemXMLValue[$Key])=='0') ||(trim($CaseImportListItemXMLValue[$Key])=='1'))
                                       $UpdatedColumnArray[$Key] = $Value;
                                    break;
                               case 'DisplayOrder':
                                    if (is_numeric($CaseImportListItemXMLValue[$Key]) && (int)($CaseImportListItemXMLValue[$Key])>=0 && (int)($CaseImportListItemXMLValue[$Key])<=255)
                                        $UpdatedColumnArray[$Key] = $Value;       
                                    break;
                               case 'CasePriority':
                                    if (is_numeric($CaseImportListItemXMLValue[$Key]) && (int)($CaseImportListItemXMLValue[$Key])>=0 && (int)($CaseImportListItemXMLValue[$Key])<count($_LANG['CasePriorities']))
                                        $UpdatedColumnArray[$Key] = $Value;
                                    break;
                               case 'CaseStatus':
                                    if(in_array($CaseImportListItemXMLValue[$Key],$_LANG['CaseStatuses']))
                                        $UpdatedColumnArray[$Key] = $Value;
                                    break;
                               default:
                                    $UpdatedColumnArray[$Key] = $Value;
                                    $CaseImportListItemXMLValue[$Key] =  $CaseImportListItemXMLValue[$Key];
                              }//end switch
                           }//end foreach
//                           print_r($UpdatedColumnArray);exit;     
                           foreach($UpdatedColumnArray as $Key => $Value)
                              $EditKeyValue[$Value] = addslashes($CaseImportListItemXMLValue[$Key]);
                           $EditKeyValue['TestUserName'] = sysAddSlash($_SESSION['TestUserName']);
                           $EditKeyValue['LastActionID'] = testGetLastActionID('Case',$EditKeyValue['CaseID']);
                           
//                           print_r($EditKeyValue);exit;
                           testEditCase($EditKeyValue);
                           unset($UpdatedColumnArray);
                           unset($EditKeyValue);
                      }//end edit case
		      else { //CaseID不存在，则新增Case         
                          $InsertColumnArray = array();
                          foreach($ImportColumnArray as $Key => $Value){
                              $CaseImportListItemXMLValue[$Key]=trim($CaseImportListItemXMLValue[$Key]);
                              $Value = trim($Value);
			      if($Value!='CaseID'){
                                $InsertColumnArray[$Key] = $Value;
                                   switch(trim($Value)){
                                      case 'ProjectID':
                                           $CaseImportListItemXMLValue[$Key] = $ValidateColumns['ProjectID'];
                                           break;
                                      case 'ModuleID':
                                           $CaseImportListItemXMLValue[$Key] =  $ValidateColumns['ModuleID'];
                                           break;
                                      case 'AssignedTo':
                                           if(!dbGetRow('TestUser', 'UserName', "UserName='{$CaseImportListItemXMLValue[$Key]}'"))
                                              $CaseImportListItemXMLValue[$Key] = 'Active';
                                           break;
                                      case 'MarkForDeletion':
                                           if((trim($CaseImportListItemXMLValue[$Key])!='1'))
                                               $CaseImportListItemXMLValue[$Key] = 0;
                                           break;
                                      case 'DisplayOrder':
                                           if(is_numeric($CaseImportListItemXMLValue[$Key]) && (int)($CaseImportListItemXMLValue[$Key])>=1 && (int)($CaseImportListItemXMLValue[$Key])<=255);
                                           else  $CaseImportListItemXMLValue[$Key] = 0;
                                           break;
                                      case 'CaseStatus':
                                           if(!in_array($CaseImportListItemXMLValue[$Key],$_LANG['CaseStatuses']))
                                              $CaseImportListItemXMLValue[$Key] = 'Active';  
                                           break;
                                    }//end switch
                               }//end if
			   }//end foreach

                           $InsertKeyValue = array();
                           foreach($InsertColumnArray as $Key => $Value)
                              $InsertKeyValue[$Value] = addslashes($CaseImportListItemXMLValue[$Key]);
                          
                           if(!array_search('CasePriority',$InsertColumnArray))
                              $InsertKeyValue['CasePriority'] = '0';
                           if(!array_search('CaseType',$InsertColumnArray))
                              $InsertKeyValue['CaseType'] = 'Functional';
                           if(!array_search('CaseMethod',$InsertColumnArray))
                              $InsertKeyValue['CaseMethod'] ='Automation';
                           if(!array_search('AssignedTo',$InsertColumnArray))
                              $InsertKeyValue['AssignedTo'] ='Active';
                           if(!array_search('DisplayOrder',$InsertColumnArray))
                              $InsertKeyValue['DisplayOrder'] = 0;
                           $InsertKeyValue['TestUserName'] = sysAddSlash($_SESSION['TestUserName']);
                           
                           testOpenCase($InsertKeyValue);
                           unset($InsertKeyValue);
                           unset($InsertColumnArray);
                        }
                       }
                    }
             }
            $ImportFailItem = $ImportTotalItem-$ImportSuccessItem;
            $FinishAlert = str_replace('{param0}',$ImportTotalItem, $_LANG['ImportFinished']);
            $FinishAlert = str_replace('{param1}',$ImportSuccessItem, $FinishAlert);
            $FinishAlert = str_replace('{param2}',$ImportFailItem, $FinishAlert);
            
            sysObFlushJs("alert('{$FinishAlert}');");
            }
            fclose($file);

            sysObFlush(jsGoto("CaseList.php"));
            exit;
}


function GetValidateColumns($ItemRow,$ImportColumnArray){ //判断Case的ProjectID、ModuleID和CaseTitle是否合法，并返回

    $ProjectModules = array();
    $ProjectName = trim($ItemRow[array_search('ProjectName', $ImportColumnArray)]);

    $CaseProjectRow = dbGetRow('TestProject', 'ProjectID', "ProjectName='{$ProjectName}'");

    if($CaseProjectRow)
    {
       $ProjectModules['ProjectID'] = $CaseProjectRow['ProjectID'];
    }
    else{

        return NULL;
    }

    if(!in_array($ProjectModules['ProjectID'],array_keys(baseGetUserACL($_SESSION['TestUserName']))) )
        return NULL; //没有权限



    $ProjectModules['ModuleID']=-1; //初始化为-1
    $ModuleListArray = testGetProjectModuleList($ProjectModules['ProjectID'],'Case');
    foreach($ModuleListArray as $Item)
    {
         if(trim($ItemRow[array_search('ModulePath', $ImportColumnArray)]) == trim($Item['NamePath']))
         {
             $ProjectModules['ModuleID'] = $Item['ModuleID'];
             break;
         }
    }

    if($ProjectModules['ModuleID']==-1)//模块路径不存�?
        return NULL;

    $ProjectModules['CaseTitle'] = trim($ItemRow[array_search('CaseTitle', $ImportColumnArray)]);

    if(!$ProjectModules['CaseTitle'])//Case标题为空
    {         return NULL;
    }

    return $ProjectModules;
}
?>
