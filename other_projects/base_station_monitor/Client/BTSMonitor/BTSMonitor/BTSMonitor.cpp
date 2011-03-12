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

// BTSMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "BTSMonitor.h"
#include "MainFrm.h"

#include "BTSMonitorDoc.h"
#include "BTSMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBTSMonitorApp

BEGIN_MESSAGE_MAP(CBTSMonitorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBTSMonitorApp::OnAppAbout)
	// 基于文件的标准文档命令
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CBTSMonitorApp 构造

CBTSMonitorApp::CBTSMonitorApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	pgkclient = NULL;
}

// 唯一的一个 CBTSMonitorApp 对象

CBTSMonitorApp theApp;


// CBTSMonitorApp 初始化

BOOL CBTSMonitorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(0);  // 加载标准 INI 文件选项

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//临界区初始化
	CLogFile::InitCriticalSection();

	//CString host="127.0.0.1:8000";
	//pgkclient =new GokuClient(host, host);

	VERIFY( 1 == InitSkinMagicLib(AfxGetInstanceHandle(), NULL, NULL, NULL ));
	VERIFY( 1 == LoadSkinFromResource(NULL, "corona" ,"SKINMAGIC" ));//加载静态皮肤资源
	VERIFY( 1 == SetDialogSkin( "Dialog" ) );

	CLoginDlg dlg;
	if(dlg.DoModal()==IDCANCEL)
	{
		return FALSE;
	}

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBTSMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CBTSMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CBTSMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBTSMonitorApp 自定义加载/保存方法

void CBTSMonitorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);

	//WARNING Menu
	bNameValid = strName.LoadString(IDS_WARNING);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_MENU_WARNING);
	
	//WarningMgr
	GetContextMenuManager()->AddMenu("WarningMgr", IDR_POPMENU_WARNMGR);

	
}

void CBTSMonitorApp::LoadCustomState()
{
}

void CBTSMonitorApp::SaveCustomState()
{
}

CView *CBTSMonitorApp::GetBaseView()
{
	if(m_pDocManager)
	{
		POSITION pos1 = m_pDocManager->GetFirstDocTemplatePosition();
		while (pos1)
		{
			CDocTemplate *pDocTemplate = m_pDocManager->GetNextDocTemplate(pos1);
			if(pDocTemplate)
			{
				POSITION pos2 = pDocTemplate->GetFirstDocPosition();
				while (pos2)
				{
					CDocument *pDoc = pDocTemplate->GetNextDoc(pos2);
					if(pDoc)
					{
						POSITION pos3 = pDoc->GetFirstViewPosition();
						while (pos3)
						{
							CView *pView = pDoc->GetNextView(pos3);
							if(pView)
							{
								if(pView->IsKindOf(RUNTIME_CLASS(CBTSMonitorView)))
									return pView;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}
// CBTSMonitorApp 消息处理程序





int CBTSMonitorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	ExitSkinMagicLib();	
	
	delete pgkclient;
	
	return CWinAppEx::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
