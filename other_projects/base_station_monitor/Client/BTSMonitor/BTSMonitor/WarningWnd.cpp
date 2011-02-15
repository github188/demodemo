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
{
	m_pRuntimePg   = new CRuntimeWarning();
	m_pCriticalPg  = new CCriticalWarning();
	m_pGereralPg   = new CGeneralWarning();

	if ( !(m_pRuntimePg && m_pCriticalPg && m_pGereralPg) )
		AfxMessageBox("������ͼ����ʧ��!");

}

CWarningWnd::~CWarningWnd()
{
	if (m_pRuntimePg)
		delete m_pRuntimePg;
	if (m_pCriticalPg )
		delete m_pCriticalPg ;
	if (m_pGereralPg )
		delete m_pGereralPg;

}


BEGIN_MESSAGE_MAP(CWarningWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
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

	// ����ѡ�����:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ������ѡ�����\n");
		return -1;      // δ�ܴ���
	}

	// �����������:
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

	if (m_pGereralPg )
	{
		m_pGereralPg->Create(IDD_CRITICAL_WARNING,  &m_wndTabs);
		m_pGereralPg->SetFont(&m_Font);
		m_pGereralPg->ShowWindow(SW_SHOW);
		m_pGereralPg->UpdateWindow();
	}

	
	// ���б��ڸ��ӵ�ѡ�:
	CString strTabName("ʵʱ�澯");
	m_wndTabs.AddTab(m_pRuntimePg, strTabName,  (UINT)0);
	strTabName="���ظ澯";
	m_wndTabs.AddTab(m_pCriticalPg, strTabName, (UINT)1);
	strTabName="һ��澯";
	m_wndTabs.AddTab(m_pGereralPg, strTabName,  (UINT)2);
	
	/*
	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("δ�ܴ����������\n");
		return -1;      // δ�ܴ���
	}

	m_wndOutputBuild.SetFont(&m_Font);
	m_wndOutputDebug.SetFont(&m_Font);
	m_wndOutputFind.SetFont(&m_Font);

	CString strTabName;
	BOOL bNameValid;

	// ���б��ڸ��ӵ�ѡ�:
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// ʹ��һЩ�����ı���д���ѡ�(���踴������)
	FillBuildWindow();
	FillDebugWindow();
	FillFindWindow();
	*/
	return 0;
}

void CWarningWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// ѡ��ؼ�Ӧ��������������:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

}

//void CWarningWnd::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//	// TODO: Add your message handler code here
//	CMFCTabCtrl* pTab = (CMFCTabCtrl*) &m_wndTabs;
//	ASSERT_VALID(pTab);
//
//	int nVisibleTab = pTab->GetFirstVisibleTabNum();
//	int nActTab = pTab->GetActiveTab();
//	CWnd *pActWindow = pTab->GetActiveWindow();
//	CWnd *pActWnd = pTab->GetActiveWnd();
//	
//	CRect rc;
//	GetClientRect(&rc);
//	if (pActWnd->m_hWnd == m_pRuntimePg->m_hWnd)
//	{
//		if (point != CPoint(-1, -1))
//		{
//			CPoint ptList = point;
//			m_pRuntimePg->ScreenToClient(&ptList);
//
//			UINT flags = 0;
//			int itemClicked = m_pRuntimePg->m_lstRuntimeWarning.HitTest(ptList, &flags);
//			if (itemClicked > -1)
//			{
//				//pTab->SetFocus();
//				m_pRuntimePg->SetFocus();
//				//HMENU hMenu = theApp.GetContextMenuManager()->GetMenuById(IDR_MENU_WARNING);
//				//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_WARNING,point.x, point.y, this);
//				theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);	
//
//			}
//		}
//
//		
//	}
//	else if (pActWnd->m_hWnd == m_pCriticalPg->m_hWnd)
//		;
//	else if (pActWnd->m_hWnd == m_pGereralPg->m_hWnd)
//		;
//	else
//	{
//		CDockablePane::OnContextMenu(pWnd, point);
//		return;
//	}
//
//}

//void CWarningWnd::OnWarningAck()
//{
//	// TODO: Add your command handler code here
//}

//void CWarningWnd::OnUpdateWarningAck(CCmdUI *pCmdUI)
//{
//	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(TRUE);
//}

//void CWarningWnd::OnWarningScroolingOff()
//{
//	// TODO: Add your command handler code here
//}

//void CWarningWnd::OnWarningScroolingOn()
//{
//	// TODO: Add your command handler code here
//}

//void CWarningWnd::OnDummyCompile()
//{
//	// TODO: Add your command handler code here
//}
