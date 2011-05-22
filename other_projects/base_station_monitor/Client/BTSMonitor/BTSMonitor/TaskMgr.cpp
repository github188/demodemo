// TaskMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "TaskMgr.h"
#include "ConfigMgr.h"

// CTaskMgr dialog

IMPLEMENT_DYNAMIC(CTaskMgr, CDialog)

CTaskMgr::CTaskMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskMgr::IDD, pParent)
	, m_sTaskID("")
	, m_nItem(0)
	, m_sStatus("")
{
	m_enumTASK_MODIFY = T_NONE;
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
	DDX_Control(pDX, IDC_CBO_DEVICE_ID, m_cboDeviceID);
	DDX_Control(pDX, IDC_CBO_CHANNEL, m_cboChannel);
}


BEGIN_MESSAGE_MAP(CTaskMgr, CDialog)
	ON_BN_CLICKED(IDC_BTN_TASK_ADD, &CTaskMgr::OnBnClickedBtnTaskAdd)
	ON_CBN_SELCHANGE(IDC_CBO_DEVICE_ID, &CTaskMgr::OnCbnSelchangeCboDeviceId)
	ON_BN_CLICKED(IDC_BTN_DELETE_TASK, &CTaskMgr::OnBnClickedBtnDeleteTask)
	ON_BN_CLICKED(IDC_BTN_MOD_SAVE_TASK, &CTaskMgr::OnBnClickedBtnModSaveTask)
	ON_BN_CLICKED(IDC_BTN_CANCEL_MODIFY, &CTaskMgr::OnBnClickedBtnCancelModify)
END_MESSAGE_MAP()


// CTaskMgr message handlers

