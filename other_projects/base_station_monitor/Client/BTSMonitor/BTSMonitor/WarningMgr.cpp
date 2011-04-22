// WarningMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarningMgr.h"
#include "include/iPlay.h"
#include "const.h"
// CWarningMgr dialog

IMPLEMENT_DYNAMIC(CWarningMgr, CDialog)

CWarningMgr::CWarningMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningMgr::IDD, pParent)
	, m_sFilter(_T(""))
	, m_nCurItem(0)
	, m_menuPt(0)
	, m_bPopMenu(false)
{

}

CWarningMgr::~CWarningMgr()
{

}

void CWarningMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEVICE_FIND, m_sFilter);
	DDX_Control(pDX, IDC_TREE_WARN_MGR_DEVICE, m_treeWarnMgr);
	DDX_Control(pDX, IDC_LST_TARGET_WARNING, m_lstFindWarnResult);
	DDX_Control(pDX, IDC_CBO_WARN_TYPE, m_cboWarnType);
	DDX_Control(pDX, IDC_CBO_WARN_ACK_TYPE, m_cboWarnAckType);
	DDX_Control(pDX, IDC_DATE_BEGIN, m_dateBegin);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_Control(pDX, IDC_CBO_START_HOUR, m_cboStartHour);
	DDX_Control(pDX, IDC_CBO_END_HOUR, m_cboEndHour);
	DDX_Control(pDX, IDC_CBO_START_MINUTE, m_cboStartMin);
	DDX_Control(pDX, IDC_CBO_END_MINUTE, m_cboEndMin);
	DDX_Control(pDX, IDC_CBO_START_SECOND, m_cboStartSec);
	DDX_Control(pDX, IDC_CBO_END_SECOND, m_cboEndSec);
	DDX_Control(pDX, IDC_WINDOW_VEDIO, m_wndWarnVedio);
}


BEGIN_MESSAGE_MAP(CWarningMgr, CDialog)
	ON_BN_CLICKED(IDC_BTN_WARN_MGR_DEVICE_FIND, &CWarningMgr::OnBnClickedBtnWarnMgrDeviceFind)
	ON_BN_CLICKED(IDC_FIND_TARGET_WARNING, &CWarningMgr::OnBnClickedFindTargetWarning)
	ON_NOTIFY(NM_CLICK, IDC_TREE_WARN_MGR_DEVICE, &CWarningMgr::OnNMClickTreeWarnMgrDevice)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CWarningMgr::OnHdnItemclickLstTargetWarning)
	ON_NOTIFY(NM_CLICK, IDC_LST_TARGET_WARNING, &CWarningMgr::OnNMClickLstTargetWarning)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_TARGET_WARNING, &CWarningMgr::OnNMDblclkLstTargetWarning)
	ON_NOTIFY(NM_RCLICK, IDC_LST_TARGET_WARNING, &CWarningMgr::OnNMRClickLstTargetWarning)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WARNINGMGR_EXPORT, &CWarningMgr::OnWarningmgrExport)
	ON_UPDATE_COMMAND_UI(ID_WARNINGMGR_EXPORT, &CWarningMgr::OnUpdateWarningmgrExport)
	ON_COMMAND(ID_WARNINGMGR_SAVEAS, &CWarningMgr::OnWarningmgrSaveas)
	ON_UPDATE_COMMAND_UI(ID_WARNINGMGR_SAVEAS, &CWarningMgr::OnUpdateWarningmgrSaveas)
END_MESSAGE_MAP()


// CWarningMgr message handlers

