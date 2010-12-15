// MediaIfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "MediaIfoDlg.h"
#include "dhplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMediaIfoDlg dialog


CMediaIfoDlg::CMediaIfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMediaIfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaIfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMediaIfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaIfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMediaIfoDlg, CDialog)
	//{{AFX_MSG_MAP(CMediaIfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaIfoDlg message handlers

BOOL CMediaIfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int len = 0;
	MEDIA_INFO tMediaInfo;
	memset(&tMediaInfo, 0, sizeof(MEDIA_INFO));

	PLAY_QueryInfo(0, PLAY_CMD_GetMediaInfo, (char*)(&tMediaInfo), sizeof(MEDIA_INFO), &len);

	CString csMediaInfo = "";

	if (tMediaInfo.lFrameRate != 0)
	{
		CString csTemp;
		csTemp.Format("Video Frame Rate     %d\n", tMediaInfo.lFrameRate);
		csMediaInfo += csTemp;

		csTemp.Format("Video resolution     %d * %d\n", tMediaInfo.lWidth, tMediaInfo.lHeight);
		csMediaInfo += csTemp;
		csMediaInfo += "---------------------------------------------\n";
	}

	if (tMediaInfo.lChannel != 0)
	{
		CString csTemp;
		csTemp.Format("Audio Channel        %d\n", tMediaInfo.lChannel);
		csMediaInfo += csTemp;

		csTemp.Format("Audio BitPerSample   %d\n", tMediaInfo.lBitPerSample);
		csMediaInfo += csTemp;

		csTemp.Format("Audio SamplesPerSec  %d\n", tMediaInfo.lSamplesPerSec);
		csMediaInfo += csTemp;
		csMediaInfo += "---------------------------------------------\n";
	}
	
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(csMediaInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
