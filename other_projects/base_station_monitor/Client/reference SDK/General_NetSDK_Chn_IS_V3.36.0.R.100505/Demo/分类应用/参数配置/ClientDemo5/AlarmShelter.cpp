// AlarmShelter.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "AlarmShelter.h"
#include "ClientDemo5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmShelter dialog


CAlarmShelter::CAlarmShelter(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmShelter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmShelter)
	m_prd1EndHour = 24;
	m_prd1StartHour = 0;
	m_prd1EndMinute = 0;
	m_prd1StartMinute = 0;
	m_prd1EndSecond = 0;
	m_prd1StartSecond = 0;
	m_prd2EndHour = 24;
	m_prd2StartHour = 0;
	m_prd2StartMinute = 0;
	m_prd2EndSecond = 0;
	m_prd3EndHour = 24;
	m_prd3EndMinute = 0;
	m_prd3StartMinute = 0;
	m_prd3EndSecond = 0;
	m_prd3StartSecond = 0;
	m_prd4EndHour = 24;
	m_prd4StartHour = 0;
	m_prd4EndMinute = 0;
	m_prd4StartMinute = 0;
	m_prd4EndSecond = 0;
	m_prd4StartSecond = 0;
	m_prd2EndMinute = 0;
	m_prd5EndHour = 24;
	m_prd5StartHour = 0;
	m_prd5EndMinute = 0;
	m_prd5StartMinute = 0;
	m_prd5EndSecond = 0;
	m_prd5StartSecond = 0;
	m_prd6EndHour = 24;
	m_prd6StartHour = 0;
	m_prd6EndMinute = 0;
	m_prd6StartMinute = 0;
	m_prd6EndSecond = 0;
	m_prd6StartSecond = 0;
	m_prd2StartSecond = 0;
	m_prd3StartHour = 0;
	m_bMatrixEn = FALSE;
	m_bLog = FALSE;
	m_dwMatrixMask = 0;
	m_dwAmount = 0;
	//}}AFX_DATA_INIT
	m_nChannel = 0;
	m_nAlarmOut = 0;
	m_nCurAlarmChannel = -1;
	m_nCurWeek = -1;
	m_bNewProtocol = FALSE;
}


