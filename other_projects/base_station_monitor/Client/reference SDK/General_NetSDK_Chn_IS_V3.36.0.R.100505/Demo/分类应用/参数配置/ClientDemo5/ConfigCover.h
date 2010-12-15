#if !defined(AFX_CONFIGCOVER_H__426AD004_1BE9_4A91_83A7_E50A813F4900__INCLUDED_)
#define AFX_CONFIGCOVER_H__426AD004_1BE9_4A91_83A7_E50A813F4900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigCover.h : header file
//
#include "dhnetsdk.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigCover dialog

class CConfigCover : public CDialog
{
// Construction
public:
	void CleanAll();
	void InitDlgInfo(DHDEV_VIDEOCOVER_CFG *pCoverInfo, BYTE nLength, BOOL bNewProtocol);
	CConfigCover(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigCover)
	enum { IDD = IDD_CONFIG_COVER };
	CComboBox	m_ctlCoverType;
	CListCtrl	m_ctlCoverLst;
	CComboBox	m_ctlChannel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigCover)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigCover)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonReadall();
	afx_msg void OnSelchangeComboChanno();
	afx_msg void OnItemchangedListCovers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InsertListView(CString strID, CString strLeft, CString strTop, CString strRight, CString strButton, CString strColor, CString strType);
	int m_nCurChannel;
	int m_nChannelCount;
	DHDEV_VIDEOCOVER_CFG m_channelCoverInfo[32];
	void SaveChannelInfo(int nNum);
	void InitListView();
	BOOL m_bNewProtocol;//32б╥ртио
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGCOVER_H__426AD004_1BE9_4A91_83A7_E50A813F4900__INCLUDED_)
