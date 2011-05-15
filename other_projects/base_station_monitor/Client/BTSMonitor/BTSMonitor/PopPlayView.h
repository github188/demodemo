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
	void SetRealImagePara(MonitorImage* pObj, CString sUUID,CString sChannelID, CString sRoute);
	void SetImageType(int  nImageType);
	int  GetImageType() {return m_nImageType;};
	void SetImageFile(CString sPicture);
	void RefreshPlayView(int status=0);
	void SetPopViewIdx(int nIdx);
	void SetImageStatus (CString sInfo);

private:
	MonitorImage *m_pMonitorImage;
	BOOL	m_bHasImage;
	UINT	m_nIDEvent, m_nViewID;

	CString m_sRealImageDir,m_sPicture, m_sImageStatus,m_strShowMsg;
	CString m_sUUID, m_sChannelID, m_sRoute;
	int		m_nStatus, m_nImageType;

};


