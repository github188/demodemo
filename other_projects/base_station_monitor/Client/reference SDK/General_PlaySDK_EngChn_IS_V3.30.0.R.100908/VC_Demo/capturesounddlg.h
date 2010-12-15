#if !defined(AFX_APTURESOUNDDLG_H__26D72189_8307_47DA_8EAD_69C6AC99CD03__INCLUDED_)
#define AFX_APTURESOUNDDLG_H__26D72189_8307_47DA_8EAD_69C6AC99CD03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// aptureSoundDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CaptureSoundDlg dialog

class CaptureSoundDlg : public CDialog
{
// Construction
public:
	CaptureSoundDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_csSoundFile;

	long	m_lBitPerSample;
	long	m_lSampleSPerSec;

	BOOL	m_bSoundCapture;

	long GetSampleSize(long BitsPerSec, long SamplesPerSec);
	void SoundRecord(BYTE* pPCMData, long lDataLen);

	void FillSamplPerSecCombox();

	FILE* m_fpSoundFile;

// Dialog Data
	//{{AFX_DATA(CaptureSoundDlg)
	enum { IDD = IDD_DIALOG_AUDIOIN };
	CComboBox	m_ComboxSampPerSec;
	CComboBox	m_ComboxBits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaptureSoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CaptureSoundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSoundfile();
	afx_msg void OnButtonRecord();
	afx_msg void OnSelchangeComboBit();
	afx_msg void OnSelchangeComboSamples();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APTURESOUNDDLG_H__26D72189_8307_47DA_8EAD_69C6AC99CD03__INCLUDED_)
