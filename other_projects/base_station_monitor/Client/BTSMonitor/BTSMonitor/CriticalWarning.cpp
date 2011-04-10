// CriticalWarning.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "CriticalWarning.h"
#include "RuntimeWarning.h"
#include "WarningWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CCriticalWarning dialog

IMPLEMENT_DYNAMIC(CCriticalWarning, CPropertyPage)

CCriticalWarning::CCriticalWarning()
	: CPropertyPage(CCriticalWarning::IDD)
	, m_nCurItem(0)
	, m_alarmIndex(0)
	,m_bOnScroll(false)
{

}

CCriticalWarning::~CCriticalWarning()
{
}

void CCriticalWarning::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CRITICAL_WARNING, m_lstCriticalWarning);
}


BEGIN_MESSAGE_MAP(CCriticalWarning, CPropertyPage)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CRITICAL_WARNING, &CCriticalWarning::OnNMRClickListCriticalWarning)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WARNING_ACK, &CCriticalWarning::OnWarningAck)
	ON_COMMAND(ID_WARNING_SCROOLING_OFF, &CCriticalWarning::OnWarningScroolingOff)
	ON_COMMAND(ID_WARNING_SCROOLING_ON, &CCriticalWarning::OnWarningScroolingOn)
END_MESSAGE_MAP()


// CCriticalWarning message handlers

BOOL CCriticalWarning::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	//olde style
	DWORD dwStyle = ::GetWindowLong(m_lstCriticalWarning.m_hWnd, GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);

	//New Style
	SetWindowLong(m_lstCriticalWarning.m_hWnd,GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);

	//Extend Style
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_lstCriticalWarning.m_hWnd, styles, styles );

	CString strHeader[] = 
	{
		"告警级别", //level
		"位置",		//place
		"端局类型", //BTSType
		"告警类型", //alarmCode :1:当前实时告警, 正在发生的告警;2:历史告警记录
		"基站",		//BTSID
		"开始时间", //start tiem
		"结束时间", //end   time
		"处理状态", //status
		""			//UUID
	};

	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	int i=0;
	for (; i<nCnt-1; i++)
		m_lstCriticalWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 120);
	m_lstCriticalWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 0);
	

	m_imagelist.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
	m_lstCriticalWarning.SetImageList(&m_imagelist,LVSIL_SMALL);
	
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



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCriticalWarning::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (IsWindow(m_lstCriticalWarning.m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);
		m_lstCriticalWarning.MoveWindow(0,0,cx,cy);
	}
}
void CCriticalWarning::AddListView(ALARM_COMING_TYPE type)
{
	//fill data
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	//pApp->pgkclient->alarmmanager.getalarmList(pApp->alarmStr);

	CString sLocation, sTemp;
	AlarmInfo* pAlarmInfo = NULL;
	
	POSITION pos = NULL;
	if (type == ALARM_NEW)
		pos = pApp->pgkclient->alarmmanager.curNewAlarmList.GetHeadPosition();
	else if (type == ALARM_REFRESH)
		pos = pApp->pgkclient->alarmmanager.curNewAlarmList.GetHeadPosition();

	while( pos!=NULL )
	{
		pAlarmInfo = pApp->pgkclient->alarmmanager.alarmList.GetNext(pos);
		
		if (!pAlarmInfo)
			continue;

		if ( atoi(pAlarmInfo->level) < 4 )
			continue;

		int lstIdx = 0;
		if (type == ALARM_NEW)
		{
			//"告警状态", //level
			m_lstCriticalWarning.InsertItem(m_alarmIndex,"",0);
			m_lstCriticalWarning.SetItem(m_alarmIndex,0,LVIF_IMAGE,"",atoi(pAlarmInfo->level),0,0,0);
		}
		else if (type == ALARM_REFRESH)
		{
			CString sUUID;
			int nItemCount = m_lstCriticalWarning.GetItemCount();
			for (int i=0; i<nItemCount; i++)
			{
				sUUID = m_lstCriticalWarning.GetItemText(i, 8);
				if (sUUID == pAlarmInfo->uuid)
				{
					lstIdx = i;
					break;
				}
			}
		}

		//"UUID",		//UUID
		//m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);
		
		//"位置",		//place
		sLocation = pApp->pgkclient->btsmanager.GetCameraPlace(pAlarmInfo->BTSID);
		m_lstCriticalWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,sLocation,0,0,0,0);

		//端局类型
		m_lstCriticalWarning.SetItem(m_alarmIndex,2,LVIF_TEXT,pAlarmInfo->BTSType,0,0,0,0);

		//"告警类型", //alarmCode :1:当前实时告警, 正在发生的告警;2:历史告警记录
		m_lstCriticalWarning.SetItem(m_alarmIndex,3,LVIF_TEXT,pAlarmInfo->alarmCode,0,0,0,0);
		
		//"基站",		//BTSID
		m_lstCriticalWarning.SetItem(m_alarmIndex,4,LVIF_TEXT,pAlarmInfo->BTSID,0,0,0,0);
		//"开始时间", //start tiem
		m_lstCriticalWarning.SetItem(m_alarmIndex,5,LVIF_TEXT,pAlarmInfo->startTime,0,0,0,0);
		//"结束时间", //end   time
		m_lstCriticalWarning.SetItem(m_alarmIndex,6,LVIF_TEXT,pAlarmInfo->endTime,0,0,0,0);
		//"处理状态" //status
		sTemp = 	pAlarmInfo->status==2 ? "超时自动处理":
					pAlarmInfo->status==3 ? "手动确认":"未处理";
			;
		m_lstCriticalWarning.SetItem(m_alarmIndex,7,LVIF_TEXT,sTemp,0,0,0,0);	

		m_lstCriticalWarning.SetItem(m_alarmIndex,8,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);	

		//if no BTSID, then continue;
		if ( pAlarmInfo->BTSID.IsEmpty() )
			continue;

	} //while...	
}
void CCriticalWarning::OnNMRClickListCriticalWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_nCurItem = pNMItemActivate->iItem;

	*pResult = 0;
}

