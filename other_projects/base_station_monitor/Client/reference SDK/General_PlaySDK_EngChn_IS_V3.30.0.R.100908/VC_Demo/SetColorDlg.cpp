// SetColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "SetColorDlg.h"
#include "dhplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg dialog


CSetColorDlg::CSetColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetColorDlg)
	//}}AFX_DATA_INIT
}


void CSetColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetColorDlg)
	DDX_Control(pDX, IDC_SLIDERS, m_SliderSaturation);
	DDX_Control(pDX, IDC_SLIDERH, m_SliderHue);
	DDX_Control(pDX, IDC_SLIDERC, m_SliderContrast);
	DDX_Control(pDX, IDC_SLIDERB, m_SliderBrightness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetColorDlg, CDialog)
	//{{AFX_MSG_MAP(CSetColorDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg message handlers

BOOL CSetColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int iBright, iContrast, iSatura, iHue;
	PLAY_GetColor(0, 0, &iBright, &iContrast, &iSatura, &iHue);
	
	//Set Color
	m_SliderBrightness.SetRange(0, 128);
	m_SliderBrightness.SetPos(iBright);
	m_SliderContrast.SetRange(0, 128);
	m_SliderContrast.SetPos(iContrast);
	m_SliderSaturation.SetRange(0, 128);
	m_SliderSaturation.SetPos(iSatura);
	m_SliderHue.SetRange(0, 128);
	m_SliderHue.SetPos(iHue);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID))
	{
	case IDC_SLIDERB:
	case IDC_SLIDERC:
	case IDC_SLIDERH:
	case IDC_SLIDERS:
		{
			int nBrightness = m_SliderBrightness.GetPos();
			int nContrast = m_SliderContrast.GetPos();
			int nSaturation = m_SliderSaturation.GetPos();
			int nHue = m_SliderHue.GetPos();			
			PLAY_SetColor(0, 0, nBrightness, nContrast, nSaturation, nHue);
		}
		break ;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSetColorDlg::SetColorSliders(void)
{
	int iBright, iContrast, iSatura, iHue;
	PLAY_GetColor(0, 0, &iBright, &iContrast, &iSatura, &iHue);
	
	//Set Color
	m_SliderBrightness.SetPos(iBright);
	m_SliderContrast.SetPos(iContrast);
	m_SliderSaturation.SetPos(iSatura);
	m_SliderHue.SetPos(iHue);
}
