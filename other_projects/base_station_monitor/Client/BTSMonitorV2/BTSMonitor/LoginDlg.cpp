// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "LoginDlg.h"
#include "util.h"
#include "Login.h"

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
	DDX_Control(pDX, IDC_CBO_USER, m_cboUser);
	DDX_Control(pDX, IDC_CBO_SERVER, m_cboServer);
	DDX_Control(pDX, IDC_CBO_PORT, m_cboPort);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_MESSAGE(MSG_LOGINOK, &CLoginDlg::LoginOK)
	ON_MESSAGE(MSG_LOGINFAIL, &CLoginDlg::LoginFail)
	ON_MESSAGE(MSG_GETBTSLISTOK, &CLoginDlg::GetBtsListOK)
	ON_MESSAGE(MSG_GETBTSLISTFAIL, &CLoginDlg::GetBtsListFail)
END_MESSAGE_MAP()


// CLoginDlg message handlers

LRESULT CLoginDlg::GetBtsListOK(WPARAM wParam, LPARAM lParam)
{
	CString *str=(CString *)wParam;
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	pApp->btsTotalStr=*str;
	delete str;
	OnOK();
	return 0;
}

LRESULT CLoginDlg::GetBtsListFail(WPARAM wParam, LPARAM lParam)
{
	::AfxMessageBox("得到基站列表失败");
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->SetWindowText("登录");
	return 0;
}

LRESULT CLoginDlg::LoginOK(WPARAM wParam, LPARAM lParam)
{
	//get the session ID
	::PostThreadMessage(loginThread->m_nThreadID, MSG_GETBTSLIST, 0,0);
	GetDlgItem(IDOK)->SetWindowText("正获得基站列表……");
	//OnOK();
	return 0;
}

LRESULT CLoginDlg::LoginFail(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("登录错误，请检查用户名密码是否正确!");
	return 0;
}

void CLoginDlg::OnBnClickedOk()
{
	CString username;
	CString password;
	GetDlgItemText(IDC_CBO_USER, username);
	GetDlgItemText(IDC_EDIT_PWD, password);

	CString host, port;
	GetDlgItemText(IDC_CBO_SERVER, host);
	GetDlgItemText(IDC_CBO_PORT, port);

	uinfo *pinfo=new uinfo();
	pinfo->uname=username;
	pinfo->password=password;
	pinfo->servIP=host;
	pinfo->servPort=port;
	//pinfo->hThreadId=::GetCurrentThreadId();
	pinfo->UIhwnd=this->m_hWnd;
	loginThread=::AfxBeginThread(loginProc, pinfo);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->SetWindowText("正在登陆中……");
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//CString server="192.168.1.200";
	//CString server="hzluming.3322.org";
	//this->SetDlgItemText(IDC_EDIT_SERVER, server);
	CFileException error;
	if ( ! m_fSaveInfo.Open("LogOnInfo.txt", CFile::modeNoTruncate | CFile::modeCreate | CFile::modeReadWrite, &error) )
	{
		CString strError;
		strError.Format( "Can't open file %s, error = %u\n", "LogOnInfo.txt", error.m_cause );
		AfxMessageBox(strError);		
	}
	else
	{
		ULONGLONG nSize = m_fSaveInfo.GetLength();
		if (m_fSaveInfo.GetLength() == 0)
		{
			CString strLogOnInfo = "default	admin	0.0.0.0	8000";
			strLogOnInfo.Append("\r\n");
			m_fSaveInfo.WriteString(strLogOnInfo);

			//user
			strLogOnInfo = "user	admin";
			strLogOnInfo.Append("\r\n");
			m_fSaveInfo.WriteString(strLogOnInfo);

			//server port
			strLogOnInfo = "server	0.0.0.0";
			strLogOnInfo.Append("\r\n");
			m_fSaveInfo.WriteString(strLogOnInfo);

			//server 
			strLogOnInfo = "port	8000";
			strLogOnInfo.Append("\r\n");
			m_fSaveInfo.WriteString(strLogOnInfo);

			m_fSaveInfo.SeekToBegin();
		}

		//Init combox
		CString strUserDefault, strServerDefault,strPortDefault;

		CString temp;
		int pos = 0;
		m_cboUser.ResetContent();
		CString rOneLine;
		while (m_fSaveInfo.ReadString(rOneLine))
		{
			pos=util::split_next(rOneLine, temp, '\t', 0);
			if (temp == "default")
			{
				pos = util::split_next(rOneLine, strUserDefault, '\t', pos+1);
				util::TrimSpecialChar(strUserDefault,0xd);

				pos = util::split_next(rOneLine, strServerDefault, '\t', pos+1);
				util::TrimSpecialChar(strServerDefault,0xd);

				pos = util::split_next(rOneLine, strPortDefault, '\t', pos+1);
				util::TrimSpecialChar(strPortDefault,0xd);

				//Remove the 
				
				continue;
			}

			if (temp == "user")
			{
				pos = util::split_next(rOneLine, temp, '\t', pos+1);
				while( !temp.IsEmpty() )
				{
					util::TrimSpecialChar(temp,0xd);
					m_cboUser.AddString(temp);
					pos = util::split_next(rOneLine, temp, '\t', pos+1);
				}

				m_cboUser.SelectString(0,strUserDefault);
				continue;
			}

			if (temp == "server")
			{
				pos = util::split_next(rOneLine, temp, '\t', pos+1);
				while( !temp.IsEmpty() )
				{
					util::TrimSpecialChar(temp,0xd);
					m_cboServer.AddString(temp);
					pos = util::split_next(rOneLine, temp, '\t', pos+1);
				}

				m_cboServer.SelectString(0,strServerDefault);
				continue;

			}

			if (temp == "port")
			{
				pos = util::split_next(rOneLine, temp, '\t', pos+1);
				while( !temp.IsEmpty() )
				{
					util::TrimSpecialChar(temp,0xd);
					m_cboPort.AddString(temp);
					pos = util::split_next(rOneLine, temp, '\t', pos+1);
				}

				m_cboPort.SelectString(0,strPortDefault);
				continue;

			}
		}

	}
	m_fSaveInfo.Close();

	//this->SetDlgItemText(IDC_EDT_PORT, "8000");


	//LogOn ICON
	CStatic*   pStatic =   (CStatic*)GetDlgItem(IDC_OGON_PIC);
	//pStatic->ModifyStyle(0xF, SS_BITMAP|SS_CENTERIMAGE);
	HICON  hIcon1  =  AfxGetApp()->LoadIcon(IDI_LOGON);
	pStatic->SetIcon(hIcon1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
