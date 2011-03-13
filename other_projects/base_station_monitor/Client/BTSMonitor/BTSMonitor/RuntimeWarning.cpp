// RuntimeWarning.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "RuntimeWarning.h"
#include "WarnPopVideo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CRuntimeWarning dialog
static BOOL bOnScroll = FALSE;
IMPLEMENT_DYNAMIC(CRuntimeWarning, CPropertyPage)

CRuntimeWarning::CRuntimeWarning()
	: CPropertyPage(CRuntimeWarning::IDD)
	, m_nCurItem(0)
	, m_alarmIndex(0)
	, m_nPopViewCount(0)
{
	int i=0;
	for (;i<cnMAX_POP_WINDOW; i++)
	{
		m_pPopVideoDlg[i] = NULL;
	}
}

CRuntimeWarning::~CRuntimeWarning()
{
	//KillTimer(WM_RUNTIME_TIMER);
	int i=0;
	for (;i<cnMAX_POP_WINDOW; i++)
	{
		if (m_pPopVideoDlg[i])
			delete m_pPopVideoDlg[i];
	}
}

void CRuntimeWarning::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_RUNTIME_WARNING, m_lstRuntimeWarning);
}


BEGIN_MESSAGE_MAP(CRuntimeWarning, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_NOTIFY(NM_RCLICK, IDC_LST_RUNTIME_WARNING, &CRuntimeWarning::OnNMRClickLstRuntimeWarning)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WARNING_ACK, &CRuntimeWarning::OnWarningAck)
	ON_COMMAND(ID_WARNING_SCROOLING_OFF, &CRuntimeWarning::OnWarningScroolingOff)
	ON_COMMAND(ID_WARNING_SCROOLING_ON, &CRuntimeWarning::OnWarningScroolingOn)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CRuntimeWarning::OnHdnItemdblclickLstRuntimeWarning)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_RUNTIME_WARNING, &CRuntimeWarning::OnNMDblclkLstRuntimeWarning)
END_MESSAGE_MAP()


// CRuntimeWarning message handlers

BOOL CRuntimeWarning::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	//olde style
	DWORD dwStyle = ::GetWindowLong(m_lstRuntimeWarning.m_hWnd, GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);

	//New Style
	SetWindowLong(m_lstRuntimeWarning.m_hWnd,GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);

	//Extend Style
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_lstRuntimeWarning.m_hWnd, styles, styles );

	CString strHeader[] = 
	{
		"�澯����", //level
		"λ��",		//place
		"�˾�����", //BTSType
		"�澯����", //alarmCode :1:��ǰʵʱ�澯, ���ڷ����ĸ澯;2:��ʷ�澯��¼
		"��վ",		//BTSID
		"��ʼʱ��", //start tiem
		"����ʱ��", //end   time
		"����״̬", //status
		""			//UUID
	};

	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	int i=0;
	for (; i<nCnt-1; i++)
		m_lstRuntimeWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 120);
	m_lstRuntimeWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 0);
	

	m_imagelist.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
	m_lstRuntimeWarning.SetImageList(&m_imagelist,LVSIL_SMALL);
	
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


	//SetTimer(WM_RUNTIME_TIMER,1000,NULL);

	i=0;
	for (; i<cnMAX_POP_WINDOW; i++)
	{
		VERIFY(m_pPopVideoDlg[i] = new CWarnPopVideo(this) );
		m_pPopVideoDlg[i]->SetPopVideoIndex(i);
		m_pPopVideoDlg[i]->Create(IDD_POP_VIDEO);//(IDD_POP_VIDEO,this); //AfxGetApp()->m_pMainWnd);
		m_pPopVideoDlg[i]->ShowWindow(SW_HIDE);

	}

	
	//AddListView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRuntimeWarning::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (IsWindow(m_lstRuntimeWarning.m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);
		m_lstRuntimeWarning.MoveWindow(0,0,cx,cy);
	}
}

