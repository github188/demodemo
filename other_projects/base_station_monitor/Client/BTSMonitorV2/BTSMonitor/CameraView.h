#pragma once

#include "ViewTree.h"

// CCameraView
class CCameraView : public CDockablePane
{
	DECLARE_DYNAMIC(CCameraView)

public:
	CCameraView();
	virtual ~CCameraView();
	virtual BOOL CanBeClosed() const
	{
		return FALSE;
	}

protected:
	CViewTree  m_ctrlCameraTree;
	CImageList m_lstImages;
	//CMFCButton* m_pBtnFind;
	CButton*	m_pBtnFind;
	CEdit*	    m_pEdtFind;
	CStatic*    m_pStcFind;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFindBTS();
	afx_msg void OnUpdateFindBTS(CCmdUI* pCmdUI);
	void InitCameraView(void);
	void AdjustLayout(void);
	void fillinData(void);
private:
	HTREEITEM m_hItemCurFind;
	HTREEITEM m_hRootItem;
public:
	HTREEITEM FindTarget(HTREEITEM  hItem, CString sName, bool bFindNext=FALSE);
	BOOL FindNewTarget(CString sFindStr);
	BOOL FindNextTarget(void);
private:
	CString m_sFindStr;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	HBRUSH m_hBrush;
};


