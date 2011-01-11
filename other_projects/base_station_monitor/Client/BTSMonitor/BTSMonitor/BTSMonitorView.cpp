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

// BTSMonitorView.cpp : CBTSMonitorView ���ʵ��
//

#include "stdafx.h"
#include "BTSMonitor.h"

#include "BTSMonitorDoc.h"
#include "BTSMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBTSMonitorView

IMPLEMENT_DYNCREATE(CBTSMonitorView, CView)

BEGIN_MESSAGE_MAP(CBTSMonitorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBTSMonitorView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBTSMonitorView ����/����

CBTSMonitorView::CBTSMonitorView()
{
	// TODO: �ڴ˴���ӹ������

}

CBTSMonitorView::~CBTSMonitorView()
{
}

BOOL CBTSMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CBTSMonitorView ����

void CBTSMonitorView::OnDraw(CDC* /*pDC*/)
{
	CBTSMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CBTSMonitorView ��ӡ


void CBTSMonitorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CBTSMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CBTSMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CBTSMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CBTSMonitorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBTSMonitorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CBTSMonitorView ���

#ifdef _DEBUG
void CBTSMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CBTSMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBTSMonitorDoc* CBTSMonitorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBTSMonitorDoc)));
	return (CBTSMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CBTSMonitorView ��Ϣ�������
