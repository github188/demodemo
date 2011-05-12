// ConfigMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "ConfigMgr.h"

///////////////////////////////////////
//Task Array

CTaskItem::CTaskItem()
{
}
CTaskItem::CTaskItem(const CTaskItem& task)
{
	sName = task.sName;
	sBTSName = task.sBTSName;
	sUUID = task.sUUID;
	sCh = task.sCh;
	nVV = task.nVV;
	sBeginDate = task.sBeginDate;
	sEndDate = task.sEndDate;
	nBeginHour = task.nBeginHour;
	nBeginMin = task.nBeginMin;
	nBeginSec = task.nBeginSec;
	nEndHour = task.nEndHour;
	nEndMin = task.nEndMin;
	nEndSec = task.nEndSec;

	//Task Status
	status = task.status; //0 reserve, 1 waiting, 2 runing
	sUUID_Old = task.sUUID_Old;
	sCh_Old   = task.sCh_Old;

}
CTaskItem& CTaskItem::operator=(CTaskItem& task)
{
	sName = task.sName;
	sBTSName = task.sBTSName;
	sUUID = task.sUUID;
	sCh = task.sCh;
	nVV = task.nVV;
	sBeginDate = task.sBeginDate;
	sEndDate = task.sEndDate;
	nBeginHour = task.nBeginHour;
	nBeginMin = task.nBeginMin;
	nBeginSec = task.nBeginSec;
	nEndHour = task.nEndHour;
	nEndMin = task.nEndMin;
	nEndSec = task.nEndSec;

	//Task Status
	status = task.status; //0 reserve, 1 waiting, 2 runing
	sUUID_Old = task.sUUID_Old;
	sCh_Old   = task.sCh_Old;

	return *this;
	
}

CTaskItem::~CTaskItem()
{

}

// CConfigMgr
CConfigMgr m_gConfigMgr;

