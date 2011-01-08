// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// rob1View.cpp : implementation of the Crob1View class
//

#include "stdafx.h"
#include "rob1.h"

#include "rob1Doc.h"
#include "rob1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Crob1View

IMPLEMENT_DYNCREATE(Crob1View, CView)

BEGIN_MESSAGE_MAP(Crob1View, CView)
END_MESSAGE_MAP()

// Crob1View construction/destruction

Crob1View::Crob1View()
{
	// TODO: add construction code here

}

Crob1View::~Crob1View()
{
}

BOOL Crob1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Crob1View drawing

void Crob1View::OnDraw(CDC* /*pDC*/)
{
	Crob1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void Crob1View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Crob1View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// Crob1View diagnostics

#ifdef _DEBUG
void Crob1View::AssertValid() const
{
	CView::AssertValid();
}

void Crob1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Crob1Doc* Crob1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Crob1Doc)));
	return (Crob1Doc*)m_pDocument;
}
#endif //_DEBUG


// Crob1View message handlers
