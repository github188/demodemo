// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo4.h"
#include "ServerDlg.h"
#include "dhplay.h"
#include "hisamr.h"
#include "Alaw_encoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lLoginHandle = 0;
	m_hTalkHandle = 0;
	m_bOpenAudioRecord = FALSE;
	m_hisEncodeHandle = NULL;
}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_COMBO_ENCODETYPE, m_ctlEncodeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, OnBtnLogout)
	ON_BN_CLICKED(IDC_BTN_STARTTALK, OnBtnStarttalk)
	ON_BN_CLICKED(IDC_BTN_STOPTALK, OnBtnStoptalk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

void CServerDlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strIp = "";
	CString strUser = "";
	CString strPwd = "";
	int nPort = 0;
	int nError = 0;
	NET_DEVICEINFO deviceInfo = {0};

	GetDlgItemText(IDC_IPADDRESS_IP, strIp);
	GetDlgItemText(IDC_EDIT_USER, strUser);
	GetDlgItemText(IDC_EDIT_PWD, strPwd);
	nPort = GetDlgItemInt(IDC_EDIT_PORT);

	LONG lLoginHandle = CLIENT_Login((LPSTR)(LPCSTR)strIp, nPort, (LPSTR)(LPCSTR)strUser, (LPSTR)(LPCSTR)strPwd, &deviceInfo, &nError);
	if (lLoginHandle != 0)
	{
		m_lLoginHandle = lLoginHandle;

		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STARTTALK)->EnableWindow(TRUE);

		m_ctlEncodeType.ResetContent();
		//Search the audio talk encode format the device supported 
		int retlen = 0;
		BOOL bSuccess = CLIENT_QueryDevState(m_lLoginHandle, DH_DEVSTATE_TALK_ECTYPE, (char*)&m_lstTalkEncode, sizeof(DHDEV_TALKFORMAT_LIST), &retlen, 2000);
		if (bSuccess && retlen == sizeof(DHDEV_TALKFORMAT_LIST))
		{
			int nIndex = 0;
			CString str;
			for (int i = 0; i < m_lstTalkEncode.nSupportNum; i++)
			{
				switch(m_lstTalkEncode.type[i].encodeType)
				{
				case DH_TALK_DEFAULT:
				case DH_TALK_PCM:
					str =  ConvertString("PCM");
					break;
				case DH_TALK_G711a:
					str = ConvertString("G711a");
					break;
				case DH_TALK_AMR:
					str = ConvertString("AMR");
					break;
				case DH_TALK_G711u:
					str = ConvertString("G711u");
					break;
				case DH_TALK_G726:
					str = ConvertString("G726");
					break;
				default:
					str = ConvertString("Unknow");
					break;
				}
				nIndex = m_ctlEncodeType.AddString(str);
				m_ctlEncodeType.SetItemData(nIndex, i);
			}
			
			if (m_ctlEncodeType.GetCount() > 0)
			{
				m_ctlEncodeType.SetCurSel(0);
			}
		}
	}
	else
	{
		m_lLoginHandle = 0;
		ShowLoginErrorReason(nError);
	}
}

void CServerDlg::OnBtnLogout() 
{
	// TODO: Add your control notification handler code here
	if(m_bOpenAudioRecord)
	{
		BOOL bSuccess = PLAY_CloseAudioRecord();
		if(bSuccess)
		{
			m_bOpenAudioRecord = FALSE;
			PLAY_Stop(100);
			PLAY_Stop(99);
			PLAY_StopSoundShare(100);
			PLAY_StopSoundShare(99);
			PLAY_CloseStream(100);
			PLAY_CloseStream(99);
		}
		else
		{
			MessageBox(ConvertString("Stop record failed!"), ConvertString("Prompt"));
		}
	}

	if(0 != m_hTalkHandle)
	{
		CLIENT_StopTalkEx(m_hTalkHandle);
		m_hTalkHandle = 0;
	}

	BOOL bRet = CLIENT_Logout(m_lLoginHandle);
	if(bRet)
	{
		m_lLoginHandle = 0;
		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STARTTALK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOPTALK)->EnableWindow(FALSE);
		
		m_ctlEncodeType.ResetContent();
	}
	else
	{
		MessageBox(ConvertString("Logout failed!"), ConvertString("Prompt"));
	}
}

//Audio data of audio talk callback function 
void CALLBACK AfxAudioDataCallBack(LONG lTalkHandle, char *pDataBuf, DWORD dwBufSize, BYTE byAudioFlag, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}
	CServerDlg *dlg = (CServerDlg *)dwUser;
	dlg->AudioData(lTalkHandle, pDataBuf,dwBufSize,byAudioFlag);
}

//Audio data of the audio talk callback 
void CServerDlg::AudioData(LONG lTalkHandle, char *pDataBuf, DWORD dwBufSize, BYTE byAudioFlag)
{
	if(m_hTalkHandle == lTalkHandle)
	{
		if(1 == byAudioFlag)
		{
			//The audio data received from the device 

			int nPort = 99;
			//For PCM format withour header , please add 128.
			if (m_curTalkMode.encodeType == DH_TALK_DEFAULT)
			{
				nPort = 100;
				for (int i = 0; i < dwBufSize; i++)
				{
					pDataBuf[i] += 128;
				}
			}

			//You can use PLAY SDK to decode to get PCM and then encode to other formats if you to get a uniform formats.
			PLAY_InputData(nPort,(BYTE *)pDataBuf,dwBufSize);
			TRACE("收到音频数据长度：%d \n",dwBufSize);
		}
	}
}

void CServerDlg::OnBtnStarttalk() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_lLoginHandle)
	{
		//	First set audio talk audio encode format
		m_curTalkMode.encodeType = DH_TALK_DEFAULT;	// DH_TALK_DEFAULT is default way. Usually it is PCM without head.
		m_curTalkMode.dwSampleRate = 8000;
		m_curTalkMode.nAudioBit = 8;

		int nIndex = m_ctlEncodeType.GetCurSel();
		if (-1 != nIndex)
		{
			DWORD dwItemData = m_ctlEncodeType.GetItemData(nIndex);
			m_curTalkMode = m_lstTalkEncode.type[dwItemData];
		}
		BOOL bSuccess = CLIENT_SetDeviceMode(m_lLoginHandle, DH_TALK_ENCODE_TYPE, &m_curTalkMode);

		//	Set server way(There is no decode in external SDK)
		bSuccess = CLIENT_SetDeviceMode(m_lLoginHandle, DH_TALK_SERVER_MODE, NULL);

		//	Then enable audio talk 
		LONG lTalkHandle = CLIENT_StartTalkEx(m_lLoginHandle, AfxAudioDataCallBack, (DWORD)this);
		if(0 != lTalkHandle)
		{
			m_hTalkHandle = lTalkHandle;
			GetDlgItem(IDC_BTN_STARTTALK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_STOPTALK)->EnableWindow(TRUE);
	
			//	Then enable local record audio function (Does not call it when it is a one-direction audio talk)
			BOOL bSuccess = StartAudioRecord();
			if(bSuccess)
			{
				m_bOpenAudioRecord = TRUE;
			}
			else
			{
				CLIENT_StopTalkEx(m_hTalkHandle);
				MessageBox(ConvertString("Start local record failed!"), ConvertString("Prompt"));
			}
		}
		else
		{
			MessageBox(ConvertString("Open talk failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Please login first!"), ConvertString("Prompt"));
	}
}

void CServerDlg::OnBtnStoptalk() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_lLoginHandle)
	{
		if(m_bOpenAudioRecord)
		{
			BOOL bSuccess = PLAY_CloseAudioRecord();
			if(bSuccess)
			{
				m_bOpenAudioRecord = FALSE;
				PLAY_Stop(100);
				PLAY_Stop(99);
				PLAY_StopSoundShare(100);
				PLAY_StopSoundShare(99);
				PLAY_CloseStream(100);
				PLAY_CloseStream(99);
			}
			else
			{
				MessageBox(ConvertString("Stop record failed!"), ConvertString("Prompt"));
			}
		}

		if(0 != m_hTalkHandle)
		{
			//Stop audio talk to the device.
			BOOL bSuccess = CLIENT_StopTalkEx(m_hTalkHandle);
			if(bSuccess)
			{
				m_hTalkHandle = 0;
				GetDlgItem(IDC_BTN_STARTTALK)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_STOPTALK)->EnableWindow(FALSE);
			}
			else
			{
				MessageBox(ConvertString("Stop talk failed!"), ConvertString("Prompt"));
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Please login first!"), ConvertString("Prompt"));
	}
}