void CAlarmShelter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmShelter)
	DDX_Control(pDX, IDC_CHECK_SPRD6, m_ctlPRD6);
	DDX_Control(pDX, IDC_CHECK_SPRD5, m_ctlPRD5);
	DDX_Control(pDX, IDC_CHECK_SPRD4, m_ctlPRD4);
	DDX_Control(pDX, IDC_CHECK_SPRD3, m_ctlPRD3);
	DDX_Control(pDX, IDC_CHECK_SPRD2, m_ctlPRD2);
	DDX_Control(pDX, IDC_CHECK_SPRD1, m_ctlPRD1);
	DDX_Control(pDX, IDC_CHECK_RECORD, m_chkRecord);
	DDX_Control(pDX, IDC_CHECK_OUTPUT, m_chkOutPut);
	DDX_Control(pDX, IDC_CHECK_LOCALTOUR, m_chkLocalTour);
	DDX_Control(pDX, IDC_CHECK_LOCALTIP, m_chkLocalIP);
	DDX_Control(pDX, IDC_CHECK_SENDMAIL, m_chkSendMail);
	DDX_Control(pDX, IDC_CHECK_UPLOAD, m_chkUpload);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_chkAlarmEn);
	DDX_Control(pDX, IDC_COMBO_ALARMWEEK, m_ctlAlarmWeek);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ctlChannel);
	DDX_Text(pDX, IDC_EDIT_SPRD1_E_HOUR, m_prd1EndHour);
	DDV_MinMaxUInt(pDX, m_prd1EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD1_S_HOUR, m_prd1StartHour);
	DDV_MinMaxUInt(pDX, m_prd1StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD1_E_MINUTE, m_prd1EndMinute);
	DDV_MinMaxUInt(pDX, m_prd1EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD1_S_MINUTE, m_prd1StartMinute);
	DDV_MinMaxUInt(pDX, m_prd1StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD1_E_SEC, m_prd1EndSecond);
	DDV_MinMaxUInt(pDX, m_prd1EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD1_S_SEC, m_prd1StartSecond);
	DDV_MinMaxUInt(pDX, m_prd1StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD2_E_HOUR, m_prd2EndHour);
	DDV_MinMaxUInt(pDX, m_prd2EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD2_S_HOUR, m_prd2StartHour);
	DDV_MinMaxUInt(pDX, m_prd2StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD2_S_MINUTE, m_prd2StartMinute);
	DDV_MinMaxUInt(pDX, m_prd2StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD2_E_SEC, m_prd2EndSecond);
	DDV_MinMaxUInt(pDX, m_prd2EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD3_E_HOUR, m_prd3EndHour);
	DDV_MinMaxUInt(pDX, m_prd3EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD3_E_MINUTE, m_prd3EndMinute);
	DDV_MinMaxUInt(pDX, m_prd3EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD3_S_MINUTE, m_prd3StartMinute);
	DDV_MinMaxUInt(pDX, m_prd3StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD3_E_SEC, m_prd3EndSecond);
	DDV_MinMaxUInt(pDX, m_prd3EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD3_S_SEC, m_prd3StartSecond);
	DDV_MinMaxUInt(pDX, m_prd3StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD4_E_HOUR, m_prd4EndHour);
	DDV_MinMaxUInt(pDX, m_prd4EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD4_S_HOUR, m_prd4StartHour);
	DDV_MinMaxUInt(pDX, m_prd4StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD4_E_MINUTE, m_prd4EndMinute);
	DDV_MinMaxUInt(pDX, m_prd4EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD4_S_MINUTE, m_prd4StartMinute);
	DDV_MinMaxUInt(pDX, m_prd4StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD4_E_SEC, m_prd4EndSecond);
	DDV_MinMaxUInt(pDX, m_prd4EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD4_S_SEC, m_prd4StartSecond);
	DDV_MinMaxUInt(pDX, m_prd4StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD2_E_MINUTE, m_prd2EndMinute);
	DDV_MinMaxUInt(pDX, m_prd2EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD5_E_HOUR, m_prd5EndHour);
	DDV_MinMaxUInt(pDX, m_prd5EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD5_S_HOUR, m_prd5StartHour);
	DDV_MinMaxUInt(pDX, m_prd5StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD5_E_MINUTE, m_prd5EndMinute);
	DDV_MinMaxUInt(pDX, m_prd5EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD5_S_MINUTE, m_prd5StartMinute);
	DDV_MinMaxUInt(pDX, m_prd5StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD5_E_SEC, m_prd5EndSecond);
	DDV_MinMaxUInt(pDX, m_prd5EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD5_S_SEC, m_prd5StartSecond);
	DDV_MinMaxUInt(pDX, m_prd5StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD6_E_HOUR, m_prd6EndHour);
	DDV_MinMaxUInt(pDX, m_prd6EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD6_S_HOUR, m_prd6StartHour);
	DDV_MinMaxUInt(pDX, m_prd6StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_SPRD6_E_MINUTE, m_prd6EndMinute);
	DDV_MinMaxUInt(pDX, m_prd6EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD6_S_MINUTE, m_prd6StartMinute);
	DDV_MinMaxUInt(pDX, m_prd6StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD6_E_SEC, m_prd6EndSecond);
	DDV_MinMaxUInt(pDX, m_prd6EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD6_S_SEC, m_prd6StartSecond);
	DDV_MinMaxUInt(pDX, m_prd6StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD2_S_SEC, m_prd2StartSecond);
	DDV_MinMaxUInt(pDX, m_prd2StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SPRD3_S_HOUR, m_prd3StartHour);
	DDV_MinMaxUInt(pDX, m_prd3StartHour, 0, 24);
	DDX_Check(pDX, IDC_CHECK_MATRIX_EN, m_bMatrixEn);
	DDX_Check(pDX, IDC_CHECK_BLOG, m_bLog);
	DDX_Text(pDX, IDC_EDIT_MATRIX_MASK, m_dwMatrixMask);
	DDV_MinMaxUInt(pDX, m_dwMatrixMask, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_AMOUNT2, m_dwAmount);
	DDV_MinMaxUInt(pDX, m_dwAmount, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmShelter, CDialog)
	//{{AFX_MSG_MAP(CAlarmShelter)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, OnSelchangeComboChannel)
	ON_CBN_SELCHANGE(IDC_COMBO_ALARMWEEK, OnSelchangeComboAlarmweek)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmShelter message handlers

