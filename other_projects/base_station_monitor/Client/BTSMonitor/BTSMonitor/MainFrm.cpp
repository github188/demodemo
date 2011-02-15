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

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "DlgFind.h"
#include "const.h"
#include "MainFrm.h"

//
#include "WarningConfig.h"
#include "WarningMgr.h"
#include "TaskMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	//ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	//ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_FIND_CAMERA, &CMainFrame::OnFindCamera)
	ON_MESSAGE(WM_NOTIFY_MESSAGE, &CMainFrame::OnNotifyMessage)
	ON_COMMAND_RANGE(ID_VV1, ID_VV25, OnMonitorWindowType)
	ON_COMMAND(ID_TASK_MGR, &CMainFrame::OnTaskMgr)
	ON_COMMAND(ID_SYSTEM_CONFIG, &CMainFrame::OnSystemConfig)
	ON_COMMAND(ID_WARNING_MGR, &CMainFrame::OnWarningMgr)
	ON_COMMAND(ID_USER_MGR, &CMainFrame::OnUserMgr)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
: m_bFindNext(FALSE)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);

	m_bFindNext = FALSE;

}

CMainFrame::~CMainFrame()
{
	//CMFCRibbonBar     m_wndRibbonBar;
	//CMFCRibbonApplicationButton m_MainButton;
	//CMFCToolBarImages m_PanelImages;
	//CMFCRibbonStatusBar  m_wndStatusBar;

	//int nCnt = m_wndStatusBar.GetCount();
	//int i=0;
	//for(i=0;i<nCnt;i++)
	//{
	//	CMFCRibbonStatusBarPane *pStatusBarPane = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetElement(i);
	//	delete pStatusBarPane;
	//}



}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndCameraView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCameraView);

	m_wndWarningView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndWarningView);

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;

	// �������ͼ��:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// ��ʼ����ť:
	strTemp="ϵͳ";
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_SYSTEMSMALL, IDB_SYSTEMLARGE);

	bNameValid = strTemp.LoadString(IDS_RIBBON_FIND_CAMERA);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FIND_CAMERA, strTemp, 0, 0));

	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_APP_EXIT, strTemp, 1, 1)); //CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp,1,1));


	strTemp = "ϵͳ";//Page1
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_MONITOR_SMALL, IDB_MONITOR_LARGE);

	strTemp = "��ش���"; //page1->pan1
	CMFCRibbonPanel* pPanelVV = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));
	pPanelVV->SetJustifyColumns();

	strTemp = "����";	 //page1-Pan1-btn
	CMFCRibbonButton* pBtnVV1 = new CMFCRibbonButton(ID_VV1, strTemp, 0, 0);
	pPanelVV->Add(pBtnVV1);

	strTemp = "4��";//page1-Pan1-btn
	CMFCRibbonButton* pBtnVV4 = new CMFCRibbonButton(ID_VV4, strTemp, 1);
	pPanelVV->Add(pBtnVV4);
	strTemp = "6��";//page1-Pan1-btn
	CMFCRibbonButton* pBtnVV6 = new CMFCRibbonButton(ID_VV6, strTemp, 2);
	pPanelVV->Add(pBtnVV6);
	strTemp = "8��";
	CMFCRibbonButton* pBtnVV8 = new CMFCRibbonButton(ID_VV8, strTemp, 3);
	pPanelVV->Add(pBtnVV8);
	strTemp = "9��";
	CMFCRibbonButton* pBtnVV9 = new CMFCRibbonButton(ID_VV9, strTemp, 4);
	pPanelVV->Add(pBtnVV9);

	strTemp = "16��";
	CMFCRibbonButton* pBtnVV16 = new CMFCRibbonButton(ID_VV16, strTemp, 5);
	pPanelVV->Add(pBtnVV16);
	strTemp = "25��";
	CMFCRibbonButton* pBtnVV25 = new CMFCRibbonButton(ID_VV25, strTemp, 6);
	pPanelVV->Add(pBtnVV25);

	//bNameValid = strTemp.LoadString(IDS_RIBBON_SELECTALL);
	//ASSERT(bNameValid);
	//pPanelVV->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, strTemp, -1));

	// ��������ӡ���ͼ�����: ////page1-Pan2-btn
	bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
	pPanelView->Add(pBtnStatusBar);


	// ��������ӡ�ϵͳ���á����: ////page1-Pan3-btn
	strTemp = "���������";
	CMFCRibbonPanel* pPanelCfgAndMgr = pCategoryHome->AddPanel(strTemp);
	
	strTemp = "����";
	CMFCRibbonButton* pBtnSysCfg = new CMFCRibbonButton(ID_SYSTEM_CONFIG, strTemp,-1,1);
	pPanelCfgAndMgr->Add(pBtnSysCfg);

	// ��������ӡ�����������: ////page1-Pan4-btn
	strTemp = "����";
	//CMFCRibbonPanel* pPanelTaskMgr = pCategoryHome->AddPanel(strTemp);
	//strTemp = "";
	CMFCRibbonButton* pBtnTaskMgr = new CMFCRibbonButton(ID_TASK_MGR, strTemp,-1,2);
	pPanelCfgAndMgr->Add(pBtnTaskMgr);


	// ��������ӡ��澯�������: ////page1-Pan5-btn
	strTemp = "�澯";
	//CMFCRibbonPanel* pPanelAlertMgr = pCategoryHome->AddPanel(strTemp);
	//strTemp = "";
	CMFCRibbonButton* pBtnAlertCfg = new CMFCRibbonButton(ID_WARNING_MGR, strTemp,-1,3);
	pPanelCfgAndMgr->Add(pBtnAlertCfg);

	// ��������ӡ��û��������: ////page1-Pan5-btn
	strTemp = "�û�";
	//CMFCRibbonPanel* pPanelUserMgr = pCategoryHome->AddPanel(strTemp);
	//strTemp = "";
	CMFCRibbonButton* pBtnUser = new CMFCRibbonButton(ID_USER_MGR, strTemp,-1,4);
	pPanelCfgAndMgr->Add(pBtnUser);


	/*/����Ҫ��ҳ
	// Ϊ�������塱�����ӡ��������:
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE);

	// �����������塱���:
	bNameValid = strTemp.LoadString(IDS_RIBBON_CLIPBOARD);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PASTE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, strTemp, 0, 0);
	pPanelClipboard->Add(pBtnPaste);

	bNameValid = strTemp.LoadString(IDS_RIBBON_CUT);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, strTemp, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_COPY);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, strTemp, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SELECTALL);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, strTemp, -1));

	// ��������ӡ���ͼ�����:
	bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
	pPanelView->Add(pBtnStatusBar);

	*/

	// ��Ԫ����ӵ�ѡ��Ҳ�:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* ��Ĭ������*/, TRUE /* �Ҷ���*/);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// ��ӿ��ٷ��ʹ���������:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FIND_CAMERA);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// �����ļ���ͼ
	CString strCameraView = "����б�";
	if (!m_wndCameraView.Create(strCameraView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CAMERA, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ���������б�����\n");
		return FALSE; // δ�ܴ���
	}
	

	// �����澯����
	CString strWarningView = "�澯";
	if (!m_wndWarningView.Create(strWarningView, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_WARNING, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}
		
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCameraView.SetIcon(hClassViewIcon, FALSE);

	HICON hWarningViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WARNING_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndWarningView.SetIcon(hWarningViewIcon, FALSE);

	
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
/*
void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // ǿ�ƹرա���ӡԤ����ģʽ
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}
*/
void CMainFrame::OnFindCamera()
{
	// TODO: Add your command handler code here
	if (m_bFindNext)
	{
		BOOL bFind = m_wndCameraView.FindNextTarget( );
		if (!bFind)
			AfxMessageBox("û���ҵ�!");
	}
	else // new finding requist
	{
		CDlgFind findDlg;
		if (IDOK == findDlg.DoModal() )
		{
			BOOL bFind = m_wndCameraView.FindNewTarget(findDlg.GetFindStr());
			if (!bFind)
				AfxMessageBox("û���ҵ�!");
		}
	}
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message >= WM_KEYFIRST &&	pMsg->message <= WM_KEYLAST)
	{
			switch(pMsg->wParam)
			{
			case VK_F3: //Find Next
				{
					if (pMsg->lParam>0)
					{
						m_bFindNext=TRUE;
						OnFindCamera();
						m_bFindNext=FALSE;
					}
				}
				break;				
			default:
				break;
			};
	}
			

	return CFrameWndEx::PreTranslateMessage(pMsg);
}
LRESULT CMainFrame::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case	MSG_SELECT_CAMERA_DEVICE:
		{
			CView *pView = GetActiveView();
			int i=0;
			//if (pView)
			//	pView->SelectVideoView(lParam);
		}
		break;
	default:;
		//ASSERT(FALSE);

	}

	return 0;
}
void CMainFrame::OnMonitorWindowType(UINT nID)
{
	// TODO: Add your command handler code here

	/*/Comment out following..
	//CView *pView = GetActiveView(); 
	if (pView)
	{
		CWnd *pWnd = pView->GetParent();
		if (pWnd && pWnd->m_hWnd)
		{
			pWnd->SetActiveWindow();
			::SendMessage(pWnd->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)(nID - ID_VV1 + 3),0);
		}
	}
	*/

	CView *pView = ( (CBTSMonitorApp*)AfxGetApp() )->GetBaseView();
	if (pView && pView->m_hWnd)
		::SendMessage(pView->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)(nID - ID_VV1 + 3),0);

}

void CMainFrame::OnTaskMgr()
{
	// TODO: Add your command handler code here
	CTaskMgr taskMgr;
	if ( IDOK == taskMgr.DoModal())
	{
		;
	}

}

void CMainFrame::OnSystemConfig()
{
	// TODO: Add your command handler code here
	CWarningConfig systemCfg;
	if ( IDOK == systemCfg.DoModal())
	{
		;
	}
}

void CMainFrame::OnWarningMgr()
{
	// TODO: Add your command handler code here
	CWarningMgr alertMgr;
	if ( IDOK == alertMgr.DoModal())
	{
		;
	}
}

void CMainFrame::OnUserMgr()
{
	// TODO: Add your command handler code here
}
