// WarningConfig.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarningConfig.h"
#include "util.h"
#include "const.h"
#include "ConfigMgr.h"

// CWarningConfig dialog

IMPLEMENT_DYNAMIC(CWarningConfig, CDialog)

CWarningConfig::CWarningConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningConfig::IDD, pParent)
	, m_strWarnSound(_T(""))
{

}

CWarningConfig::~CWarningConfig()
{
}

void CWarningConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PICTURE_WINDOW, m_cboPicWin);
	DDX_Control(pDX, IDC_CMB_PICTURE_PLAY_INTERVAL, m_cboPlayInterval);
	DDX_Control(pDX, IDC_CHK_NO_WARNING, m_chkWarnEnable);
	DDX_Text(pDX, IDC_WARNING_WAVE, m_strWarnSound);
	DDX_Control(pDX, IDC_CMB_POPWND_COUNT, m_cboPopWinCnt);
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

	m_cboPicWin.ResetContent();
	int i=0;
	CString str;
	while(i<=25)
	{
		str.Format("%d",i++);
		m_cboPicWin.AddString(str);
	}

	m_cboPlayInterval.ResetContent();
	i=0;
	int nDuring = 10;
	while(i<=10)
	{
		str.Format("%d",nDuring*i);
		m_cboPlayInterval.AddString(str);
		i++;
	}

	m_cboPopWinCnt.ResetContent();
	i=0;
	while(i<=8)
	{
		str.Format("%d",i);
		m_cboPopWinCnt.AddString(str);
		i++;
	}


	//Init Config Data
	util::InitApp();
	char *pConfigurationFile = util::GetAppPath();
	strcat(pConfigurationFile, CONFIG_FILE);
	char lpDefault[1024];
	memset(lpDefault,0,1024);
	DWORD dwRet = GetPrivateProfileString("SYS_CFG", "PicWinIndex","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_cboPicWin.SelectString(-1,lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "PlayPicInterval","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_cboPlayInterval.SelectString(-1,lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "MaxPopWarnWin","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_cboPopWinCnt.SelectString(-1,lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "PopWarnSound","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_strWarnSound = lpDefault;

	dwRet = GetPrivateProfileString("SYS_CFG", "PopWarnEnable","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0 && lpDefault[0]=='1')
		m_chkWarnEnable.SetCheck(1);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWarningConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//Init Config Data
	util::InitApp();
	char *pConfigurationFile = util::GetAppPath();
	strcat(pConfigurationFile, CONFIG_FILE);

	//Get Picture monitor setting window
	CString strVal;
	int n=0;
	int nIndex = m_cboPicWin.GetCurSel();
	if (nIndex != LB_ERR)
	{
		n = m_cboPicWin.GetLBTextLen(nIndex);
		m_cboPicWin.GetLBText(nIndex,strVal.GetBuffer(n));

		//Save to Config File
		WritePrivateProfileString("SYS_CFG", "PicWinIndex",	strVal,	pConfigurationFile);
		m_gConfigMgr.SetPicWinIndex(atoi(strVal));
	}
	//Get Picture Play Interval
	nIndex = m_cboPlayInterval.GetCurSel();
	if (nIndex != LB_ERR)
	{
		n = m_cboPlayInterval.GetLBTextLen(nIndex);
		m_cboPlayInterval.GetLBText(nIndex,strVal.GetBuffer(n));

		//Save to Config File
		WritePrivateProfileString("SYS_CFG", "PlayPicInterval",	strVal,	pConfigurationFile);
		m_gConfigMgr.SetPlayPicInterval(atoi(strVal));
	}

	//Get Max Pop Showing Windows
	nIndex = m_cboPopWinCnt.GetCurSel();
	if (nIndex != LB_ERR)
	{
		n = m_cboPopWinCnt.GetLBTextLen(nIndex);
		m_cboPopWinCnt.GetLBText(nIndex,strVal.GetBuffer(n));

		//Save to Config File
		WritePrivateProfileString("SYS_CFG", "MaxPopWarnWin",	strVal,	pConfigurationFile);
		m_gConfigMgr.SetMaxPopWarnWin(atoi(strVal));
	}

	GetWindowText(strVal);
	if ( !strVal.IsEmpty() )
	{
		WritePrivateProfileString("SYS_CFG", "PopWarnSound", strVal,	pConfigurationFile);
		m_gConfigMgr.SetPopWarnSound(strVal);
	}

	if (m_chkWarnEnable.GetCheck())
	{
		strVal = "1";
		WritePrivateProfileString("SYS_CFG", "PopWarnEnable",	strVal,	pConfigurationFile);
		m_gConfigMgr.SetPopWarnEnable(true);
	}
	else
	{
		strVal = "0";
		WritePrivateProfileString("SYS_CFG", "PopWarnEnable",	strVal,	pConfigurationFile);
		m_gConfigMgr.SetPopWarnEnable(false);

	}


	OnOK();
}

void CWarningConfig::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
