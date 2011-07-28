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
		int nRet = pApp->pgkclient->ModifyPW(m_strOldPw,m_strNewPw);

		if (nRet==1)
		{
			AfxMessageBox("�Ự����!");
			return;
		}
		else if (nRet == 2)
		{
			AfxMessageBox("���������!");
			return; 
		}
		else
			AfxMessageBox("���óɹ�!");
	}

}

BOOL CUserMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString sUser, sPw, sSid;
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	pApp->pgkclient->GetUserInfo(sUser,sPw, sSid);

	m_strUser = sUser;

	UpdateData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
