// DlgImage.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "DlgImage.h"
#include "MakeHtml.h"

// CDlgImage dialog

IMPLEMENT_DYNCREATE(CDlgImage, CDHtmlDialog)

CDlgImage::CDlgImage(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDlgImage::IDD, CDlgImage::IDH, pParent)
	, m_sHtmlFile(_T(""))
{
//    m_isInit = false;
    // Create the scroll helper and attach it to this dialog.
//    m_scrollHelper = new CScrollHelper;
//    m_scrollHelper->AttachWnd(this);

    // We also set the display size equal to the dialog size.
    // This is the size of the dialog in pixels as laid out
    // in the resource editor.
//    m_scrollHelper->SetDisplaySize(633, 312);

    // Create the dialog.
    //Create(IDD_DLG_IMAGE, pParent);

}

CDlgImage::~CDlgImage()
{
//	delete m_scrollHelper;
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDlgImage::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_DIALOG|DOCHOSTUIFLAG_FLAT_SCROLLBAR); 

//	m_isInit = true;

 //   DisplayScrollInfo();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDlgImage, CDHtmlDialog)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	ON_WM_MOUSEHWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgImage)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDlgImage message handlers

HRESULT CDlgImage::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDlgImage::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CDlgImage::LoadImages( )
{

	if (m_sHtmlFile.IsEmpty())
		Navigate(_T("file:///")+m_sHtmlFile);
	else
		Navigate(_T("file:///")+m_sHtmlFile);

}

void CDlgImage::Initialize(CString sPicDir,int nCol, int nWidth, int nHeight)
{
	m_sHtmlFile.Empty();

	CMakeHtml makeHtml(sPicDir,  nCol,  nWidth, nHeight);
	makeHtml.AddTable(0);

	char szFind[MAX_PATH];
	CString sFileImage;
	WIN32_FIND_DATA FindFileData;

	strcpy_s(szFind,MAX_PATH,sPicDir);  //strcpy_s(szFind,strlen(sPicDir),sPicDir); will be crash.why..  
	strcat_s(szFind,MAX_PATH-strlen(szFind),"\\*.*" );

	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    return;

	int nPic = 0;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//if(FindFileData.cFileName[0]!='.')
			//{
			//	strcpy(szFile,sPicDir);
			//	strcat(szFile,"\\");
			//	strcat(szFile,FindFileData.cFileName);
			//	find(szFile);
			//}
		}
		else
		{
			sFileImage.Format("%s%s%s",sPicDir,"\\",FindFileData.cFileName);

			int nLen = sFileImage.GetLength();
			CString strType = sFileImage.Mid(sFileImage.Find('.')+1);
			if (strType.Find("jpg") != -1)
			{

				if ( nPic%nCol==0 )
				{
					if (nPic>0)
						makeHtml.CompleteRow();

					makeHtml.AddRow();
					makeHtml.AddColPic(sFileImage);
				}
				else
					makeHtml.AddColPic(sFileImage);

				nPic++;
			}

			 
		}

		if(!FindNextFile(hFind,&FindFileData))    
			break;
	}

	makeHtml.CompleteRow();
	makeHtml.CompleteTable();
	makeHtml.CompleteHtml();
	
	FindClose(hFind);

//	m_scrollHelper->SetDisplaySize(nWidth*nCol, nHeight*(nPic/2+1));

	m_sHtmlFile = makeHtml.GetHtmlFile();

	LoadImages( );
}

void CDlgImage::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);
    
//	m_scrollHelper->OnSize(nType, cx, cy);

//    DisplayScrollInfo();

	// TODO: Add your message handler code here
}

int CDlgImage::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
//	SetFocus();
//    DisplayScrollInfo();

	return CDHtmlDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDHtmlDialog::OnPaint() for painting messages
    DisplayScrollPos();
}

void CDlgImage::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default
//    BOOL wasScrolled = m_scrollHelper->OnMouseWheel(nFlags, zDelta, pt);

//    DisplayScrollInfo();

    //return wasScrolled;

	return CDHtmlDialog::OnMouseHWheel(nFlags, zDelta, pt);
}

void CDlgImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
//	m_scrollHelper->OnHScroll(nSBCode, nPos, pScrollBar);

//    DisplayScrollInfo();
	
	CDHtmlDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgImage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
//    m_scrollHelper->OnVScroll(nSBCode, nPos, pScrollBar);

//    DisplayScrollInfo();

	CDHtmlDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgImage::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
 //   m_scrollHelper->DetachWnd();

    // Delete the C++ instance so the parent does not have
    // to worry about it.
    //delete this;

	CDHtmlDialog::PostNcDestroy();
}
void CDlgImage::DisplayScrollInfo()
{
	/*
    if ( !::IsWindow(m_hWnd) )
        return;
    if ( !m_isInit )
        return;

    CString text;

    // Show the scroll position.
    DisplayScrollPos();
	*/

 }

void CDlgImage::DisplayScrollPos()
{
    CString text;

    // Change the caption text of the MDI child frame window to show
    // the current scroll position.
//    text.Format("TestScroll (ScrollPos: %d, %d)", m_scrollHelper->GetScrollPos().cx, m_scrollHelper->GetScrollPos().cy);
//    CWnd* pWnd = GetParentFrame();
//    if ( pWnd != NULL )
//   {
//        pWnd->SetWindowText(text);
//    }
}
BOOL CDlgImage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
//	BOOL wasScrolled = m_scrollHelper->OnMouseWheel(nFlags, zDelta, pt);

//	DisplayScrollInfo();

	//return wasScrolled;
	
	return CDHtmlDialog::OnMouseWheel(nFlags, zDelta, pt);
}
