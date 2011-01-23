// RuntimeWarning.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "RuntimeWarning.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
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
		m_lstRuntimeWarning.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 88);

	m_imagelist.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
	m_lstRuntimeWarning.SetImageList(&m_imagelist,LVSIL_SMALL);
	
	CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();
	HICON hIcon = pApp->LoadIconA(IDI_UNKNOWN);
	if (hIcon)
		m_imagelist.Add(hIcon);

	hIcon = pApp->LoadIconA(IDI_UNACK);
	if (hIcon)
		m_imagelist.Add(hIcon);

	hIcon = pApp->LoadIconA(IDI_ACK);
	if (hIcon)
		m_imagelist.Add(hIcon);

	AddListView();

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
	CString sUUID;
	CString str("100");
	for (int i=0; i<10; i++)
	{
		sUUID.Format("%s%d", str, i);

		m_lstRuntimeWarning.InsertItem(0,sUUID);
		//LV_ITEM lvitemAdd = {0};
		//lvitemAdd.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		//lvitemAdd.iItem = nIndex ;
		//lvitemAdd.iSubItem = 0;
		//lvitemAdd.pszText =_T("毛毛1");;
		//lvitemAdd.iImage = n;
		//lvitemAdd.lParam = (LPARAM)hwnd;(某个窗口的窗口句柄)
		m_lstRuntimeWarning.SetItem(0,1,LVIF_TEXT|LVIF_IMAGE,"State",-1,0,0,111);
	}
}