void CTaskMgr::OnBnClickedBtnTaskAdd()
{
	// TODO: Add your control notification handler code here
	CString sTaskName, sDevice, sCh, sVV, sBeginDate, sEndDate, sBeginHH, sBeginMM,sBeginSS, sEndHH,sEndMM, sEndSS;

	//Check the data valid
	GetDlgItemText(IDC_EDIT_TASK_NAME, sTaskName);
	GetDlgItemText(IDC_CBO_DEVICE_ID,  sDevice);
	GetDlgItemText(IDC_CBO_CHANNEL,    sCh);
	GetDlgItemText(IDC_CBO_TARGET_WINDOW, sVV);

	GetDlgItemText(IDC_DATETIME_START, sBeginDate);
	GetDlgItemText(IDC_DATETIME_END,   sEndDate);
	
	GetDlgItemText(IDC_CBO_START_HOUR,   sBeginHH);
	GetDlgItemText(IDC_CBO_START_MINUTE, sBeginMM);
	GetDlgItemText(IDC_CBO_START_SECOND, sBeginSS);
	
	GetDlgItemText(IDC_CBO_END_HOUR,   sEndHH);
	GetDlgItemText(IDC_CBO_END_MINUTE, sEndMM);
	GetDlgItemText(IDC_CBO_END_SECOND, sEndSS);


	if ( sTaskName.IsEmpty() )
	{
		AfxMessageBox("请指定任务名称");
		return;
	}

	if ( sDevice.IsEmpty() )
	{
		AfxMessageBox("请指定基站设备");
		return;
	}

	if ( sCh.IsEmpty() )
	{
		AfxMessageBox("请指定监控通道");
		return;
	}
	if ( sVV.IsEmpty() )
	{
		AfxMessageBox("请指定任务执行窗口");
		return;
	}
	if ( sBeginDate.IsEmpty() )
	{
		AfxMessageBox("请指定任开始日期");
		return;
	}
	if ( sEndDate.IsEmpty() )
	{
		AfxMessageBox("请指定任务结束日期");
		return;
	}

	if ( sBeginHH.IsEmpty()||sBeginMM.IsEmpty()||sBeginSS.IsEmpty() )
	{
		AfxMessageBox("请指定任开始时间");
		return;
	}

	if ( sEndHH.IsEmpty()||sEndMM.IsEmpty()||sEndSS.IsEmpty() )
	{
		AfxMessageBox("请指定任结束时间");
		return;
	}


	/*/..Add to the Task List
	CTaskItem *pObjTask = NULL;
	VERIFY(pObjTask = new CTaskItem());
	pObjTask->sName = sTaskName;
	CString sTemp;
	int pos = util::split_next(sDevice,sTemp,'_',0);
	pObjTask->sUUID = sTemp;
	pos= util::split_next(sDevice,sTemp,'_',pos+1);
	pObjTask->sBTSName = sTemp;
	pos= util::split_next(sCh,sTemp,':',0);
	pObjTask->sCh = sTemp;
	pObjTask->nVV = atoi(sVV);
	pObjTask->sBeginDate = sBeginDate;
	pObjTask->sEndDate   = sEndDate;
	pObjTask->nBeginHour = atoi(sBeginHH);
	pObjTask->nBeginMin = atoi(sBeginMM);
	pObjTask->nBeginSec = atoi(sBeginSS);

	pObjTask->nEndHour = atoi(sEndHH);
	pObjTask->nEndMin = atoi(sEndMM);
	pObjTask->nEndSec = atoi(sEndSS);

	pObjTask->status = 1;//waiting..
	pObjTask->sUUID_Old.Empty();
	pObjTask->sCh_Old.Empty();

	CObArray* pObjArray = m_gConfigMgr.GetTaskList();
	pObjArray->Add(pObjTask);

	//..Add to the List view
	m_lstTask.InsertItem(0,"");
	m_lstTask.SetItem(0,0,LVIF_TEXT,pObjTask->sName,0,0,0,0);
	m_lstTask.SetItem(0,1,LVIF_TEXT,pObjTask->sUUID +"_"+pObjTask->sBTSName,0,0,0,0);
	m_lstTask.SetItem(0,2,LVIF_TEXT,pObjTask->sCh,0,0,0,0);
	sTemp.Format("%d",pObjTask->nVV);
	m_lstTask.SetItem(0,3,LVIF_TEXT,sTemp,0,0,0,0);
	m_lstTask.SetItem(0,4,LVIF_TEXT,pObjTask->sBeginDate,0,0,0,0);
	m_lstTask.SetItem(0,5,LVIF_TEXT,pObjTask->sEndDate,0,0,0,0);
	sTemp.Format("%02d:%02d:%02d",pObjTask->nBeginHour,pObjTask->nBeginMin,pObjTask->nBeginSec);
	m_lstTask.SetItem(0,6,LVIF_TEXT,sTemp,0,0,0,0);
	sTemp.Format("%02d:%02d:%02d",pObjTask->nEndHour,pObjTask->nEndMin,pObjTask->nEndSec);
	m_lstTask.SetItem(0,7,LVIF_TEXT,sTemp,0,0,0,0);

	sTemp = pObjTask->status==1 ? "Waiting" :
		pObjTask->status==2 ? "Monitoring" :
		pObjTask->status==3 ? "Finished": "Unknown";
	m_lstTask.SetItem(0,8,LVIF_TEXT,sTemp,0,0,0,0);
	*///-----------------------------------------------------------------------------------------


	//...
	//#TS:<taskID>$<name>$<uuid>$<channel>$<windowID>$<startDate>$<endDate>$<weeks>$<startTime>$<endTime>$<minShowTime>$<showOrder>$<status>
	//TS:1$test1$1001$1$1$0$0$0$08:30$08:32$10$1$1
	//TS:2$test2$1002$1$1$0$0$0$08:31$08:31$10$1$2
	//TS:3$test3$1001$2$2$0$0$1,2,3,4$08:32$11:23$3$1$1
	CString sTaskID, sUUID, sWeek,  sStartTime, sEndTime, sMinShowTime, sShowOrder;
	CString sTemp, sBTSName;
	int pos = util::split_next(sDevice,sTemp,'_',0);
	sUUID = sTemp;
	pos= util::split_next(sDevice,sTemp,'_',pos+1);
	sBTSName = sTemp;
	pos= util::split_next(sCh,sTemp,':',0);
	sCh = sTemp;

	sStartTime.Format("%02d:%02d:%02d",atoi(sBeginHH),atoi(sBeginMM),atoi(sBeginSS));
	sEndTime.Format("%02d:%02d:%02d",atoi(sEndHH),atoi(sEndMM),atoi(sEndSS));

	CString sStatus = "2";

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if( pApp->pgkclient->saveTaskInfo(sTaskID, sTaskName, sUUID, sCh, sVV,  sWeek,  sBeginDate, sEndDate,  sStartTime, sEndTime, sMinShowTime, sShowOrder,sStatus))
	{

		//sTaskID 
		m_lstTask.SetItem(0,0,LVIF_TEXT,sTaskID,0,0,0,0); //taskname

		m_lstTask.InsertItem(0,"");
		m_lstTask.SetItem(0,1,LVIF_TEXT,sTaskName,0,0,0,0); //taskname
		
		m_lstTask.SetItem(0,2,LVIF_TEXT,sDevice,0,0,0,0); //sUUID_BTSName

		m_lstTask.SetItem(0,3,LVIF_TEXT,sCh,0,0,0,0); //sCH

		m_lstTask.SetItem(0,4,LVIF_TEXT,sVV,0,0,0,0);//nVV, WinID

		//startDate
		m_lstTask.SetItem(0,5,LVIF_TEXT,sBeginDate,0,0,0,0);

		 //endDate
		m_lstTask.SetItem(0,6,LVIF_TEXT,sEndDate,0,0,0,0);

		 //weeks
		//m_lstTask.SetItem(0,6,LVIF_TEXT,weeks,0,0,0,0);

		//starttime
		m_lstTask.SetItem(0,7,LVIF_TEXT,sStartTime,0,0,0,0);

		//endtime
		m_lstTask.SetItem(0,8,LVIF_TEXT,sEndTime,0,0,0,0);

		//minShowTime
		m_lstTask.SetItem(0,9,LVIF_TEXT,sMinShowTime,0,0,0,0);

		//showOrder
		m_lstTask.SetItem(0,10,LVIF_TEXT,sShowOrder,0,0,0,0);

		//sTemp = pObjTask->status==1 ? "Waiting" :
		//	pObjTask->status==2 ? "Monitoring" :
		//	pObjTask->status==3 ? "Finished": "Unknown";
		m_lstTask.SetItem(0,11,LVIF_TEXT,"idle",0,0,0,0);
	}
	else
	{
		AfxMessageBox("添加失败！");
	}
}