BOOL CWarningMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//Extend Style
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_lstFindWarnResult.m_hWnd, styles, styles );

	CString strHeader[] = 
	{
		"告警级别", //level
		"位置",		//place
		"端局类型", //BTSType
		"告警类型", //alarmCode :1:当前实时告警, 正在发生的告警;　2:历史告警记录
		"基站",		//BTSID
		"开始时间", //start tiem
		"结束时间", //end   time
		"处理状态", //status
		""
	};

	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	int i=0;
	for (; i<nCnt-1; i++)
		m_lstFindWarnResult.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 100);
	m_lstFindWarnResult.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 0);
	

	//List Warning 
	m_imagelist.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
	m_lstFindWarnResult.SetImageList(&m_imagelist,LVSIL_SMALL);
	
	CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();
	HICON hIcon = pApp->LoadIconA(IDI_LEVEL_1);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_2);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_3);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_4);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_5);
	if (hIcon)		m_imagelist.Add(hIcon);

	//Tree View 
	m_treeImages.Create(IDB_CAMERA_VIEW, 16, 0, RGB(255, 0, 255));
	m_treeWarnMgr.SetImageList(&m_treeImages, TVSIL_NORMAL);

	//Init combo 
	InitVedioDeviceTree();
	
	//Init Warning Type
	m_cboWarnType.ResetContent();
	m_cboWarnType.AddString("");
	m_cboWarnType.AddString("所有告警"); //all
	m_cboWarnType.AddString("视频遮掩"); //1000　未定义
	m_cboWarnType.AddString("红外");     //1001 未定义
	m_cboWarnType.AddString("视频丢失"); //1002
	m_cboWarnType.AddString("动态侦测"); //1003
	m_cboWarnType.AddString("连接超时"); //2001

	//Init Ack type
	m_cboWarnAckType.ResetContent();
	m_cboWarnAckType.AddString("所有类型"); //all
	m_cboWarnAckType.AddString("未确认");  //1
	m_cboWarnAckType.AddString("告警超时自动确认"); //2
	m_cboWarnAckType.AddString("手动确认"); //3

	//Init Time
	m_cboStartHour.ResetContent();
	m_cboEndHour.ResetContent();
	m_cboStartMin.ResetContent();
	m_cboStartSec.ResetContent();
	m_cboEndMin.ResetContent();
	m_cboEndSec.ResetContent();
	i=0;
	CString strTime;
	for(; i<24; i++) //Hour
	{
		strTime.Format("%d",i);
		m_cboStartHour.AddString(strTime);
		m_cboEndHour.AddString(strTime);
	}

	i=0;
	for(; i<60; i++) //minute
	{
		strTime.Format("%d",i);
		m_cboStartMin.AddString(strTime);
		m_cboEndMin.AddString(strTime);

		m_cboStartSec.AddString(strTime);
		m_cboEndSec.AddString(strTime);
	}

	bool bDebug=false;
	int nCount = 0;
	if (bDebug)
	{
		//"告警状态", //level
		m_lstFindWarnResult.InsertItem(nCount,"",0);
		m_lstFindWarnResult.SetItem(nCount,0,LVIF_IMAGE,"",1,0,0,0);

		//"UUID",		//UUID
		//m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);
		
		//"位置",		//place
		CString sLocation = "位置"; //m_btsMgr.GetCameraPlace(pAlarmInfo->uuid);
		m_lstFindWarnResult.SetItem(nCount,1,LVIF_TEXT,sLocation,0,0,0,0);

		//端局类型
		m_lstFindWarnResult.SetItem(nCount,2,LVIF_TEXT,"端局类型",0,0,0,0);

		//"告警类型", //alarmCode :1:当前实时告警, 正在发生的告警;2:历史告警记录
		m_lstFindWarnResult.SetItem(nCount,3,LVIF_TEXT,"1001",0,0,0,0);
		
		//"基站",		//BTSID
		m_lstFindWarnResult.SetItem(nCount,4,LVIF_TEXT,"btsid",0,0,0,0);
		//"开始时间", //start tiem
		m_lstFindWarnResult.SetItem(nCount,5,LVIF_TEXT,"2011-03-08 15:30:00",0,0,0,0);
		//"结束时间", //end   time
		m_lstFindWarnResult.SetItem(nCount,6,LVIF_TEXT,"2011-03-08 15:30:00",0,0,0,0);
		//"处理状态" //status
		CString sTemp = "手动确认";
		m_lstFindWarnResult.SetItem(nCount,7,LVIF_TEXT,sTemp,0,0,0,0);	

		m_lstFindWarnResult.SetItem(nCount,8,LVIF_TEXT,"uuid",0,0,0,0);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//Find Device...
void CWarningMgr::OnBnClickedBtnWarnMgrDeviceFind()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_DEVICE_FIND,str);

	if ( str.IsEmpty() ) return;

	BOOL bFind = FindNewTarget( str );
	if (!bFind)
		AfxMessageBox("没有找到!");

}
//Find target warning...
void CWarningMgr::OnBnClickedFindTargetWarning()
{
	// TODO: Add your control notification handler code here
	//Clear old find Result
	m_lstFindWarnResult.DeleteAllItems();

	//int nWarnLevel = m_cboWarnType.GetCurSel();
	int nCategory = m_cboWarnType.GetCurSel();
	CString sCategory = (nCategory == 0) ? "all" :
						(nCategory == 1) ? "1000":
						(nCategory == 2) ? "1001":
						(nCategory == 3) ? "1002":
						(nCategory == 4) ? "1003":
						(nCategory == 5) ? "2001": "all";


	int nAckType   = m_cboWarnAckType.GetCurSel();
	CString sAckType =  (nAckType == 0) ? "all" :
						(nAckType == 1) ? "1":
					    (nAckType == 2) ? "2":"3";
	// get as a CTime
	CTime timeStartDate,timeEndDate ;
	CString strStartDate,strEndDate;
	DWORD dwResult = m_dateBegin.GetTime(timeStartDate);
	if (dwResult == GDT_VALID)
		strStartDate = timeStartDate.Format(_T("%Y-%m-%d"));
	else
	{
		AfxMessageBox(_T("Start Time not set!"));
		return;
	}

	dwResult = m_dateEnd.GetTime(timeEndDate);
	if (dwResult == GDT_VALID)
		strEndDate = timeEndDate.Format(_T("%Y-%m-%d"));
	else
	{
		AfxMessageBox(_T("Start Time not set!"));
		return;
	}
	
	CString strStartTime, strEndTime;
	
	int nStartHour = m_cboStartHour.GetCurSel();
	if (nStartHour == -1) nStartHour = 0;

	int nEndHour = m_cboEndHour.GetCurSel();
	if (nEndHour == -1)	nEndHour=0;

	int nStartMin = m_cboStartMin.GetCurSel();
	if (nStartMin == -1) nStartMin = 0;

	int nEndMin = m_cboEndMin.GetCurSel();
	if (nEndMin == -1)	nEndMin = 0;

	int nStartSec = m_cboStartSec.GetCurSel();
	if (nStartSec == -1) nStartSec = 0;

	int nEndSec    = m_cboEndSec.GetCurSel();
	if (nEndSec == -1)	nEndSec = 0;
	
	strStartTime.Format("%02d:%02d:%02d",nStartHour,nStartMin,nStartSec);
	strEndTime.Format("%02d:%02d:%02d",nEndHour,nEndMin,nEndSec);

	//Get Current Selected Item UUID	
	CString sLevel, sLimit, sOffset, sQAlarmStr;
	sLimit.Empty();
	sOffset.Empty();
	sQAlarmStr.Empty();

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	//pApp->pgkclient->queryAlarmInfo(category,uuid,startDate,starttime,type,level,limit,offset,qalarmStr);
	//m_btsMgr.buildbtsTree(pApp->btsTotalStr, &m_treeWarnMgr);
	CString sNameItem = m_treeWarnMgr.GetItemText(m_hItemCurFind);
	CString sUUID; // = m_btsMgr.GetCameraUUID(sNameItem); 
	util::split_next(sNameItem,sUUID,'_',0);
	pApp->pgkclient->queryAlarmInfo(sCategory,sUUID,strStartDate,strStartTime,sAckType,sLevel,sLimit,sOffset,sQAlarmStr);
	if (sQAlarmStr.IsEmpty())
	{
		AfxMessageBox("没有相关告警!");
		return;
	}

	//显示相关告警信息在列表中
	m_alarmMgr.getalarmList(sQAlarmStr,true);
	CString sLocation, sTemp;
	int nCount = 0;
	AlarmInfo* pAlarmInfo = NULL;	
	POSITION pos = m_alarmMgr.alarmList.GetHeadPosition();
	while( pos!=NULL )
	{
		pAlarmInfo = m_alarmMgr.alarmList.GetNext(pos);
		if (pAlarmInfo)
		{
			//"告警状态", //level
			m_lstFindWarnResult.InsertItem(nCount,"",0);
			m_lstFindWarnResult.SetItem(nCount,0,LVIF_IMAGE,"",atoi(pAlarmInfo->level),0,0,0);

			//"UUID",		//UUID
			//m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);
			
			//"位置",		//place
			sLocation = m_btsMgr.GetCameraPlace(pAlarmInfo->BTSID);
			m_lstFindWarnResult.SetItem(nCount,1,LVIF_TEXT,sLocation,0,0,0,0);

			//端局类型
			m_lstFindWarnResult.SetItem(nCount,2,LVIF_TEXT,pAlarmInfo->BTSType,0,0,0,0);

			//"告警类型", //alarmCode :1:当前实时告警, 正在发生的告警;2:历史告警记录
			m_lstFindWarnResult.SetItem(nCount,3,LVIF_TEXT,pAlarmInfo->alarmCode,0,0,0,0);
			
			//"基站",		//BTSID
			m_lstFindWarnResult.SetItem(nCount,4,LVIF_TEXT,pAlarmInfo->BTSID,0,0,0,0);
			//"开始时间", //start tiem
			m_lstFindWarnResult.SetItem(nCount,5,LVIF_TEXT,pAlarmInfo->startTime,0,0,0,0);
			//"结束时间", //end   time
			m_lstFindWarnResult.SetItem(nCount,6,LVIF_TEXT,pAlarmInfo->endTime,0,0,0,0);
			//"处理状态" //status
			sTemp = pAlarmInfo->status==1 ? "未处理":
				pAlarmInfo->status==2 ? "超时自动处理":"手动确认";
			m_lstFindWarnResult.SetItem(nCount,7,LVIF_TEXT,sTemp,0,0,0,0);	
	
			m_lstFindWarnResult.SetItem(nCount,8,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);	

			nCount++;

		}// ArmList
	}
	
}

