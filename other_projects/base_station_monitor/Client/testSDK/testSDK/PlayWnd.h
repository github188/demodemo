#pragma once


// CPlayWnd
#define	VIDEO_BACK_COLOR	RGB(51,54,110)

class CPlayWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayWnd)

public:
	CPlayWnd();
	virtual ~CPlayWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


