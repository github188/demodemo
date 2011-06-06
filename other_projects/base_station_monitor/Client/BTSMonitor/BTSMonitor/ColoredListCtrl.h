#pragma once


// CColoredListCtrl

class CColoredListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CColoredListCtrl)

public:
	CColoredListCtrl();
	virtual ~CColoredListCtrl();
//attribute
public:
	COLORREF m_colRow1;
	COLORREF m_colRow2;

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


