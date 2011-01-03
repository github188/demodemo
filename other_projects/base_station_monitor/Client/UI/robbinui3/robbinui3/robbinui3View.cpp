// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// robbinui3View.cpp : Crobbinui3View 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Crobbinui3View::OnFilePrintPreview)
END_MESSAGE_MAP()

// Crobbinui3View 构造/析构

Crobbinui3View::Crobbinui3View()
{
	// TODO: 在此处添加构造代码

}

Crobbinui3View::~Crobbinui3View()
{
}

BOOL Crobbinui3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Crobbinui3View 绘制

void Crobbinui3View::OnDraw(CDC* /*pDC*/)
{
	Crobbinui3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// Crobbinui3View 打印


void Crobbinui3View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL Crobbinui3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Crobbinui3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Crobbinui3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// Crobbinui3View 诊断

#ifdef _DEBUG
void Crobbinui3View::AssertValid() const
{
	CView::AssertValid();
}

void Crobbinui3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Crobbinui3Doc* Crobbinui3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Crobbinui3Doc)));
	return (Crobbinui3Doc*)m_pDocument;
}
#endif //_DEBUG


// Crobbinui3View 消息处理程序