void CALLBACK AudioCallFunction(LPBYTE pDataBuffer, DWORD DataLength, long nUser)
{
	if(nUser == 0)
	{
		return;
	}
	CServerDlg *dlg = (CServerDlg *)nUser;
	dlg->AudioRecord(pDataBuffer, DataLength);
}

void CServerDlg::AudioRecord(LPBYTE pDataBuffer, DWORD DataLength)
{
	char* pCbData = NULL;
	pCbData = new char[DataLength+100];
	if (NULL == pCbData) 
	{
		return;
	}
	int  iCbLen = 0;
	
	if (m_curTalkMode.encodeType == DH_TALK_DEFAULT || m_curTalkMode.encodeType == DH_TALK_PCM)	
	{
		if (m_curTalkMode.nAudioBit == 8)
		{
			for( int j = 0 ; j < DataLength; j++)
			{
				*(pDataBuffer + j) += 128;
			}
		}

		pCbData[0]=0x00;
		pCbData[1]=0x00;
		pCbData[2]=0x01;
		pCbData[3]=0xF0;
		
		pCbData[4]=m_curTalkMode.nAudioBit==8?0x07:0x0C;
		pCbData[5]=0x02;//8k
		*(DWORD*)(pCbData+6)=DataLength;
		memcpy(pCbData+8, pDataBuffer, DataLength);
		
		iCbLen = 8+DataLength;
	}
	else if (m_curTalkMode.encodeType == DH_TALK_G711a)
	{
		if (g711a_Encode((char*)pDataBuffer, pCbData+8, DataLength, &iCbLen) != 1)
		{
			goto end;
		}
		
		//Dahua bit stream format frame head 
		pCbData[0]=0x00;
		pCbData[1]=0x00;
		pCbData[2]=0x01;
		pCbData[3]=0xF0;

		pCbData[4]=0x0E; //G711A
		pCbData[5]=0x02;
		pCbData[6]=BYTE(iCbLen&0xff);
		pCbData[7]=BYTE(iCbLen>>8);
		
		iCbLen += 8;
	}
	else if (m_curTalkMode.encodeType == DH_TALK_AMR)
	{
		//AMR encode
		iCbLen = AMR_Encode_Frame(m_hisEncodeHandle,(HI_S16*)pDataBuffer,
			(HI_U8*)(pCbData+8),MR102,MIME);
		if (iCbLen <= 0)
		{
			goto end;
		}
		
		pCbData[0]=0x00;
		pCbData[1]=0x00;
		pCbData[2]=0x01;
		pCbData[3]=0xF0;
		
		pCbData[4]=0x14; //AMR
		pCbData[5]=0x02;
		pCbData[6]=BYTE(iCbLen&0xff);
		pCbData[7]=BYTE(iCbLen>>8);

		iCbLen += 8;
	}
	else if (m_curTalkMode.encodeType == DH_TALK_G711u)
	{
		if (g711u_Encode((char*)pDataBuffer, pCbData+8, DataLength, &iCbLen) != 1)
		{
			goto end;
		}
		
		//Dahua bit stream format frame head 
		pCbData[0]=0x00;
		pCbData[1]=0x00;
		pCbData[2]=0x01;
		pCbData[3]=0xF0;

		pCbData[4]=0x0A; //G711u
		pCbData[5]=0x02;
		pCbData[6]=BYTE(iCbLen&0xff);
		pCbData[7]=BYTE(iCbLen>>8);
		
		iCbLen += 8;
	}
	else
	{
		goto end;
	}

	// Send the data from the PC to DVR
	CLIENT_TalkSendData(m_hTalkHandle, (char *)pCbData, iCbLen);

end:
	if (pCbData != NULL)
	{
		delete[] pCbData;
	}
}