/*
 *	Function£ºDisplay camera masking alarm information
 *	Input parameter£º
 *	pAlarmInfo£ºAlarm configuration structure 
 *	nAlarmIn£ºAlarm input channel amount
 *	nAlarmOut£ºAlarm output channel amount
 *	nChannel£ºChannel amount
 */
void CAlarmShelter::InitDlgInfo(DHDEV_ALARM_SCHEDULE_EX *pAlarmInfo, BYTE nAlarmIn, BYTE nAlarmOut, BYTE nChannel, BOOL bNewProtocol) 
{
	if(pAlarmInfo != NULL)
	{
		int i =0;
		m_nAlarmOut = nAlarmOut;
		m_nChannel = nChannel;
		m_bNewProtocol = bNewProtocol;
		
		for(i=0;i<DH_MAX_VIDEO_IN_NUM_EX;i++)
		{
			m_AlarmShelterInfo[i] = pAlarmInfo->struBlind[i];
		}
		
		
		//Set channel dropdowm menu 
		m_ctlChannel.ResetContent();
		CString strAlarm;
		int nIndex = 0;
		for (i = 0; i < nChannel; i++)
		{
			strAlarm.Format(ConvertString("Channel %d"), i+1);
			nIndex = m_ctlChannel.AddString(strAlarm);
			m_ctlChannel.SetItemData(nIndex,i);
		}
		
		//Alarm output 
		for(i=0;i<nAlarmOut;i++)
		{
			GetDlgItem(IDC_CHECK_ALMOP1 + i)->ShowWindow(SW_SHOW);
		}
		for(i=nAlarmOut;i<32;i++)
		{
			GetDlgItem(IDC_CHECK_ALMOP1 + i)->ShowWindow(SW_HIDE);
		}
		
		//Activation record 
		for(i=0;i<nChannel;i++)
		{
			GetDlgItem(IDC_CHECK_RECCH1 + i)->ShowWindow(SW_SHOW);
		}
		for(i=nChannel;i<32;i++)
		{
			GetDlgItem(IDC_CHECK_RECCH1 + i)->ShowWindow(SW_HIDE);
		}
		
		if (m_ctlChannel.GetCount() > 0)
		{
			m_ctlChannel.SetCurSel(0);
			OnSelchangeComboChannel();		
		}
	}
}

//Clear information
void CAlarmShelter::CleanAll()
{
	m_ctlChannel.ResetContent();
	m_chkAlarmEn.SetCheck(0);
	SetDlgItemText(IDC_EDIT_LEVEL,"");
	m_chkUpload.SetCheck(0);
	m_chkLocalIP.SetCheck(0);
	m_chkLocalTour.SetCheck(0);
	m_chkSendMail.SetCheck(0);
	m_chkRecord.SetCheck(0);
	m_chkOutPut.SetCheck(0);
	
	int i=0;
	for (i = 0; i < m_nAlarmOut; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ALMOP1+i))->SetCheck(0);
	}
	for (i = 0; i < m_nChannel; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_RECCH1+i))->SetCheck(0);
	}

	for(i = 0; i < 32; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ALMOP1+i))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_CHECK_RECCH1+i))->ShowWindow(SW_SHOW);
	}
	
	m_ctlAlarmWeek.SetCurSel(-1);
	for(i=0;i<6;i++)
	{
		DH_TSECT periodInfo = {0};
		periodInfo.iEndHour = 24;
		ShowPeriodInfo(&periodInfo,i+1);
	}
	
	m_nChannel = 0;
	m_nAlarmOut = 0;
	m_nCurAlarmChannel = -1;
	m_nCurWeek = -1;
}

