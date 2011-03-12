// UserMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "UserMgr.h"


// CUserMgr dialog

IMPLEMENT_DYNAMIC(CUserMgr, CDialog)

CUserMgr::CUserMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMgr::IDD, pParent)
	, m_strUser(_T(""))
	, m_strOldPw(_T(""))
	, m_strNewPw(_T(""))
	, m_strNewPw2(_T(""))
{

}

CUserMgr::~CUserMgr()
{
}

void CUserMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strOldPw);
	DDX_Text(pDX, IDC_EDT_NEW_PW, m_strNewPw);
	DDX_Text(pDX, IDC_EDT_NEW_PW2, m_strNewPw2);
}


BEGIN_MESSAGE_MAP(CUserMgr, CDialog)
	ON_BN_CLICKED(IDC_BTN_MOD, &CUserMgr::OnBnClickedBtnMod)
END_MESSAGE_MAP()


// CUserMgr message handlers


void CUserMgr::OnBnClickedBtnMod()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString sSID;
	if (m_strNewPw.Compare(m_strNewPw2) == 0)
	{
		//
		CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
		int nRet = 0; //pApp->pgkclient->ModifyUserInfo(sSID,m_strOldPw,m_strNewPw);

		if (nRet==1)
		{
			AfxMessageBox("会话过期!");
			return;
		}
		else if (nRet == 2)
		{
			AfxMessageBox("旧密码错误!");
			return; 
		}
		else
			AfxMessageBox("设置成功!");
	}

}

BOOL CUserMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString sUser, sPw;
	int nSid;
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	pApp->pgkclient->GetUserInfo(sUser,sPw, nSid);

	m_strUser = sUser;

	UpdateData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