BOOL CServerDlg::StartAudioRecord()
{
	//	First confirm decode port.DH_TALK_DEFAULT is 100 port number and then rest is 99 port number.
	int nPort = 99;
	if (m_curTalkMode.encodeType == DH_TALK_DEFAULT)
	{
		nPort = 100;
	}

	//	Then specify frame length 
	int nFrameLength = 1024;
	switch(m_curTalkMode.encodeType)
	{
	case DH_TALK_DEFAULT:
	case DH_TALK_PCM:
		nFrameLength = 1024;
		break;
	case DH_TALK_G711a:
		nFrameLength = 1280;
		break;
	case DH_TALK_AMR:
		nFrameLength = 320;
	    break;
	case DH_TALK_G711u:
		nFrameLength = 320;
	    break;
	case DH_TALK_G726:
		nFrameLength = 320;
	    break;
	default:
	    break;
	}

	BOOL bRet = FALSE;

	//	Then call PLAYSDK library to begin recording audio 
	bool bOpenRet = PLAY_OpenStream(nPort,0,0,1024*100);
	if(bOpenRet)
	{
		BOOL bPlayRet = PLAY_Play(nPort,0);
		if(bPlayRet)
		{
			PLAY_PlaySoundShare(nPort);
			BOOL bSuccess = PLAY_OpenAudioRecord(AudioCallFunction,m_curTalkMode.nAudioBit,m_curTalkMode.dwSampleRate,nFrameLength,0,(DWORD)this);
			if(bSuccess)
			{
				bRet = TRUE;
			}
			else
			{
				PLAY_StopSoundShare(nPort);
				PLAY_Stop(nPort);
				PLAY_CloseStream(nPort);
			}
		}
		else
		{
			PLAY_CloseStream(nPort);
		}
	}

	return bRet;
}

void CServerDlg::ShowLoginErrorReason(int nError)
{
	if(1 == nError)		MessageBox(ConvertString("incorrect password!"), ConvertString("Prompt"));
	else if(2 == nError)	MessageBox(ConvertString("no this account! "), ConvertString("Prompt"));
	else if(3 == nError)	MessageBox(ConvertString("time out!"), ConvertString("Prompt"));
	else if(4 == nError)	MessageBox(ConvertString("account have been logined!"), ConvertString("Prompt"));
	else if(5 == nError)	MessageBox(ConvertString("account have been locked!"), ConvertString("Prompt"));
	else if(6 == nError)	MessageBox(ConvertString("The user has listed into illegal!"), ConvertString("Prompt"));
	else if(7 == nError)	MessageBox(ConvertString("The system is busy!"), ConvertString("Prompt"));
	else if(9 == nError)	MessageBox(ConvertString("You Can't find the network server!"), ConvertString("Prompt"));
	else	MessageBox(ConvertString("Login falied!"), ConvertString("Prompt"));
}

BOOL CServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_IPADDRESS_IP, "10.7.4.24");
	SetDlgItemText(IDC_EDIT_PORT, "37777");
	SetDlgItemText(IDC_EDIT_USER, "3");
	UpdateData(FALSE);
	
	if (HI_SUCCESS != AMR_Encode_Init(&m_hisEncodeHandle, 0)) 
	{
		m_hisEncodeHandle = 0;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_hisEncodeHandle != NULL) 
	{
		AMR_Encode_Exit(&m_hisEncodeHandle);
	}

	if(m_bOpenAudioRecord)
	{
		PLAY_CloseAudioRecord();
		PLAY_Stop(100);
		PLAY_Stop(99);
		PLAY_StopSoundShare(100);
		PLAY_StopSoundShare(99);
		PLAY_CloseStream(100);
		PLAY_CloseStream(99);
	}

	if(0 != m_hTalkHandle)
	{
		CLIENT_StopTalkEx(m_hTalkHandle);
	}

	if (m_lLoginHandle != 0)
	{
		CLIENT_Logout(m_lLoginHandle);
	}
}










































