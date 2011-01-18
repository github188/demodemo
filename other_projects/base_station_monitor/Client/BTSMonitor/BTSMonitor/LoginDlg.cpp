// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "LoginDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg message handlers

void CLoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString username;
	CString password;
	this->GetDlgItemText(IDC_EDIT_USER, username);
	((CEdit *)this->GetDlgItem(IDC_EDIT_PWD))->GetWindowText(password);
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	
	if(pApp->pgkclient->login(username, password)==0)
	{
		pApp->btsTotalStr.Empty();
		pApp->pgkclient->listbtstree(pApp->btsTotalStr);
		OnOK();
	}
	else
	{
		this->MessageBox("登录错误，请检查用户名密码是否正确");
	}
}
