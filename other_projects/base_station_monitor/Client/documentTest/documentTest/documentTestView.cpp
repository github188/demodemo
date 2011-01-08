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

// documentTestView.cpp : CdocumentTestView 类的实现
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
	ON_COMMAND(ID_TEST_PLAYVIDEO, &CdocumentTestView::OnTestPlayvideo)
	ON_COMMAND(ID_FILE_NEW, &CdocumentTestView::OnFileNew)
END_MESSAGE_MAP()

// CdocumentTestView 构造/析构

CdocumentTestView::CdocumentTestView()
{
	// TODO: 在此处添加构造代码
	frameWidth=3;
}

CdocumentTestView::~CdocumentTestView()
{
}

BOOL CdocumentTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CdocumentTestView 绘制

void CdocumentTestView::OnDraw(CDC* /*pDC*/)
{
	CdocumentTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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


// CdocumentTestView 诊断

#ifdef _DEBUG
void CdocumentTestView::AssertValid() const
{
	CView::AssertValid();
}

void CdocumentTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdocumentTestDoc* CdocumentTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdocumentTestDoc)));
	return (CdocumentTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CdocumentTestView 消息处理程序

int CdocumentTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	 //CRect rect(20,20,100,50);
	 //CButton *m_ctrlButton=new CButton();
	 //CComboBox *m_combobox=new CComboBox();
	 //m_ctrlButton->Create(L"Button1",WS_CHILD|WS_VISIBLE,rect,this,NULL);
	 ////创建按扭控件
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

int play_video(int sessionId, char *pBuffer, int len){
	wstring log;
	log.append(L"play video session:");
	int2str(log, sessionId);
	log.append(L" buffer len:");
	int2str(log, len);
	write_log(log);
	while(PLAY_InputData(sessionId, (BYTE*)pBuffer, len)==FALSE)
	{
		::Sleep(1000);
	}
	return 1;
}

void CdocumentTestView::clickOnTreeItem()
{
	GokuClient *client; //("127.0.0.1");
	wstring host(L"127.0.0.1:8000");
	client = new GokuClient(host, host);

	for(int i=0;i<9;i++)
	{
		BOOL bOpenRet = PLAY_OpenStream(i,0,0,1024*100);
		if(bOpenRet)
		{
			CPlayWnd *pwnd=(CPlayWnd *)playwndList.GetAt(playwndList.FindIndex(i));
			pwnd->ShowWindow(SW_SHOW);
			HWND hwnd=pwnd->GetSafeHwnd();
			BOOL bPlayRet=PLAY_Play(i, hwnd);
			//int *tmp=new int(i);
			host = L"1001000";
			int2str(host, i);
			client->replay(host, play_video, i);
			//start a thread to receive the video information.
			//mythread = AfxBeginThread(recvThread, tmp);
		}
	}
}
void CdocumentTestView::OnTestPlayvideo()
{
	// TODO: Add your command handler code here
	clickOnTreeItem();
}

void CdocumentTestView::OnFileNew()
{
	// TODO: Add your command handler code here
	clickOnTreeItem();
}