void CWarningMgr::InitVedioDeviceTree(void)
{
	bool bTest = false;
	if (bTest)
	{
		enum {euHZ, euYH};
		char sCitys[2][10] = {"杭州","余杭"};
		char sHZCityDistrict[6][10] = {"上城区", "下城区","西湖区","江干区","下沙区","滨江区"};
		//char sHZ_ShangCQ[6][10] = {"ShangCQ1","ShangCQ2","ShangCQ3","ShangCQ4","ShangCQ5","ShangCQ6"};
		char sHZ_CQ_CAMERA[6][10] = {"ShangCQ","XiaCQ","XiHQ","JiangGQ","XiaSQ","BinJQ"};

		char sYHCityDistrict[6][10] = {"余一区", "余二区","余三区","余四区","余五区","余六区"};
		char sYH_CQ_CAMERA[6][10] = {"YuYQ","YuEQ","YuSQ","YuSQ","YuWQ","YuLQ"};

		//char *pDistrict[2][]={sHZCityDistrict, sYHCityDistrict};

		HTREEITEM m_hRootItem = m_treeWarnMgr.InsertItem("BTS List", 0, 0);
		m_treeWarnMgr.SetItemState(m_hRootItem, TVIS_BOLD, TVIS_BOLD);

		int nCitys = 2;
		int nDistrict = 6;
		int nCamera	  = 6;
		CString sCity, sDistrict, sCamera;
		for (int nC=0; nC<nCitys; nC++)
		{
			sCity = sCitys[nC];
			HTREEITEM hCity = m_treeWarnMgr.InsertItem(sCity, 1, 1, m_hRootItem);
			switch(nC)
			{
			case euHZ:
				{
					for (int nDS = 0; nDS<nDistrict; nDS++)
					{
		
						sDistrict = sHZCityDistrict[nDS];
						HTREEITEM hDistrict = m_treeWarnMgr.InsertItem(sDistrict, 1,1, hCity);
						for(int nCM=0; nCM<nCamera; nCM++) //Each District have six camera
						{
								sCamera.Format("%s_Camera_%d",sHZ_CQ_CAMERA[nDS], nCM);
								m_treeWarnMgr.InsertItem(sCamera,2,2, hDistrict);
						}
					}
				}
				break;
			case euYH:
				{
					for (int nDS = 0; nDS<nDistrict; nDS++)
					{
		
						sDistrict = sYHCityDistrict[nDS];
						HTREEITEM hDistrict = m_treeWarnMgr.InsertItem(sDistrict,1,1, hCity);
						for(int nCM=0; nCM<nCamera; nCM++) //Each District have six camera
						{
								sCamera.Format("%s_Camera_%d",sYH_CQ_CAMERA[nDS], nCM);
								m_treeWarnMgr.InsertItem(sCamera,2,2, hDistrict);
						}
					}
				}
				break;
			default:
				break;
			}
		}

		return;
	}

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	m_btsMgr.buildbtsTree(pApp->btsTotalStr, &m_treeWarnMgr);
	

	/*
	POSITION pos = pApp->pgkclient->btsmanager.btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		pApp->pgkclient->btsmanager.btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo)
		{			
			if(pBTSInfo->parentuuid=="0")
			{
				HTREEITEM htitem=pBTSInfo->insertRootIntoTree(&m_treeWarnMgr);
			}
			else
			{
				BTSInfo *pParentInfo=pApp->pgkclient->btsmanager.btsmap[util::str2int(pBTSInfo->parentuuid)];
				pBTSInfo->insertIntoTree(&m_treeWarnMgr, pParentInfo->getPosInTree());
			}

		}
	}
	*/

	
	//buildbtsTree(pApp->btsTotalStr, &m_treeWarnMgr);
}
HTREEITEM CWarningMgr::FindTarget(HTREEITEM  hItem, CString sName, bool bFindNext)
{
	if (hItem == NULL)
		hItem = m_treeWarnMgr.GetRootItem();

	if (hItem == NULL)
		return NULL;

	CString szString = m_treeWarnMgr.GetItemText(hItem);
	if (szString.Find(sName) > -1)
	{
		m_treeWarnMgr.SelectSetFirstVisible(hItem);
		m_treeWarnMgr.SelectItem(hItem);
		return hItem;
	}


	if (m_treeWarnMgr.ItemHasChildren(hItem) == FALSE)
		return NULL;

	//递归查找hItem的所有子节点
	HTREEITEM hRes = NULL;
	HTREEITEM hItemChild = m_treeWarnMgr.GetChildItem(hItem);
	while (hItemChild)
	{
		hRes = FindTarget(hItemChild, sName); //查以hItem为根的枝
		if (hRes != NULL)					  //如果在以hItem为根的枝里找到，返回结果
			return hRes;
		else								  //否则，查找与hItem同级的下一个枝
			hItemChild = m_treeWarnMgr.GetNextSiblingItem(hItemChild);
	} // end of while(hItem != NULL, has next item)

	return NULL; 
}

BOOL CWarningMgr::FindNewTarget(CString sFindStr)
{
	m_hItemCurFind = FindTarget(NULL, sFindStr);

	BOOL bRet = FALSE;
	if (m_hItemCurFind)
	{
		m_sFindStr = sFindStr;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CWarningMgr::FindNextTarget(void)
{
	if (m_sFindStr.IsEmpty())
		return FALSE;

	BOOL bRet = FALSE;

	HTREEITEM hItemFind = NULL;
	HTREEITEM hItemChild = m_treeWarnMgr.GetChildItem(m_hItemCurFind);
	if (hItemChild)
		hItemFind = FindTarget(hItemChild, m_sFindStr);

	if (hItemFind)
	{
		m_hItemCurFind = hItemFind;
		return TRUE;
	}


	//Brother's Item need to find.
	HTREEITEM hItemBrother=m_treeWarnMgr.GetNextSiblingItem(m_hItemCurFind); //ignor itself
	while(hItemBrother)
	{
		hItemFind = FindTarget(hItemBrother, m_sFindStr);
		if (hItemFind)
		{
			m_hItemCurFind = hItemFind;
			return TRUE;
		}
		hItemBrother=m_treeWarnMgr.GetNextSiblingItem(hItemBrother);
	}

	//Father's Item Need to find
	HTREEITEM hItemFather = m_treeWarnMgr.GetParentItem(m_hItemCurFind);
	while(hItemFather)
	{
		CString szString = m_treeWarnMgr.GetItemText(hItemFather); //ignor itself
		hItemBrother = hItemFather; //polling all item of Father's brother
		do //all of the brother...
		{
			hItemBrother = m_treeWarnMgr.GetNextSiblingItem(hItemBrother);
			if (hItemBrother)
				hItemFind = FindTarget(hItemBrother, m_sFindStr);
			else
				break;

			if (hItemFind)
			{
				m_hItemCurFind = hItemFind;
				return TRUE;
			}
		}
		while(hItemBrother); 

		hItemFather = m_treeWarnMgr.GetParentItem(hItemFather); //polling father's father
	}
	
	//-----------------------

	return bRet;
}
BOOL CWarningMgr::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message >= WM_KEYFIRST &&	pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_F3: //Find Next
			{
				if (pMsg->lParam>0)
				{
					//m_bFindNext=TRUE;
					BOOL bFind = FindNextTarget( );
					if (!bFind)
						AfxMessageBox("没有找到!");
				}
			}
			break;				
		default:
			break;
		};
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CWarningMgr::OnNMClickTreeWarnMgrDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
    UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_treeWarnMgr.ScreenToClient(&curPoint);
	HTREEITEM htItem = m_treeWarnMgr.HitTest(curPoint, &nFlags);

	if ( htItem && (TVHT_ONITEM & nFlags) )
	{
		//CString str = m_treeWarnMgr.GetItemText(htItem);
		m_hItemCurFind = htItem;
	}

	*pResult = 0;
}

void CWarningMgr::OnHdnItemclickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CWarningMgr::OnNMClickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_nCurItem = pNMItemActivate->iItem;

	*pResult = 0;
}

//Play the target warning vedio
void CWarningMgr::OnNMDblclkLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMItemActivate->iItem == -1)
	{
		*pResult = 0;
		return;
	}

	CString sUUID		= m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 8); 
	CString sStartTime  = m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 6); 
	CString sEndTime    = m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 7); 


	int nActView = 0; 
	BOOL bDebug = FALSE;
	if (bDebug)
	{
		PLAY_CloseFile(nActView);

		CString sVVFile;
		CString path="F:\\Projects\\Video\\BTSMonitor\\test\\";
		CString sVideo[] = {"test01.h264","test02.h264","test03.h264","test04.h264","test05.h264","test06.h264","test07.h264"};

		
		sVVFile = path + sVideo[1];
		
		BOOL bPlayFile = TRUE;
		if (bPlayFile)
		{
			PLAY_OpenFile(nActView, sVVFile.GetBuffer());
		
			PLAY_Play(nActView, m_wndWarnVedio.m_hWnd);
		}

	}
	else
	{

		CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();

		//Close 
		PLAY_CloseStream(nActView);

		//Open New 
		PLAY_SetStreamOpenMode(nActView, STREAME_REALTIME);
		BOOL bOpenRet = PLAY_OpenStream(nActView,0,0,1024*900);
		if(bOpenRet)
		{
			PLAY_Play(nActView, m_wndWarnVedio.m_hWnd);
			//Play Remote Vedio runatime			
			pApp->pgkclient->replay(sUUID, play_video, nActView);
		}

	}

	*pResult = 0;
}

