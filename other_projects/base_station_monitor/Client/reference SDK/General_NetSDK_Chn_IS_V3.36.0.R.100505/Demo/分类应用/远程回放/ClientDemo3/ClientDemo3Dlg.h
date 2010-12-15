// ClientDemo3Dlg.h : header file
//

#if !defined(AFX_CLIENTDEMO3DLG_H__5F0C6987_3711_4A4D_8240_CEEC190FD725__INCLUDED_)
#define AFX_CLIENTDEMO3DLG_H__5F0C6987_3711_4A4D_8240_CEEC190FD725__INCLUDED_

#include "dhnetsdk.h"
#include "PlaybackByFile.h"	// Added by ClassView
#include "PlaybackByTime.h"	// Added by ClassView
#include "ExSliderCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDemo3Dlg dialog
//Playback mode 
enum DH_PlayBackType{
	EM_FILEBACK,
	EM_TIMEBACK,
};

class CClientDemo3Dlg : public CDialog
{
// Construction
public:
	//Download by time
	BOOL DownLoadByTime(int nChannelId, LPNET_TIME tmStart, LPNET_TIME tmEnd, BOOL bLoad);
	//Playback by time
	void PlayBackByTime(int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime);
	//Download by file
	BOOL DownLoadByFile(NET_RECORDFILE_INFO fileInfo,BOOL bLoad);
	//Playback by file
	void PlayBackByFile(NET_RECORDFILE_INFO fileInfo);
	//Interface to callback 
	void DeviceDisConnect(LONG lLoginID, char *sDVRIP,LONG nDVRPort);
	void ReceivePlayPos(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize);
	void TimeDownLoadCallBack(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo);
	//Callback function when device disconnected.
	friend void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	//Process status callback 
	friend void CALLBACK PlayCallBack(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);
	//Download callback in time mode
	friend void CALLBACK TimeDownLoadPos(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, DWORD dwUser);
	//Data callback
	friend int CALLBACK DataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);
	
	int ReceiveRealData(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize);
	void PlayBackByTimeServerMode(int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime);
	void PlayBackByFileServerMode(NET_RECORDFILE_INFO fileInfo);
	void ExSliderButtonUp();
	void ExSliderButtonDown();

	long m_pDownLoadFile;
	long m_pDownLoadByTime;

	CClientDemo3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDemo3Dlg)
	enum { IDD = IDD_CLIENTDEMO3_DIALOG };
	CExSliderCtrl	m_ctlPlayBack;
	CTabCtrl	m_Tab;
	CIPAddressCtrl	m_ctlDvrIP;
	CString	m_strUserName;
	CString	m_strPwd;
	UINT	m_nPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDemo3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDemo3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnLogout();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReleasedcaptureSliderPlayposition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonSlow();
	afx_msg void OnButtonFast();
	afx_msg void OnButtonFrameto();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonNormalspeed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ClosePlayBack();
	DH_PlayBackMode m_ePlayMode;
	CTime ConvertNetTimeToCTime(LPNET_TIME lpNetTime);
	DH_PlayBackType m_PlayBackType;
	void LastError();
	UINT m_nTimer;
	DWORD m_dwCurValue;
	DWORD m_dwTotalSize;
	long m_hPlayBack;
	int m_nChannelCount;
	void SetDlgState(CWnd *pWnd, BOOL bShow);
	void DoTab(int nTab);
	CPlaybackByTime *m_pTabTimeDlg;
	CPlaybackByFile *m_pTabFileDlg;
	void InitTabControl();
	void ShowLoginErrorReason(int nError);
	CString GetDvrIP();
	long m_LoginID;
	void InitNetSDK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDEMO3DLG_H__5F0C6987_3711_4A4D_8240_CEEC190FD725__INCLUDED_)