BOOL CTaskMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitTaskLstView();

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

	//#TS:<taskID>$<name>$<uuid>$<channel>$<windowID>$<startDate>$<endDate>$<weeks>$<startTime>$<endTime>$<minShowTime>$<showOrder>$<status>
	CString strHeader[] = 
	{
		"任务ID",
		"任务名称", //
		"监控基站",
		"监控通道",
		"执行窗口",
		"开始日期",
		"结束日期",
		"开始时间",
		"结束时间",
		"监控时间",
		"监控顺序",
		"任务状态" //等待、执行中、完成
	};
	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	int nWidth[] = {0,88,168,56,56,68,68,68,68,68,68,58};
	for (int i=0; i<nCnt; i++)
		m_lstTask.InsertColumn(i,strHeader[i],LVCFMT_CENTER, nWidth[i]);

	int i=0;
	CString str;
	m_cboStartHour.ResetContent();
	m_cboEndHour.ResetContent();
	for (;i<24;i++)
	{
		str.Format("%2d", i);
		m_cboStartHour.AddString(str);
		m_cboEndHour.AddString(str);
	}
	
	m_cboStartMinute.ResetContent();
	m_cboEndMinute.ResetContent();
	for(i=0; i<60;i++)
	{
		str.Format("%02d", i);
		m_cboStartMinute.AddString(str);
		m_cboEndMinute.AddString(str);
	}

	m_cboStartSecond.ResetContent();
	m_cboEndSecond.ResetContent();
	for(i=0;i<60;i++)
	{
		str.Format("%02d", i);
		m_cboStartSecond.AddString(str);
		m_cboEndSecond.AddString(str);
	}

	m_cboTargetWindow.ResetContent();
	for(i=0;i<25;i++)
	{
		str.Format("%d", i+1);
		m_cboTargetWindow.AddString(str);
	}	

	//UUID _ Name;
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	int nCount = pApp->pgkclient->btsmanager.btsmap.GetCount();
	if (nCount>0)
	{
		m_cboDeviceID.ResetContent();
		POSITION pos = pApp->pgkclient->btsmanager.btsmap.GetStartPosition();
		int iKey;
		BTSInfo* pBTSInfo=NULL;
		CString strTemp;
		while (pos != NULL)
		{
			pApp->pgkclient->btsmanager.btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
			if (pBTSInfo && (!pBTSInfo->channelInfo.IsEmpty()) )
			{
				strTemp.Format("%s_%s",pBTSInfo->uuid, pBTSInfo->name);
				m_cboDeviceID.AddString(strTemp);
			}
		}
	}

	m_cboChannel.ResetContent();

	/*/Init the listview--------------------------------------------
	CObArray* pObjArray = m_gConfigMgr.GetTaskList();
	int nTaskCount = 0;
	CTaskItem *pObjTask = NULL;
	CString sTemp;
	if (pObjArray)
	{
		nTaskCount = pObjArray->GetCount();
		for(i=0; i<nTaskCount; i++)
		{
			VERIFY(pObjTask = (CTaskItem*)pObjArray->GetAt(i));
			if (pObjTask->status == 4)
				continue;

			m_lstTask.InsertItem(0,"");
			m_lstTask.SetItem(0,0,LVIF_TEXT,pObjTask->sName,0,0,0,0);
			m_lstTask.SetItem(0,1,LVIF_TEXT,pObjTask->sUUID +"_"+pObjTask->sBTSName,0,0,0,0);
			m_lstTask.SetItem(0,2,LVIF_TEXT,pObjTask->sCh,0,0,0,0);
			sTemp.Format("%d",pObjTask->nVV);
			m_lstTask.SetItem(0,3,LVIF_TEXT,sTemp,0,0,0,0);
			m_lstTask.SetItem(0,4,LVIF_TEXT,pObjTask->sBeginDate,0,0,0,0);
			m_lstTask.SetItem(0,5,LVIF_TEXT,pObjTask->sEndDate,0,0,0,0);
			sTemp.Format("%02d:%02d:%02d",pObjTask->nBeginHour,pObjTask->nBeginMin,pObjTask->nBeginSec);
			m_lstTask.SetItem(0,6,LVIF_TEXT,sTemp,0,0,0,0);
			sTemp.Format("%02d:%02d:%02d",pObjTask->nEndHour,pObjTask->nEndMin,pObjTask->nEndSec);
			m_lstTask.SetItem(0,7,LVIF_TEXT,sTemp,0,0,0,0);

			sTemp = pObjTask->status==1 ? "Waiting" :
				pObjTask->status==2 ? "Monitoring" :
				pObjTask->status==3 ? "Finished": "Unknown";
			m_lstTask.SetItem(0,8,LVIF_TEXT,sTemp,0,0,0,0);

		}
	}
	*///-------------------------------------------------------------
	CString sTaskID;	
	CString sTaskName;
	CString sBTSID;
	CString sCh;	
	CString sWindow;		
	CString sBeginDate;
	CString sEndDate;
	CString sBeginTime;
	CString sEndTime;
	CString sWeek;
	CString sDuring;
	CString sSequence;
	CString sStatus;

	CString sTaskList;
	if ( pApp->pgkclient->getTaskList(sTaskList))
	{
		//#TS:<taskID>$<name>$<uuid>$<channel>$<windowID>$<startDate>$<endDate>$<weeks>$<startTime>$<endTime>$<minShowTime>$<showOrder>$<status>
		//TS:1$test1$1001$1$1$0$0$0$08:30$08:32$10$1$1
		//TS:2$test2$1002$1$1$0$0$0$08:31$08:31$10$1$2
		//TS:3$test3$1001$2$2$0$0$1,2,3,4$08:32$11:23$3$1$1

		//1.status＝ '1', "运行状态"，status='2', "暂停运行"
		//2. 如果status=9, 根据taskID删除一个计划任务。   

		CString temp;
		int pos = util::split_next(sTaskList, temp, '$', 0);
		pos = util::split_next(sTaskList, temp, '\n', pos+1);
		int nTaskCount = atoi(temp);
		CString sOneTask;
		for(i=0; i<nTaskCount; i++)
		{
			pos = util::split_next(sTaskList, sOneTask, '\n', pos+1);
			if (sOneTask.IsEmpty())
				break;
			int posLine = util::split_next(sOneTask, sTaskID, '$', 0);
			posLine = util::split_next(sOneTask, sTaskName, '$', posLine+1); //Name
			posLine = util::split_next(sOneTask, sBTSID, '$', posLine+1); //sUUID
			posLine = util::split_next(sOneTask, sCh, '$', posLine+1); //sCh
			posLine = util::split_next(sOneTask, sWindow, '$', posLine+1); //nVV, WinID
			posLine = util::split_next(sOneTask, sBeginDate, '$', posLine+1); //startDate
			posLine = util::split_next(sOneTask, sEndDate, '$', posLine+1); //endDate
			posLine = util::split_next(sOneTask, sWeek, '$', posLine+1); //weeks
			posLine = util::split_next(sOneTask, sBeginTime, '$', posLine+1); //starttime
			posLine = util::split_next(sOneTask, sEndTime, '$', posLine+1); //endtime
			posLine = util::split_next(sOneTask, sDuring, '$', posLine+1); //minShowTime
			posLine = util::split_next(sOneTask, sSequence, '$', posLine+1); //showOrder
			posLine = util::split_next(sOneTask, sStatus, '$', posLine+1); //status
			int nStatus = atoi(sStatus);
			if (nStatus==9) //Finished...
				continue;
			
			//taskID 
			m_lstTask.InsertItem(0,"");
			m_lstTask.SetItem(0,0,LVIF_TEXT,sTaskID,0,0,0,0);
			//..

			//taskName
			m_lstTask.SetItem(0,1,LVIF_TEXT,sTaskName,0,0,0,0);
			
			//Possition
			POSITION position = pApp->pgkclient->btsmanager.btsmap.GetStartPosition();
			int iKey;
			BTSInfo *pBTSInfo=NULL;
			while (position != NULL)
			{
				pApp->pgkclient->btsmanager.btsmap.GetNextAssoc(position, iKey, pBTSInfo);
				if (pBTSInfo && (pBTSInfo->uuid ==sBTSID) )
					break;
			}
			m_lstTask.SetItem(0,2,LVIF_TEXT,sBTSID +"_"+pBTSInfo->name,0,0,0,0);

			//Channel
			m_lstTask.SetItem(0,3,LVIF_TEXT,sCh,0,0,0,0);

			//Window
			m_lstTask.SetItem(0,4,LVIF_TEXT,sWindow,0,0,0,0);

			//Begin Date
			m_lstTask.SetItem(0,5,LVIF_TEXT,sBeginDate,0,0,0,0);

			//End Date
			m_lstTask.SetItem(0,6,LVIF_TEXT,sEndDate,0,0,0,0);

			//Week
			//

			//sBeginTime
			//sTemp.Format("%02d:%02d:%02d",pObjTask->nBeginHour,pObjTask->nBeginMin,pObjTask->nBeginSec);
			m_lstTask.SetItem(0,7,LVIF_TEXT,sBeginTime,0,0,0,0);

			//End Time
			//sTemp.Format("%02d:%02d:%02d",pObjTask->nEndHour,pObjTask->nEndMin,pObjTask->nEndSec);
			m_lstTask.SetItem(0,8,LVIF_TEXT,sEndTime,0,0,0,0);

			//Show time
			m_lstTask.SetItem(0,9,LVIF_TEXT,sDuring,0,0,0,0);
			
			//Show Order
			m_lstTask.SetItem(0,10,LVIF_TEXT,sSequence,0,0,0,0);

			//Status..
			CString str = nStatus==1 ? "runing" : 
				nStatus==2 ? "idle" :
				nStatus==9 ? "finished" : "unknown";
			m_lstTask.SetItem(0,11,LVIF_TEXT,str,0,0,0,0);
		}
	}


}