void CWarningMgr::OnNMRClickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
    UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_menuPt = curPoint;
	m_lstFindWarnResult.ScreenToClient(&curPoint);
	int nItem = m_lstFindWarnResult.HitTest(curPoint, &nFlags);

	m_nCurItem = pNMItemActivate->iItem;

//	if ( nItem && (TVHT_ONITEM & nFlags) )
//	{
//		m_hItemCurFind = htItem;
//	}

	CMenu myMenu ;
	myMenu.LoadMenu( IDR_POPMENU_WARNMGR ) ;
	CMenu *pMenu = myMenu.GetSubMenu(0) ;
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, m_menuPt.x, m_menuPt.y, this ) ;

	m_bPopMenu = true;

	*pResult = 0;
}

void CWarningMgr::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	//if (m_bPopMenu)
	//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPMENU_WARNMGR, m_menuPt.x, m_menuPt.y, this, TRUE);	

	//m_bPopMenu = false;
}

void CWarningMgr::OnWarningmgrExport()
{
	// TODO: Add your command handler code here
	CFileDialog FileDlg(TRUE,_T(""),"WarningInfomation.txt",OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,NULL,NULL);
	FileDlg.m_ofn.lpstrTitle = "";
	char sDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH,sDir);
	FileDlg.m_ofn.lpstrInitialDir = sDir;
	CString strMapPath;
	if(FileDlg.DoModal()==IDOK)
	{
		strMapPath = FileDlg.GetPathName();
		//if(!FileExists(m_strMapPath))
		//	return;
	}

	//save listview info to the file
	CFileException error;
	CFile fVideoInfo;//(strMapPath,
	if ( !fVideoInfo.Open(strMapPath,CFile::modeCreate|CFile::modeWrite, &error) )
	{
		char   szCause[255];
		CString strFormatted;
		error.GetErrorMessage(szCause, 255);
		// (in real life, it's probably more
		// appropriate to read this from
		//  a string resource so it would be easy to
		// localize)
		strFormatted = _T("The data file could not be opened because of this error: ");
		strFormatted += szCause;
		AfxMessageBox(strFormatted);
		return;
	}
	
	//File Title
	char lpBuf[MAX_PATH];
	memset(lpBuf,0,MAX_PATH);
	char *pTitle = "告警级别\t位置\t端局类型\t告警类型\t基站\t开始时间\t结束时间\t处理状态";
	sprintf_s(lpBuf,MAX_PATH, "%s\r\n", pTitle);
	fVideoInfo.Write(lpBuf,strlen(lpBuf));
	
	LVITEM lvItem;
	CString strTemp;
	int nCnt = m_lstFindWarnResult.GetItemCount();
	for (int i=0; i<nCnt; i++)
	{
		//"告警状态", //level
		lvItem.mask = LVIF_IMAGE;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		m_lstFindWarnResult.GetItem(&lvItem);
		_itoa_s(lvItem.iImage,lpBuf,sizeof(lpBuf), 10);
		fVideoInfo.Write(lpBuf, strlen(lpBuf));
		fVideoInfo.Write("\t", 1);
		
		//"位置",		//place
		strTemp = m_lstFindWarnResult.GetItemText(i,1) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//端局类型 .BTSType
		strTemp = m_lstFindWarnResult.GetItemText(i,2) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"告警类型", //alarmCode :1:当前实时告警, 正在发生的告警;2:历史告警记录
		strTemp = m_lstFindWarnResult.GetItemText(i,3) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());
		
		//"基站",		//BTSID
		strTemp = m_lstFindWarnResult.GetItemText(i,4) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"开始时间", //start tiem
		strTemp = m_lstFindWarnResult.GetItemText(i,5) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"结束时间", //end   time
		strTemp = m_lstFindWarnResult.GetItemText(i,6) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"处理状态" //status	sTemp = pAlarmInfo->status==1 ? "未处理":pAlarmInfo->status==2 ? "超时自动处理":"手动确认";	
		strTemp = m_lstFindWarnResult.GetItemText(i,7) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		strTemp = m_lstFindWarnResult.GetItemText(i,8) + "\r\n";	
		fVideoInfo.Write(strTemp, strTemp.GetLength());
	}

	//..
	fVideoInfo.Close();
}

void CWarningMgr::OnUpdateWarningmgrExport(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CWarningMgr::OnWarningmgrSaveas()
{
	// TODO: Add your command handler code here


}

void CWarningMgr::OnUpdateWarningmgrSaveas(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
