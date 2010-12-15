// AVIConvertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "AVIConvertDlg.h"
#include "dhplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define AVICONV_PORT		450

/////////////////////////////////////////////////////////////////////////////
DWORD WINAPI AVIConvert_Proc(LPVOID pParam)
{
	CAVIConvertDlg* pThis = (CAVIConvertDlg*)pParam;
	
	pThis->AVIConv_Thread();
	
	return 0;
}

void CALLBACK AVIConvertCBFunc(long nPort, long lMediaChangeType, long lUser, BOOL *pbIfContinue, char *sNewFileName)
{
	CAVIConvertDlg *pThis = (CAVIConvertDlg *)lUser;

	switch (lMediaChangeType)
	{
	case AVI_MEDIACHANGE_FRAMERATE:
	case AVI_MEDIACHANGE_RESOLUTION:
		{
			CString csNewFileName, csTemp;
			*pbIfContinue = TRUE;
			int iPos = pThis->m_csTargetFile.Find(".avi");
			csNewFileName = pThis->m_csTargetFile.Mid(0, iPos);
			csTemp.Format("_%d.avi", pThis->m_iChangeCount);
			csNewFileName += csTemp;
			memcpy(sNewFileName, csNewFileName.GetBuffer(0), csNewFileName.GetLength());
			pThis->m_iChangeCount++;
		}
		break;
	default:
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CAVIConvertDlg dialog


CAVIConvertDlg::CAVIConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAVIConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAVIConvertDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAVIConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAVIConvertDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAVIConvertDlg, CDialog)
	//{{AFX_MSG_MAP(CAVIConvertDlg)
	ON_BN_CLICKED(IDC_BUTTON_SRCFILE, OnButtonSrcfile)
	ON_BN_CLICKED(IDC_BUTTON_TARGFILE, OnButtonTargfile)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnButtonConvert)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVIConvertDlg message handlers
BOOL CAVIConvertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_csSourceFile = "";
	m_csTargetFile = "";
	m_iChangeCount = 0;
	m_bAVIConvEnable = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAVIConvertDlg::OnButtonSrcfile() 
{
	CFileDialog FileChooser(TRUE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"All files(*.*)|*.*||");
	//choose file
	if (FileChooser.DoModal()==IDOK)
	{
		m_csSourceFile = FileChooser.GetPathName() ;
	}
	GetDlgItem(IDC_EDIT_SRCFILE)->SetWindowText(m_csSourceFile);

	int iPos = m_csSourceFile.Find(".dav");
	m_csTargetFile = m_csSourceFile.Mid(0, iPos);
	m_csTargetFile += ".avi";

	GetDlgItem(IDC_EDIT_TARGFILE)->SetWindowText(m_csTargetFile);	
}

void CAVIConvertDlg::OnButtonTargfile() 
{
	CFileDialog FileChooser(FALSE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"avi files(*.avi)|*.avi||");
	//choose file
	if (FileChooser.DoModal()==IDOK)
	{
		m_csTargetFile = FileChooser.GetPathName() ;
	}

	int iPos = m_csTargetFile.Find(".avi");
	if (iPos == -1)
	{
		m_csTargetFile += ".avi";
	}

	GetDlgItem(IDC_EDIT_TARGFILE)->SetWindowText(m_csTargetFile);	
}

void CAVIConvertDlg::OnButtonConvert() 
{
	if (m_csSourceFile == "") return ;

	m_bAVIConvEnable = TRUE;

	DWORD dwThreadId = 0;
	HANDLE hAVIConvThread = CreateThread(NULL, 0, AVIConvert_Proc, this, 0, &dwThreadId);

	GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(FALSE);	
}