CConfigMgr::CConfigMgr()
{
	BOOL bTest = TRUE;
	if (bTest)
	{
		CString sDate("2001-7-9");

		CString sY, sM,sD; //Year, Month , Day
		int pos = util::split_next(sDate, sY, '-', 0);
		pos = util::split_next(sDate, sM, '-', pos+1);
		pos = util::split_next(sDate, sD, '-', pos+1);

	}
	//Load Config Data
	util::InitApp();
	char *pConfigurationFile = util::GetAppPath();
	strcat(pConfigurationFile, CONFIG_FILE);
	char lpDefault[1024];
	memset(lpDefault,0,1024);
	DWORD dwRet = GetPrivateProfileString("SYS_CFG", "PicWinIndex","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_nPicWinIndex = atoi(lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "PlayPicInterval","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_nPlayPicInterval = atoi(lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "MaxPopWarnWin","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_nMaxPopWarnWin = atoi(lpDefault);

	dwRet = GetPrivateProfileString("SYS_CFG", "PopWarnSound","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_sPopWarnSound = lpDefault;

	dwRet = GetPrivateProfileString("SYS_CFG", "PopWarnEnable","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0 && lpDefault[0]=='1')
		m_bPopWarnEnable = true;
	else
		m_bPopWarnEnable = false;


	//Task Cfg
	m_pArrTask = new CObArray();
	if (!m_pArrTask)
		return;

	CStdioFile	m_fTaskList;
	CFileException error;
	if ( !m_fTaskList.Open("TaskInfo.txt", CFile::modeNoTruncate | CFile::modeCreate | CFile::modeReadWrite, &error) )
	{
		CString strError;
		strError.Format( "Can't open file %s, error = %u\n", "TaskInfo.txt", error.m_cause );
		AfxMessageBox(strError);	
		return;
	}
	else
	{
		//ULONGLONG nSize = m_fTaskList.GetLength();
		//if (m_fTaskList.GetLength() == 0)
		//	return;

		//Read All Task Info to ArrTask...
		CString sOneTask, sTemp;
		int pos = 0;
		while (m_fTaskList.ReadString(sOneTask))
		{
			CTaskItem *pItem = NULL;
			VERIFY(pItem = new CTaskItem());
			pos = util::split_next(sOneTask, pItem->sName,		'\t', 0);
			pos = util::split_next(sOneTask, pItem->sBTSName,	'\t', pos+1);
			pos = util::split_next(sOneTask, pItem->sUUID,		'\t', pos+1);
			pos = util::split_next(sOneTask, pItem->sCh,		'\t', pos+1);
			pos = util::split_next(sOneTask, sTemp,				'\t', pos+1);
			pItem->nVV = atoi(sTemp);
			pos = util::split_next(sOneTask, pItem->sBeginDate, '\t', pos+1);
			pos = util::split_next(sOneTask, pItem->sEndDate,	'\t', pos+1);
			pos = util::split_next(sOneTask, sTemp,			    '\t', pos+1);
			pItem->nBeginHour = atoi(sTemp);
			pos = util::split_next(sOneTask, sTemp, '\t', pos+1);
			pItem->nBeginMin = atoi(sTemp);
			pos = util::split_next(sOneTask, sTemp, '\t', pos+1);
			pItem->nBeginSec = atoi(sTemp);

			pos = util::split_next(sOneTask, sTemp, '\t', pos+1);
			pItem->nEndHour = atoi(sTemp);
			pos = util::split_next(sOneTask, sTemp, '\t', pos+1);
			pItem->nEndMin = atoi(sTemp);
			pos = util::split_next(sOneTask, sTemp, '\t', pos+1);
			pItem->nEndSec = atoi(sTemp);

			pos = util::split_next(sOneTask, sTemp, '\t', pos+1);
			pItem->status = atoi(sTemp);
			pos = util::split_next(sOneTask, pItem->sUUID_Old,	'\t', pos+1);
			pos = util::split_next(sOneTask, pItem->sCh_Old,	 '\t', pos+1);

			m_pArrTask->Add(pItem);
		}

	}
	m_fTaskList.Close();
	/*
	dwRet = GetPrivateProfileString("SYS_TASK", "count","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_sPopWarnSound = lpDefault;
	dwRet = GetPrivateProfileString("SYS_TASK", "size","",lpDefault,sizeof(lpDefault),pConfigurationFile);
	if (dwRet>0)
		m_sPopWarnSound = lpDefault;

	lpDefault
	dwRet = GetPrivateProfileStruct("SYS_TASK", "taskinfo",lpDefault,nSize,pConfigurationFile);
	if (dwRet>0)
		m_sPopWarnSound = lpDefault;
	*/

	//Control Process Event Init
	m_hCtrlTask[0] = ::CreateEvent(NULL,TRUE,FALSE,NULL);  //manual 
	m_hCtrlTask[1] = ::CreateEvent(NULL,FALSE,FALSE,NULL); //auto Exit
}

CConfigMgr::~CConfigMgr()
{
	//Save Config Data
	util::InitApp();
	char *pConfigurationFile = util::GetAppPath();
	strcat(pConfigurationFile, CONFIG_FILE);

	CString strVal;

	strVal.Format("%d",m_nPicWinIndex);
	WritePrivateProfileString("SYS_CFG", "PicWinIndex",		strVal,	pConfigurationFile);
	
	strVal.Format("%d",m_nPlayPicInterval);
	WritePrivateProfileString("SYS_CFG", "PlayPicInterval",	strVal,	pConfigurationFile);

	strVal.Format("%d",m_nMaxPopWarnWin);
	WritePrivateProfileString("SYS_CFG", "MaxPopWarnWin",	strVal,	pConfigurationFile);

	strVal = m_sPopWarnSound;
	WritePrivateProfileString("SYS_CFG", "PopWarnSound",	strVal,	pConfigurationFile);


	if (m_bPopWarnEnable)
		WritePrivateProfileString("SYS_CFG", "PopWarnEnable",	"1",	pConfigurationFile);
	else
		WritePrivateProfileString("SYS_CFG", "PopWarnEnable",	"0",	pConfigurationFile);

	//Free all Memory m_pArrTask
	CStdioFile	m_fTaskList;
	CFileException error;
	int nTaskCount = m_pArrTask->GetCount();
	int i=0;
	CTaskItem *pObjItem=NULL;
	if ( !m_fTaskList.Open("TaskInfo.txt", CFile::modeCreate | CFile::modeReadWrite, &error) )
	{
		CString strError;
		strError.Format( "Can't open file %s, error = %u\n", "TaskInfo.txt", error.m_cause );
		AfxMessageBox(strError);	
		for (i=0; i<nTaskCount; i++)
		{
			pObjItem = (CTaskItem *)m_pArrTask->GetAt(i);

			m_pArrTask->RemoveAt(i);
			delete pObjItem;
		}

		delete m_pArrTask;
		m_pArrTask = NULL;
	}


	CString sOneTask;
	int nStatus = 0;
	for (i=0; i<nTaskCount; i++)
	{
		pObjItem = (CTaskItem *)m_pArrTask->GetAt(i);
	
		if (!pObjItem) continue;

		//Write Task to File
		if (pObjItem->status == 2)
			nStatus = 1;
		else
			nStatus = pObjItem->status;

		sOneTask.Format("%s\t%s\t%s\t%s\t%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t\r\n",
						pObjItem->sName,
						pObjItem->sBTSName,
						pObjItem->sUUID,
						pObjItem->sCh,
						pObjItem->nVV,
						pObjItem->sBeginDate,
						pObjItem->sEndDate,
						pObjItem->nBeginHour,
						pObjItem->nBeginMin,
						pObjItem->nBeginSec,
						pObjItem->nEndHour,
						pObjItem->nEndMin,
						pObjItem->nEndSec,
						nStatus,//if monitoring ,should be save as waiting..., next restart time, 
						pObjItem->sUUID_Old,
						pObjItem->sCh_Old);


		m_fTaskList.WriteString(sOneTask);

		m_pArrTask->RemoveAt(i);
		delete pObjItem;
		pObjItem = NULL;
	}

	delete m_pArrTask;
	m_pArrTask = NULL;

	//Release Event
	for(int i=0; i<2; i++)
		::CloseHandle(m_hCtrlTask[i]);
}


// CConfigMgr member functions