//Save
BOOL CAlarmShelter::SaveAll()
{
	BOOL bSuccess = FALSE;
	if(-1 != m_ctlChannel.GetCurSel())
	{
		bSuccess = SaveAlarmChannelInfo(m_nCurAlarmChannel);
	}
	return bSuccess;
}

//Channel option switch 
void CAlarmShelter::OnSelchangeComboChannel() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = (int)m_ctlChannel.GetItemData(nIndex);
		
		//First save channel information 
		SaveAlarmChannelInfo(m_nCurAlarmChannel);

		if (m_bNewProtocol == TRUE)
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->GetBlindAlarmConfigInfo(&m_AlarmShelterInfo[nNum], nIndex);
		}
		
		
		//Display channel corresponding information 
		m_chkAlarmEn.SetCheck((m_AlarmShelterInfo[nNum].byBlindEnable)?1:0);
		SetDlgItemInt(IDC_EDIT_LEVEL,m_AlarmShelterInfo[nNum].byBlindLevel);
		
		DWORD dwActMask = m_AlarmShelterInfo[nNum].struHandle.dwActionMask;
		GetDlgItem(IDC_CHECK_UPLOAD)->EnableWindow((dwActMask & DH_ALARM_UPLOAD)?TRUE:FALSE);
		GetDlgItem(IDC_CHECK_LOCALTIP)->EnableWindow((dwActMask & DH_ALARM_TIP)?TRUE:FALSE);
		GetDlgItem(IDC_CHECK_LOCALTOUR)->EnableWindow((dwActMask & DH_ALARM_TOUR)?TRUE:FALSE);
		GetDlgItem(IDC_CHECK_SENDMAIL)->EnableWindow((dwActMask & DH_ALARM_MAIL)?TRUE:FALSE);
		
		m_chkUpload.SetCheck((m_AlarmShelterInfo[nNum].struHandle.dwActionFlag&DH_ALARM_UPLOAD)?1:0);
		m_chkLocalIP.SetCheck((m_AlarmShelterInfo[nNum].struHandle.dwActionFlag&DH_ALARM_TIP)?1:0);
		m_chkLocalTour.SetCheck((m_AlarmShelterInfo[nNum].struHandle.dwActionFlag&DH_ALARM_TOUR)?1:0);
		m_chkSendMail.SetCheck((m_AlarmShelterInfo[nNum].struHandle.dwActionFlag&DH_ALARM_MAIL)?1:0);
		
		m_chkRecord.SetCheck((m_AlarmShelterInfo[nNum].struHandle.dwActionFlag&DH_ALARM_RECORD)?1:0);
		m_chkOutPut.SetCheck((m_AlarmShelterInfo[nNum].struHandle.dwActionFlag&DH_ALARM_OUT)?1:0);
		
		int i=0;
		for (i = 0; i < m_nAlarmOut; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ALMOP1+i))->SetCheck(m_AlarmShelterInfo[nNum].struHandle.byRelAlarmOut[i]?1:0);
		}
		for (i = 0; i < m_nChannel; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_RECCH1+i))->SetCheck(m_AlarmShelterInfo[nNum].struHandle.byRecordChannel[i]?1:0);
		}
		
		//Time period 
		if (m_ctlAlarmWeek.GetCount() > 0)
		{
			m_ctlAlarmWeek.SetCurSel(0);
			OnSelchangeComboAlarmweek();
		}
		m_dwMatrixMask = (unsigned short)m_AlarmShelterInfo[nNum].struHandle.dwMatrix;
		m_dwAmount = m_AlarmShelterInfo[nNum].struHandle.bySnapshotTimes;
		m_bLog = m_AlarmShelterInfo[nNum].struHandle.bLog;
		m_bMatrixEn = m_AlarmShelterInfo[nNum].struHandle.bMatrixEn;
		
		UpdateData(FALSE);
		
		m_nCurAlarmChannel = nNum;
	}
}

