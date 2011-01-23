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
		AfxMessageBox("创建视图对象失败!");

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

	if (m_pGereralPg )
	{
		m_pGereralPg->Create(IDD_CRITICAL_WARNING,  &m_wndTabs);
		m_pGereralPg->SetFont(&m_Font);
		m_pGereralPg->ShowWindow(SW_SHOW);
		m_pGereralPg->UpdateWindow();
	}

	
	// 将列表窗口附加到选项卡:
	CString strTabName("实时告警");
	m_wndTabs.AddTab(m_pRuntimePg, strTabName,  (UINT)0);
	strTabName="严重告警";
	m_wndTabs.AddTab(m_pCriticalPg, strTabName, (UINT)1);
	strTabName="一般告警";
	m_wndTabs.AddTab(m_pGereralPg, strTabName,  (UINT)2);
	
	/*
	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}

	m_wndOutputBuild.SetFont(&m_Font);
	m_wndOutputDebug.SetFont(&m_Font);
	m_wndOutputFind.SetFont(&m_Font);

	CString strTabName;
	BOOL bNameValid;

	// 将列表窗口附加到选项卡:
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// 使用一些虚拟文本填写输出选项卡(无需复杂数据)
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
	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

}