void CTaskMgr::OnCbnSelchangeCboDeviceId()
{
	// TODO: Add your control notification handler code here
	m_cboChannel.ResetContent();

    int nSet = m_cboDeviceID.GetCurSel();
    if (nSet == -1)   return;
	
	CString sDevice, sUUID,sName, sCh;
    m_cboDeviceID.GetLBText(nSet, sDevice);
	int nPos = util::split_next(sDevice,sUUID,'_',0);
	nPos = util::split_next(sDevice, sName, '_', nPos+1);

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	POSITION pos = pApp->pgkclient->btsmanager.btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	CString strTemp;
	while (pos != NULL)
	{
		pApp->pgkclient->btsmanager.btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if ( pBTSInfo && pBTSInfo->uuid == sUUID )
		{
			int nCnt = pBTSInfo->ch_list.GetCount();
			for(int i=0; i<nCnt;i++)
				m_cboChannel.AddString(pBTSInfo->ch_list.GetAt(i));
		}
	}

}

void CTaskMgr::OnBnClickedBtnDeleteTask()
{
	// TODO: Add your control notification handler code here
	//m_lstTask.G
	POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
	if (pos == NULL)
	   TRACE0("No items were selected!\n");
	else
	{
		CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
		while (pos)
		{
			int nItem = m_lstTask.GetNextSelectedItem(pos);
			TRACE1("Item %d was selected!\n", nItem);
			// you could do your own processing on nItem here

			CString sTaskID		= m_lstTask.GetItemText(nItem,0);
			CString sTaskName	= m_lstTask.GetItemText(nItem,1);
			CString sBTSID		= m_lstTask.GetItemText(nItem,2);
			CString sCh			= m_lstTask.GetItemText(nItem,3);
			CString sWindow		= m_lstTask.GetItemText(nItem,4);
			CString sBeginDate	= m_lstTask.GetItemText(nItem,5);
			CString sEndDate	= m_lstTask.GetItemText(nItem,6);
			CString sBeginTime	= m_lstTask.GetItemText(nItem,7);
			CString sEndTime	= m_lstTask.GetItemText(nItem,8);
			CString sDuring		= m_lstTask.GetItemText(nItem,9);
			CString sSequence	= m_lstTask.GetItemText(nItem,10);
			CString sStatus		= m_lstTask.GetItemText(nItem,11);
			CString sUUID;
			int pos = util::split_next(sBTSID,sUUID,'_',0);

			if( pApp->pgkclient->saveTaskInfo(sTaskID, 
				sTaskName, 
				sUUID, 
				sCh, 
				sWindow,  
				"",  
				sBeginDate, 
				sEndDate,  
				sBeginTime, 
				sEndTime, 
				sDuring, 
				sSequence,
				"9")) //Delete
			{

				//CObArray* pObjArray = m_gConfigMgr.GetTaskList();
				//int nTaskCount = 0;
				//CTaskItem *pObjTask = NULL;
				//CString sTemp;
				//if (pObjArray)
				//{
				//	nTaskCount = pObjArray->GetCount();
				//	for(int i=0; i<nTaskCount; i++)
				//	{
				//		VERIFY(pObjTask = (CTaskItem*)pObjArray->GetAt(i));
				//		if (!pObjTask) continue;
				//
				//		if (pObjTask->sName == sTaskName)
				//		{
				//			pObjTask->status = 4;
				//			break;
				//		}
				//	}
				//} //pObjArray
				
				m_lstTask.DeleteItem(nItem);
				
			}
			else
			{
				AfxMessageBox("删除失败!");
			}
		}//while Pos
	}

}

