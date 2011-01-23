#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRuntimeWarning dialog

class CRuntimeWarning : public CPropertyPage
{
	DECLARE_DYNAMIC(CRuntimeWarning)

public:
	CRuntimeWarning();
	virtual ~CRuntimeWarning();

// Dialog Data
	enum { IDD = IDD_RUNTIME_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMFCListCtrl  m_lstRuntimeWarning;

public:
	void AddListView(void);
private:
	CImageList m_imagelist;
};
