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

// documentTestView.cpp : CdocumentTestView ���ʵ��
//

#include "stdafx.h"
#include "documentTest.h"

#include "documentTestDoc.h"
#include "documentTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdocumentTestView

IMPLEMENT_DYNCREATE(CdocumentTestView, CView)

BEGIN_MESSAGE_MAP(CdocumentTestView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CdocumentTestView ����/����

CdocumentTestView::CdocumentTestView()
{
	// TODO: �ڴ˴���ӹ������
	frameWidth=3;
}

CdocumentTestView::~CdocumentTestView()
{
}

BOOL CdocumentTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CdocumentTestView ����

void CdocumentTestView::OnDraw(CDC* /*pDC*/)
{
	CdocumentTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CdocumentTestView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CdocumentTestView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CdocumentTestView ���

#ifdef _DEBUG
void CdocumentTestView::AssertValid() const
{
	CView::AssertValid();
}

void CdocumentTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdocumentTestDoc* CdocumentTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdocumentTestDoc)));
	return (CdocumentTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CdocumentTestView ��Ϣ�������

int CdocumentTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	 //CRect rect(20,20,100,50);
	 //CButton *m_ctrlButton=new CButton();
	 //CComboBox *m_combobox=new CComboBox();
	 //m_ctrlButton->Create(L"Button1",WS_CHILD|WS_VISIBLE,rect,this,NULL);
	 ////������Ť�ؼ�
	 //CFont *pFont=CFont::FromHandle((HFONT)::GetStockObject(ANSI_VAR_FONT));
	 //CRect rect1(150,20,350,100);
	 //m_combobox->Create(WS_CHILD|WS_VISIBLE|CBS_SIMPLE|CBS_NOINTEGRALHEIGHT|WS_VSCROLL,rect1,this,NULL);

	//CRect rect1(10,10,350,100);
	//CRect rectTotal;
	//this->GetClientRect(&rectTotal);
	//CPlayWnd *pw=new CPlayWnd();
	//pw->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rectTotal, this, 2010);

	//CRect rectTotal;
	//this->GetClientRect(&rectTotal);
	//int widthTotal=rectTotal.Width();
	//int heightTotal=rectTotal.Height();
	//int widthEach=(widthTotal-4*frameWidth)/3;
	//int heightEach=(heightTotal-4*frameWidth)/3;

	//for(int i=0;i<3;i++)
	//{
	//	for(int j=0;j<3;j++)
	//	{
	//		CRect rectEach((i+1)*frameWidth+i*widthEach, (j+1)*frameWidth+j*heightEach, 
	//			(i+1)*frameWidth+(i+1)*widthEach, (j+1)*frameWidth+(j+1)*heightEach);

	//		CPlayWnd *pw=new CPlayWnd();
	//		pw->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rectEach, this, 2010);
	//		this->playwndList.AddTail(pw);
	//	}
	//}

	return 0;
}

void CdocumentTestView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CRect rectTotal;
	this->GetClientRect(&rectTotal);
	int widthTotal=rectTotal.Width();
	int heightTotal=rectTotal.Height();
	int widthEach=(widthTotal-4*frameWidth)/3;
	int heightEach=(heightTotal-4*frameWidth)/3;

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			CRect rectEach((i+1)*frameWidth+i*widthEach, (j+1)*frameWidth+j*heightEach, 
				(i+1)*frameWidth+(i+1)*widthEach, (j+1)*frameWidth+(j+1)*heightEach);

			CPlayWnd *pw=new CPlayWnd();
			pw->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rectEach, this, 2010);
			this->playwndList.AddTail(pw);
		}
	}
}

void CdocumentTestView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//if(nType!=SIZE_MINIMIZED)
	//{
	//	int widthTotal=mRect.Width();
	//	int heightTotal=mRect.Height();
	//	int widthEach=(widthTotal-4*frameWidth)/3;
	//	int heightEach=(heightTotal-4*frameWidth)/3;

	//	if(this->playwndList.GetCount())
	//	{
	//		for(int i=0;i<3;i++)
	//		{
	//			for(int j=0;j<3;j++)
	//			{
	//				CRect rectEach((i+1)*frameWidth+i*widthEach, (j+1)*frameWidth+j*heightEach, 
	//					(i+1)*frameWidth+(i+1)*widthEach, (j+1)*frameWidth+(j+1)*heightEach);

	//				((CPlayWnd*)this->playwndList.GetAt(playwndList.FindIndex(i*3+j)))->resize(rectEach, cx, cy);
	//			}
	//		}
	//	}
	//	GetClientRect(&mRect);
	//}
}
