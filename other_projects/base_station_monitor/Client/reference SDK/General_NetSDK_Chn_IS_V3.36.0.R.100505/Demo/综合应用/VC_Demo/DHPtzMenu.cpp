// DHPtzMenu.cpp : implementation file
//

#include "stdafx.h"
#include "netsdkdemo.h"
#include "DHPtzMenu.h"
#include "NetSDKDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDHPtzMenu dialog


CDHPtzMenu::CDHPtzMenu(CWnd* pParent /*=NULL*/)
	: CDialog(CDHPtzMenu::IDD, pParent)
{
	m_DeviceID = 0;
	m_Channel = 0;
	//{{AFX_DATA_INIT(CDHPtzMenu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDHPtzMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDHPtzMenu)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDHPtzMenu, CDialog)
	//{{AFX_MSG_MAP(CDHPtzMenu)
	ON_BN_CLICKED(IDC_OPR_CANCEL, OnOprCancel)
	ON_BN_CLICKED(IDC_OPR_CLOSEMENU, OnOprClosemenu)
	ON_BN_CLICKED(IDC_OPR_DOWN, OnOprDown)
	ON_BN_CLICKED(IDC_OPR_LEFT, OnOprLeft)
	ON_BN_CLICKED(IDC_OPR_OK, OnOprOk)
	ON_BN_CLICKED(IDC_OPR_OPENMENU, OnOprOpenmenu)
	ON_BN_CLICKED(IDC_OPR_RIGHT, OnOprRight)
	ON_BN_CLICKED(IDC_OPR_UP, OnOprUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDHPtzMenu message handlers
void CDHPtzMenu::SetPtzParam(LONG iHandle, int iChannel)
{
	m_DeviceID = iHandle;
	m_Channel = iChannel;

}
void CDHPtzMenu::OnOprCancel() 
{
	PtzMemuControl(DH_EXTPTZ_MENUCANCEL);
}

void CDHPtzMenu::OnOprClosemenu() 
{
	PtzMemuControl(DH_EXTPTZ_CLOSEMENU);
}

void CDHPtzMenu::OnOprDown() 
{
	PtzMemuControl(DH_EXTPTZ_MENUDOWN);
}

void CDHPtzMenu::OnOprLeft() 
{
	PtzMemuControl(DH_EXTPTZ_MENULEFT);
}

void CDHPtzMenu::OnOprOk() 
{
	PtzMemuControl(DH_EXTPTZ_MENUOK);
}

void CDHPtzMenu::OnOprOpenmenu() 
{
	PtzMemuControl(DH_EXTPTZ_OPENMENU);
}

void CDHPtzMenu::OnOprRight() 
{
	PtzMemuControl(DH_EXTPTZ_MENURIGHT);
}

void CDHPtzMenu::OnOprUp() 
{
	PtzMemuControl(DH_EXTPTZ_MENUUP);
}

void CDHPtzMenu::PtzMemuControl(DWORD dwCommand)
{
	if(!m_DeviceID)
	{
		MessageBox(ConvertString(MSG_PTZCTRL_NOCHANNEL));
	}
	BOOL ret = CLIENT_DHPTZControl(m_DeviceID, m_Channel,dwCommand ,0,0,0,FALSE);
	if(!ret)
	{	
		((CNetSDKDemoDlg *)GetParent())->LastError();//Zhengdh 06.11.24
		MessageBox(ConvertString(MSG_PTZCTRL_CTRLFAILED));
	}
	
}

BOOL CDHPtzMenu::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	return TRUE;
}
