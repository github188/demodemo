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

#include "PlayView.h"
#include "const.h"
#include "BTSMonitorDoc.h"
#include "BTSMonitorView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBTSMonitorView

IMPLEMENT_DYNCREATE(CBTSMonitorView, CView)

BEGIN_MESSAGE_MAP(CBTSMonitorView, CView)
	// 标准打印命令
	//ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBTSMonitorView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_PLAYVIEW_SELECTED, &CBTSMonitorView::OnPlayviewSelected)
//	ON_COMMAND(ID_VV1, &CBTSMonitorView::OnVv1)
END_MESSAGE_MAP()

// CBTSMonitorView 构造/析构

CBTSMonitorView::CBTSMonitorView()
{
	// TODO: 在此处添加构造代码
	for (int i=0; i<cnMAX_VV; i++)
	{
		m_vvControl.vvInfo[i].vv = NULL;

		m_vvControl.vvInfo[i].rc.left = 0;
		m_vvControl.vvInfo[i].rc.top = 0;
		m_vvControl.vvInfo[i].rc.right = 0;
		m_vvControl.vvInfo[i].rc.bottom = 0;
	}

	m_vvControl.vvStatus.activeid = 0; //fist one
	m_vvControl.vvStatus.fullwindow = FALSE;
	m_vvControl.vvStatus.vvcount = VV_6;
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
/*
void CBTSMonitorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CBTSMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}
*/
//void CBTSMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: 添加额外的打印前进行的初始化过程
//}

//void CBTSMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: 添加打印后进行的清理过程
//}

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

void CBTSMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (IsWindow(m_hWnd) && cx>0 && cy>0 && m_vvControl.vvInfo[0].vv && IsWindow(m_vvControl.vvInfo[0].vv->m_hWnd))
	{
		//Hide All View firstly
		for (int n=0; n<cnMAX_VV; n++)
		{
			( (CPlayView*)m_vvControl.vvInfo[n].vv)->HidePlayView();
			( (CPlayView*)m_vvControl.vvInfo[n].vv)->ShowWindow(SW_HIDE);
		}

		int nSplit = VV_1 == m_vvControl.vvStatus.vvcount ? 1:
					 VV_4 == m_vvControl.vvStatus.vvcount ? 2:
					 VV_6 == m_vvControl.vvStatus.vvcount ? 3:
					 VV_9 == m_vvControl.vvStatus.vvcount ? 3:
					 VV_8 == m_vvControl.vvStatus.vvcount ? 4:
					 VV_16 == m_vvControl.vvStatus.vvcount ? 4:
					 VV_25 == m_vvControl.vvStatus.vvcount ? 5 : 3;

		CRect rect, rc;
		GetClientRect(&rect);
		int x, y, xx,yy;
		xx  = rect.Width() /nSplit;
		yy  = rect.Height()/nSplit; 
		
		//for (int i=0; i<cnMAX_VV; i++)
		switch(m_vvControl.vvStatus.vvcount)
		{
		case VV_1:
		case VV_4:
		case VV_9:
		case VV_16:
		case VV_25:
			{

				for (int i=0; i<m_vvControl.vvStatus.vvcount; i++)
				{	
					x = xx * (i%nSplit); //取余，0/1/2， 0/1/2，0/1/2, 每行三个Video Window
					y = yy * (i/nSplit); //取整，0/1/2
					//width = xx * (i+1);
					//height= yy * (i+1);
					if (m_vvControl.vvInfo[i].vv && IsWindow(m_vvControl.vvInfo[i].vv->m_hWnd))
					{
						//Show Window...
						( (CPlayView*)m_vvControl.vvInfo[i].vv)->ShowPlayView();
						( (CPlayView*)m_vvControl.vvInfo[i].vv)->ShowWindow(SW_SHOW);

						//Move window the target place...
						m_vvControl.vvInfo[i].vv->MoveWindow(x,y,xx,yy);

						m_vvControl.vvInfo[i].rc.left		= x;
						m_vvControl.vvInfo[i].rc.top		= y;
						m_vvControl.vvInfo[i].rc.right	= x+xx;
						m_vvControl.vvInfo[i].rc.bottom	= y+yy; 

					}

				}
			}
			break;
		case VV_6:
			{
				int nV=0;
				//................2...5
				//as is VV_9, (0,1,3,4) 6,7,8
				for (int i=0; i<VV_9 && nV<VV_6; i++)
				{	
					x = xx * (i%nSplit); //取余，0/1/2， 0/1/2，0/1/2, 每行三个Video Window
					y = yy * (i/nSplit); //取整，0/1/2
					//width = xx * (i+1);
					//height= yy * (i+1);
					if (m_vvControl.vvInfo[nV].vv && IsWindow(m_vvControl.vvInfo[nV].vv->m_hWnd))
					{
						if (nV==0)
						{
							//Show Window...
							( (CPlayView*)m_vvControl.vvInfo[i].vv)->ShowPlayView();
							( (CPlayView*)m_vvControl.vvInfo[i].vv)->ShowWindow(SW_SHOW);
							m_vvControl.vvInfo[nV].vv->MoveWindow(x,y,xx*2,yy*2);
							nV++;

							m_vvControl.vvInfo[i].rc.left		= x;
							m_vvControl.vvInfo[i].rc.top		= y;
							m_vvControl.vvInfo[i].rc.right	= x+xx*2;
							m_vvControl.vvInfo[i].rc.bottom	= y+yy*2; 
						}
						else if (i==1 || i==3 || i==4)
							continue;
						else
						{
							//Show Window...
							( (CPlayView*)m_vvControl.vvInfo[nV].vv)->ShowPlayView();
							( (CPlayView*)m_vvControl.vvInfo[nV].vv)->ShowWindow(SW_SHOW);
							m_vvControl.vvInfo[nV].vv->MoveWindow(x,y,xx,yy);
							nV++;

							m_vvControl.vvInfo[i].rc.left		= x;
							m_vvControl.vvInfo[i].rc.top		= y;
							m_vvControl.vvInfo[i].rc.right	= x+xx;
							m_vvControl.vvInfo[i].rc.bottom	= y+yy; 

						}

					}

				}


			}
			break;
		case VV_8:
			{
				int nV=0;
				//................... 3,.......7.......11
				//as is VV_16, (0,1,2    4,5,6,  8,9,10  ),12,13,14,15
				for (int i=0; i<VV_16; i++)
				{	
					x = xx * (i%nSplit); //取余，
					y = yy * (i/nSplit); //取整，
					//width = xx * (i+1);
					//height= yy * (i+1);
					if (m_vvControl.vvInfo[i].vv && IsWindow(m_vvControl.vvInfo[i].vv->m_hWnd))
					{
						if (i==0)
						{
							//Show Window...
							( (CPlayView*)m_vvControl.vvInfo[nV].vv)->ShowPlayView();
							( (CPlayView*)m_vvControl.vvInfo[nV].vv)->ShowWindow(SW_SHOW);
							m_vvControl.vvInfo[nV].vv->MoveWindow(x,y,xx*3,yy*3);
							nV++;

							m_vvControl.vvInfo[i].rc.left		= x;
							m_vvControl.vvInfo[i].rc.top		= y;
							m_vvControl.vvInfo[i].rc.right	= x+xx*3;
							m_vvControl.vvInfo[i].rc.bottom	= y+yy*3; 
						}
						else if (i==1 || i==2 || i==4 || i==5 || i==6 || i==8 || i==9 || i==10)
							continue;
						else
						{
							//Show Window...
							( (CPlayView*)m_vvControl.vvInfo[nV].vv)->ShowPlayView();
							( (CPlayView*)m_vvControl.vvInfo[nV].vv)->ShowWindow(SW_SHOW);
							m_vvControl.vvInfo[nV].vv->MoveWindow(x,y,xx,yy);
							nV++;

							m_vvControl.vvInfo[i].rc.left		= x;
							m_vvControl.vvInfo[i].rc.top		= y;
							m_vvControl.vvInfo[i].rc.right	= x+xx;
							m_vvControl.vvInfo[i].rc.bottom	= y+yy; 

						}

					}

				}
			}
			break;
		default:
			break;
		}

	}
}

void CBTSMonitorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	//Create VVs
	CRect rect;
	for (int i=0; i<cnMAX_VV; i++)
	{
		rect.top	= 0; 
		rect.bottom	= 0;//50;
		rect.left	= 0;//70*i;
		rect.right	= 0;//70*(i+1);

		m_vvControl.vvInfo[i].vv = (CPlayView*)RUNTIME_CLASS(CPlayView)->CreateObject();
		// AFX_WS_DEFAULT_VIEW代表(WS_BORDER | WS_VISIBLE | WS_CHILD)
		//m_vvControl.vvInfo[i].vv->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, this, 1000+i);
		m_vvControl.vvInfo[i].vv->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 2000+i);

		m_vvControl.vvInfo[i].vv->ShowWindow(SW_SHOW);

		((CPlayView*)m_vvControl.vvInfo[i].vv)->SetVedioIndex(i);

		//Default is the first one Video window
		((CPlayView*)m_vvControl.vvInfo[i].vv)->SetActiveViewID(0);
	}
	
	
	CRect rc;
	GetClientRect(&rc);
	rc.top+=1;
	rc.left+=1;

	MoveWindow(&rc);
	
}

void CBTSMonitorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox("Monitor View");
	CView::OnLButtonDown(nFlags, point);
}

void CBTSMonitorView::SelectVideoView(int nVideoViewID)
{
	//select the same vedio and Not force selected, then do nothing
	if ( nVideoViewID == m_vvControl.vvStatus.activeid )
		return;

	//Set current selected  video index
	m_vvControl.vvStatus.activeid = nVideoViewID;
	for (int i=0; i<cnMAX_VV; i++)
	{
		if (i != m_vvControl.vvStatus.activeid) //set all unselect video window...
		{
			((CPlayView*)m_vvControl.vvInfo[i].vv)->SetActiveViewID(nVideoViewID);
			((CPlayView*)m_vvControl.vvInfo[i].vv)->DrawSelectedWindow();
		}
	}
	//specialy set the selected windows
	((CPlayView*)m_vvControl.vvInfo[nVideoViewID].vv)->SetActiveViewID(nVideoViewID);
	((CPlayView*)m_vvControl.vvInfo[nVideoViewID].vv)->DrawSelectedWindow();
}
LRESULT CBTSMonitorView::OnPlayviewSelected(WPARAM wParam, LPARAM lParam)
{
	BOOL bIsMsgVV = FALSE;
	switch (wParam)
	{
	case	MSG_SELECT_VIDEO:
		{
			SelectVideoView(lParam);
			TRACE("Selected\r\n");
		}
		break;
	case	MSG_FULL_WINDOW:
		TRACE("FULL Window\r\n");
		break;
	case	MSG_VV_1:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_1;
		}
		break;
	case	MSG_VV_4:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_4;
		}
		break;
	case	MSG_VV_6:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_6;
		}
		break;
	case	MSG_VV_8:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_8;
		}
		break;
	case	MSG_VV_9:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_9;
		}
		break;
	case	MSG_VV_16:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_16;
		}
		break;
	case	MSG_VV_25:
		{
			bIsMsgVV = TRUE;
			m_vvControl.vvStatus.vvcount = VV_25;
		}
		break;

	default:;
		//ASSERT(FALSE);

	}

	if (bIsMsgVV)
	{
		CRect rc;
		GetClientRect(&rc);
		OnSize(0,rc.Width(), rc.Height());
	}

	return 0;
}