//Period switch 
void CAlarmShelter::OnSelchangeComboAlarmweek() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = m_ctlChannel.GetItemData(nIndex);
		nIndex = m_ctlAlarmWeek.GetCurSel();
		if(CB_ERR != nIndex)
		{
			//First save week period information 
			SaveWeekInfo(m_nCurAlarmChannel,m_nCurWeek);
			//Show week period information 
			for(int i=0;i<6;i++)
			{
				DH_TSECT periodInfo = m_AlarmShelterInfo[nNum].stSect[nIndex][i];
				ShowPeriodInfo(&periodInfo,i+1);
			}
			
			m_nCurWeek = nIndex;
		}
	}
}

//Save channel information
BOOL CAlarmShelter::SaveAlarmChannelInfo(int nNum)
{
	BOOL bValid = TRUE;
	if(-1 < nNum && -1 < m_nCurWeek)
	{
		DH_BLIND_CFG_EX *pInfo = &(m_AlarmShelterInfo[nNum]);
		if(NULL != pInfo)
		{
			pInfo->byBlindEnable = m_chkAlarmEn.GetCheck();
			pInfo->byBlindLevel = GetDlgItemInt(IDC_EDIT_LEVEL);
			
			if(1 == m_chkUpload.GetCheck())
			{
				pInfo->struHandle.dwActionFlag |= DH_ALARM_UPLOAD;
			}
			else
			{
				pInfo->struHandle.dwActionFlag &= (~DH_ALARM_UPLOAD);
			}
			
			if(1 == m_chkLocalIP.GetCheck())
			{
				pInfo->struHandle.dwActionFlag |= DH_ALARM_TIP;
			}
			else
			{
				pInfo->struHandle.dwActionFlag &= (~DH_ALARM_TIP);
			}
			
			if(1 == m_chkLocalTour.GetCheck())
			{
				pInfo->struHandle.dwActionFlag |= DH_ALARM_TOUR;
			}
			else
			{
				pInfo->struHandle.dwActionFlag &= (~DH_ALARM_TOUR);
			}
			
			if(1 == m_chkSendMail.GetCheck())
			{
				pInfo->struHandle.dwActionFlag |= DH_ALARM_MAIL;
			}
			else
			{
				pInfo->struHandle.dwActionFlag &= (~DH_ALARM_MAIL);
			}
			
			if(1 == m_chkRecord.GetCheck())
			{
				pInfo->struHandle.dwActionFlag |= DH_ALARM_RECORD;
			}
			else
			{
				pInfo->struHandle.dwActionFlag &= (~DH_ALARM_RECORD);
			}
			
			if(1 == m_chkOutPut.GetCheck())
			{
				pInfo->struHandle.dwActionFlag |= DH_ALARM_OUT;
			}
			else
			{
				pInfo->struHandle.dwActionFlag &= (~DH_ALARM_OUT);
			}
			
			int i=0;
			for (i = 0; i < m_nAlarmOut; i++)
			{
				pInfo->struHandle.byRelAlarmOut[i] = ((CButton*)GetDlgItem(IDC_CHECK_ALMOP1+i))->GetCheck();
			}
			for (i = 0; i < m_nChannel; i++)
			{
				pInfo->struHandle.byRecordChannel[i] = ((CButton*)GetDlgItem(IDC_CHECK_RECCH1+i))->GetCheck();
			}
			
			//add by Nick
			UpdateData(TRUE);
			pInfo->struHandle.dwMatrix = m_dwMatrixMask;
			pInfo->struHandle.bySnapshotTimes = m_dwAmount;
			pInfo->struHandle.bLog = m_bLog;
			pInfo->struHandle.bMatrixEn = m_bMatrixEn;

			//Save week period information 
			bValid = SaveWeekInfo(nNum,m_nCurWeek);
		}
	}
	return bValid;
}

