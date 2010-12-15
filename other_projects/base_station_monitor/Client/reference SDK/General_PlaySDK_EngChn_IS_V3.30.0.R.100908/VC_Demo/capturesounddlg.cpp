// aptureSoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "CaptureSoundDlg.h"
#include "dhplay.h"
#include <MMSystem.h>


#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////////
void CALLBACK AudioCallFunction(LPBYTE pDataBuffer, DWORD DataLength, long nUser)
{
	if(nUser == 0)
	{
		return;
	}
	CaptureSoundDlg *dlg = (CaptureSoundDlg *)nUser;
	dlg->SoundRecord(pDataBuffer, DataLength);
}

/////////////////////////////////////////////////////////////////////////////
// CaptureSoundDlg dialog


CaptureSoundDlg::CaptureSoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CaptureSoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CaptureSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CaptureSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CaptureSoundDlg)
	DDX_Control(pDX, IDC_COMBO_SAMPLES, m_ComboxSampPerSec);
	DDX_Control(pDX, IDC_COMBO_BIT, m_ComboxBits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CaptureSoundDlg, CDialog)
	//{{AFX_MSG_MAP(CaptureSoundDlg)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDFILE, OnButtonSoundfile)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_CBN_SELCHANGE(IDC_COMBO_BIT, OnSelchangeComboBit)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLES, OnSelchangeComboSamples)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CaptureSoundDlg message handlers

BOOL CaptureSoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_csSoundFile = "";

	m_bSoundCapture = FALSE;

	m_fpSoundFile = NULL;

	int index = m_ComboxBits.AddString("8  bit");
	m_ComboxBits.SetItemData(index, 8);

	index = m_ComboxBits.AddString("16 bit");
	m_ComboxBits.SetItemData(index, 16);
	
	m_ComboxBits.SetCurSel(1);

	FillSamplPerSecCombox();

	m_lBitPerSample = m_ComboxBits.GetItemData(m_ComboxBits.GetCurSel());
	m_lSampleSPerSec = m_ComboxSampPerSec.GetItemData(m_ComboxSampPerSec.GetCurSel());

	//UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CaptureSoundDlg::OnButtonSoundfile() 
{
	CFileDialog FileChooser(FALSE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"pcm files(*.pcm)|*.pcm||");
	//choose file
	if (FileChooser.DoModal()==IDOK)
	{
		m_csSoundFile = FileChooser.GetPathName() ;
	}
	
	int iPos = m_csSoundFile.Find(".pcm");
	if (iPos == -1)
	{
		m_csSoundFile += ".pcm";
	}
	
	GetDlgItem(IDC_EDIT_SOUNDFILE)->SetWindowText(m_csSoundFile);
}

void CaptureSoundDlg::OnButtonRecord() 
{
	if (!m_bSoundCapture)
	{
		m_bSoundCapture = TRUE;

		if (m_fpSoundFile != NULL)
		{
			fclose(m_fpSoundFile);
			m_fpSoundFile = NULL;
		}

		GetDlgItem(IDC_EDIT_SOUNDFILE)->GetWindowText(m_csSoundFile);

		m_fpSoundFile = fopen(m_csSoundFile.GetBuffer(0), "wb");
		m_csSoundFile.ReleaseBuffer();

		if (m_fpSoundFile == NULL)
		{
			MessageBox("Create sound file failed!!");
			return;
		}

		BOOL bRet = PLAY_OpenAudioRecord(AudioCallFunction, m_lBitPerSample, m_lSampleSPerSec, 
			GetSampleSize(m_lBitPerSample, m_lSampleSPerSec), 0, (long)this);
		if (!bRet)
		{
			fclose(m_fpSoundFile);
			m_fpSoundFile = NULL;

			MessageBox("Open sound capture failed!!");
			return;
		}
		
		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("stop");
	}
	else
	{
		m_bSoundCapture = FALSE;
		PLAY_CloseAudioRecord();

		if (m_fpSoundFile != NULL)
		{
			fclose(m_fpSoundFile);
			m_fpSoundFile = NULL;
		}

		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("Record");
	}
	
}

long CaptureSoundDlg::GetSampleSize(long BitsPerSec, long SamplesPerSec)
{
	long lSampleBitRate = BitsPerSec*SamplesPerSec;

	long lSampleLen = 40*lSampleBitRate/16000;

	if (lSampleLen <= 320)
	{
		lSampleLen = 320;
	}
	else if (lSampleLen > 320 && lSampleLen <= 640)
	{
		lSampleLen = 640;
	}
	else
	{
		lSampleLen = 1024;
	}

	return lSampleLen;
}

void CaptureSoundDlg::SoundRecord(BYTE* pPCMData, long lDataLen)
{
	if (pPCMData == NULL || lDataLen == 0) return;

	fwrite(pPCMData, 1, lDataLen, m_fpSoundFile);
}


void CaptureSoundDlg::FillSamplPerSecCombox()
{
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("8000  Hz"), 8000);
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("11025 Hz"), 11025);
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("16000 Hz"), 16000);
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("22050 Hz"), 22050);
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("32000 Hz"), 32000);
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("44100 Hz"), 44100);
	m_ComboxSampPerSec.SetItemData(m_ComboxSampPerSec.AddString("96000 Hz"), 96000);

	m_ComboxSampPerSec.SetCurSel(0);
}

void CaptureSoundDlg::OnSelchangeComboBit() 
{
	m_lBitPerSample = m_ComboxBits.GetItemData(m_ComboxBits.GetCurSel());	
}

void CaptureSoundDlg::OnSelchangeComboSamples() 
{
	m_lSampleSPerSec = m_ComboxSampPerSec.GetItemData(m_ComboxSampPerSec.GetCurSel());	
}
