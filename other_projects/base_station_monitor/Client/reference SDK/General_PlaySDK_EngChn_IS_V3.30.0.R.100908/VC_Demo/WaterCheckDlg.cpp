// WaterCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "WaterCheckDlg.h"
#include "dhplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WATERMARK_PORT			499

#define SAFE_DELETE(x)				{if ((x)!=NULL) {delete (x); (x)=NULL;}} 
#define SAFE_DELETEBUFF(x)			{if((x)!=NULL) {delete [] (x); (x) = NULL;}}
#define SAFE_CLOSEHANDLE(x)			{if ((x)!=NULL) {CloseHandle((x)); (x)=NULL;}}

/////////////////////////////////////////////////////////////////////////////
DWORD WINAPI Watermark_CheckProc(LPVOID pParam)
{
	CWaterCheckDlg* pThis = (CWaterCheckDlg*)pParam;
	
	pThis->Watermark_Thread();
	
	return 0;
}

void CALLBACK MediaDecCBFun(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
}

int CALLBACK CBWaterMarkFuncEx(long nPort, char* buf, long lTimeStamp, long lInfoType, long len, long reallen, long lCheckResult, long nUser)
{
	CWaterCheckDlg *pThis = (CWaterCheckDlg *)nUser;

	switch (lCheckResult)
	{
	case 1: // no error
		if (lInfoType == WATERMARK_DATA_TEXT)
		{
			char* cWatermark = new char[reallen+1];
			memset(cWatermark, 0, reallen+1);
			memcpy(cWatermark, buf, reallen);

			pThis->GetDlgItem(IDC_EDIT_WATREMARK)->SetWindowText(cWatermark);

			delete[] cWatermark;
		}
		break;
	case 2: // water mark error 
		{
			CString csItemNum, csErrorType, csTimeStemp;

			csItemNum.Format("%d", pThis->m_lCheckErrorNum);
			csErrorType = "Watermark verify error";
			pThis->GetTimeStamp(lTimeStamp, csTimeStemp);
			//Insert Track to list
			int lTrackNum = pThis->m_WaterInfoList.GetItemCount();
			pThis->m_WaterInfoList.InsertItem(lTrackNum, csItemNum);
			pThis->m_WaterInfoList.SetItemText(lTrackNum, 1, csErrorType);
			pThis->m_WaterInfoList.SetItemText(lTrackNum, 2, csTimeStemp);
		}
		pThis->m_lCheckErrorNum ++;
		break;
	case 3: // frame data check error
		{
			CString csItemNum, csErrorType, csTimeStemp;
			
			csItemNum.Format("%d", pThis->m_lCheckErrorNum);
			csErrorType = "Frame data verify error";
			pThis->GetTimeStamp(lTimeStamp, csTimeStemp);
			//Insert Track to list
			int lTrackNum = pThis->m_WaterInfoList.GetItemCount();
			pThis->m_WaterInfoList.InsertItem(lTrackNum, csItemNum);
			pThis->m_WaterInfoList.SetItemText(lTrackNum, 1, csErrorType);
			pThis->m_WaterInfoList.SetItemText(lTrackNum, 2, csTimeStemp);
		}
		pThis->m_lCheckErrorNum ++;
		break;
	case 4: // frame num check error
		{
			CString csItemNum, csErrorType, csTimeStemp;
			
			csItemNum.Format("%d", pThis->m_lCheckErrorNum);
			csErrorType = "Frame number discontinuity";
			pThis->GetTimeStamp(lTimeStamp, csTimeStemp);
			//Insert Track to list
			int lTrackNum = pThis->m_WaterInfoList.GetItemCount();
			pThis->m_WaterInfoList.InsertItem(lTrackNum, csItemNum);
			pThis->m_WaterInfoList.SetItemText(lTrackNum, 1, csErrorType);
			pThis->m_WaterInfoList.SetItemText(lTrackNum, 2, csTimeStemp);
		}
		pThis->m_lCheckErrorNum ++;
		break;
	default:
		break;
	}
	
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CWaterCheckDlg dialog


CWaterCheckDlg::CWaterCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaterCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaterCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_csFilePath = "";
}