void CTaskMgr::OnBnClickedBtnModSaveTask()
{
	// TODO: Add your control notification handler code here
	switch(m_enumTASK_MODIFY)
	{
		case T_NONE:
			{
				POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
				if (pos == NULL)
				{
					m_sTaskID="";
					m_nItem=-1;
					m_sStatus="";

				   TRACE0("No items were selected!\n");
				}
				else
				{
					while (pos)
					{
						m_nItem = m_lstTask.GetNextSelectedItem(pos);
					
						m_sTaskID			= m_lstTask.GetItemText(m_nItem,0);
						CString sTaskName	= m_lstTask.GetItemText(m_nItem,1);
						CString sBTSID		= m_lstTask.GetItemText(m_nItem,2);
						CString sCh			= m_lstTask.GetItemText(m_nItem,3);
						CString sWindow		= m_lstTask.GetItemText(m_nItem,4);
						CString sBeginDate	= m_lstTask.GetItemText(m_nItem,5);
						CString sEndDate	= m_lstTask.GetItemText(m_nItem,6);
						CString sBeginTime	= m_lstTask.GetItemText(m_nItem,7);
						CString sEndTime	= m_lstTask.GetItemText(m_nItem,8);
						CString sDuring		= m_lstTask.GetItemText(m_nItem,9);
						CString sSequence	= m_lstTask.GetItemText(m_nItem,10);
						m_sStatus			= m_lstTask.GetItemText(m_nItem,11);

						SetDlgItemText(IDC_EDIT_TASK_NAME, sTaskName);

						m_cboDeviceID.SelectString(0,sBTSID);
						//CString sChan;
						//sChan.Format("%s:通道%d",sCh, sCh);
						//m_cboChannel.SelectString(0,sChan);
						m_cboChannel.SetCurSel(atoi(sCh)-1);
						//m_cboTargetWindow.SelectString(0,sWindow);
						m_cboTargetWindow.SetCurSel(atoi(sWindow)-1);

						CString sYY, sMM, sDD;
						int pos = util::split_next(sBeginDate,sYY,'-',0);
						pos = util::split_next(sBeginDate,sMM,'-',pos+1);
						sDD = sBeginDate.Mid(pos+1);
						CTime tbTime(atoi(sYY), atoi(sMM), atoi(sDD), 0, 0, 0);
						m_dtStart.SetTime(&tbTime);

						pos = util::split_next(sEndDate,sYY,'-',0);
						pos = util::split_next(sEndDate,sMM,'-',pos+1);
						sDD = sEndDate.Mid(pos+1);
						CTime teTime(atoi(sYY), atoi(sMM), atoi(sDD), 0, 0, 0);
						m_dtEnd.SetTime(&teTime);
						
						CString sbHH,sbMM,sbSS;
						pos = util::split_next(sBeginTime,sbHH,':',0);
						pos = util::split_next(sBeginTime,sbMM,':',pos+1);
						sbSS = sBeginTime.Mid(pos+1);

						m_cboStartHour.SelectString(0,sbHH);
						m_cboStartMinute.SelectString(0,sbMM);
						m_cboStartSecond.SelectString(0,sbSS);

						CString seHH,seMM,seSS;
						pos = util::split_next(sEndTime,seHH,':',0);
						pos = util::split_next(sEndTime,seMM,':',pos+1);
						seSS = sBeginTime.Mid(pos+1);
						m_cboEndHour.SelectString(0,seHH);
						m_cboEndMinute.SelectString(0,seMM);
						m_cboEndSecond.SelectString(0,seSS);

						CButton *pBtnSave = (CButton *)GetDlgItem(IDC_BTN_MOD_SAVE_TASK);
						if (pBtnSave)
							pBtnSave->SetWindowText("保存修改");

						
						m_enumTASK_MODIFY = T_MODIFY;

						//Disable other button...
						CButton *pBtnDel = (CButton *)GetDlgItem(IDC_BTN_DELETE_TASK);
						if (pBtnDel)
							pBtnDel->EnableWindow(false);
						CButton *pBtnAdd = (CButton *)GetDlgItem(IDC_BTN_TASK_ADD);
						if (pBtnAdd)
							pBtnAdd->EnableWindow(false);

						break;
						
					} //while Pos
				} //else if 
			}
			break;
		case T_MODIFY:
			{

				if ( !ModifyTask() )
					break;

				CButton *pBtnSave = (CButton *)GetDlgItem(IDC_BTN_MOD_SAVE_TASK);
				if (pBtnSave)
					pBtnSave->SetWindowText("修改任务");

				m_enumTASK_MODIFY = T_NONE;

				//enable other button...
				CButton *pBtnDel = (CButton *)GetDlgItem(IDC_BTN_DELETE_TASK);
				if (pBtnDel)
					pBtnDel->EnableWindow(true);
				CButton *pBtnAdd = (CButton *)GetDlgItem(IDC_BTN_TASK_ADD);
				if (pBtnAdd)
					pBtnAdd->EnableWindow(true);

			}
			break;
		case T_SAVE:
			{
			}
			break;
		default:
			break;
	}

}

