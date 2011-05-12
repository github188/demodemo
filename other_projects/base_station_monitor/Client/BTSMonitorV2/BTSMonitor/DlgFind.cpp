// DlgFind.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "DlgFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgFind dialog

IMPLEMENT_DYNAMIC(CDlgFind, CDialog)

CDlgFind::CDlgFind(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFind::IDD, pParent)
	, m_sFindStr(_T(""))
{

}

CDlgFind::~CDlgFind()
{
}

void CDlgFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINDSTR, m_sFindStr);
}


BEGIN_MESSAGE_MAP(CDlgFind, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgFind::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFind message handlers

BOOL CDlgFind::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFind::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_sFindStr.IsEmpty())
	{
		AfxMessageBox("请输入要查找的关键字!");
		return;
	}

	OnOK();
}

CString CDlgFind::GetFindStr(void)
{
	return m_sFindStr;
}