void CCriticalWarning::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_WARNING, point.x, point.y, this, TRUE);	

}

void CCriticalWarning::OnWarningAck()
{
	// TODO: Add your command handler code here
	if (m_nCurItem<0)
		return;


	//DWORD dwImage = m_lstRuntimeWarning.GetItemData(m_nCurItem);
	//if (dwImage == WARNING_UNKNOWN || dwImage == WARNING_ACK)
	//	return;

	//WARNING_UNACK
	CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();

	CString sUUID = m_lstCriticalWarning.GetItemText(m_nCurItem,8);
	if ( pApp->pgkclient->confirmAlarm(sUUID) )
	{
		LV_ITEM lvitem = {0};
		lvitem.iItem = m_nCurItem;
		lvitem.iSubItem = 0; //UUID
		lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		m_lstCriticalWarning.GetItem(&lvitem);

		m_lstCriticalWarning.SetItem(m_nCurItem,0,LVIF_IMAGE,"",WARNING_ACK,0,0,0);
		m_lstCriticalWarning.DeleteItem(m_nCurItem);
		m_nCurItem = -1;

		//if this warning is critical warning, should be delete the counterpart item in the critical listview
		CWarningWnd* pParent = (CWarningWnd*)GetParent();//DeleteItemByUUID(sUUID);
		if (pParent)
			pParent->m_pRuntimePg->DeleteItemByUUID(sUUID);
	}
	else
		AfxMessageBox("告警确认失败!");


}

void CCriticalWarning::OnWarningScroolingOff()
{
	// TODO: Add your command handler code here
	m_bOnScroll = false;
	
	m_lstCriticalWarning.EnsureVisible(m_nCurItem,TRUE);
}

void CCriticalWarning::OnWarningScroolingOn()
{
	// TODO: Add your command handler code here
	m_bOnScroll = true;
}

bool CCriticalWarning::DeleteItemByUUID(CString sUUID)
{
	int i=0;
	CString curUUID;
	bool bRet = false;
	int nCnt = m_lstCriticalWarning.GetItemCount();
	for(i=0; i<nCnt; i++)
	{
		curUUID = m_lstCriticalWarning.GetItemText(i,8);
		if (curUUID == sUUID)
		{
			m_lstCriticalWarning.DeleteItem(i);
			bRet = true;
			break;
		}
	}

	return bRet;

}
