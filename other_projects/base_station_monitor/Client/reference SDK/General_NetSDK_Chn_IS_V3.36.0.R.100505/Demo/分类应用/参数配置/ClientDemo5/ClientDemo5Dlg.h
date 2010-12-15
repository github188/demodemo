// ClientDemo5Dlg.h : header file
//

#if !defined(AFX_CLIENTDEMO5DLG_H__71084F64_0DFF_43C2_8AD9_6F5AE7FE9253__INCLUDED_)
#define AFX_CLIENTDEMO5DLG_H__71084F64_0DFF_43C2_8AD9_6F5AE7FE9253__INCLUDED_

#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#include "ConfigServer.h"	// Added by ClassView
#include "ConfigComm.h"	    // Added by ClassView
#include "ConfigChannel.h"	// Added by ClassView
#include "ConfigAlarm.h"	// Added by ClassView
#include "ConfigRecord.h"	// Added by ClassView
#include "ConfigNetwork.h"	// Added by ClassView
#include "ConfigCover.h"	// Added by ClassView
#include "ConfigStrategy.h"
#include "Configfile.h"
#include "DlgIPC.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDemo5Dlg dialog

class CClientDemo5Dlg : public CDialog
{
// Construction
public:
	void SetCoverConfigInfo(DHDEV_VIDEOCOVER_CFG *pCoverInfo, int nLength);	
	void GetCoverConfigInfo();

	void SetAlarmConfigInfo(DHDEV_ALARM_SCHEDULE_EX *pAlarmInfo);	
	//Get alarm property configuration information(ALL)
	void GetAlarmConfigInfo();
	//Save network configuration information
	void SetNetWorkConfigInfo(DHDEV_NET_CFG *pNetIOInfo, DHDEV_MAIL_CFG *pMailInfo);
	//Get network configuration information
	void GetNetWorkConfigInfo();
	//Set video configuration information
	void SetRecordConfigInfo(DHDEV_RECORD_CFG *pRecordInfo,BYTE nLength);
	//Get video configuration information
	void GetRecordConfigInfo();
	//Save COM information
	void SetCommConfigInfo(DHDEV_COMM_CFG_EX *pCommInfo, int nCurChannel = 0);
	//Get COM information
	void GetCommConfigInfo();
	//Save video channel property configuration information
	void SetChannelConfigInfo(DHDEV_CHANNEL_CFG *pChannelInfo, int nLength);
	//Get video channel property
	void GetChannelConfigInfo();
	// more than 32 channels configuration
	void SetOneCoverConfigInfo(DHDEV_VIDEOCOVER_CFG *pInfo, int nCurChannel = 0);
	void GetOneCoverConfigInfo(DHDEV_VIDEOCOVER_CFG *pInfo, int nCurChannel = 0);
	void SetAlarmInConfigInfo(DH_ALARMIN_CFG_EX *pInfo, int nCurChannel = 0);
	void SetMotionAlarmConfigInfo(DH_MOTION_DETECT_CFG_EX *pInfo, int nCurChannel = 0);
	void SetBlindAlarmConfigInfo(DH_BLIND_CFG_EX *pInfo, int nCurChannel = 0);
	void SetLossAlarmConfigInfo(DH_VIDEO_LOST_CFG_EX *pInfo, int nCurChannel = 0);
	void GetAlarmInConfigInfo(DH_ALARMIN_CFG_EX *pInfo, int nCurChannel = 0);
	void GetMotionAlarmConfigInfo(DH_MOTION_DETECT_CFG_EX *pInfo, int nCurChannel = 0);
	void GetBlindAlarmConfigInfo(DH_BLIND_CFG_EX *pInfo, int nCurChannel = 0);
	void GetLossAlarmConfigInfo(DH_VIDEO_LOST_CFG_EX *pInfo, int nCurChannel = 0);
	void SetOneRecordConfigInfo(DHDEV_RECORD_CFG *pInfo, int nCurChannel = 0);
	void GetOneRecordConfigInfo(DHDEV_RECORD_CFG *pInfo, int nCurChannel = 0);
	void SetOneChannelConfigInfo(DHDEV_CHANNEL_CFG *pInfo, int nCurChannel = 0);
	void GetOneChannelConfigInfo(DHDEV_CHANNEL_CFG *pInfo, int nCurChannel = 0);

	//Set device property configuration information
	void SetDeviceConfigInfo(LPDHDEV_SYSTEM_ATTR_CFG sysConfigInfo);
	//Get device property configuration information
	void GetDeviceConfigInfo();

	void SetStrategyConfigInfo(DHDEV_TRANSFER_STRATEGY_CFG* pTransInfo,DHDEV_DOWNLOAD_STRATEGY_CFG* pDownloadInfo);
	void GetStrategyConfigInfo();

	//Configuration file
	LONG ImportConfigFile(char* szPathName, fDownLoadPosCallBack cbUploadPos, DWORD dwUser);
	LONG ExportConfigFile(char* szFileName, DH_CONFIG_FILE_TYPE emConfigFileType, fDownLoadPosCallBack cbUploadPos, DWORD dwUser);

	//Interface for callback
	void DeviceDisConnect(LONG lLoginID, char *sDVRIP,LONG nDVRPort);
	//Callback function when device disconnected
	friend void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	
	CClientDemo5Dlg(CWnd* pParent = NULL);	// standard constructor

	void ResolutionSizetoInt(int nResolution, int *nWidth, int *nHeight);
	int ResolutionInttoSize(int nWidth, int nHeight);
	void ParseEventHandle(CFG_ALARM_MSG_HANDLE stuHandlle, DH_MSG_HANDLE_EX *pstruHandle);
	void BuildEventHandle(DH_MSG_HANDLE_EX stuHandlle, CFG_ALARM_MSG_HANDLE *pstruHandle);
	void Change_Assic_UTF8(char *pStrGBKData, int nlen, char *pbuf, int buflen);
	void Change_Utf8_Assic(unsigned char * pUTF8, char *destbuf);

// Dialog Data
	//{{AFX_DATA(CClientDemo5Dlg)
	enum { IDD = IDD_CLIENTDEMO5_DIALOG };
	CComboBox	m_ctlRevertType;
	CTabCtrl	m_ctlMainTab;
	CIPAddressCtrl	m_ctlDvrIP;
	UINT	m_nPort;
	CString	m_strUserName;
	CString	m_strPwd;
	BOOL m_b3ProDev;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDemo5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	//add by yu_peng
	//因为一次只能登陆一个设备，所以最多多一个页面
	BOOL AddPage(CString strTitle, CDialog* dlgPage, UINT uID);
	BOOL DeletePage();

	enum PAGETYPE{NONE, IPC, DVR};
	PAGETYPE FromDeviceType(NET_DEVICE_TYPE eDeviceType);
	//end add
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDemo5Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnLogout();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRevert();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nDevProtoVer;
	CConfigCover m_coverDlg;
	void InitCombo();
	void SetDlgState(CWnd *pWnd, BOOL bShow);
	void DoTab(int nTab);
	CConfigNetwork m_networkDlg;
	CConfigRecord m_recordDlg;
	CConfigAlarm m_alarmDlg;
	CConfigChannel m_channelDlg;
	CConfigComm m_commDlg;
	CConfigServer m_serverDlg;
	CConfigStrategy m_StrategyDlg;
	CConfigFile		m_configFile;
	//CDlgIPC			m_configIPC;
	CDialog*	m_dlgDeviceType;

	void InitTabControl();
	void ShowLoginErrorReason(int nError);
	CString GetDvrIP();
	long m_LoginID;
	void InitNetSDK();
	int m_nVideoStandard;

	//16路以上设备的配置
	CFG_ENCODE_INFO m_stuEncodeInfo;
	CFG_PTZ_INFO m_PTZInfo;
	CFG_RECORD_INFO m_RecordInfo;
	CFG_ALARMIN_INFO m_AlarmInfo;
	CFG_MOTION_INFO m_MotionInfo;
	CFG_SHELTER_INFO m_BlindInfo;
	CFG_VIDEOLOST_INFO m_VideoLostInfo;	
	CFG_VIDEO_COVER m_VideoCoveInfo;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDEMO5DLG_H__71084F64_0DFF_43C2_8AD9_6F5AE7FE9253__INCLUDED_)