void CTaskMgr::OnBnClickedBtnCancelModify()
{
	// TODO: Add your control notification handler code here
	m_enumTASK_MODIFY = T_NONE;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_BTN_MOD_SAVE_TASK);
	if (pBtn)
		pBtn->SetWindowText("修改任务");
	//enable other button...
	CButton *pBtnDel = (CButton *)GetDlgItem(IDC_BTN_DELETE_TASK);
	if (pBtnDel)
		pBtnDel->EnableWindow(true);
	CButton *pBtnAdd = (CButton *)GetDlgItem(IDC_BTN_TASK_ADD);
	if (pBtnAdd)
		pBtnAdd->EnableWindow(true);

	//Clear all Info
	SetDlgItemText(IDC_EDIT_TASK_NAME, "");
	SetDlgItemText(IDC_CBO_DEVICE_ID,  "");
	SetDlgItemText(IDC_CBO_CHANNEL,    "");
	SetDlgItemText(IDC_CBO_TARGET_WINDOW, "");

	CTime tTime = CTime::GetCurrentTime();
	m_dtStart.SetTime(&tTime);
	m_dtEnd.SetTime(&tTime);

	SetDlgItemText(IDC_CBO_START_HOUR,   "");
	SetDlgItemText(IDC_CBO_START_MINUTE, "");
	SetDlgItemText(IDC_CBO_START_SECOND, "");
	
	SetDlgItemText(IDC_CBO_END_HOUR,   "");
	SetDlgItemText(IDC_CBO_END_MINUTE, "");
	SetDlgItemText(IDC_CBO_END_SECOND, "");

}