void CAVIConvertDlg::AVIConv_Thread()
{
	CProgressCtrl *pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_CONVERT);
	pProgCtrl->SetRange(0, 1000);
	pProgCtrl->SetPos(0);
	
	PLAY_SetStreamOpenMode(AVICONV_PORT, STREAME_FILE);
	PLAY_OpenStream(AVICONV_PORT, NULL, 0, 1024 * 100);
	
	//PLAY_SetDecCallBack(AVICONV_PORT, MediaDecCBFun);
	//PLAY_SetDecCBStream(AVICONV_PORT, 3);
	
	if (!PLAY_Play(AVICONV_PORT, NULL))
	{
		PLAY_CloseStream(AVICONV_PORT);
	}

	GetDlgItem(IDC_EDIT_TARGFILE)->GetWindowText(m_csTargetFile);
	// 回调用于处理变帧率，变分辨率的文件的AVI转换	
	BOOL bRet = PLAY_StartAVIConvert(AVICONV_PORT, m_csTargetFile.GetBuffer(0), AVIConvertCBFunc, (long)this);
	if (!bRet)
	{
		PLAY_Stop(AVICONV_PORT);
		PLAY_CloseStream(AVICONV_PORT);

		MessageBox("AVI Convert Failed!!");
		return ;
	}
	
	CFile SourceFile;
	if ( !SourceFile.Open(m_csSourceFile, CFile::modeRead | CFile::shareDenyNone) )
	{
		PLAY_StopAVIConvert(AVICONV_PORT);
		PLAY_Stop(AVICONV_PORT);
		PLAY_CloseStream(AVICONV_PORT);
	}
	
	DWORD dwFileLen = SourceFile.GetLength();	
	
	const int BUFLEN = 8 * 1024;
	BYTE InfoBuf[BUFLEN];
	DWORD nRead;
	DWORD dwCurPos = 0;
	
	try
	{
		while (m_bAVIConvEnable)
		{
			memset(InfoBuf, 0, sizeof(InfoBuf));
			nRead = SourceFile.Read(InfoBuf, BUFLEN);
			
			if (nRead <= 0)
			{
				break;
			}
			
			dwCurPos += nRead;			
			
			while (m_bAVIConvEnable && !PLAY_InputData( AVICONV_PORT, InfoBuf, nRead))//阻塞为false
			{
				Sleep(5);
			}
			
			double fpress = (double)dwCurPos/(double)dwFileLen;
			
			int iProgressPos = (int)(fpress*1000.0);
			if (m_bAVIConvEnable)
				pProgCtrl->SetPos(iProgressPos);
		}
	}
	catch (CException* e)
	{
		PLAY_StopAVIConvert(AVICONV_PORT);
		PLAY_Stop(AVICONV_PORT);
		PLAY_CloseStream(AVICONV_PORT);
		SourceFile.Close();
		e->Delete();
	}
	
	while (m_bAVIConvEnable && !(PLAY_GetSourceBufferRemain(AVICONV_PORT) == 0))
	{
		//解码结束， number=0时，解码出来的数据都已显示或回调完全
		//number < num时，解码基本完成
		while (m_bAVIConvEnable && PLAY_GetSourceBufferRemain(AVICONV_PORT) > 0)
		{
			if (PLAY_GetBufferValue(AVICONV_PORT, BUF_VIDEO_RENDER) < 1) 
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
	
	PLAY_StopAVIConvert(AVICONV_PORT);
	PLAY_Stop(AVICONV_PORT);
	PLAY_CloseStream(AVICONV_PORT);
	SourceFile.Close();
	
	if (m_bAVIConvEnable)
	{
		MessageBox("AVI convert complete!!");
		
		m_bAVIConvEnable = FALSE;		
	}
	pProgCtrl->SetPos(0);
	GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(TRUE);
}

void CAVIConvertDlg::OnCancel() 
{
	if (m_bAVIConvEnable)
	{
		m_bAVIConvEnable = FALSE;
		GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(TRUE);
		return ;
	}
	
	CDialog::OnCancel();
}

void CAVIConvertDlg::OnClose() 
{
	if (m_bAVIConvEnable)
	{
		m_bAVIConvEnable = FALSE;
	}
	
	CDialog::OnClose();
}
