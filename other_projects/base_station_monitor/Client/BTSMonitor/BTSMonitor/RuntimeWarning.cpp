// RuntimeWarning.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "RuntimeWarning.h"


// CRuntimeWarning dialog

IMPLEMENT_DYNAMIC(CRuntimeWarning, CPropertyPage)

CRuntimeWarning::CRuntimeWarning()
	: CPropertyPage(CRuntimeWarning::IDD)
{

}

CRuntimeWarning::~CRuntimeWarning()
{
}

void CRuntimeWarning::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_RUNTIME_WARNING, m_lstRuntimeWarning);
}


BEGIN_MESSAGE_MAP(CRuntimeWarning, CPropertyPage)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRuntimeWarning message handlers

BOOL CRuntimeWarning::OnInitDialog()
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
		"结束时间"
	};
	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	for (int i=0; i<nCnt; i++)
		m_lstRuntimeWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 88);


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

void CRuntimeWarning::AddListView(void)
{

}
