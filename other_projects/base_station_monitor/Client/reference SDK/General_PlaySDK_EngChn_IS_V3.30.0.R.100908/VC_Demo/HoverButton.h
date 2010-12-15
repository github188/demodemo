//------------------------------------------------------------------------------
// File: HoverButton.h
//
// Desc: bitmap button class.
//
// Copyright (c)  All rights reserved.
//------------------------------------------------------------------------------
#ifndef __HOVER_BUTTON_H_
#define __HOVER_BUTTON_H_


/////////////////////////////////////////////////////////////////////////////
// CHoverButton window

class CHoverButton : public CBitmapButton
{
public:
	//指示鼠标是否在按钮上面
    BOOL m_bHover;
    //按钮是否跟踪到鼠标
    BOOL m_bTracking;
    //保存图片的变量
    CBitmap mybitmap;
    //按钮尺寸
    CSize m_ButtonSize;

	BOOL m_bButtonEnable;

public:
	CHoverButton();
	BOOL LoadBitmap(UINT bitmapid);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL EnableWindow(BOOL bEnable);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CHoverButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHoverButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#endif //__HOVER_BUTTON_H_