bool CTaskMgr::ModifyTask()
{
	if (m_nItem<0) return false;

	if (m_sTaskID.IsEmpty()) return false;

	CString sTaskName, sDevice, sCh, sVV, sBeginDate, sEndDate, sBeginHH, sBeginMM,sBeginSS, sEndHH,sEndMM, sEndSS;

	//Check the data valid
	GetDlgItemText(IDC_EDIT_TASK_NAME, sTaskName);
	GetDlgItemText(IDC_CBO_DEVICE_ID,  sDevice);
	GetDlgItemText(IDC_CBO_CHANNEL,    sCh);
	GetDlgItemText(IDC_CBO_TARGET_WINDOW, sVV);

	GetDlgItemText(IDC_DATETIME_START, sBeginDate);
	GetDlgItemText(IDC_DATETIME_END,   sEndDate);
	
	GetDlgItemText(IDC_CBO_START_HOUR,   sBeginHH);
	GetDlgItemText(IDC_CBO_START_MINUTE, sBeginMM);
	GetDlgItemText(IDC_CBO_START_SECOND, sBeginSS);
	
	GetDlgItemText(IDC_CBO_END_HOUR,   sEndHH);
	GetDlgItemText(IDC_CBO_END_MINUTE, sEndMM);
	GetDlgItemText(IDC_CBO_END_SECOND, sEndSS);


	if ( sTaskName.IsEmpty() )
	{
		AfxMessageBox("请指定任务名称");
		return false;
	}

	if ( sDevice.IsEmpty() )
	{
		AfxMessageBox("请指定基站设备");
		return false;
	}

	if ( sCh.IsEmpty() )
	{
		AfxMessageBox("请指定监控通道");
		return false;
	}
	if ( sVV.IsEmpty() )
	{
		AfxMessageBox("请指定任务执行窗口");
		return false;
	}
	if ( sBeginDate.IsEmpty() )
	{
		AfxMessageBox("请指定任开始日期");
		return false;
	}
	if ( sEndDate.IsEmpty() )
	{
		AfxMessageBox("请指定任务结束日期");
		return false;
	}

	if ( sBeginHH.IsEmpty()||sBeginMM.IsEmpty()||sBeginSS.IsEmpty() )
	{
		AfxMessageBox("请指定任开始时间");
		return false;
	}

	if ( sEndHH.IsEmpty()||sEndMM.IsEmpty()||sEndSS.IsEmpty() )
	{
		AfxMessageBox("请指定任结束时间");
		return false;
	}

	//...
	//#TS:<taskID>$<name>$<uuid>$<channel>$<windowID>$<startDate>$<endDate>$<weeks>$<startTime>$<endTime>$<minShowTime>$<showOrder>$<status>
	//TS:1$test1$1001$1$1$0$0$0$08:30$08:32$10$1$1
	//TS:2$test2$1002$1$1$0$0$0$08:31$08:31$10$1$2
	//TS:3$test3$1001$2$2$0$0$1,2,3,4$08:32$11:23$3$1$1
	CString sUUID, sWeek,  sStartTime, sEndTime, sMinShowTime, sShowOrder;
	CString sTemp, sBTSName;
	int pos = util::split_next(sDevice,sTemp,'_',0);
	sUUID = sTemp;
	pos= util::split_next(sDevice,sTemp,'_',pos+1);
	sBTSName = sTemp;
	pos= util::split_next(sCh,sTemp,':',0);
	sCh = sTemp;

	sStartTime.Format("%02d:%02d:%02d",atoi(sBeginHH),atoi(sBeginMM),atoi(sBeginSS));
	sEndTime.Format("%02d:%02d:%02d",atoi(sEndHH),atoi(sEndMM),atoi(sEndSS));

	CString sStatus = m_sStatus=="idle"? "2" : "1";

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if( pApp->pgkclient->saveTaskInfo(m_sTaskID, sTaskName, sUUID, sCh, sVV,  sWeek,  sBeginDate, sEndDate,  sStartTime, sEndTime, sMinShowTime, sShowOrder,sStatus))
	{

		//sTaskID 
		m_lstTask.SetItem(m_nItem,0,LVIF_TEXT,m_sTaskID,0,0,0,0); //taskname

		m_lstTask.SetItem(m_nItem,1,LVIF_TEXT,sTaskName,0,0,0,0); //taskname
		
		m_lstTask.SetItem(m_nItem,2,LVIF_TEXT,sDevice,0,0,0,0); //sUUID_BTSName

		m_lstTask.SetItem(m_nItem,3,LVIF_TEXT,sCh,0,0,0,0); //sCH

		m_lstTask.SetItem(m_nItem,4,LVIF_TEXT,sVV,0,0,0,0);//nVV, WinID

		//startDate
		m_lstTask.SetItem(m_nItem,5,LVIF_TEXT,sBeginDate,0,0,0,0);

		 //endDate
		m_lstTask.SetItem(m_nItem,6,LVIF_TEXT,sEndDate,0,0,0,0);

		 //weeks
		//m_lstTask.SetItem(0,6,LVIF_TEXT,weeks,0,0,0,0);

		//starttime
		m_lstTask.SetItem(m_nItem,7,LVIF_TEXT,sStartTime,0,0,0,0);

		//endtime
		m_lstTask.SetItem(m_nItem,8,LVIF_TEXT,sEndTime,0,0,0,0);

		//minShowTime
		m_lstTask.SetItem(m_nItem,9,LVIF_TEXT,sMinShowTime,0,0,0,0);

		//showOrder
		m_lstTask.SetItem(m_nItem,10,LVIF_TEXT,sShowOrder,0,0,0,0);

		m_lstTask.SetItem(m_nItem,11,LVIF_TEXT,m_sStatus,0,0,0,0);

		AfxMessageBox("修改成功!");

		return true;
	}

	AfxMessageBox("修改失败!");

	return false;

}