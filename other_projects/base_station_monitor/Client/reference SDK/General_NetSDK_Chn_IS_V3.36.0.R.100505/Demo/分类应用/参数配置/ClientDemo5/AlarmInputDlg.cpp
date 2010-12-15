// AlarmInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "AlarmInputDlg.h"
#include "ClientDemo5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmInputDlg dialog


CAlarmInputDlg::CAlarmInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmInputDlg)
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
	m_dwMatrixMask = 0;
	m_dwAmount = 0;
	m_bMatrixEn = FALSE;
	m_bLog = FALSE;
	//}}AFX_DATA_INIT
	m_nCurAlarmChannel = -1;
	m_nCurWeek = -1;
	m_nAlarmOut = 0;
	m_nChannel = 0;
	m_bNewProtocol = FALSE;
}


void CAlarmInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmInputDlg)
	DDX_Control(pDX, IDC_CHECK_PRD6, m_ctlPRD6);
	DDX_Control(pDX, IDC_CHECK_PRD5, m_ctlPRD5);
	DDX_Control(pDX, IDC_CHECK_PRD4, m_ctlPRD4);
	DDX_Control(pDX, IDC_CHECK_PRD3, m_ctlPRD3);
	DDX_Control(pDX, IDC_CHECK_PRD2, m_ctlPRD2);
	DDX_Control(pDX, IDC_CHECK_PRD1, m_ctlPRD1);
	DDX_Control(pDX, IDC_CHECK_ALARMENABLE, m_ctlAlarmEn);
	DDX_Control(pDX, IDC_CHECK_UPLOAD, m_chkUpload);
	DDX_Control(pDX, IDC_CHECK_SENDMAIL, m_chkSendMail);
	DDX_Control(pDX, IDC_CHECK_RECORD, m_chkRecord);
	DDX_Control(pDX, IDC_CHECK_OUTPUT, m_chkOutPut);
	DDX_Control(pDX, IDC_CHECK_LOCALTOUR, m_chkLocalTour);
	DDX_Control(pDX, IDC_CHECK_LOCALTIP, m_chkLocalIP);
	DDX_Control(pDX, IDC_COMBO_ALARMWEEK, m_ctlAlarmWeek);
	DDX_Control(pDX, IDC_COMBO_ALARMTYPE, m_ctlAlarmType);
	DDX_Control(pDX, IDC_COMBO_ALARMIN, m_ctlAlarmIn);
	DDX_Text(pDX, IDC_EDIT_PRD1_E_HOUR, m_prd1EndHour);
	DDV_MinMaxUInt(pDX, m_prd1EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD1_S_HOUR, m_prd1StartHour);
	DDV_MinMaxUInt(pDX, m_prd1StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD1_E_MINUTE, m_prd1EndMinute);
	DDV_MinMaxUInt(pDX, m_prd1EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD1_S_MINUTE, m_prd1StartMinute);
	DDV_MinMaxUInt(pDX, m_prd1StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD1_E_SEC, m_prd1EndSecond);
	DDV_MinMaxUInt(pDX, m_prd1EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD1_S_SEC, m_prd1StartSecond);
	DDV_MinMaxUInt(pDX, m_prd1StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD2_E_HOUR, m_prd2EndHour);
	DDV_MinMaxUInt(pDX, m_prd2EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD2_S_HOUR, m_prd2StartHour);
	DDV_MinMaxUInt(pDX, m_prd2StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD2_S_MINUTE, m_prd2StartMinute);
	DDV_MinMaxUInt(pDX, m_prd2StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD2_E_SEC, m_prd2EndSecond);
	DDV_MinMaxUInt(pDX, m_prd2EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD3_E_HOUR, m_prd3EndHour);
	DDV_MinMaxUInt(pDX, m_prd3EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD3_E_MINUTE, m_prd3EndMinute);
	DDV_MinMaxUInt(pDX, m_prd3EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD3_S_MINUTE, m_prd3StartMinute);
	DDV_MinMaxUInt(pDX, m_prd3StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD3_E_SEC, m_prd3EndSecond);
	DDV_MinMaxUInt(pDX, m_prd3EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD3_S_SEC, m_prd3StartSecond);
	DDV_MinMaxUInt(pDX, m_prd3StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD4_E_HOUR, m_prd4EndHour);
	DDV_MinMaxUInt(pDX, m_prd4EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD4_S_HOUR, m_prd4StartHour);
	DDV_MinMaxUInt(pDX, m_prd4StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD4_E_MINUTE, m_prd4EndMinute);
	DDV_MinMaxUInt(pDX, m_prd4EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD4_S_MINUTE, m_prd4StartMinute);
	DDV_MinMaxUInt(pDX, m_prd4StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD4_E_SEC, m_prd4EndSecond);
	DDV_MinMaxUInt(pDX, m_prd4EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD4_S_SEC, m_prd4StartSecond);
	DDV_MinMaxUInt(pDX, m_prd4StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD2_E_MINUTE, m_prd2EndMinute);
	DDV_MinMaxUInt(pDX, m_prd2EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD5_E_HOUR, m_prd5EndHour);
	DDV_MinMaxUInt(pDX, m_prd5EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD5_S_HOUR, m_prd5StartHour);
	DDV_MinMaxUInt(pDX, m_prd5StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD5_E_MINUTE, m_prd5EndMinute);
	DDV_MinMaxUInt(pDX, m_prd5EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD5_S_MINUTE, m_prd5StartMinute);
	DDV_MinMaxUInt(pDX, m_prd5StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD5_E_SEC, m_prd5EndSecond);
	DDV_MinMaxUInt(pDX, m_prd5EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD5_S_SEC, m_prd5StartSecond);
	DDV_MinMaxUInt(pDX, m_prd5StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD6_E_HOUR, m_prd6EndHour);
	DDV_MinMaxUInt(pDX, m_prd6EndHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD6_S_HOUR, m_prd6StartHour);
	DDV_MinMaxUInt(pDX, m_prd6StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_PRD6_E_MINUTE, m_prd6EndMinute);
	DDV_MinMaxUInt(pDX, m_prd6EndMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD6_S_MINUTE, m_prd6StartMinute);
	DDV_MinMaxUInt(pDX, m_prd6StartMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD6_E_SEC, m_prd6EndSecond);
	DDV_MinMaxUInt(pDX, m_prd6EndSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD6_S_SEC, m_prd6StartSecond);
	DDV_MinMaxUInt(pDX, m_prd6StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD2_S_SEC, m_prd2StartSecond);
	DDV_MinMaxUInt(pDX, m_prd2StartSecond, 0, 59);
	DDX_Text(pDX, IDC_EDIT_PRD3_S_HOUR, m_prd3StartHour);
	DDV_MinMaxUInt(pDX, m_prd3StartHour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_MATRIX_MASK, m_dwMatrixMask);
	DDV_MinMaxUInt(pDX, m_dwMatrixMask, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, m_dwAmount);
	DDV_MinMaxUInt(pDX, m_dwAmount, 0, 255);
	DDX_Check(pDX, IDC_CHECK_MATRIX_EN, m_bMatrixEn);
	DDX_Check(pDX, IDC_CHECK_BLOG, m_bLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmInputDlg, CDialog)
	//{{AFX_MSG_MAP(CAlarmInputDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ALARMIN, OnSelchangeComboAlarmin)
	ON_CBN_SELCHANGE(IDC_COMBO_ALARMWEEK, OnSelchangeComboAlarmweek)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAlarmInputDlg::InitDlgInfo(DHDEV_ALARM_SCHEDULE_EX *pAlarmInfo, BYTE nAlarmIn, BYTE nAlarmOut, BYTE nChannel, BOOL bNewProtocol)
{
	if(pAlarmInfo != NULL)
	{
		int i = 0;
		m_nAlarmOut = nAlarmOut;
		m_nChannel = nChannel;
		m_bNewProtocol = bNewProtocol;
		
		for(i = 0; i<DH_MAX_ALARM_IN_NUM_EX; i++)
		{
			m_AlarmInInfo[i] = pAlarmInfo->struLocalAlmIn[i];
		}	
		
		//Set alarm input dropdown menu 
		m_ctlAlarmIn.ResetContent();
		CString strAlarmIn;
		int nIndex = 0;
		for (i = 0; i < nAlarmIn; i++)
		{
			strAlarmIn.Format(ConvertString("Alarm Input %d"), i+1);
			nIndex = m_ctlAlarmIn.AddString(strAlarmIn);
			m_ctlAlarmIn.SetItemData(nIndex,i);
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
		
		if (m_ctlAlarmIn.GetCount() > 0)
		{
			m_ctlAlarmIn.SetCurSel(0);	
			OnSelchangeComboAlarmin();		
		}
	}
}


//Clear
void CAlarmInputDlg::CleanAll()
{
	m_ctlAlarmIn.ResetContent();
	m_ctlAlarmEn.SetCheck(0);
	m_ctlAlarmType.SetCurSel(-1);
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

	m_nCurAlarmChannel = -1;
	m_nCurWeek = -1;
	m_nAlarmOut = 0;
	m_nChannel = 0;
}

//Save
BOOL CAlarmInputDlg::SaveAll()
{
	BOOL bSuccess = FALSE;
	if(-1 != m_ctlAlarmIn.GetCurSel())
	{
		bSuccess = SaveAlarmChannelInfo(m_nCurAlarmChannel);
	}
	return TRUE;//bSuccess;
}

//Alarm input option switch 
void CAlarmInputDlg::OnSelchangeComboAlarmin() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlAlarmIn.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = (int)m_ctlAlarmIn.GetItemData(nIndex);
		
		//First save alarm input channel information
		SaveAlarmChannelInfo(m_nCurAlarmChannel);

		if (m_bNewProtocol == TRUE)
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->GetAlarmInConfigInfo(&m_AlarmInInfo[nNum],nIndex);
		}
	
		//Display alarm input channel corresponding information 
		m_ctlAlarmEn.SetCheck(m_AlarmInInfo[nNum].byAlarmEn?1:0);
		m_ctlAlarmType.SetCurSel(m_AlarmInInfo[nNum].byAlarmType);
		
		DWORD dwActMask = m_AlarmInInfo[nNum].struHandle.dwActionMask;
		GetDlgItem(IDC_CHECK_UPLOAD)->EnableWindow((dwActMask & DH_ALARM_UPLOAD)?TRUE:FALSE);
		GetDlgItem(IDC_CHECK_LOCALTIP)->EnableWindow((dwActMask & DH_ALARM_TIP)?TRUE:FALSE);
		GetDlgItem(IDC_CHECK_LOCALTOUR)->EnableWindow((dwActMask & DH_ALARM_TOUR)?TRUE:FALSE);
		GetDlgItem(IDC_CHECK_SENDMAIL)->EnableWindow((dwActMask & DH_ALARM_MAIL)?TRUE:FALSE);
		
		m_chkUpload.SetCheck((m_AlarmInInfo[nNum].struHandle.dwActionFlag&DH_ALARM_UPLOAD)?1:0);
		m_chkLocalIP.SetCheck((m_AlarmInInfo[nNum].struHandle.dwActionFlag&DH_ALARM_TIP)?1:0);
		m_chkLocalTour.SetCheck((m_AlarmInInfo[nNum].struHandle.dwActionFlag&DH_ALARM_TOUR)?1:0);
		m_chkSendMail.SetCheck((m_AlarmInInfo[nNum].struHandle.dwActionFlag&DH_ALARM_MAIL)?1:0);

		m_chkRecord.SetCheck((m_AlarmInInfo[nNum].struHandle.dwActionFlag&DH_ALARM_RECORD)?1:0);
		m_chkOutPut.SetCheck((m_AlarmInInfo[nNum].struHandle.dwActionFlag&DH_ALARM_OUT)?1:0);

		int i=0;
		for (i = 0; i < m_nAlarmOut; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ALMOP1+i))->SetCheck(m_AlarmInInfo[nNum].struHandle.byRelAlarmOut[i]?1:0);
		}
		for (i = 0; i < m_nChannel; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_RECCH1+i))->SetCheck(m_AlarmInInfo[nNum].struHandle.byRecordChannel[i]?1:0);
		}
		
		//Time period
		if (m_ctlAlarmWeek.GetCount() > 0)
		{
			m_ctlAlarmWeek.SetCurSel(0);
			OnSelchangeComboAlarmweek();
		}

		m_dwMatrixMask = (unsigned short)m_AlarmInInfo[nNum].struHandle.dwMatrix;
		m_dwAmount = m_AlarmInInfo[nNum].struHandle.bySnapshotTimes;
		m_bLog = m_AlarmInInfo[nNum].struHandle.bLog;
		m_bMatrixEn = m_AlarmInInfo[nNum].struHandle.bMatrixEn;

		UpdateData(FALSE);
		
		m_nCurAlarmChannel = nNum;
	}
}

//Period switch 
void CAlarmInputDlg::OnSelchangeComboAlarmweek() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlAlarmIn.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = m_ctlAlarmIn.GetItemData(nIndex);
		nIndex = m_ctlAlarmWeek.GetCurSel();
		if(CB_ERR != nIndex)
		{
			//First save week period information
			SaveWeekInfo(m_nCurAlarmChannel,m_nCurWeek);
			//Display week period information
			for(int i=0;i<6;i++)
			{
				DH_TSECT periodInfo = m_AlarmInInfo[nNum].stSect[nIndex][i];
				ShowPeriodInfo(&periodInfo,i+1);
			}
			
			m_nCurWeek = nIndex;
		}
	}
}

//Save alarm channel information 
BOOL CAlarmInputDlg::SaveAlarmChannelInfo(int nNum)
{
	BOOL bValid = TRUE;
	if(-1 < nNum && -1 < m_nCurWeek)
	{
		DH_ALARMIN_CFG_EX *pInfo = &(m_AlarmInInfo[nNum]);
		if(NULL != pInfo)
		{
			pInfo->byAlarmEn = m_ctlAlarmEn.GetCheck();
			pInfo->byAlarmType = m_ctlAlarmType.GetCurSel();

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
BOOL CAlarmInputDlg::SaveWeekInfo(int nNum, int nWeek)
{
	BOOL bValid = TRUE;
	if(-1 < nNum && -1 < nWeek)
	{
		if(UpdateData(TRUE))
		{
			DH_TSECT *pPeriodInfo = &(m_AlarmInInfo[nNum].stSect[nWeek][0]);
			pPeriodInfo->bEnable = m_ctlPRD1.GetCheck();
			pPeriodInfo->iBeginHour = m_prd1StartHour;
			pPeriodInfo->iBeginMin = m_prd1StartMinute;
			pPeriodInfo->iBeginSec = m_prd1StartSecond;
			pPeriodInfo->iEndHour = m_prd1EndHour;
			pPeriodInfo->iEndMin = m_prd1EndMinute;
			pPeriodInfo->iEndSec = m_prd1EndSecond;
			
			pPeriodInfo = &(m_AlarmInInfo[nNum].stSect[nWeek][1]);
			pPeriodInfo->bEnable = m_ctlPRD2.GetCheck();
			pPeriodInfo->iBeginHour = m_prd2StartHour;
			pPeriodInfo->iBeginMin = m_prd2StartMinute;
			pPeriodInfo->iBeginSec = m_prd2StartSecond;
			pPeriodInfo->iEndHour = m_prd2EndHour;
			pPeriodInfo->iEndMin = m_prd2EndMinute;
			pPeriodInfo->iEndSec = m_prd2EndSecond;
			
			pPeriodInfo = &(m_AlarmInInfo[nNum].stSect[nWeek][2]);
			pPeriodInfo->bEnable = m_ctlPRD3.GetCheck();
			pPeriodInfo->iBeginHour = m_prd3StartHour;
			pPeriodInfo->iBeginMin = m_prd3StartMinute;
			pPeriodInfo->iBeginSec = m_prd3StartSecond;
			pPeriodInfo->iEndHour = m_prd3EndHour;
			pPeriodInfo->iEndMin = m_prd3EndMinute;
			pPeriodInfo->iEndSec = m_prd3EndSecond;
			
			pPeriodInfo = &(m_AlarmInInfo[nNum].stSect[nWeek][3]);
			pPeriodInfo->bEnable = m_ctlPRD4.GetCheck();
			pPeriodInfo->iBeginHour = m_prd4StartHour;
			pPeriodInfo->iBeginMin = m_prd4StartMinute;
			pPeriodInfo->iBeginSec = m_prd4StartSecond;
			pPeriodInfo->iEndHour = m_prd4EndHour;
			pPeriodInfo->iEndMin = m_prd4EndMinute;
			pPeriodInfo->iEndSec = m_prd4EndSecond;
			
			pPeriodInfo = &(m_AlarmInInfo[nNum].stSect[nWeek][4]);
			pPeriodInfo->bEnable = m_ctlPRD5.GetCheck();
			pPeriodInfo->iBeginHour = m_prd5StartHour;
			pPeriodInfo->iBeginMin = m_prd5StartMinute;
			pPeriodInfo->iBeginSec = m_prd5StartSecond;
			pPeriodInfo->iEndHour = m_prd5EndHour;
			pPeriodInfo->iEndMin = m_prd5EndMinute;
			pPeriodInfo->iEndSec = m_prd5EndSecond;
			
			pPeriodInfo = &(m_AlarmInInfo[nNum].stSect[nWeek][5]);
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

//Display period information
void CAlarmInputDlg::ShowPeriodInfo(DH_TSECT *pTSect, BYTE nNum)
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

BOOL CAlarmInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	ConvertComboBox(m_ctlAlarmWeek);
	ConvertComboBox(m_ctlAlarmType);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarmInputDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}





























































































































































































































































































































































































































































































































































