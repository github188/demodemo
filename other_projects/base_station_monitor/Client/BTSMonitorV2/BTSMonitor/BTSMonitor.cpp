// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ �������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI �����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ���ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// BTSMonitor.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CBTSMonitorApp ����

CBTSMonitorApp::CBTSMonitorApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	pgkclient = NULL;
}

// Ψһ��һ�� CBTSMonitorApp ����

CBTSMonitorApp theApp;


// CBTSMonitorApp ��ʼ��

BOOL CBTSMonitorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//�ٽ�����ʼ��
	CLogFile::InitCriticalSection();

	//CString host="127.0.0.1:8000";
	//pgkclient =new GokuClient(host, host);

	VERIFY( 1 == InitSkinMagicLib(AfxGetInstanceHandle(), NULL, NULL, NULL ));
	VERIFY( 1 == LoadSkinFromResource(NULL, "corona" ,"SKINMAGIC" ));//���ؾ�̬Ƥ����Դ
	VERIFY( 1 == SetDialogSkin( "Dialog" ) );

	CLoginDlg dlg;
	if(dlg.DoModal()==IDCANCEL)
	{
		return FALSE;
	}

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBTSMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CBTSMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CBTSMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBTSMonitorApp �Զ������/���淽��

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

	//PlayView Menu
	GetContextMenuManager()->AddMenu("PlayView", IDR_POPUP_PLAYVIEW);
	
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
void CBTSMonitorApp::StartMonitorBTS(int nVV, CString sUUID, CString sCh, int nCategory)
{
	CBTSMonitorView *pView  = (CBTSMonitorView*)GetBaseView();

	if (pView)
		pView->StartMonitorBTS(nVV, sUUID, sCh,nCategory);
}
void CBTSMonitorApp::StopMonitorBTS(int nViewIndex)
{
	CBTSMonitorView *pView  = (CBTSMonitorView*)GetBaseView();

	if (pView)
		pView->StopMonitorBTS(nViewIndex);

}

// CBTSMonitorApp ��Ϣ��������


int CBTSMonitorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	ExitSkinMagicLib();	
	
	delete pgkclient;

	CLogFile::DelCriticalSection();

	return CWinAppEx::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}