// CriticalWarning.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "CriticalWarning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CCriticalWarning dialog

IMPLEMENT_DYNAMIC(CCriticalWarning, CPropertyPage)

CCriticalWarning::CCriticalWarning()
	: CPropertyPage(CCriticalWarning::IDD)
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
END_MESSAGE_MAP()


// CCriticalWarning message handlers

BOOL CCriticalWarning::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strHeader[] = 
	{
		"UUID",
		"位置",
		"端局类型",
		"告警类型",
		"基站",
		"开始时间",
		"结束时间",
		"确认状态"
	};
	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	for (int i=0; i<nCnt; i++)
		m_lstCriticalWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 88);

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
