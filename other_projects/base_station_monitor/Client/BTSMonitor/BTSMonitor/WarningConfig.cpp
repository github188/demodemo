// WarningConfig.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarningConfig.h"


// CWarningConfig dialog

IMPLEMENT_DYNAMIC(CWarningConfig, CDialog)

CWarningConfig::CWarningConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningConfig::IDD, pParent)
{

}

CWarningConfig::~CWarningConfig()
{
}

void CWarningConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarningConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CWarningConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWarningConfig::OnBnClickedCancel)
END_MESSAGE_MAP()


// CWarningConfig message handlers

BOOL CWarningConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWarningConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CWarningConfig::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
