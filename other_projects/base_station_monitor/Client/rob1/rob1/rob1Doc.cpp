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

// rob1Doc.cpp : implementation of the Crob1Doc class
//

#include "stdafx.h"
#include "rob1.h"

#include "rob1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Crob1Doc

IMPLEMENT_DYNCREATE(Crob1Doc, CDocument)

BEGIN_MESSAGE_MAP(Crob1Doc, CDocument)
END_MESSAGE_MAP()


// Crob1Doc construction/destruction

Crob1Doc::Crob1Doc()
{
	// TODO: add one-time construction code here

}

Crob1Doc::~Crob1Doc()
{
}

BOOL Crob1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Crob1Doc serialization

void Crob1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Crob1Doc diagnostics

#ifdef _DEBUG
void Crob1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Crob1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Crob1Doc commands
