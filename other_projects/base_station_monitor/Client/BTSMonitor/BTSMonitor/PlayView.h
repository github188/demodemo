#pragma once


// CPlayView view
class MonitorImage;
class CPlayView : public CView
{
	DECLARE_DYNCREATE(CPlayView)

protected:
	CPlayView();           // protected constructor used by dynamic creation
	virtual ~CPlayView();

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
//	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	int m_nViewID;
public:
	void SetVedioIndex(int nViewID);
	int GetVedioIndex(void);
	void DrawSelectedWindow(void);
	void SetActiveViewID(int  nActiveViewID);
private:
	int m_nActiveViewID;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
private:
	bool m_bIsVisible;
	MonitorImage* m_pMonitorImage;
	BOOL m_bIsFullScreen;
	CWnd *m_pSaveParent;
	CString m_sPicture;
public:
	void HidePlayView(void);
	void ShowPlayView(void);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPlayviewClose();
	afx_msg void OnPlayviewFullscreen();
private:
	void FullScreen(void);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL  ShowPicture(CDC*  pDC, CString strPicName, int nWidth , int nHeight);
private:
	int m_nImageType;
	bool m_bHasImage;
public:
	void SetImageFile(CString sPicture);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetTimerIDEvent(int nIDEvent);
private:
	int m_nIDEvent;
public:
	void StartImgMonitor(void);
	void StopImgMonitor(void);
	void SetRealImagePara(MonitorImage* pObj, CString sUUID,CString sChannelID, CString sRoute);
	void SetImageType(int  nImageType);
	int  GetImageType() {return m_nImageType;};

private:
	CString m_sRealImageDir;
	CString m_sUUID, m_sChannelID, m_sRoute;
};