CWaterCheckDlg::~CWaterCheckDlg()
{
	if (m_hCheckThread != NULL)
	{
		m_bCheckEnable = FALSE;
		DWORD dwRes = WaitForSingleObject(m_hCheckThread, INFINITE);
		if (WAIT_OBJECT_0 == dwRes)
		{
			SAFE_CLOSEHANDLE(m_hCheckThread);
		}
	}
}

void CWaterCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaterCheckDlg)
	DDX_Control(pDX, IDC_LIST_WATERINFO, m_WaterInfoList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaterCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CWaterCheckDlg)
	ON_BN_CLICKED(IDC_BUTTON_CHECKFILE, OnCheckfile)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnCheck)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterCheckDlg message handlers
BOOL CWaterCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bCheckEnable = FALSE;
	m_hCheckThread = NULL;
	GetDlgItem(IDC_EDIT_CHECKFILEPATH)->SetWindowText(m_csFilePath);

	if (m_csFilePath != "")
	{
		GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(FALSE);
	}

	m_lCheckErrorNum = 0;

	// Init Track List
	DWORD dwStyle=::GetWindowLong(m_WaterInfoList.m_hWnd,GWL_STYLE);
	//设置为报表形式
	SetWindowLong(m_WaterInfoList.m_hWnd,GWL_STYLE,dwStyle|LVS_REPORT);
	DWORD ExStyle =m_WaterInfoList.GetExtendedStyle();
	//设置为整栏选择和网格线
	m_WaterInfoList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	
	m_WaterInfoList.InsertColumn(0, "Num", LVCFMT_LEFT, 40);
	m_WaterInfoList.InsertColumn(1, "Error Type", LVCFMT_LEFT, 200);
	m_WaterInfoList.InsertColumn(2, "Time Stamp", LVCFMT_LEFT, 150);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWaterCheckDlg::OnClose() 
{
	if (m_hCheckThread != NULL)
	{
		m_bCheckEnable = FALSE;
	}

	CDialog::OnClose();
}

void CWaterCheckDlg::OnCheckfile() 
{
	CFileDialog FileChooser(TRUE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"All files(*.*)|*.*||");
	//choose file
	if (FileChooser.DoModal()==IDOK)
	{
		m_csFilePath = FileChooser.GetPathName() ;
	}
	GetDlgItem(IDC_EDIT_CHECKFILEPATH)->SetWindowText(m_csFilePath);

	if (m_csFilePath != "")
	{
		GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(FALSE);
	}

	m_lCheckErrorNum = 0;
	m_WaterInfoList.DeleteAllItems();
	GetDlgItem(IDC_EDIT_WATREMARK)->SetWindowText("");
}

void CWaterCheckDlg::OnCheck() 
{
	if (!m_bCheckEnable)
	{
		m_bCheckEnable = TRUE;
		m_lCheckErrorNum = 0;
		m_WaterInfoList.DeleteAllItems();
		
		DWORD dwThreadId = 0;
		HANDLE hCheckThread = CreateThread(NULL, 0, Watermark_CheckProc, this, 0, &dwThreadId);

		GetDlgItem(IDC_EDIT_WATREMARK)->SetWindowText("");
		GetDlgItem(IDC_BUTTON_CHECK)->SetWindowText("stop");
	}
	else
	{
		m_bCheckEnable = FALSE;
		m_WaterInfoList.DeleteAllItems();
		GetDlgItem(IDC_BUTTON_CHECK)->SetWindowText("check");
	}
}

