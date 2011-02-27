#pragma once
#include "afxwin.h"


// CWarnPopVideo dialog
class CWarnPopVideo : public CDialog
{
	DECLARE_DYNAMIC(CWarnPopVideo)

public:
	CWarnPopVideo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarnPopVideo();

// Dialog Data
	enum { IDD = IDD_POP_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CView* m_pPopView;
	void SetPopVideoIndex(int nIndex);
	int GetPopVideoIndex(void);
private:
	unsigned int m_nPopIndex;
public:
	afx_msg void OnBnClickedOk();
	void FullScreenPopVideo(void);
private:
	bool m_bFullScreen;
	WINDOWPLACEMENT m_struOldPlacement;
public:
	void SetVideoPara(CString sUUID, CString sChannel, CString sStartTime, CString sEndTime);
private:
	CString m_sUUID;
	CString m_sChannel;
	CString m_sStartTime;
	CString m_sEndTime;
public:
	afx_msg void OnClose();
	BOOL IsShowing(void);
private:
	BOOL m_bShowing;
	void PlayVideo(void);
};
