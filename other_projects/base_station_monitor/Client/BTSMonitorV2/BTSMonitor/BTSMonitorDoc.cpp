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

// BTSMonitorDoc.cpp : CBTSMonitorDoc ���ʵ��
//

#include "stdafx.h"
#include "BTSMonitor.h"

#include "BTSMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBTSMonitorDoc

IMPLEMENT_DYNCREATE(CBTSMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CBTSMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CBTSMonitorDoc ����/����

CBTSMonitorDoc::CBTSMonitorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CBTSMonitorDoc::~CBTSMonitorDoc()
{
}

BOOL CBTSMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	this->SetTitle(_T("�й��ƶ�ͨ�Ź�˾"));

	return TRUE;
}




// CBTSMonitorDoc ���л�

void CBTSMonitorDoc::Serialize(CArchive& ar)
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


// CBTSMonitorDoc ���

#ifdef _DEBUG
void CBTSMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBTSMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBTSMonitorDoc ����
