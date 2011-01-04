#pragma once


// CPlayWnd
//#define	VIDEO_BACK_COLOR	RGB(51,54,110)
#define	VIDEO_BACK_COLOR	RGB(0,0,0)

class CPlayWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayWnd)

	CRect playerRect;

public:
	CPlayWnd();
	virtual ~CPlayWnd();
	void resize(CRect &rectParent, int cx, int cy);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


