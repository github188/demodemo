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

// robbinui3View.cpp : Crobbinui3View ���ʵ��
//

#include "stdafx.h"
#include "robbinui3.h"

#include "robbinui3Doc.h"
#include "robbinui3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Crobbinui3View

IMPLEMENT_DYNCREATE(Crobbinui3View, CView)

BEGIN_MESSAGE_MAP(Crobbinui3View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Crobbinui3View::OnFilePrintPreview)
END_MESSAGE_MAP()

// Crobbinui3View ����/����

Crobbinui3View::Crobbinui3View()
{
	// TODO: �ڴ˴���ӹ������

}

Crobbinui3View::~Crobbinui3View()
{
}

BOOL Crobbinui3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Crobbinui3View ����

void Crobbinui3View::OnDraw(CDC* /*pDC*/)
{
	Crobbinui3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Crobbinui3View ��ӡ


void Crobbinui3View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL Crobbinui3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Crobbinui3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Crobbinui3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void Crobbinui3View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Crobbinui3View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// Crobbinui3View ���

#ifdef _DEBUG
void Crobbinui3View::AssertValid() const
{
	CView::AssertValid();
}

void Crobbinui3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Crobbinui3Doc* Crobbinui3View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Crobbinui3Doc)));
	return (Crobbinui3Doc*)m_pDocument;
}
#endif //_DEBUG


// Crobbinui3View ��Ϣ�������
