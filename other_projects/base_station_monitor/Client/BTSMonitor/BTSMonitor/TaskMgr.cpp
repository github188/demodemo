// TaskMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "TaskMgr.h"


// CTaskMgr dialog

IMPLEMENT_DYNAMIC(CTaskMgr, CDialog)

CTaskMgr::CTaskMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskMgr::IDD, pParent)
{

}

CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBO_END_HOUR, m_cboEndHour);
	DDX_Control(pDX, IDC_CBO_END_MINUTE, m_cboEndMinute);
	DDX_Control(pDX, IDC_CBO_END_SECOND, m_cboEndSecond);
	DDX_Control(pDX, IDC_CBO_START_HOUR, m_cboStartHour);
	DDX_Control(pDX, IDC_CBO_START_MINUTE, m_cboStartMinute);
	DDX_Control(pDX, IDC_CBO_START_SECOND, m_cboStartSecond);
	DDX_Control(pDX, IDC_CBO_TARGET_WINDOW, m_cboTargetWindow);
	DDX_Control(pDX, IDC_DATETIME_START, m_dtStart);
	DDX_Control(pDX, IDC_DATETIME_END, m_dtEnd);
	DDX_Control(pDX, IDC_EDIT_TASK_NAME, m_edtTaskName);
	DDX_Control(pDX, IDC_LST_TASK, m_lstTask);
}


BEGIN_MESSAGE_MAP(CTaskMgr, CDialog)
	ON_BN_CLICKED(IDC_BTN_TASK_ADD, &CTaskMgr::OnBnClickedBtnTaskAdd)
END_MESSAGE_MAP()


// CTaskMgr message handlers

void CTaskMgr::OnBnClickedBtnTaskAdd()
{
	// TODO: Add your control notification handler code here
}

BOOL CTaskMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTaskMgr::InitTaskLstView(void)
{
	//olde style
	DWORD dwStyle = ::GetWindowLong(m_lstTask.m_hWnd, GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);

	//New Style
	SetWindowLong(m_lstTask.m_hWnd,GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);

	//Extend Style
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_lstTask.m_hWnd, styles, styles );

	CString strHeader[] = 
	{
		"任务名称",
		"窗口",
		"开始日期",
		"结束日期",
		"执行开始时间",
		"执行结束时间",
		"设备ID",
		"任务状态" //等待、执行中、完成
	};
	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	for (int i=0; i<nCnt; i++)
		m_lstTask.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 88);

	//Icon Support...
	//m_imagelist.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
	//m_lstTask.SetImageList(&m_imagelist,LVSIL_SMALL);
	//CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();
	//HICON hIcon = pApp->LoadIconA(IDI_UNKNOWN);
	//if (hIcon)
	//	m_imagelist.Add(hIcon);
	//hIcon = pApp->LoadIconA(IDI_UNACK);
	//if (hIcon)
	//	m_imagelist.Add(hIcon);
	//hIcon = pApp->LoadIconA(IDI_ACK);
	//if (hIcon)
	//	m_imagelist.Add(hIcon);

	int i=0;
	CString str;
	m_cboStartHour.ResetContent();
	m_cboEndHour.ResetContent();
	for (;i<24;i++)
	{
		str.Format("%d", i);
		m_cboStartHour.AddString(str);
		m_cboEndHour.AddString(str);
	}
	
	m_cboStartMinute.ResetContent();
	m_cboEndMinute.ResetContent();
	for(i=0; i<60;i++)
	{
		str.Format("%d", i);
		m_cboStartMinute.AddString(str);
		m_cboEndMinute.AddString(str);
	}

	m_cboStartSecond.ResetContent();
	m_cboEndSecond.ResetContent();
	for(i=0;i<60;i++)
	{
		str.Format("%d", i);
		m_cboStartSecond.AddString(str);
		m_cboEndSecond.AddString(str);
	}

	m_cboTargetWindow.ResetContent();
	for(i=0;i<25;i++)
	{
		str.Format("%d", i+1);
		m_cboTargetWindow.AddString(str);
	}	

}
