#if !defined(AFX_PLAYBACKBYTIME_H__35EAB30F_C6FB_4466_84AC_BF1C76B9623E__INCLUDED_)
#define AFX_PLAYBACKBYTIME_H__35EAB30F_C6FB_4466_84AC_BF1C76B9623E__INCLUDED_

#include "dhnetsdk.h"
#include "PlaybackByFile.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlaybackByTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlaybackByTime dialog

class CPlaybackByTime : public CDialog
{
// Construction
public:
	//Set channel dropdown menu 
	void InitComboBox(int nChannel,long lLoginID);
	//Log out and then clear
	void Clean();

	long m_hLoginID;
	DWORD m_dwCurValue;
	DWORD m_dwTotalSize;
	CPlaybackByTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlaybackByTime)
	enum { IDD = IDD_PLAYBACK_BYTIME };
	CComboBox	m_ctlPlayMode;
	CComboBox	m_ctlLoadChannel;
	CProgressCtrl	m_ctlLoadPro;
	CComboBox	m_ctlChannel;
	CTime	m_timeFrom;
	CTime	m_timeLoadFrom;
	CTime	m_timeLoadTo;
	CTime	m_timeTo;
	COleDateTime	m_dateFrom;
	COleDateTime	m_dateLoadFrom;
	COleDateTime	m_dateLoadTo;
	COleDateTime	m_dateTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaybackByTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlaybackByTime)
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonDownload();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT m_nTimer;
	NET_TIME ConvertTime(CTime date,CTime time);
	NET_TIME ConvertToDateTime(const COleDateTime& date, const CTime& time);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYBACKBYTIME_H__35EAB30F_C6FB_4466_84AC_BF1C76B9623E__INCLUDED_)
