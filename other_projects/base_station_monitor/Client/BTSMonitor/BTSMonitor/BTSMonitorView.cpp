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

#include "PlayView.h"
#include "const.h"
#include "BTSMonitorDoc.h"
#include "BTSMonitorView.h"

#include "include/iPlay.h"
#include "util.h"
#include "logfile.h"
#include "GokuClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBTSMonitorView

IMPLEMENT_DYNCREATE(CBTSMonitorView, CView)

BEGIN_MESSAGE_MAP(CBTSMonitorView, CView)
	// ��׼��ӡ����
	//ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBTSMonitorView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_PLAYVIEW_SELECTED, &CBTSMonitorView::OnPlayviewSelected)
//	ON_COMMAND(ID_VV1, &CBTSMonitorView::OnVv1)
END_MESSAGE_MAP()

// CBTSMonitorView ����/����

CBTSMonitorView::CBTSMonitorView()
{
	// TODO: �ڴ˴���ӹ������
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
/*
void CBTSMonitorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CBTSMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}
*/
//void CBTSMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
//}

//void CBTSMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: ��Ӵ�ӡ����е��������
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
					x = xx * (i%nSplit); //ȡ�࣬0/1/2�� 0/1/2��0/1/2, ÿ������Video Window
					y = yy * (i/nSplit); //ȡ����0/1/2
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
					x = xx * (i%nSplit); //ȡ�࣬0/1/2�� 0/1/2��0/1/2, ÿ������Video Window
					y = yy * (i/nSplit); //ȡ����0/1/2
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
					x = xx * (i%nSplit); //ȡ�࣬
					y = yy * (i/nSplit); //ȡ����
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
		// AFX_WS_DEFAULT_VIEW����(WS_BORDER | WS_VISIBLE | WS_CHILD)
		//m_vvControl.vvInfo[i].vv->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, this, 1000+i);
		m_vvControl.vvInfo[i].vv->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, ID_PLAYVIDEO_VIEW+i);

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
		{
			AfxMessageBox("FULL Window is still under developing!");
			/*
			if (m_vvControl.vvStatus.fullwindow)
			{
				SetWindowPlacement(&m_vvControl.vvStatus.oldplacement);
				m_vvControl.vvStatus.fullwindow = FALSE;
			}
			else
			{
				GetWindowPlacement(&m_vvControl.vvStatus.oldplacement);
				CRect WindowRect;
				GetWindowRect(&WindowRect);
				CRect ClientRect;
				RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);
				ClientToScreen(&ClientRect);
				// ��ȡ��Ļ�ķֱ���
				int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
				int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
				//������������Ŀͻ���ȫ����ʾ����(0,0) ��(nFullWidth, nFullHeight)����, ��(0,0)��
				//(nFullWidth, nFullHeight)������������ԭ���ںͳ�������֮��� �ͻ���λ�ü�Ĳ�ֵ, �͵õ�
				//ȫ����ʾ�Ĵ���λ�� .
				
				//m_FullScreenRect.left=WindowRect.left��ClientRect.left;
				//m_FullScreenRect.top=WindowRect.top��ClientRect.top;
				//m_FullScreenRect.right=WindowRect.right��ClientRect.right��nFullWidth;
				//m_FullScreenRect.bottom=WindowRect.bottom��ClientRect.bottom��nFullHeight;
				//m_bFullScreen=TRUE; // ����ȫ����ʾ��־Ϊ TRUE
				
				CRect rcFull;
				rcFull.top = 0;
				rcFull.left= 0;
				rcFull.right = nFullWidth;
				rcFull.bottom = nFullHeight;

				// ����ȫ����ʾ״̬
				WINDOWPLACEMENT wndpl;
				wndpl.length=sizeof(WINDOWPLACEMENT);
				wndpl.flags=0;
				wndpl.showCmd=SW_SHOWNORMAL;
				wndpl.rcNormalPosition=rcFull; //m_FullScreenRect;
				SetWindowPlacement(&wndpl);

				m_vvControl.vvStatus.fullwindow = TRUE;

			}
			*/
		}
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
	case	MSG_SELECT_CAMERA_DEVICE:
		{
			char* p = (char*)lParam;
			CString strCamera(p);
			StartMonitorBTS(strCamera);
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

void CBTSMonitorView::StartMonitorBTS(CString strBtsInfo)
{
	
	CString sVVFile;
	/*
	CString path="F:\\Projects\\Video\\BTSMonitor\\test\\";
	CString sVideo[] = {"test01.h264","test02.h264","test03.h264","test04.h264","test05.h264","test06.h264","test07.h264"};
	if ( strBtsInfo.Find("1") > -1)
	{
		sVVFile = path + sVideo[0];
	}
	else if (strBtsInfo.Find("2") > -1)
	{
		sVVFile = path + sVideo[1];
	}
	else if (strBtsInfo.Find("3") > -1)
	{
		sVVFile = path + sVideo[2];
	}
	else if (strBtsInfo.Find("4") > -1)
	{
		sVVFile = path + sVideo[3];
	}
	else if (strBtsInfo.Find("5") > -1)
	{
		sVVFile = path + sVideo[4];
	}
	else if (strBtsInfo.Find("6") > -1)
	{
		sVVFile = path + sVideo[5];
	}
	else
		sVVFile = path + sVideo[6];
	*/

	int nActView = m_vvControl.vvStatus.activeid;
	
	BOOL bPlayFile = FALSE;
	if (bPlayFile)
	{
		PLAY_OpenFile(nActView, sVVFile.GetBuffer());
	
		PLAY_Play(nActView, m_vvControl.vvInfo[nActView].vv->m_hWnd);
	}
	else //Play From the Server, remote Camera
	{	

		CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();

		//GokuClient *client; //("127.0.0.1");
		//wstring host(L"127.0.0.1:8000");
		//CString host("192.168.1.200:8000");
		CString host("127.0.0.1:8000");
		CString sUUID, sChannelID;
		int pos = util::split_next(strBtsInfo,sUUID,'$',0);
		pos = util::split_next(strBtsInfo,sChannelID,'$',pos+1);
		//client = new GokuClient(host, host);
	
		//for(int i=0;i<9;i++)
		//{
			PLAY_SetStreamOpenMode(nActView, STREAME_REALTIME);
			BOOL bOpenRet = PLAY_OpenStream(nActView,0,0,1024*900);
			if(bOpenRet)
			{
				//CPlayWnd *pwnd=(CPlayWnd *)playwndList.GetAt(playwndList.FindIndex(i));
				//pwnd->ShowWindow(SW_SHOW);
				//HWND hwnd=pwnd->GetSafeHwnd();
				//BOOL bPlayRet=PLAY_Play(i, hwnd);
				PLAY_Play(nActView, m_vvControl.vvInfo[nActView].vv->m_hWnd);

				//int *tmp=new int(i);
				//util::int2str(host, nActView);
				//client->replay(host, play_video, nActView); //Play Local Vedio

				//Play Remote Vedio runatime			
				pApp->pgkclient->real_play(sUUID, sChannelID, play_video, nActView);
				
				//pApp->pgkclient->replay(host, play_video, nActView);
				//start a thread to receive the video information.
				//mythread = AfxBeginThread(recvThread, tmp);
			}
		//}
		
	}	
	

}

int play_video(int  sessionId, char * pBuffer, int  len)
{
	//wstring log;
	//log.append(L"play video session:");
	CString sLog("play video session:");
	util::int2str(sLog, sessionId);
	//log.append(L" buffer len:");
	sLog += " buffer len:";
	util::int2str(sLog, len);
	CLogFile::WriteLog(sLog);
	while(PLAY_InputData(sessionId, (BYTE*)pBuffer, len)==FALSE)
	{
		::Sleep(45);
	}
	return 1;
}
