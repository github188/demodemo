// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "LoginDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
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
	CString host;
	this->GetDlgItemText(IDC_EDIT_SERVER, host);
	host.Append(":8000");

	if (pApp->pgkclient == NULL)
		pApp->pgkclient =new GokuClient(host, host);
	
#ifdef _DEBUG
	username = "test1";
	password = "pass";
#endif

	if(pApp->pgkclient->login(username, password)==0)
	{
		pApp->btsTotalStr.Empty();
		pApp->alarmStr.Empty();
		pApp->pgkclient->listbtstree(pApp->btsTotalStr);
		pApp->pgkclient->getAlarmStr(pApp->alarmStr);
		OnOK();
	}
	else
	{
		this->MessageBox("登录错误，请检查用户名密码是否正确");
	}
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString server="127.0.0.1";
	this->SetDlgItemText(IDC_EDIT_SERVER, server);


	//LogOn ICON
	CStatic*   pStatic =   (CStatic*)GetDlgItem(IDC_OGON_PIC);
	//pStatic->ModifyStyle(0xF, SS_BITMAP|SS_CENTERIMAGE);
	HICON  hIcon1  =  AfxGetApp()->LoadIcon(IDI_LOGON);
	pStatic->SetIcon(hIcon1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
