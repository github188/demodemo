// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// documentTestDoc.cpp : CdocumentTestDoc ���ʵ��
//

#include "stdafx.h"
#include "documentTest.h"

#include "documentTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdocumentTestDoc

IMPLEMENT_DYNCREATE(CdocumentTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CdocumentTestDoc, CDocument)
END_MESSAGE_MAP()


// CdocumentTestDoc ����/����

CdocumentTestDoc::CdocumentTestDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CdocumentTestDoc::~CdocumentTestDoc()
{
}

BOOL CdocumentTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CdocumentTestDoc ���л�

void CdocumentTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CdocumentTestDoc ���

#ifdef _DEBUG
void CdocumentTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdocumentTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CdocumentTestDoc ����
