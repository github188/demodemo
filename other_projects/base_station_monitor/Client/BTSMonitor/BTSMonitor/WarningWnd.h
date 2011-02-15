#pragma once
#include "afxtabctrl.h"
#include "afxwin.h"

// CWarningWnd
class CRuntimeWarning;
class CCriticalWarning;
class CGeneralWarning;

class CWarningWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CWarningWnd)

public:
	CWarningWnd();
	virtual ~CWarningWnd();

//Atribute
public:
	virtual BOOL CanBeClosed() const
	{
		return FALSE;
	}

	CRuntimeWarning  *m_pRuntimePg;
	CCriticalWarning *m_pCriticalPg;
	CGeneralWarning  *m_pGereralPg;;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	CMFCTabCtrl m_wndTabs;
	CFont m_Font;
public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
//	afx_msg void OnWarningAck();
//	afx_msg void OnUpdateWarningAck(CCmdUI *pCmdUI);
//	afx_msg void OnWarningScroolingOff();
//	afx_msg void OnWarningScroolingOn();
//	afx_msg void OnDummyCompile();
};


