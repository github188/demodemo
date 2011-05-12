#pragma once


// CPopPlayView view
class MonitorImage;
class CPopPlayView : public CView
{
	DECLARE_DYNCREATE(CPopPlayView)

protected:
	CPopPlayView();           // protected constructor used by dynamic creation
	virtual ~CPopPlayView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	BOOL ShowPicture(CDC*  pDC, CString strPicName, int nWidth, int nHeight);
	void SetTimerIDEvent(int nIDEvent);
	void StartImgMonitor(void);
	void StopImgMonitor(void);
	void SetMonitorImageObj(MonitorImage* pObj);
	void SetImageType(int  nImageType);
	int  GetImageType() {return m_nImageType;};
	void SetImageFile(CString sPicture);

private:
	CString m_sPicture;
	int		m_nImageType;
	MonitorImage *m_pMonitorImage;
	BOOL	m_bHasImage;
	UINT	m_nIDEvent;
	CString m_sRealImageDir;
};