//Save week period information
BOOL CAlarmShelter::SaveWeekInfo(int nNum, int nWeek)
{
	BOOL bValid = TRUE;
	if(-1 < nNum && -1 < nWeek)
	{
		if(UpdateData(TRUE))
		{
			DH_TSECT *pPeriodInfo = &(m_AlarmShelterInfo[nNum].stSect[nWeek][0]);
			pPeriodInfo->bEnable = m_ctlPRD1.GetCheck();
			pPeriodInfo->iBeginHour = m_prd1StartHour;
			pPeriodInfo->iBeginMin = m_prd1StartMinute;
			pPeriodInfo->iBeginSec = m_prd1StartSecond;
			pPeriodInfo->iEndHour = m_prd1EndHour;
			pPeriodInfo->iEndMin = m_prd1EndMinute;
			pPeriodInfo->iEndSec = m_prd1EndSecond;
			
			pPeriodInfo = &(m_AlarmShelterInfo[nNum].stSect[nWeek][1]);
			pPeriodInfo->bEnable = m_ctlPRD2.GetCheck();
			pPeriodInfo->iBeginHour = m_prd2StartHour;
			pPeriodInfo->iBeginMin = m_prd2StartMinute;
			pPeriodInfo->iBeginSec = m_prd2StartSecond;
			pPeriodInfo->iEndHour = m_prd2EndHour;
			pPeriodInfo->iEndMin = m_prd2EndMinute;
			pPeriodInfo->iEndSec = m_prd2EndSecond;
			
			pPeriodInfo = &(m_AlarmShelterInfo[nNum].stSect[nWeek][2]);
			pPeriodInfo->bEnable = m_ctlPRD3.GetCheck();
			pPeriodInfo->iBeginHour = m_prd3StartHour;
			pPeriodInfo->iBeginMin = m_prd3StartMinute;
			pPeriodInfo->iBeginSec = m_prd3StartSecond;
			pPeriodInfo->iEndHour = m_prd3EndHour;
			pPeriodInfo->iEndMin = m_prd3EndMinute;
			pPeriodInfo->iEndSec = m_prd3EndSecond;
			
			pPeriodInfo = &(m_AlarmShelterInfo[nNum].stSect[nWeek][3]);
			pPeriodInfo->bEnable = m_ctlPRD4.GetCheck();
			pPeriodInfo->iBeginHour = m_prd4StartHour;
			pPeriodInfo->iBeginMin = m_prd4StartMinute;
			pPeriodInfo->iBeginSec = m_prd4StartSecond;
			pPeriodInfo->iEndHour = m_prd4EndHour;
			pPeriodInfo->iEndMin = m_prd4EndMinute;
			pPeriodInfo->iEndSec = m_prd4EndSecond;
			
			pPeriodInfo = &(m_AlarmShelterInfo[nNum].stSect[nWeek][4]);
			pPeriodInfo->bEnable = m_ctlPRD5.GetCheck();
			pPeriodInfo->iBeginHour = m_prd5StartHour;
			pPeriodInfo->iBeginMin = m_prd5StartMinute;
			pPeriodInfo->iBeginSec = m_prd5StartSecond;
			pPeriodInfo->iEndHour = m_prd5EndHour;
			pPeriodInfo->iEndMin = m_prd5EndMinute;
			pPeriodInfo->iEndSec = m_prd5EndSecond;
			
			pPeriodInfo = &(m_AlarmShelterInfo[nNum].stSect[nWeek][5]);
			pPeriodInfo->bEnable = m_ctlPRD6.GetCheck();
			pPeriodInfo->iBeginHour = m_prd6StartHour;
			pPeriodInfo->iBeginMin = m_prd6StartMinute;
			pPeriodInfo->iBeginSec = m_prd6StartSecond;
			pPeriodInfo->iEndHour = m_prd6EndHour;
			pPeriodInfo->iEndMin = m_prd6EndMinute;
			pPeriodInfo->iEndSec = m_prd6EndSecond;
		}
		else
		{
			bValid = FALSE;
		}
	}
	return bValid;
}

