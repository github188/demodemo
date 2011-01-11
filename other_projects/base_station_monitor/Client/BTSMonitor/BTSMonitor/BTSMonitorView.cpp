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

// BTSMonitorView.cpp : CBTSMonitorView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBTSMonitorView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBTSMonitorView 构造/析构

CBTSMonitorView::CBTSMonitorView()
{
	// TODO: 在此处添加构造代码

}

CBTSMonitorView::~CBTSMonitorView()
{
}

BOOL CBTSMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBTSMonitorView 绘制

void CBTSMonitorView::OnDraw(CDC* /*pDC*/)
{
	CBTSMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CBTSMonitorView 打印


void CBTSMonitorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CBTSMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBTSMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBTSMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CBTSMonitorView 诊断

#ifdef _DEBUG
void CBTSMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CBTSMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBTSMonitorDoc* CBTSMonitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBTSMonitorDoc)));
	return (CBTSMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CBTSMonitorView 消息处理程序
