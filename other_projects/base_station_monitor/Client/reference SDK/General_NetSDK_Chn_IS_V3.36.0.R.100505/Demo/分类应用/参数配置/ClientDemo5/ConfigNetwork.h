#if !defined(AFX_CONFIGNETWORK_H__B84400D8_26E7_47CA_B76C_EF6F4B0BC07D__INCLUDED_)
#define AFX_CONFIGNETWORK_H__B84400D8_26E7_47CA_B76C_EF6F4B0BC07D__INCLUDED_

#include "dhnetsdk.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigNetwork.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigNetwork dialog

class CConfigNetwork : public CDialog
{
// Construction
public:
	void CleanAll();
	void InitDlgInfo(DHDEV_NET_CFG *pNetIOInfo,BYTE nNetIOCount,DHDEV_MAIL_CFG *pMailInfo);
	CConfigNetwork(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigNetwork)
	enum { IDD = IDD_CONFIG_NETWORK };
	CIPAddressCtrl	m_ctlMailIP;
	CIPAddressCtrl	m_ctlNetIPMask;
	CIPAddressCtrl	m_ctlNetIP;
	CIPAddressCtrl	m_ctlNetGateWay;
	CIPAddressCtrl	m_ctlHostIP;
	CButton	m_ctlChkRemote;
	CComboBox	m_ctlRemoteHost;
	CComboBox	m_ctlEthernet;
	CComboBox	m_ctlNetInterface;
	BOOL	m_bTransmit;
	BOOL	m_bValid;
	BOOL	m_bDHCP;
	BOOL	m_bDefault;
	BOOL	m_bMailEn;
	BOOL	m_bMailSSLEn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigNetwork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigNetwork)
	afx_msg void OnButtonApply();
	afx_msg void OnButtonReadall();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboEthernet();
	afx_msg void OnSelchangeComboRemotehost();
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	DH_REMOTE_HOST GetRemoteHostType(int nType);
	void SaveRemoteHost(int nNum);
	int m_nCurRemoteHost;
	int m_nCurEthernet;
	void SaveNetWork(int nNum);
	DHDEV_NET_CFG m_NetIOInfo;
	DHDEV_MAIL_CFG m_MailInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGNETWORK_H__B84400D8_26E7_47CA_B76C_EF6F4B0BC07D__INCLUDED_)
