// StrategyTrans.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "StrategyTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStrategyTrans dialog


CStrategyTrans::CStrategyTrans(CWnd* pParent /*=NULL*/)
	: CDialog(CStrategyTrans::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStrategyTrans)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStrategyTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrategyTrans)
	DDX_Control(pDX, IDC_COMBO_TRANS, m_ccbTrans);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_cbEnable);
	//}}AFX_DATA_MAP
}

void CStrategyTrans::InitDlgInfo(DHDEV_TRANSFER_STRATEGY_CFG* pTransInfo)
{
	if(NULL == pTransInfo)
	{
		return ;
	}
	m_cbEnable.SetCheck(pTransInfo->bEnable);
	m_ccbTrans.SetCurSel(pTransInfo->iStrategy);
}
void CStrategyTrans::SaveAll(void)
{
	m_stuTrans.bEnable = m_cbEnable.GetCheck();
	int nIndex = m_ccbTrans.GetCurSel();
	m_stuTrans.iStrategy = m_ccbTrans.GetItemData(nIndex);
	m_stuTrans.dwSize = sizeof(m_stuTrans);
}
void CStrategyTrans::CleanAll()
{
	m_ccbTrans.SetCurSel(-1);
	m_cbEnable.SetCheck(0);
}
BEGIN_MESSAGE_MAP(CStrategyTrans, CDialog)
	//{{AFX_MSG_MAP(CStrategyTrans)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrategyTrans message handlers

BOOL CStrategyTrans::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	int nIndex = m_ccbTrans.AddString(ConvertString("quality first"));
	m_ccbTrans.SetItemData(nIndex,0);
	nIndex = m_ccbTrans.AddString(ConvertString("fluency first"));
	m_ccbTrans.SetItemData(nIndex,1);
	nIndex = m_ccbTrans.AddString(ConvertString("auto"));
	m_ccbTrans.SetItemData(nIndex,2);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStrategyTrans::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}








































































































































































































































































































































































































































































































































