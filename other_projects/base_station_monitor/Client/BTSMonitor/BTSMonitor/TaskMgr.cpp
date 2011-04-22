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
	DDX_Control(pDX, IDC_CBO_DEVICE_ID, m_cboDeviceID);
	DDX_Control(pDX, IDC_CBO_CHANNEL, m_cboChannel);
}


BEGIN_MESSAGE_MAP(CTaskMgr, CDialog)
	ON_BN_CLICKED(IDC_BTN_TASK_ADD, &CTaskMgr::OnBnClickedBtnTaskAdd)
	ON_CBN_SELCHANGE(IDC_CBO_DEVICE_ID, &CTaskMgr::OnCbnSelchangeCboDeviceId)
	ON_BN_CLICKED(IDC_BTN_DELETE_TASK, &CTaskMgr::OnBnClickedBtnDeleteTask)
END_MESSAGE_MAP()


// CTaskMgr message handlers

void CTaskMgr::OnBnClickedBtnTaskAdd()
{
	// TODO: Add your control notification handler code here
	CString sTaskName, sDevice, sCh, sVV, sBeginDate, sEndDate, sBeginHH, sBeginMM,sBeginSS, sEndHH,sEndMM, sEndSS;

	//Check the data valid
	GetDlgItemText(IDC_EDIT_TASK_NAME, sTaskName);
	GetDlgItemText(IDC_CBO_DEVICE_ID, sDevice);
	GetDlgItemText(IDC_CBO_CHANNEL, sCh);
	GetDlgItemText(IDC_CBO_TARGET_WINDOW, sVV);

	GetDlgItemText(IDC_DATETIME_START, sBeginDate);
	GetDlgItemText(IDC_DATETIME_END, sEndDate);
	
	GetDlgItemText(IDC_CBO_START_HOUR, sBeginHH);
	GetDlgItemText(IDC_CBO_START_MINUTE, sBeginMM);
	GetDlgItemText(IDC_CBO_START_SECOND, sBeginSS);
	
	GetDlgItemText(IDC_CBO_END_HOUR, sEndHH);
	GetDlgItemText(IDC_CBO_END_MINUTE, sEndMM);
	GetDlgItemText(IDC_CBO_END_SECOND, sEndSS);


	if ( sTaskName.IsEmpty() )
	{
		AfxMessageBox("��ָ����������");
		return;
	}

	if ( sDevice.IsEmpty() )
	{
		AfxMessageBox("��ָ����վ�豸");
		return;
	}

	if ( sCh.IsEmpty() )
	{
		AfxMessageBox("��ָ�����ͨ��");
		return;
	}
	if ( sVV.IsEmpty() )
	{
		AfxMessageBox("��ָ������ִ�д���");
		return;
	}
	if ( sBeginDate.IsEmpty() )
	{
		AfxMessageBox("��ָ���ο�ʼ����");
		return;
	}
	if ( sEndDate.IsEmpty() )
	{
		AfxMessageBox("��ָ�������������");
		return;
	}

	if ( sBeginHH.IsEmpty()||sBeginMM.IsEmpty()||sBeginSS.IsEmpty() )
	{
		AfxMessageBox("��ָ���ο�ʼʱ��");
		return;
	}

	if ( sEndHH.IsEmpty()||sEndMM.IsEmpty()||sEndSS.IsEmpty() )
	{
		AfxMessageBox("��ָ���ν���ʱ��");
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

	sStartTime.Format("%02d:%02d:%02d",sBeginHH,sBeginMM,sBeginSS);
	sEndTime.Format("%02d:%02d:%02d",sEndHH,sEndMM,sEndSS);

	int nStatus = 2;

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if( pApp->pgkclient->saveTaskInfo(sTaskID, sTaskName, sUUID, sCh, sVV,  sWeek,  sBeginDate, sEndDate,  sStartTime, sEndTime, sMinShowTime, sShowOrder))
	{

		//sTaskID 
		//m_lstTask.SetItem(0,0,LVIF_TEXT,sTaskID,0,0,0,0); //taskname

		m_lstTask.InsertItem(0,"");
		m_lstTask.SetItem(0,0,LVIF_TEXT,sTaskName,0,0,0,0); //taskname
		
		m_lstTask.SetItem(0,1,LVIF_TEXT,sDevice,0,0,0,0); //sUUID_BTSName

		m_lstTask.SetItem(0,2,LVIF_TEXT,sCh,0,0,0,0); //sCH

		m_lstTask.SetItem(0,3,LVIF_TEXT,sVV,0,0,0,0);//nVV, WinID

		//startDate
		m_lstTask.SetItem(0,4,LVIF_TEXT,sBeginDate,0,0,0,0);

		 //endDate
		m_lstTask.SetItem(0,5,LVIF_TEXT,sEndDate,0,0,0,0);

		 //weeks
		//m_lstTask.SetItem(0,6,LVIF_TEXT,weeks,0,0,0,0);

		//starttime
		m_lstTask.SetItem(0,6,LVIF_TEXT,sStartTime,0,0,0,0);

		//endtime
		m_lstTask.SetItem(0,7,LVIF_TEXT,sEndTime,0,0,0,0);

		//minShowTime
		//m_lstTask.SetItem(0,7,LVIF_TEXT,temp,0,0,0,0);

		//showOrder
		//m_lstTask.SetItem(0,7,LVIF_TEXT,showOrder,0,0,0,0);

		//sTemp = pObjTask->status==1 ? "Waiting" :
		//	pObjTask->status==2 ? "Monitoring" :
		//	pObjTask->status==3 ? "Finished": "Unknown";
		CString sStatus = nStatus==1 ? "runing" : 
			nStatus==2 ? "idle" :
			nStatus==9 ? "finished" : "unknown";
		m_lstTask.SetItem(0,8,LVIF_TEXT,sStatus,0,0,0,0);
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

	CString strHeader[] = 
	{
		"��������", //
		"��ػ�վ",
		"���ͨ��",
		"ִ�д���",
		"��ʼ����",
		"��������",
		"��ʼʱ��",
		"����ʱ��",
		"����״̬" //�ȴ���ִ���С����
	};
	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	for (int i=0; i<nCnt; i++)
		m_lstTask.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 88);


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
	CString sTaskList;
	if ( pApp->pgkclient->getTaskList(sTaskList))
	{
		//#TS:<taskID>$<name>$<uuid>$<channel>$<windowID>$<startDate>$<endDate>$<weeks>$<startTime>$<endTime>$<minShowTime>$<showOrder>$<status>
		//TS:1$test1$1001$1$1$0$0$0$08:30$08:32$10$1$1
		//TS:2$test2$1002$1$1$0$0$0$08:31$08:31$10$1$2
		//TS:3$test3$1001$2$2$0$0$1,2,3,4$08:32$11:23$3$1$1

		//1.status�� '1', "����״̬"��status='2', "��ͣ����"
		//2. ���status=9, ����taskIDɾ��һ���ƻ�����   

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
			
			//taskID 
			int posLine = util::split_next(sOneTask, temp, '$', 0);
			//..

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //Name
			m_lstTask.InsertItem(0,"");
			m_lstTask.SetItem(0,0,LVIF_TEXT,temp,0,0,0,0);
			
			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //sUUID
			POSITION position = pApp->pgkclient->btsmanager.btsmap.GetStartPosition();
			int iKey;
			BTSInfo *pBTSInfo=NULL;
			while (position != NULL)
			{
				pApp->pgkclient->btsmanager.btsmap.GetNextAssoc(position, iKey, pBTSInfo);
				if (pBTSInfo && (pBTSInfo->uuid ==temp) )
					break;
			}
			m_lstTask.SetItem(0,1,LVIF_TEXT,temp +"_"+pBTSInfo->name,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //sCh
			m_lstTask.SetItem(0,2,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //nVV, WinID
			m_lstTask.SetItem(0,3,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //startDate
			m_lstTask.SetItem(0,4,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //endDate
			m_lstTask.SetItem(0,5,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //weeks
			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //starttime
			//sTemp.Format("%02d:%02d:%02d",pObjTask->nBeginHour,pObjTask->nBeginMin,pObjTask->nBeginSec);
			m_lstTask.SetItem(0,6,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //starttime
			//sTemp.Format("%02d:%02d:%02d",pObjTask->nEndHour,pObjTask->nEndMin,pObjTask->nEndSec);
			m_lstTask.SetItem(0,7,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //minShowTime
			//m_lstTask.SetItem(0,7,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //showOrder
			//m_lstTask.SetItem(0,7,LVIF_TEXT,temp,0,0,0,0);

			posLine = util::split_next(sOneTask, temp, '$', posLine+1); //status
			//sTemp = pObjTask->status==1 ? "Waiting" :
			//	pObjTask->status==2 ? "Monitoring" :
			//	pObjTask->status==3 ? "Finished": "Unknown";
			int nStatus = atoi(temp);
			CString sStatus = nStatus==1 ? "runing" : 
				nStatus==2 ? "idle" :
				nStatus==9 ? "finished" : "unknown";
			m_lstTask.SetItem(0,8,LVIF_TEXT,sStatus,0,0,0,0);
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
		while (pos)
		{
			int nItem = m_lstTask.GetNextSelectedItem(pos);
			TRACE1("Item %d was selected!\n", nItem);
			// you could do your own processing on nItem here
			
			CString sTaskName = m_lstTask.GetItemText(nItem,0);

			CObArray* pObjArray = m_gConfigMgr.GetTaskList();
			int nTaskCount = 0;
			CTaskItem *pObjTask = NULL;
			CString sTemp;
			if (pObjArray)
			{
				nTaskCount = pObjArray->GetCount();
				for(int i=0; i<nTaskCount; i++)
				{
					VERIFY(pObjTask = (CTaskItem*)pObjArray->GetAt(i));
					if (pObjTask->sName == sTaskName)
					{
						pObjTask->status = 4;
						break;
					}
				}
			} //pObjArray
			
			m_lstTask.DeleteItem(nItem);
			
		} //Pos
	}

}