void CRuntimeWarning::AddListView(ALARM_COMING_TYPE type)
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
		if (pAlarmInfo)
		{
			int lstIdx = 0;
			if (type == ALARM_NEW)
			{
				//"�澯״̬", //level
				m_lstRuntimeWarning.InsertItem(m_alarmIndex,"",0);
				m_lstRuntimeWarning.SetItem(m_alarmIndex,0,LVIF_IMAGE,"",atoi(pAlarmInfo->level),0,0,0);
			}
			else if (type == ALARM_REFRESH)
			{
				CString sUUID;
				int nItemCount = m_lstRuntimeWarning.GetItemCount();
				for (int i=0; i<nItemCount; i++)
				{
					sUUID = m_lstRuntimeWarning.GetItemText(i, 8);
					if (sUUID == pAlarmInfo->uuid)
					{
						lstIdx = i;
						break;
					}
				}
			}

			//"UUID",		//UUID
			//m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);
			
			//"λ��",		//place
			sLocation = pApp->pgkclient->btsmanager.GetCameraPlace(pAlarmInfo->uuid);
			m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,sLocation,0,0,0,0);

			//�˾�����
			m_lstRuntimeWarning.SetItem(m_alarmIndex,2,LVIF_TEXT,pAlarmInfo->BTSType,0,0,0,0);

			//"�澯����", //alarmCode :1:��ǰʵʱ�澯, ���ڷ����ĸ澯;2:��ʷ�澯��¼
			m_lstRuntimeWarning.SetItem(m_alarmIndex,3,LVIF_TEXT,pAlarmInfo->alarmCode,0,0,0,0);
			
			//"��վ",		//BTSID
			m_lstRuntimeWarning.SetItem(m_alarmIndex,4,LVIF_TEXT,pAlarmInfo->BTSID,0,0,0,0);
			//"��ʼʱ��", //start tiem
			m_lstRuntimeWarning.SetItem(m_alarmIndex,5,LVIF_TEXT,pAlarmInfo->startTime,0,0,0,0);
			//"����ʱ��", //end   time
			m_lstRuntimeWarning.SetItem(m_alarmIndex,6,LVIF_TEXT,pAlarmInfo->endTime,0,0,0,0);
			//"����״̬" //status
			sTemp = 	pAlarmInfo->status==2 ? "��ʱ�Զ�����":
						pAlarmInfo->status==3 ? "�ֶ�ȷ��":"δ����";
				;
			m_lstRuntimeWarning.SetItem(m_alarmIndex,7,LVIF_TEXT,sTemp,0,0,0,0);	
	
			m_lstRuntimeWarning.SetItem(m_alarmIndex,8,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);	

			//if no BTSID, then continue;
			if ( pAlarmInfo->BTSID.IsEmpty() )
				continue;

			//.pop the warning vedio windows
			if (m_nPopViewCount<cnMAX_POP_WINDOW && pAlarmInfo->category == "1")
			{
				for (int i=0; i<cnMAX_POP_WINDOW; i++)
				{
					if (m_pPopVideoDlg[i]->IsShowing() == FALSE)
					{
						//m_pPopVideoDlg[i]->SetPopVideoIndex(i);
						m_pPopVideoDlg[i]->SetVideoPara(pAlarmInfo->BTSID,pAlarmInfo->uuid, sLocation,pAlarmInfo->ChannelID,pAlarmInfo->startTime, pAlarmInfo->endTime);
						//m_pPopVideoDlg[m_nPopViewCount]->Create(IDD_POP_VIDEO,this);
						//m_pPopVideoDlg[m_nPopViewCount]->Create(IDD_POP_VIDEO,AfxGetApp()->m_pMainWnd);
						m_pPopVideoDlg[i]->ShowWindow(SW_SHOW);
						m_pPopVideoDlg[i]->PlayVideo();
						break;
					}
				}
			}

		}// ArmList
	}
	/*
	static bool bShow = FALSE;
	if (!bShow)
	{
		for (int i=0; i<cnMAX_POP_WINDOW; i++)
		{
			m_pPopVideoDlg[i]->Create(IDD_POP_VIDEO, AfxGetApp()->m_pMainWnd);
			m_pPopVideoDlg[i]->ShowWindow(SW_SHOW);
		}

		bShow = TRUE;
	}
	*/
	
}

void CRuntimeWarning::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int  nn = 0;
	if (nn<100)
	{
		//Warning Status
		m_lstRuntimeWarning.InsertItem(nn,"",0);
		m_lstRuntimeWarning.SetItemData(nn,0);

		//UUID
		CString sUUID;
		CString str("100");
		sUUID.Format("%s%d", str, nn);
		m_lstRuntimeWarning.SetItem(nn,1,LVIF_TEXT,sUUID,0,0,0,0);

		//Location
		m_lstRuntimeWarning.SetItem(nn,2,LVIF_TEXT,"����",0,0,0,0);

		//BTS Type 1,2,3,4,5
		m_lstRuntimeWarning.SetItem(nn,3,LVIF_TEXT,"BTS Type",0,0,0,0);

		//Warning 
		int nWarnLevel = nn%9;
		CString sWarning = (nWarnLevel < 4) ? "һ��澯" : 
			(nWarnLevel == 5) ? "���ظ澯" : "�޸澯";
		m_lstRuntimeWarning.SetItem(nn,4,LVIF_TEXT,sWarning,0,0,0,0);

		//Change Image 
		if (nWarnLevel <= 5)
		{
			m_lstRuntimeWarning.SetItem(nn,0,LVIF_IMAGE,"",1,0,0,0);
			m_lstRuntimeWarning.SetItemData(nn,1);
		}
		else if (nWarnLevel > 5 && nWarnLevel < 9)
		{
			m_lstRuntimeWarning.SetItem(nn,0,LVIF_IMAGE,"",2,0,0,0);
			m_lstRuntimeWarning.SetItemData(nn,2);
		}


		//BTS Name 
		m_lstRuntimeWarning.SetItem(nn,5,LVIF_TEXT,"Ǯ��������",0,0,0,0);

		CTime timeGet = CTime::GetCurrentTime();
		CString strTime = timeGet.Format("%m/%d/%y %H-%M-%S");;

		m_lstRuntimeWarning.SetItem(nn,6,LVIF_TEXT,strTime,0,0,0,0);
		m_lstRuntimeWarning.SetItem(nn,7,LVIF_TEXT,strTime,0,0,0,0);
	}

	if (bOnScroll)
	{
		m_lstRuntimeWarning.EnsureVisible(nn,TRUE);
	}
	nn++;

	
	CPropertyPage::OnTimer(nIDEvent);
}

