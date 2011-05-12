// WarningWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarningWnd.h"

#include "RuntimeWarning.h"
#include "CriticalWarning.h"
#include "GeneralWarning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CWarningWnd

IMPLEMENT_DYNAMIC(CWarningWnd, CDockablePane)

CWarningWnd::CWarningWnd()
: m_nWarnUpdateTimerID(0)
{
	m_pRuntimePg   = new CRuntimeWarning();
	m_pCriticalPg  = new CCriticalWarning();
//	m_pGereralPg   = new CGeneralWarning();

//	if ( !(m_pRuntimePg && m_pCriticalPg && m_pGereralPg) )
	if ( !(m_pRuntimePg && m_pCriticalPg) )
		AfxMessageBox("创建视图对象失败!");

}

CWarningWnd::~CWarningWnd()
{
	//KillTimer(m_nWarnUpdateTimerID);

	if (m_pRuntimePg)
		delete m_pRuntimePg;
	if (m_pCriticalPg )
		delete m_pCriticalPg ;
//	if (m_pGereralPg )
//		delete m_pGereralPg;

}


BEGIN_MESSAGE_MAP(CWarningWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
//	ON_WM_CONTEXTMENU()
//	ON_COMMAND(ID_WARNING_ACK, &CWarningWnd::OnWarningAck)
//	ON_UPDATE_COMMAND_UI(ID_WARNING_ACK, &CWarningWnd::OnUpdateWarningAck)
//	ON_COMMAND(ID_WARNING_SCROOLING_OFF, &CWarningWnd::OnWarningScroolingOff)
//	ON_COMMAND(ID_WARNING_SCROOLING_ON, &CWarningWnd::OnWarningScroolingOn)
//	ON_COMMAND(ID_DUMMY_COMPILE, &CWarningWnd::OnDummyCompile)
END_MESSAGE_MAP()



// CWarningWnd message handlers
int CWarningWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (m_pRuntimePg)
	{
		m_pRuntimePg->Create(IDD_RUNTIME_WARNING,   &m_wndTabs);
		m_pRuntimePg->SetFont(&m_Font);
		m_pRuntimePg->ShowWindow(SW_SHOW);
		m_pRuntimePg->UpdateWindow();
	}
	if (m_pCriticalPg )
	{
		m_pCriticalPg->Create(IDD_CRITICAL_WARNING, &m_wndTabs);
		m_pCriticalPg->SetFont(&m_Font);
		m_pCriticalPg->ShowWindow(SW_SHOW);
		m_pCriticalPg->UpdateWindow();
	}

//	if (m_pGereralPg )
//	{
//		m_pGereralPg->Create(IDD_CRITICAL_WARNING,  &m_wndTabs);
//		m_pGereralPg->SetFont(&m_Font);
//		m_pGereralPg->ShowWindow(SW_SHOW);
//		m_pGereralPg->UpdateWindow();
//	}

	
	// 将列表窗口附加到选项卡:
	CString strTabName("实时告警");
	m_wndTabs.AddTab(m_pRuntimePg, strTabName,  (UINT)0);
	strTabName="严重告警";
	m_wndTabs.AddTab(m_pCriticalPg, strTabName, (UINT)1);
//	strTabName="一般告警";
//	m_wndTabs.AddTab(m_pGereralPg, strTabName,  (UINT)2);

	m_nWarnUpdateTimerID = SetTimer(WM_WARNING_TIMER,WARNING_UPDATE_DURING,0);

	return 0;
}

void CWarningWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

}

void CWarningWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	
	if ( pApp->pgkclient->btsmanager.btsmap.IsEmpty() )
		return; //the BTS Device listview is still not built.


#ifdef _DEBUG	
	
//	pApp->pgkclient->getAlarmStr(pApp->alarmStr);
//	pApp->pgkclient->alarmmanager.getalarmList(pApp->alarmStr);

#else

	CString alarmStr;
	pApp->pgkclient->getRealTimeAlarmStr(alarmStr);
	pApp->pgkclient->alarmmanager.getalarmList(alarmStr);

#endif

	if (!pApp->pgkclient->alarmmanager.curRefreshAlarmList.IsEmpty())
	{
		m_pRuntimePg->AddListView(ALARM_REFRESH);
		m_pCriticalPg->AddListView(ALARM_REFRESH);
	}

	if (!pApp->pgkclient->alarmmanager.curNewAlarmList.IsEmpty())
	{
		m_pRuntimePg->AddListView(ALARM_NEW);
		m_pCriticalPg->AddListView(ALARM_NEW);
	}

	CDockablePane::OnTimer(nIDEvent);
}