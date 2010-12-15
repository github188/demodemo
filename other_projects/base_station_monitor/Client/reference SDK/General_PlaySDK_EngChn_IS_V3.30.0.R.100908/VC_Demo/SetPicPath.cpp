// SetPicPath.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "SetPicPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetPicPath dialog


CSetPicPath::CSetPicPath(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPicPath::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetPicPath)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetPicPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetPicPath)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetPicPath, CDialog)
	//{{AFX_MSG_MAP(CSetPicPath)
	ON_BN_CLICKED(IDC_BUTTON_PATHSET, OnPathset)
	ON_EN_CHANGE(IDC_EDIT_PICPATH, OnChangeEditPicpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetPicPath message handlers

void CSetPicPath::OnPathset() 
{
	LPMALLOC   pMalloc;   
	if (::SHGetMalloc(&pMalloc) == NOERROR)   
	{   
		BROWSEINFO		bi;
		LPITEMIDLIST	pidl; 
		char pszBuffer[MAX_PATH];   
		memset(pszBuffer, 0, MAX_PATH);
		
		bi.hwndOwner		= GetSafeHwnd();   
		bi.pidlRoot			= NULL;   
		bi.pszDisplayName   = pszBuffer;   
		bi.lpszTitle		= _T("Select a Starting Directory");   
		bi.ulFlags			= BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
		bi.lpfn				= NULL;   
		bi.lParam			= 0;   
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)   
		{   
			if (::SHGetPathFromIDList(pidl, pszBuffer))   
			{    
				m_csPicPath.Format("%s", pszBuffer);
				GetDlgItem(IDC_EDIT_PICPATH)->SetWindowText(pszBuffer);  
			}   
			pMalloc->Free(pidl);   
		}
		pMalloc->Release();
	}
	
}

BOOL CSetPicPath::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_PICPATH)->SetWindowText(m_csPicPath); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetPicPath::OnChangeEditPicpath() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	GetDlgItem(IDC_EDIT_PICPATH)->GetWindowText(m_csPicPath); 
	
}
