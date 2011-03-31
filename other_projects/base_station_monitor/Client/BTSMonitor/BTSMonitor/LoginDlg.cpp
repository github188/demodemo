// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "LoginDlg.h"
#include "util.h"

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
	m_fSaveInfo.Close();
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
END_MESSAGE_MAP()


// CLoginDlg message handlers

void CLoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString username;
	CString password;
	this->GetDlgItemText(IDC_CBO_USER, username);
	((CEdit *)this->GetDlgItem(IDC_EDIT_PWD))->GetWindowText(password);
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	CString sHost,host, port;
	GetDlgItemText(IDC_CBO_SERVER, host);
	GetDlgItemText(IDC_CBO_PORT, port);
	sHost = host;
	host.Append(":");
	host.Append(port);

	CWaitCursor wait;
	if (pApp->pgkclient == NULL)
	{
		pApp->pgkclient =new GokuClient(host, host);
	}
	else 
	{
		if (!pApp->pgkclient->IsConnected())
		{
			pApp->pgkclient->socket->InitSocketParameter(host,host);
			pApp->pgkclient->ReConnectServer();
		}
	}

	//if ( pApp->pgkclient->IsConnected() )
	{
		if(pApp->pgkclient->login(username, password)==0)
		{
			pApp->btsTotalStr.Empty();
			pApp->alarmStr.Empty();
			pApp->pgkclient->listbtstree(pApp->btsTotalStr);

			//Save default data
			m_fSaveInfo.SetLength(0);

			CString sDefault = CString("default") + CString("\t") + username + CString("\t") + sHost + CString("\t") + port + CString("\r\n");
			m_fSaveInfo.WriteString(sDefault);

			int i=0;
			//User Info
			CString sUserInfo = "user";
			if (LB_ERR == m_cboUser.FindString(0,username))
			{
				sUserInfo.Append("\t");
				sUserInfo.Append(username);
			}
			int n = m_cboUser.GetCount();
			CString rString;
			for(i=0; i<n; i++)
			{
				m_cboUser.GetLBText(i,rString);
				if ( rString.IsEmpty() )
					continue;
				rString.Append("\t");
				sUserInfo.Append(rString);
			}
			sUserInfo.Append("\r\n");
			m_fSaveInfo.WriteString(sUserInfo);

			//Server Info
			CString sServerInfo = "server";
			if (LB_ERR == m_cboServer.FindString(0,sHost))
			{
				sServerInfo.Append("\t");
				sServerInfo.Append(sHost);
			}
			n = m_cboServer.GetCount();
			for(i=0; i<n; i++)
			{
				m_cboServer.GetLBText(i,rString);
				if ( rString.IsEmpty() )
					continue;
				rString.Append("\t");
				sServerInfo.Append(rString);
			}
			sServerInfo.Append("\r\n");
			m_fSaveInfo.WriteString(sServerInfo);

			//Port Info
			CString sPortInfo = "port";
			if (LB_ERR == m_cboPort.FindString(0,port))
			{
				sPortInfo.Append("\t");
				sPortInfo.Append(port);
			}
			n = m_cboPort.GetCount();
			for(i=0; i<n; i++)
			{
				m_cboPort.GetLBText(i,rString);
				if ( rString.IsEmpty() )
					continue;
				rString.Append("\t");
				sPortInfo.Append(rString);
			}
			sPortInfo.Append("\r\n");
			m_fSaveInfo.WriteString(sPortInfo);
			//
			OnOK();
		}
		else
			AfxMessageBox("登录错误，请检查用户名密码是否正确!");
	}
	
	wait.Restore();
	//Socket connect failed.

}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//CString server="192.168.1.200";
	//CString server="hzluming.3322.org";
	//this->SetDlgItemText(IDC_EDIT_SERVER, server);
	CFileException error;
	if ( ! m_fSaveInfo.Open("LogOnInfo.txt", CFile::modeCreate | CFile::modeReadWrite, &error) )
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
				pos = util::split_next(rOneLine, strServerDefault, '\t', pos+1);
				pos = util::split_next(rOneLine, strPortDefault, '\t', pos+1);
				
				continue;
			}

			if (temp == "user")
			{
				pos = util::split_next(rOneLine, temp, '\t', pos+1);
				strUserDefault = temp;
				while( !temp.IsEmpty() )
				{
					m_cboUser.AddString(temp);
					pos = util::split_next(rOneLine, temp, '\t', pos+1);
				}

				m_cboUser.SelectString(0,strUserDefault);
				continue;
			}

			if (temp == "server")
			{
				pos = util::split_next(rOneLine, temp, '\t', pos+1);
				strServerDefault = temp;
				while( !temp.IsEmpty() )
				{
					m_cboServer.AddString(temp);
					pos = util::split_next(rOneLine, temp, '\t', pos+1);
				}

				m_cboServer.SelectString(0,strServerDefault);
				continue;

			}

			if (temp == "port")
			{
				pos = util::split_next(rOneLine, temp, '\t', pos+1);
				strPortDefault = temp;
				while( !temp.IsEmpty() )
				{
					m_cboPort.AddString(temp);
					pos = util::split_next(rOneLine, temp, '\t', pos+1);
				}

				m_cboPort.SelectString(0,strPortDefault);
				continue;

			}
		}

	}

	//this->SetDlgItemText(IDC_EDT_PORT, "8000");


	//LogOn ICON
	CStatic*   pStatic =   (CStatic*)GetDlgItem(IDC_OGON_PIC);
	//pStatic->ModifyStyle(0xF, SS_BITMAP|SS_CENTERIMAGE);
	HICON  hIcon1  =  AfxGetApp()->LoadIcon(IDI_LOGON);
	pStatic->SetIcon(hIcon1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
