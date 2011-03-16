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
	, m_strPort(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PORT, m_strPort);
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
	CString host, port;
	GetDlgItemText(IDC_EDIT_SERVER, host);
	GetDlgItemText(IDC_EDT_PORT, port);
	host.Append(":");
	host.Append(port);

	CWaitCursor wait;
	if (pApp->pgkclient == NULL)
	{
		pApp->pgkclient =new GokuClient(host, host);
	}
	else 
	{
		pApp->pgkclient->socket->InitSocketParameter(host,host);
		pApp->pgkclient->ReConnectServer();
	}

	if ( pApp->pgkclient->IsConnected() )
	{
		if(pApp->pgkclient->login(username, password)==0)
		{
			pApp->btsTotalStr.Empty();
			pApp->alarmStr.Empty();
			pApp->pgkclient->listbtstree(pApp->btsTotalStr);

			OnOK();
		}
		else
			AfxMessageBox("��¼���������û��������Ƿ���ȷ");
	}
	
	wait.Restore();
	//Socket connect failed.

}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//CString server="192.168.1.200";
	CString server="60.177.179.182";
	this->SetDlgItemText(IDC_EDIT_SERVER, server);

	this->SetDlgItemText(IDC_EDT_PORT, "8000");


	//LogOn ICON
	CStatic*   pStatic =   (CStatic*)GetDlgItem(IDC_OGON_PIC);
	//pStatic->ModifyStyle(0xF, SS_BITMAP|SS_CENTERIMAGE);
	HICON  hIcon1  =  AfxGetApp()->LoadIcon(IDI_LOGON);
	pStatic->SetIcon(hIcon1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
