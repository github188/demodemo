// QueryAlarmState.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo1.h"
#include "QueryAlarmState.h"
#include "ClientDemo1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQueryAlarmState dialog


CQueryAlarmState::CQueryAlarmState(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryAlarmState::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQueryAlarmState)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CQueryAlarmState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueryAlarmState)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQueryAlarmState, CDialog)
	//{{AFX_MSG_MAP(CQueryAlarmState)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnDoubleclickedButtonQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueryAlarmState message handlers

void CQueryAlarmState::OnDoubleclickedButtonQuery() 
{
	// TODO: Add your control notification handler code here
	int nRet =0 ;
	int nRetLen =0;
	memset(&m_stuComAlarm,0,sizeof(m_stuComAlarm));
	unsigned long lLogin =((CClientDemo1Dlg *)(GetParent()->GetParent()))->m_LoginID;
	nRet = CLIENT_QueryDevState(lLogin,DH_DEVSTATE_COMM_ALARM,(char*)&m_stuComAlarm,sizeof(m_stuComAlarm),&nRetLen);
	if(nRet <= 0 && nRetLen == 0)
	{
		AfxMessageBox(ConvertString("Get alarm failed!"));
	}
	//Display alarm 
	SetAlarmInfo();
}

void CQueryAlarmState::SetAlarmInfo()
{
	int ID1 = IDC_CHECK1;
	int ID2 = IDC_CHECK17;
	int ID3 = IDC_CHECK33;
	for(int i=0;i<16;i++)
	{
		if(i<m_stuComAlarm.alarminputcount)
		{
			((CButton*)GetDlgItem(ID1 + i))->SetCheck(m_stuComAlarm.alarm[i]);
			((CButton*)GetDlgItem(ID2 + i))->SetCheck(m_stuComAlarm.videolost[i]);
			((CButton*)GetDlgItem(ID3 + i))->SetCheck(m_stuComAlarm.motiondection[i]);
			((CButton*)GetDlgItem(ID1 + i))->EnableWindow(1);
			((CButton*)GetDlgItem(ID2 + i))->EnableWindow(1);
			((CButton*)GetDlgItem(ID3 + i))->EnableWindow(1);
		}
		else
		{
			((CButton*)GetDlgItem(ID1 + i))->EnableWindow(0);
			((CButton*)GetDlgItem(ID2 + i))->EnableWindow(0);
			((CButton*)GetDlgItem(ID3 + i))->EnableWindow(0);
		}
	}
}


BOOL CQueryAlarmState::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




































































