void CWaterCheckDlg::Watermark_Thread()
{
	CProgressCtrl *pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_CHECK);
	pProgCtrl->SetRange(0, 1000);
	pProgCtrl->SetPos(0);

	PLAY_SetStreamOpenMode( WATERMARK_PORT, STREAME_FILE);
	PLAY_OpenStream(WATERMARK_PORT, NULL, 0, 1024 * 100);
	
	PLAY_SetDecCallBack(WATERMARK_PORT, MediaDecCBFun);
	PLAY_SetDecCBStream(WATERMARK_PORT, 3);
	
	if (!PLAY_Play(WATERMARK_PORT, NULL))
	{
		PLAY_CloseStream(WATERMARK_PORT);
	}
		
	PLAY_SetWaterMarkCallBackEx(WATERMARK_PORT, CBWaterMarkFuncEx, (long)this);
	
	CFile SourceFile;
	if ( !SourceFile.Open(m_csFilePath, CFile::modeRead | CFile::shareDenyNone) )
	{
		PLAY_Stop(WATERMARK_PORT);
		PLAY_CloseStream(WATERMARK_PORT);
	}

	DWORD dwFileLen = SourceFile.GetLength();	
	
	const int BUFLEN = 8 * 1024;
	BYTE InfoBuf[BUFLEN];
	DWORD nRead;
	DWORD dwCurPos = 0;
	
	try
	{
		while (m_bCheckEnable)
		{
			memset(InfoBuf, 0, sizeof(InfoBuf));
			nRead = SourceFile.Read(InfoBuf, BUFLEN);
			
			if (nRead <= 0)
			{
				break;
			}

			dwCurPos += nRead;			
			
			while (m_bCheckEnable && !PLAY_InputData( WATERMARK_PORT, InfoBuf, nRead))//阻塞为false
			{
				Sleep(5);
			}

			double fpress = (double)dwCurPos/(double)dwFileLen;

			int iProgressPos = (int)(fpress*1000.0);
			if (m_bCheckEnable)
				pProgCtrl->SetPos(iProgressPos);
		}
	}
	catch (CException* e)
	{
		PLAY_Stop(WATERMARK_PORT);
		PLAY_CloseStream(WATERMARK_PORT);
		SourceFile.Close();
		e->Delete();
	}
	
	while (m_bCheckEnable && !(PLAY_GetSourceBufferRemain(WATERMARK_PORT) == 0))
	{
		//解码结束， number=0时，解码出来的数据都已显示或回调完全
		//number < num时，解码基本完成
		while (m_bCheckEnable && PLAY_GetSourceBufferRemain(WATERMARK_PORT) > 0)
		{
			if (PLAY_GetBufferValue(WATERMARK_PORT, BUF_VIDEO_RENDER) < 1) 
			{
				//解码结束的相关操作
				Sleep(5);
				break;
			}
			else
			{
				Sleep(10);
			}
		}
	}
	
	pProgCtrl->SetPos(1000);
	
	PLAY_Stop(WATERMARK_PORT);
	PLAY_CloseStream(WATERMARK_PORT);
	SourceFile.Close();

	if (m_bCheckEnable)
	{
		MessageBox("Watermark Check Complete!!");
		
		m_bCheckEnable = FALSE;		
		GetDlgItem(IDC_BUTTON_CHECK)->SetWindowText("check");
	}
	pProgCtrl->SetPos(0);
}

BOOL CWaterCheckDlg::GetTimeStamp(long lTimeStamp, CString& csTime)
{
	time_t tTimeStamp = (time_t)lTimeStamp;
	tm *curtm = localtime(&tTimeStamp) ; 
	if (curtm == 0) //bug: localtime计算失败 
	{
		return FALSE; 
	} 
	
	int nHour	= curtm->tm_hour; 
	int nMinute	= curtm->tm_min; 
	int nSecond = curtm->tm_sec; 
	int nDay	= curtm->tm_mday - 1; 
	int nMonth	= curtm->tm_mon; 
	int nYear	= curtm->tm_year +1900;
	
	csTime.Format("%d-%02d-%02d %02d:%02d:%02d", nYear,nMonth, nDay, nHour, nMinute, nSecond);
	
	return TRUE;
}


