// NewAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo1.h"
#include "NewAlarm.h"
#include "resource.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewAlarm dialog
void CNewAlarm::SetAlarmInfo(BYTE *dwAlarm,DWORD dwID)
{

	for(int i=0;i<16;i++) 
	{
		if(1 == dwAlarm[i])
		{
			//TRACE("%d \n",i);
			((CButton*)GetDlgItem(dwID + i))->SetCheck(1);
			
		}
		else
		{
			((CButton*)GetDlgItem(dwID + i))->SetCheck(0);
		}
	}
}

void CNewAlarm::SetSoundAlarmInfo(DH_NEW_SOUND_ALARM_STATE &stuSoundAlarm,DWORD	dwID)
{
	char szBuf[2048] ={0};
	int nCount =stuSoundAlarm.channelcount;
	for(int i=0;i<nCount;i++)
	{
		char szTempBuf[256];
		memset(szTempBuf,0,sizeof(char)*256);
		sprintf(szTempBuf
			,"channel:%d type:%d volume:%d \n"
			,stuSoundAlarm.SoundAlarmInfo[i].channel
			,stuSoundAlarm.SoundAlarmInfo[i].alarmType
			,stuSoundAlarm.SoundAlarmInfo[i].volume);
		strcat(szBuf,szTempBuf);
	}
	SetDlgItemText(dwID,szBuf);
}
void CNewAlarm::SetCheckBox(int nChannelCount, int nInputAlarmCount)
{
		
}

void CNewAlarm::SetAlarmDecoderAlarm(ALARM_DECODER_ALARM & stuAlarmDecoderAlarm)
{
	if(stuAlarmDecoderAlarm.nAlarmDecoderNum <= 0)
	{
		return ;
	}
	UINT uID[8];//7:Now it supports max 8 alarm decoders.
	uID[0] = IDC_ARRAY_DECODER00;
	uID[1] = IDC_ARRAY_DECODER10;
	uID[2] = IDC_ARRAY_DECODER20;
	uID[3] = IDC_ARRAY_DECODER30;
	uID[4] = IDC_ARRAY_DECODER40;
	uID[5] = IDC_ARRAY_DECODER50;
	uID[6] = IDC_ARRAY_DECODER60;
	uID[7] = IDC_ARRAY_DECODER70;
	
	//Clear all CheckBox.
	for(int iClear = 0; iClear < 8; iClear++)
	{
		for(int jCheck = 0; jCheck < 8; jCheck++)
		{
			((CButton *)GetDlgItem(uID[iClear] + jCheck))->SetCheck(0);
		}
	}

	WORD wAlarmData = 0;//To get alarm data 
	for(int i = 0; i < stuAlarmDecoderAlarm.nAlarmDecoderNum; i++)
	{
		wAlarmData = stuAlarmDecoderAlarm.stuAlarmDecoder[i].dwAlarmDecoder;
		for(int j = 0; j < 8; j++)//8:Bit means the 8 input ports of alarm decoder 
		{
			((CButton *)GetDlgItem(uID[i] + j))->SetCheck(wAlarmData & 0x0001);//Get the alarm information from the lowest bit 
			wAlarmData >>= 0x0001;
		}
	}
}

CNewAlarm::CNewAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNewAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAlarm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAlarm, CDialog)
	//{{AFX_MSG_MAP(CNewAlarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAlarm message handlers

BOOL CNewAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_SetWndStaticText(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




































































































