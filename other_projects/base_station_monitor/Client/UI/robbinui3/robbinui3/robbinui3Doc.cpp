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

// robbinui3Doc.cpp : Crobbinui3Doc ���ʵ��
//

#include "stdafx.h"
#include "robbinui3.h"

#include "robbinui3Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Crobbinui3Doc

IMPLEMENT_DYNCREATE(Crobbinui3Doc, CDocument)

BEGIN_MESSAGE_MAP(Crobbinui3Doc, CDocument)
END_MESSAGE_MAP()


// Crobbinui3Doc ����/����

Crobbinui3Doc::Crobbinui3Doc()
{
	// TODO: �ڴ����һ���Թ������

}

Crobbinui3Doc::~Crobbinui3Doc()
{
}

BOOL Crobbinui3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// Crobbinui3Doc ���л�

void Crobbinui3Doc::Serialize(CArchive& ar)
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


// Crobbinui3Doc ���

#ifdef _DEBUG
void Crobbinui3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Crobbinui3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Crobbinui3Doc ����

void Crobbinui3Doc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument::SetTitle(_T("��Ƶ���ϵͳ"));
}
