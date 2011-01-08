// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rob1.h"
#include "LoginDlg.h"

#include "MainFrm.h"

CString g_val=L"1234";

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
	//check the username and password.
	//UpdateData(TRUE);
	CString username;
	CString password;
	this->GetDlgItemText(IDC_EDIT1, username);
	((CEdit *)this->GetDlgItem(IDC_EDIT2))->GetWindowText(password);
	if(username==L"tttt")
	{
		g_val=L"abcd";
		//CMainFrame * pmainfrm=(CMainFrame *)AfxGetApp()->m_pMainWnd;
		//pmainfrm->store_val=L"test";
		Crob1App *pApp=(Crob1App *)AfxGetApp();
		pApp->appData=_T("99999");
		OnOK();
	}
	else
	{
		this->MessageBox(L"Error");
		//OnCancel();
	}
}