//Display period informaiton
void CAlarmShelter::ShowPeriodInfo(DH_TSECT *pTSect, BYTE nNum)
{
	switch(nNum) {
	case 1:
		m_ctlPRD1.SetCheck((pTSect->bEnable)?1:0);
		m_prd1StartHour = pTSect->iBeginHour;
		m_prd1StartMinute = pTSect->iBeginMin;
		m_prd1StartSecond = pTSect->iBeginSec;
		m_prd1EndHour = pTSect->iEndHour;
		m_prd1EndMinute = pTSect->iEndMin;
		m_prd1EndSecond = pTSect->iEndSec;
		break;
	case 2:
		m_ctlPRD2.SetCheck((pTSect->bEnable)?1:0);
		m_prd2StartHour = pTSect->iBeginHour;
		m_prd2StartMinute = pTSect->iBeginMin;
		m_prd2StartSecond = pTSect->iBeginSec;
		m_prd2EndHour = pTSect->iEndHour;
		m_prd2EndMinute = pTSect->iEndMin;
		m_prd2EndSecond = pTSect->iEndSec;
		break;
	case 3:
		m_ctlPRD3.SetCheck((pTSect->bEnable)?1:0);
		m_prd3StartHour = pTSect->iBeginHour;
		m_prd3StartMinute = pTSect->iBeginMin;
		m_prd3StartSecond = pTSect->iBeginSec;
		m_prd3EndHour = pTSect->iEndHour;
		m_prd3EndMinute = pTSect->iEndMin;
		m_prd3EndSecond = pTSect->iEndSec;
		break;
	case 4:
		m_ctlPRD4.SetCheck((pTSect->bEnable)?1:0);
		m_prd4StartHour = pTSect->iBeginHour;
		m_prd4StartMinute = pTSect->iBeginMin;
		m_prd4StartSecond = pTSect->iBeginSec;
		m_prd4EndHour = pTSect->iEndHour;
		m_prd4EndMinute = pTSect->iEndMin;
		m_prd4EndSecond = pTSect->iEndSec;
		break;
	case 5:
		m_ctlPRD5.SetCheck((pTSect->bEnable)?1:0);
		m_prd5StartHour = pTSect->iBeginHour;
		m_prd5StartMinute = pTSect->iBeginMin;
		m_prd5StartSecond = pTSect->iBeginSec;
		m_prd5EndHour = pTSect->iEndHour;
		m_prd5EndMinute = pTSect->iEndMin;
		m_prd5EndSecond = pTSect->iEndSec;
		break;
	case 6:
		m_ctlPRD6.SetCheck((pTSect->bEnable)?1:0);
		m_prd6StartHour = pTSect->iBeginHour;
		m_prd6StartMinute = pTSect->iBeginMin;
		m_prd6StartSecond = pTSect->iBeginSec;
		m_prd6EndHour = pTSect->iEndHour;
		m_prd6EndMinute = pTSect->iEndMin;
		m_prd6EndSecond = pTSect->iEndSec;
		break;
	default:
		break;
	}
	UpdateData(FALSE);
}

BOOL CAlarmShelter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_SetWndStaticText(this);
	ConvertComboBox(m_ctlAlarmWeek);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarmShelter::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CAlarmShelter::UpdateSchedule()
{
	UpdateData(TRUE);
	int nIndex = m_ctlChannel.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = m_ctlChannel.GetItemData(nIndex);
		nIndex = m_ctlAlarmWeek.GetCurSel();
		if(CB_ERR != nIndex)
		{
			//Show week period information 
			for (int i = 0; i < 6; i++)
			{
				DH_TSECT periodInfo = m_AlarmShelterInfo[nNum].stSect[nIndex][i];
				ShowPeriodInfo(&periodInfo,i+1);
			}
			
			m_nCurWeek = nIndex;
		}
	}
}








































































































































































































































































































































































































































































































































