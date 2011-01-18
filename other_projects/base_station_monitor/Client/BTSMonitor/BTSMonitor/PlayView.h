#pragma once


// CPlayView view

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
public:
	void HidePlayView(void);
	void ShowPlayView(void);
};