void CRuntimeWarning::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CPropertyPage::OnRButtonUp(nFlags, point);
}

void CRuntimeWarning::OnNMRClickLstRuntimeWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	//POINT mouse;
	//GetCursorPos(&mouse);

	m_nCurItem = pNMItemActivate->iItem;

	*pResult = 0;
}


void CRuntimeWarning::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_WARNING, point.x, point.y, this, TRUE);	
}

void CRuntimeWarning::OnWarningAck()
{
	// TODO: Add your command handler code here
	if (m_nCurItem<0)
		return;


	//DWORD dwImage = m_lstRuntimeWarning.GetItemData(m_nCurItem);
	//if (dwImage == WARNING_UNKNOWN || dwImage == WARNING_ACK)
	//	return;

	//WARNING_UNACK
	CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();

	CString sUUID = m_lstRuntimeWarning.GetItemText(m_nCurItem,8);
	if ( pApp->pgkclient->confirmAlarm(sUUID) )
	{
		m_lstRuntimeWarning.SetItem(m_nCurItem,0,LVIF_IMAGE,"",WARNING_ACK,0,0,0);
		//m_lstRuntimeWarning.GetItemData(WARNING_ACK);
		m_lstRuntimeWarning.DeleteItem(m_nCurItem);
		m_nCurItem = -1;
	}
	else
		AfxMessageBox("�澯ȷ��ʧ��!");


	/*
	UINT nSelCnt = m_lstRuntimeWarning.GetSelectedCount();
	POSITION pos = m_lstRuntimeWarning.GetFirstSelectedItemPosition();
	if (pos!=NULL)
	{
		while(pos)
		{
			int nSel = m_lstRuntimeWarning.GetNextSelectedItem( pos );

		}	
	}
	*/
}

void CRuntimeWarning::OnWarningScroolingOff( )
{
	// TODO: Add your command handler code here
	//m_lstRuntimeWarning.EnsureVisible(m_nCurItem,TRUE);
	bOnScroll = FALSE;
	
	m_lstRuntimeWarning.EnsureVisible(m_nCurItem,TRUE);

}

void CRuntimeWarning::OnWarningScroolingOn( )
{
	// TODO: Add your command handler code here
	//m_lstRuntimeWarning.EnsureVisible(m_nCurItem,FALSE);
	//m_lstRuntimeWarning.EnsureVisible(0,TRUE);
	bOnScroll = TRUE;

}

//void CRuntimeWarning::OnHdnItemdblclickLstRuntimeWarning(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CRuntimeWarning::OnNMDblclkLstRuntimeWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ( (pNMItemActivate!= NULL) && (pNMItemActivate->iItem > -1) )
	{
		//LV_ITEM lvitem = {0};
		//lvitem.iItem = pNMItemActivate->iItem;
		//lvitem.iSubItem = 1; //UUID
		//lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		//m_lstRuntimeWarning.GetItem(&...)
		CString sUUID = m_lstRuntimeWarning.GetItemText(pNMItemActivate->iItem, 1); 
		CString sStartTime = m_lstRuntimeWarning.GetItemText(pNMItemActivate->iItem, 6); 
		CString sEndTime   = m_lstRuntimeWarning.GetItemText(pNMItemActivate->iItem, 7); 

		//ͨ��UUID������ʷ��Ƶ
		//ReplayWarningVideo(sUUID, sStartTime,sEndTIme);
	}

	*pResult = 0;
}

void CRuntimeWarning::DecPopVedioCount(void)
{
	m_nPopViewCount--;
}
void CRuntimeWarning::IncPopVedioCount(void)
{
	m_nPopViewCount++;
}

bool CRuntimeWarning::AckedWarning(CString sUUID)
{
	bool bAcked = false;
	int nCnt = m_lstRuntimeWarning.GetItemCount();
	for (int i=0; i<nCnt; i++)
	{
		if (m_lstRuntimeWarning.GetItemText(i,8) == sUUID)
		{

			CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();
			if ( pApp->pgkclient->confirmAlarm(sUUID) )
			{
				//m_lstRuntimeWarning.SetItem(m_nCurItem,0,LVIF_IMAGE,"",WARNING_ACK,0,0,0);
				//m_lstRuntimeWarning.GetItemData(WARNING_ACK);
				m_lstRuntimeWarning.DeleteItem(i);
				bAcked = true;
			}
			else
			{
				AfxMessageBox("�澯ȷ��ʧ��!");
				bAcked = false;
			}

			break;
		}
	}

	return bAcked;
}
