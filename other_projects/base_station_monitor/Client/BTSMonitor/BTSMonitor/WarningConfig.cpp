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
	DDX_Control(pDX, IDC_CHK_NO_WARNING, m_chkWarnEnable);
	DDX_Text(pDX, IDC_WARNING_WAVE, m_strWarnSound);
	DDX_Control(pDX, IDC_CMB_POPWND_COUNT, m_cboPopWinCnt);
}


BEGIN_MESSAGE_MAP(CWarningConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CWarningConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWarningConfig::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_WAVE_BROWSE, &CWarningConfig::OnBnClickedWaveBrowse)
END_MESSAGE_MAP()


// CWarningConfig message handlers

BOOL CWarningConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString str;
	m_cboPopWinCnt.ResetContent();
	int i=0;
	while(i<=8)
	{
		str.Format("%d",i);
		m_cboPopWinCnt.AddString(str);
		i++;
	}


	//Init Config Data
	util::InitApp();
	//char *pConfigurationFile = util::GetAppPath();
	//strcat_s(pConfigurationFile,strlen(CONFIG_FILE), CONFIG_FILE);
	char szCfgPath[1024];
	ZeroMemory(szCfgPath,1024);
	char *pConfigurationFile = szCfgPath;
	strcat_s(pConfigurationFile,1024,util::GetAppPath());
	strcat_s(pConfigurationFile,1024 - strlen(pConfigurationFile), CONFIG_FILE);

	char lpDefault[1024];
	memset(lpDefault,0,1024);

	DWORD dwRet = GetPrivateProfileString("SYS_CFG", "MaxPopWarnWin","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_cboPopWinCnt.SelectString(-1,lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "PopWarnSound","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_strWarnSound = lpDefault;

	dwRet = GetPrivateProfileString("SYS_CFG", "PopWarnEnable","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0 && lpDefault[0]=='1')
		m_chkWarnEnable.SetCheck(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWarningConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//Init Config Data
	util::InitApp();
	//char *pConfigurationFile = util::GetAppPath();
	//strcat_s(pConfigurationFile,strlen(CONFIG_FILE), CONFIG_FILE);
	char szCfgPath[1024];
	ZeroMemory(szCfgPath,1024);
	char *pConfigurationFile = szCfgPath;
	strcat_s(pConfigurationFile,1024,util::GetAppPath());
	strcat_s(pConfigurationFile,1024 - strlen(pConfigurationFile), CONFIG_FILE);

	//Get Picture monitor setting window
	CString strVal;
	int n=0;

	//Get Max Pop Showing Windows
	int nIndex = m_cboPopWinCnt.GetCurSel();
	if (nIndex != LB_ERR)
	{
		n = m_cboPopWinCnt.GetLBTextLen(nIndex);
		m_cboPopWinCnt.GetLBText(nIndex,strVal.GetBuffer(n));

		//Save to Config File
		WritePrivateProfileString("SYS_CFG", "MaxPopWarnWin",	strVal,	pConfigurationFile);
		m_gConfigMgr.SetMaxPopWarnWin(atoi(strVal));
	}

	
	strVal = m_strWarnSound;
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

void CWarningConfig::OnBnClickedWaveBrowse()
{
	// TODO: Add your control notification handler code here
	char *strFilter = "WAV File(*.wav)|*.wav||";
	CFileDialog FileDlg(TRUE,"",NULL,0,strFilter,this);

	util::InitApp();
	//char *pWaveDir = util::GetAppPath();
	//strcat_s(pWaveDir, strlen("Wave"),"Wave");
	char szCfgPath[1024];
	ZeroMemory(szCfgPath,1024);
	char *pWaveDir = szCfgPath;
	strcat_s(pWaveDir,1024,util::GetAppPath());
	strcat_s(pWaveDir,1024 - strlen(pWaveDir), "Wave");

	FileDlg.m_ofn.lpstrInitialDir = pWaveDir;

	if(FileDlg.DoModal()==IDOK)
	{
		CString strFileName = FileDlg.GetFileName();
		CString strPathName = FileDlg.GetPathName();
		BOOL    bOnlyRead   = FileDlg.GetReadOnlyPref();
		m_strWarnSound = strPathName;
		UpdateData(FALSE);
	}

}
