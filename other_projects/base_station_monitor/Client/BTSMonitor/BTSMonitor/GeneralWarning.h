#pragma once


// CGeneralWarning dialog

class CGeneralWarning : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralWarning)

public:
	CGeneralWarning();
	virtual ~CGeneralWarning();

// Dialog Data
	enum { IDD = IDD_GENERAL_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMFCListCtrl  m_lstGeneralWarning;
};
