// ConfigChannel.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "ConfigChannel.h"
#include "ClientDemo5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigChannel dialog


CConfigChannel::CConfigChannel(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigChannel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigChannel)
	m_nBrightness = 0;
	m_nContrast = 0;
	m_nSaturation = 0;
	m_nHue = 0;
	m_nGain = 0;
	m_bAudioOverlay = FALSE;
	m_nNoise = 0;
	m_nVolume = 0;
	m_bVolumeEn = FALSE;
	//}}AFX_DATA_INIT
	m_nChannelCount = 0;
	m_nCurChannel = -1;
	m_nCurStream = -1;
	m_nCurOSD = -1;
	m_nProtoVer = 4;
	m_nVideoStandard = 0;
	m_bNewProtocol = FALSE;
}


void CConfigChannel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigChannel)
	DDX_Control(pDX, IDC_COMBO_BLINDTYPE, m_ctlBlindType);
	DDX_Control(pDX, IDC_CHECK_OSDSHOW, m_ctlOSDShow);
	DDX_Control(pDX, IDC_COMBO_OSD, m_ctlOSD);
	DDX_Control(pDX, IDC_COMBO_AUDIOFMT, m_ctlAudioFmt);
	DDX_Control(pDX, IDC_COMBO_IMGQUALITY, m_ctlImageQuality);
	DDX_Control(pDX, IDC_COMBO_FRAME, m_ctlFrame);
	DDX_Control(pDX, IDC_COMBO_BRCTRL, m_ctlBRCtl);
	DDX_Control(pDX, IDC_CHECK_GAIN, m_ctlGain);
	DDX_Control(pDX, IDC_CHECK_AUDIO, m_ctlAudio);
	DDX_Control(pDX, IDC_CHECK_VIDEO, m_ctlVideo);
	DDX_Control(pDX, IDC_COMBO_STREAM, m_ctlStream);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_ctlResolution);
	DDX_Control(pDX, IDC_COMBO_ENCODETYPE, m_ctlEncodeType);
	DDX_Control(pDX, IDC_COMBO_CHANNO, m_ctlChannel);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_nBrightness);
	DDV_MinMaxUInt(pDX, m_nBrightness, 0, 100);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_nContrast);
	DDV_MinMaxUInt(pDX, m_nContrast, 0, 100);
	DDX_Text(pDX, IDC_EDIT_SATURATION, m_nSaturation);
	DDV_MinMaxUInt(pDX, m_nSaturation, 0, 100);
	DDX_Text(pDX, IDC_EDIT_HUE, m_nHue);
	DDV_MinMaxUInt(pDX, m_nHue, 0, 100);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_nGain);
	DDV_MinMaxUInt(pDX, m_nGain, 0, 100);
	DDX_Check(pDX, IDC_CHECK_AUDIO_OVERLAY, m_bAudioOverlay);
	DDX_Text(pDX, IDC_EDIT_NOISE, m_nNoise);
	DDX_Text(pDX, IDC_EDIT_VOLUME, m_nVolume);
	DDX_Check(pDX, IDC_CHECK_VOLUME, m_bVolumeEn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigChannel, CDialog)
	//{{AFX_MSG_MAP(CConfigChannel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_READALL, OnButtonReadall)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNO, OnSelchangeComboChanno)
	ON_CBN_SELCHANGE(IDC_COMBO_STREAM, OnSelchangeComboStream)
	ON_CBN_SELCHANGE(IDC_COMBO_OSD, OnSelchangeComboOsd)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigChannel message handlers

//Save
void CConfigChannel::OnButtonApply() 
{
	BOOL bSuccess = UpdateData(TRUE);
	if (!bSuccess)
	{
		return;
	}

	// TODO: Add your control notification handler code here
	if(-1 != m_ctlChannel.GetCurSel())
	{
		SaveChannelInfo(m_nCurChannel);

		// Verify resolution and frame rate. It shall below the DSP encode capacity.
		if (m_bNewProtocol == FALSE)
		{
			if (IsValidForInput())
			{
				//Call father window to save data
				((CClientDemo5Dlg*)AfxGetMainWnd())->SetChannelConfigInfo(m_channelInfo,m_nChannelCount);
			}
		}
		else
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->SetOneChannelConfigInfo(&m_channelInfo[m_nCurChannel], m_nCurChannel);
		}	
	}
	else
	{
		MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
	}
}

//Read
void CConfigChannel::OnButtonReadall() 
{
	// TODO: Add your control notification handler code here
	//First clear
	CleanAll();
	//Call father window to read data 
	((CClientDemo5Dlg*)AfxGetMainWnd())->GetChannelConfigInfo();
}

/*
 *	Function:display channel video information.
 *	Please pay attention to the resolution position and frame rate position
 *	Note：DHDEV_DSP_ENCODECAP is DSP capacity information. dwMaxEncodePower means the highest encode capacity DSP supported. wMaxSupportChannel means the max video channel each DSP supported.
 *	wChannelMaxSetSync measn synchronization or not. 0-not synchronized (DSP each channel resolution may not be the same.), 1-synchronized. (The modification may fail if DSP each channel resolution is not the same. )
 *	Resolution*Frame rate(channel 0) + Resolution*frame rate(channel 1) + Resolution*frame rate(channel 2) + Resolution*Frame rate(channel 3) <= dwMaxEncodePower
 */
void CConfigChannel::InitDlgInfo(DHDEV_CHANNEL_CFG *pChannelInfo, int nChannelCount, int nVideoStandard, DHDEV_DSP_ENCODECAP &stDspInfo, int nProtoVer, BOOL bNewProtocol)
{
	m_stDspInfo = stDspInfo;
	m_nProtoVer = nProtoVer;
	m_nVideoStandard = nVideoStandard;
	m_bNewProtocol = bNewProtocol;
	
	int nIndex = 0;
	CString str;

	// Frame rate dropdown menu(Depends on video format and protocol version)
	m_ctlFrame.ResetContent();
	if (nProtoVer >= 5)
	{
		if (0 == nVideoStandard)		//PAL
		{
			for (int i = 1; i <= 25; i++)
			{
				str.Format("%d", i);
				nIndex = m_ctlFrame.AddString(str);
				m_ctlFrame.SetItemData(nIndex, i);
			}
		}
		else if (1 == nVideoStandard)	//NTSC
		{
			for (int i = 1; i <= 30; i++)
			{
				str.Format("%d", i);
				nIndex = m_ctlFrame.AddString(str);
				m_ctlFrame.SetItemData(nIndex, i);
			}
		}
	}
	else
	{
		if (0 == nVideoStandard)		//PAL
		{
			nIndex = m_ctlFrame.AddString("1");
			m_ctlFrame.SetItemData(nIndex, 0);
			nIndex = m_ctlFrame.AddString("2");
			m_ctlFrame.SetItemData(nIndex, 1);
			nIndex = m_ctlFrame.AddString("3");
			m_ctlFrame.SetItemData(nIndex, 2);
			nIndex = m_ctlFrame.AddString("6");
			m_ctlFrame.SetItemData(nIndex, 3);
			nIndex = m_ctlFrame.AddString("12");
			m_ctlFrame.SetItemData(nIndex, 4);
			nIndex = m_ctlFrame.AddString("25");
			m_ctlFrame.SetItemData(nIndex, 5);
		}
		else if (1 == nVideoStandard)	//NTSC
		{
			nIndex = m_ctlFrame.AddString("1");
			m_ctlFrame.SetItemData(nIndex, 0);
			nIndex = m_ctlFrame.AddString("2");
			m_ctlFrame.SetItemData(nIndex, 1);
			nIndex = m_ctlFrame.AddString("4");
			m_ctlFrame.SetItemData(nIndex, 2);
			nIndex = m_ctlFrame.AddString("7");
			m_ctlFrame.SetItemData(nIndex, 3);
			nIndex = m_ctlFrame.AddString("15");
			m_ctlFrame.SetItemData(nIndex, 4);
			nIndex = m_ctlFrame.AddString("20");
			m_ctlFrame.SetItemData(nIndex, 5);
			nIndex = m_ctlFrame.AddString("30");
			m_ctlFrame.SetItemData(nIndex, 6);
		}
	}
	
	// Channel name dropdown menu 
	m_ctlChannel.ResetContent();
	m_nChannelCount = nChannelCount>32?32:nChannelCount;
	for(int i = 0; i < m_nChannelCount; i++)
	{
		str.Format("%d",i);
		nIndex = m_ctlChannel.AddString(str);
		m_ctlChannel.SetItemData(nIndex,i);
		//Save channel video information 
		m_channelInfo[i] = pChannelInfo[i];
	}

	//m_ctlOSD
	m_ctlOSD.ResetContent();
	if (nChannelCount > 16)
	{
		char *OSDName[8] = {"Channel OSD", "Time OSD"};
		for (int k = 0; k < 2; k++)
		{
			int nIndex = m_ctlOSD.AddString(ConvertString(OSDName[k]));
			m_ctlEncodeType.SetItemData(nIndex,k);
		}
	}
	else
	{
		char *OSDName[8] = {"Channel OSD", "Time OSD", "Area Shelter"};
		for (int k = 0; k < 3; k++)
		{
			int nIndex = m_ctlOSD.AddString(ConvertString(OSDName[k]));
			m_ctlEncodeType.SetItemData(nIndex,k);
		}
	}
	m_ctlOSD.SetCurSel(0);
	
	// Set dropdown menu in decode mode by encode subnet mask setup.
	char *ecName[8] = {"MPEG4", "MS-MPEG4", "MPEG2", "MPEG1", "H.263", "MJPG", "FCC-MPEG4", "H.264"};
	m_ctlEncodeType.ResetContent();
	for (int j = 0; j < 8; j++)
	{
		//dwEncodeModeMask is encode mode subnet mask. Each bit means the encode mode setup the device supported. 
		if (m_stDspInfo.dwEncodeModeMask & (0x01<<j))
		{
			int nIndex = m_ctlEncodeType.AddString(ecName[j]);
			m_ctlEncodeType.SetItemData(nIndex,j);
		}
	}

	// If synchronized, the channel resolution in each DSP shall be the same.
	if (1 == m_stDspInfo.wChannelMaxSetSync)
	{
		CString strText = ConvertString("(notice:");
		for (int i = 0; i < m_nChannelCount/m_stDspInfo.wMaxSupportChannel; i++)
		{
			CString str;
			str.Format(ConvertString(" %d~%dchannel"), i*m_stDspInfo.wMaxSupportChannel, i*m_stDspInfo.wMaxSupportChannel+m_stDspInfo.wMaxSupportChannel-1);
			strText = strText + str;
		}
		strText += ConvertString(" demand the same sampling rate!)");
		SetDlgItemText(IDC_STATIC_SHOW, strText);
	}

	//Select channel 0
	if(0 < m_ctlChannel.GetCount())
	{
		m_ctlChannel.SetCurSel(0);
		OnSelchangeComboChanno();
	}
}

//Channel switch 
void CConfigChannel::OnSelchangeComboChanno() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = (int)m_ctlChannel.GetItemData(nIndex);

		//First save channel information 
		SaveChannelInfo(m_nCurChannel);

		if (m_bNewProtocol == TRUE)//32路配置取单个通道
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->GetOneChannelConfigInfo(&m_channelInfo[nNum], nIndex);
		}
		
		//Display channel related information 
		DHDEV_CHANNEL_CFG Info = m_channelInfo[nNum];

		//Set channel name
		SetDlgItemText(IDC_EDIT_CHANNAME,Info.szChannelName);

		//Set bit stream
		m_ctlStream.ResetContent();
		int nIndex = m_ctlStream.AddString(ConvertString("Main Data stream"));
		m_ctlStream.SetItemData(nIndex,0);

		//Check there is extra stream in accordance with capacity. 
		if (m_stDspInfo.dwStreamCap & (0x01<<1))
		{
			nIndex = m_ctlStream.AddString(ConvertString("Sub Data stream"));
			m_ctlStream.SetItemData(nIndex,3);
		}

		m_ctlStream.SetCurSel(0);
		OnSelchangeComboStream();

		//Set video color property 
		m_nBrightness = Info.stColorCfg[0].byBrightness;
		m_nSaturation = Info.stColorCfg[0].bySaturation;
		m_nContrast = Info.stColorCfg[0].byContrast;
		m_nHue = Info.stColorCfg[0].byHue;
		m_nGain = Info.stColorCfg[0].byGain;
		m_ctlGain.SetCheck(Info.stColorCfg[0].byGainEn?1:0);

		//Noise
		m_nNoise = Info.bNoise;
		m_nVolume = Info.bVolume;
		m_bVolumeEn = Info.bVolumeEnable;

		UpdateData(FALSE);
		//OSD type 
		if (m_ctlOSD.GetCount() > 0)
		{
			// Set ChannelName_OSD
			m_ctlOSD.SetCurSel(0);
		}
		OnSelchangeComboOsd();

		m_nCurChannel = nNum;
	}
}

//Bit stream switch 
void CConfigChannel::OnSelchangeComboStream() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlStream.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nItem = m_ctlStream.GetItemData(nIndex);
		nIndex = m_ctlChannel.GetCurSel();
		if(CB_ERR != nIndex)
		{
			int nNum = m_ctlChannel.GetItemData(nIndex);
			//First save current bit stream information 
			SaveStreamInfo(m_nCurChannel,m_nCurStream);
			m_nCurStream = nItem;
			//Display bit stream information 
			ShowStreamInfo(nNum,nItem);
		}
	}
}

//Save bit stream information 
void CConfigChannel::SaveStreamInfo(int nNum, int nItem)
{
	DH_VIDEOENC_OPT *pStreamInfo = NULL;
	if(0 <= nItem && 3 >nItem)
	{
		pStreamInfo = &(m_channelInfo[nNum].stMainVideoEncOpt[nItem]);
	}
	else if(3 <= nItem && 6 >nItem)
	{
		pStreamInfo = &(m_channelInfo[nNum].stAssiVideoEncOpt[nItem-3]);
	}
	if(NULL != pStreamInfo)
	{
		pStreamInfo->byVideoEnable = m_ctlVideo.GetCheck();
		pStreamInfo->byAudioEnable = m_ctlAudio.GetCheck();
		pStreamInfo->byBitRateControl = m_ctlBRCtl.GetCurSel();
		int nIndex = m_ctlFrame.GetCurSel();
		pStreamInfo->byFramesPerSec = (BYTE)m_ctlFrame.GetItemData(nIndex);

		//CBR. It begins with 0. the value ranges from 50~4*1024 (K)
		pStreamInfo->wLimitStream = GetDlgItemInt(IDC_EDIT_LIMITSTREAM);

		//I frame interval value 
		pStreamInfo->bIFrameInterval = GetDlgItemInt(IDC_EDIT_IFRAME);
		
		//Decode mode 
		nIndex = m_ctlEncodeType.GetCurSel();
		pStreamInfo->byEncodeMode = (BYTE)m_ctlEncodeType.GetItemData(nIndex);

		//Resolution 
		nIndex = m_ctlResolution.GetCurSel();
		pStreamInfo->byImageSize = (BYTE)m_ctlResolution.GetItemData(nIndex);

		//Image quality 
		pStreamInfo->byImageQlty = m_ctlImageQuality.GetCurSel() + 1;
		//Audio encode 
		pStreamInfo->wFormatTag = m_ctlAudioFmt.GetCurSel();
		//Track amount 
		pStreamInfo->nChannels = GetDlgItemInt(IDC_EDIT_TRACKNUM);
		//Sampling depth 
		pStreamInfo->wBitsPerSample = GetDlgItemInt(IDC_EDIT_BITPERSMPL);
		//Sampling interval 
		pStreamInfo->nSamplesPerSec = GetDlgItemInt(IDC_EDIT_SMPLPERSEC);
		//Audio overlay
		pStreamInfo->bAudioOverlay = m_bAudioOverlay;
	}
}

//Save channel information 
void CConfigChannel::SaveChannelInfo(int nNum)
{
	if(-1 < nNum && -1 < m_nCurStream && -1 < m_nCurOSD)
	{
		DHDEV_CHANNEL_CFG *pInfo = &(m_channelInfo[nNum]);
		if(NULL != pInfo)
		{
			//Channel name 
			CString str = "";
			GetDlgItemText(IDC_EDIT_CHANNAME,str);
			strcpy(pInfo->szChannelName,str);
			
			//Video color property 
			if (m_nBrightness >= 0 && m_nBrightness <= 100) 
			{
				pInfo->stColorCfg[0].byBrightness = m_nBrightness;
			}

			if (m_nSaturation >=0 && m_nSaturation <= 100) 
			{
				pInfo->stColorCfg[0].bySaturation = m_nSaturation;
			}

			if (m_nContrast >= 0 && m_nContrast <= 100) 
			{
				pInfo->stColorCfg[0].byContrast = m_nContrast;
			}

			if (m_nHue >= 0 && m_nHue <= 100) 
			{
				pInfo->stColorCfg[0].byHue = m_nHue;
			}
			
			if (m_nGain >= 0 && m_nGain <= 100)
			{
				pInfo->stColorCfg[0].byGain = m_nGain;
			}

			pInfo->stColorCfg[0].byGainEn = m_ctlGain.GetCheck();
			//Save bit stream information 
			SaveStreamInfo(nNum,m_nCurStream);
			//Save OSD type informaiton
			SaveOSDInfo(nNum,m_nCurOSD);

			//Noise
			pInfo->bNoise = m_nNoise;
			pInfo->bVolume = m_nVolume;
			pInfo->bVolumeEnable = m_bVolumeEn;
		}
	}
}

//Save OSD type information
void CConfigChannel::SaveOSDInfo(int nNum, int nItem)
{
	if(-1 < nNum && -1 < nItem)
	{
		DH_ENCODE_WIDGET *pOSDInfo = NULL;
		if(0 == nItem)
		{
			pOSDInfo = &(m_channelInfo[nNum].stChannelOSD);
		}
		else if(1 == nItem)
		{
			pOSDInfo = &(m_channelInfo[nNum].stTimeOSD);
		}
		else if(2 == nItem)
		{
			pOSDInfo = &(m_channelInfo[nNum].stBlindCover[0]);
			m_channelInfo[nNum].byBlindEnable = m_ctlBlindType.GetCurSel();
		}
		if(NULL != pOSDInfo)
		{
			pOSDInfo->bShow = m_ctlOSDShow.GetCheck();
			DWORD dwTemp = (DWORD)GetDlgItemInt(IDC_EDIT_FORE_TRANS);
			dwTemp = dwTemp << 24;
			dwTemp = dwTemp + ((DWORD)GetDlgItemInt(IDC_EDIT_FORE_BLUE) << 16);
			dwTemp = dwTemp + ((DWORD)GetDlgItemInt(IDC_EDIT_FORE_GREEN) << 8);
			dwTemp = dwTemp + (DWORD)GetDlgItemInt(IDC_EDIT_FORE_RED);
			pOSDInfo->rgbaFrontground = dwTemp;

			dwTemp = (DWORD)GetDlgItemInt(IDC_EDIT_BACK_TRANS);
			dwTemp = dwTemp << 24;
			dwTemp = dwTemp + ((DWORD)GetDlgItemInt(IDC_EDIT_BACK_BLUE) << 16);
			dwTemp = dwTemp + ((DWORD)GetDlgItemInt(IDC_EDIT_BACK_GREEN) << 8);
			dwTemp = dwTemp + (DWORD)GetDlgItemInt(IDC_EDIT_BACK_RED);
			pOSDInfo->rgbaBackground = dwTemp;

			pOSDInfo->rcRect.left = (long)GetDlgItemInt(IDC_EDIT_RECT_LFET);
			pOSDInfo->rcRect.top = (long)GetDlgItemInt(IDC_EDIT_RECT_TOP);
			pOSDInfo->rcRect.right = (long)GetDlgItemInt(IDC_EDIT_RECT_RIGHT);
			pOSDInfo->rcRect.bottom = (long)GetDlgItemInt(IDC_EDIT_RECT_BOTTOM);
		}
	}
}

//Display bit stream information 
void CConfigChannel::ShowStreamInfo(int nNum, int nItem)
{
	DHDEV_CHANNEL_CFG Info = m_channelInfo[nNum];
	DH_VIDEOENC_OPT StreamInfo = {0};
	DWORD dwImageSizeMask = 0;
	if(0 <= nItem && 3 >nItem)
	{
		StreamInfo = Info.stMainVideoEncOpt[nItem];
		dwImageSizeMask = m_stDspInfo.dwImageSizeMask;
	}
	else if(3 <= nItem && 6 >nItem)
	{
		StreamInfo = Info.stAssiVideoEncOpt[nItem-3];
		BYTE bImageSize = Info.stMainVideoEncOpt[0].byImageSize;
		dwImageSizeMask = m_stDspInfo.dwImageSizeMask_Assi[bImageSize];
	}

	// Resolution dropdown menu (dwImageSizeMask is resolution subnet mask. Each bit means the resolution setup the device supported.)
	m_ctlResolution.ResetContent();
	char *rsName[20] = {"D1", "HD1", "BCIF", "CIF", "QCIF", "VGA", "QVGA", "SVCD", "QQVGA", 
		"SVGA", "XVGA", "WXGA", "SXGA", "WSXGA", "UXGA", "WUXGA", "LFT", "720", "1080", "1_3M"};
	int i = 0;
	for (i = 0; i < 20; i++)
	{
		if (dwImageSizeMask & (0x01<<i))
		{
			int nIndex = m_ctlResolution.AddString(rsName[i]);
			m_ctlResolution.SetItemData(nIndex,i);
		}
	}

	//Enable video 
	m_ctlVideo.SetCheck(StreamInfo.byVideoEnable);

	//Enable audio 
	m_ctlAudio.SetCheck(StreamInfo.byAudioEnable);

	SetDlgItemInt(IDC_EDIT_LIMITSTREAM, StreamInfo.wLimitStream);
	SetDlgItemInt(IDC_EDIT_IFRAME, StreamInfo.bIFrameInterval);

	//Bit stream control
	m_ctlBRCtl.SetCurSel(StreamInfo.byBitRateControl);

	//Frame rate 
	if (m_nProtoVer >= 5)
	{
		m_ctlFrame.SetCurSel(StreamInfo.byFramesPerSec-1);
	}
	else
	{
		m_ctlFrame.SetCurSel(StreamInfo.byFramesPerSec);
	}
	
	//Decode mode 
	int nItemCount = m_ctlEncodeType.GetCount();
	for(i = 0; i < nItemCount; i++)
	{
		BYTE nTagData = (BYTE)(m_ctlEncodeType.GetItemData(i));
		if(nTagData == StreamInfo.byEncodeMode)
		{
			m_ctlEncodeType.SetCurSel(i);
			break;
		}
	}
	
	//Resolution 
	nItemCount = m_ctlResolution.GetCount();
	for(i = 0; i < nItemCount; i++)
	{
		BYTE nTagData = (BYTE)(m_ctlResolution.GetItemData(i));
		if(nTagData == StreamInfo.byImageSize)
		{
			m_ctlResolution.SetCurSel(i);
			break;
		}
	}

	//Video quality 
	m_ctlImageQuality.SetCurSel(StreamInfo.byImageQlty - 1);

	//Audio encode 
	BYTE nAudioFmt = StreamInfo.wFormatTag;
	if(0 == nAudioFmt)
	{
		m_ctlAudioFmt.SetCurSel(0);
	}
	else
	{
		m_ctlAudioFmt.SetCurSel(-1);
	}
	//Track amount 
	SetDlgItemInt(IDC_EDIT_TRACKNUM,StreamInfo.nChannels);
	//Sampling depth 
	SetDlgItemInt(IDC_EDIT_BITPERSMPL,StreamInfo.wBitsPerSample);
	//Sampling rate 
	SetDlgItemInt(IDC_EDIT_SMPLPERSEC,StreamInfo.nSamplesPerSec);

	m_bAudioOverlay = StreamInfo.bAudioOverlay;
}

//OSD type switch 
void CConfigChannel::OnSelchangeComboOsd() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = (int)m_ctlChannel.GetItemData(nIndex);
		//First save OSD type information 
		SaveOSDInfo(m_nCurChannel,m_nCurOSD);
		//Display OSD related informaiton
		DHDEV_CHANNEL_CFG Info = m_channelInfo[nNum];
		int nOSD = m_ctlOSD.GetCurSel();
		m_nCurOSD = nOSD;
		//OSD property structure 
		DH_ENCODE_WIDGET DH_OSD = {0};
		if(0 == nOSD)
		{
			DH_OSD = Info.stChannelOSD;
			GetDlgItem(IDC_STATIC_BLINDTYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_BLINDTYPE)->ShowWindow(SW_HIDE);
		}
		else if(1 == nOSD)
		{
			DH_OSD = Info.stTimeOSD;
			GetDlgItem(IDC_STATIC_BLINDTYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_BLINDTYPE)->ShowWindow(SW_HIDE);
		}
		else if(2 == nOSD)
		{
			DH_OSD = Info.stBlindCover[0];
			GetDlgItem(IDC_STATIC_BLINDTYPE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_BLINDTYPE)->ShowWindow(SW_SHOW);
			m_ctlBlindType.SetCurSel(Info.byBlindEnable);
		}

		m_ctlOSDShow.SetCheck(DH_OSD.bShow);
		SetDlgItemInt(IDC_EDIT_FORE_RED, DH_OSD.rgbaFrontground & 0xFF);
		SetDlgItemInt(IDC_EDIT_FORE_GREEN, (DH_OSD.rgbaFrontground>>8) & 0xFF);
		SetDlgItemInt(IDC_EDIT_FORE_BLUE, (DH_OSD.rgbaFrontground>>16) & 0xFF);
		SetDlgItemInt(IDC_EDIT_FORE_TRANS, (DH_OSD.rgbaFrontground>>24) & 0xFF);
		
		SetDlgItemInt(IDC_EDIT_BACK_RED, DH_OSD.rgbaBackground & 0xFF);
		SetDlgItemInt(IDC_EDIT_BACK_GREEN, (DH_OSD.rgbaBackground>>8) & 0xFF);
		SetDlgItemInt(IDC_EDIT_BACK_BLUE, (DH_OSD.rgbaBackground>>16) & 0xFF);
		SetDlgItemInt(IDC_EDIT_BACK_TRANS, (DH_OSD.rgbaBackground>>24) & 0xFF);

		SetDlgItemInt(IDC_EDIT_RECT_LFET, DH_OSD.rcRect.left);
		SetDlgItemInt(IDC_EDIT_RECT_TOP, DH_OSD.rcRect.top);
		SetDlgItemInt(IDC_EDIT_RECT_RIGHT, DH_OSD.rcRect.right);
		SetDlgItemInt(IDC_EDIT_RECT_BOTTOM, DH_OSD.rcRect.bottom);
	}
}

//Clear 
void CConfigChannel::CleanAll()
{
	m_ctlChannel.SetCurSel(-1);
	m_ctlStream.SetCurSel(-1);
	m_ctlBRCtl.SetCurSel(-1);
	m_ctlFrame.SetCurSel(-1);
	m_ctlEncodeType.SetCurSel(-1);
	m_ctlResolution.SetCurSel(-1);
	m_ctlImageQuality.SetCurSel(-1);
	m_ctlAudioFmt.SetCurSel(-1);
	m_ctlOSD.SetCurSel(-1);

	SetDlgItemText(IDC_EDIT_CHANNAME,"");
	SetDlgItemText(IDC_EDIT_TRACKNUM,"");
	SetDlgItemText(IDC_EDIT_BITPERSMPL,"");
	SetDlgItemText(IDC_EDIT_SMPLPERSEC,"");
	SetDlgItemText(IDC_EDIT_BRIGHTNESS,"");
	SetDlgItemText(IDC_EDIT_CONTRAST,"");
	SetDlgItemText(IDC_EDIT_SATURATION,"");
	SetDlgItemText(IDC_EDIT_HUE,"");
	SetDlgItemText(IDC_EDIT_GAIN,"");
	SetDlgItemText(IDC_EDIT_FORE_RED, "");
	SetDlgItemText(IDC_EDIT_FORE_GREEN, "");
	SetDlgItemText(IDC_EDIT_FORE_BLUE, "");
	SetDlgItemText(IDC_EDIT_FORE_TRANS, "");
	SetDlgItemText(IDC_EDIT_BACK_RED, "");
	SetDlgItemText(IDC_EDIT_BACK_GREEN, "");
	SetDlgItemText(IDC_EDIT_BACK_BLUE, "");
	SetDlgItemText(IDC_EDIT_BACK_TRANS, "");
	SetDlgItemText(IDC_EDIT_RECT_LFET, "");
	SetDlgItemText(IDC_EDIT_RECT_TOP, "");
	SetDlgItemText(IDC_EDIT_RECT_RIGHT, "");
	SetDlgItemText(IDC_EDIT_RECT_BOTTOM, "");
	SetDlgItemText(IDC_STATIC_SHOW, "");
	
	m_ctlVideo.SetCheck(0);
	m_ctlAudio.SetCheck(0);
	m_ctlGain.SetCheck(0);
	m_ctlOSDShow.SetCheck(0);

	GetDlgItem(IDC_STATIC_BLINDTYPE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_BLINDTYPE)->ShowWindow(SW_HIDE);

	m_nChannelCount = 0;
	m_nCurChannel = -1;
	m_nCurStream = -1;
	m_nCurOSD = -1;
}

/*
 *	Summary：Verify resolution and frame rate
 */
BOOL CConfigChannel::IsValidForInput()
{
	DWORD dwMaxEncodePower = m_stDspInfo.dwMaxEncodePower;
	WORD wMaxSupportChannel = m_stDspInfo.wMaxSupportChannel;
	WORD wChannelMaxSetSync = m_stDspInfo.wChannelMaxSetSync;
	BOOL bDoubleFrame = (m_stDspInfo.dwStreamCap & (0x01<<1)) ? TRUE:FALSE;

	// Check it is synchronized or not. The resolution shall be the same if you want to be synchronized. 
	if (wChannelMaxSetSync == 1)
	{
		for (int i = 0; i < m_nChannelCount/wMaxSupportChannel; i++)
		{
			int j = 0;
			BYTE byMainImageSize = m_channelInfo[i*wMaxSupportChannel].stMainVideoEncOpt[0].byImageSize;
			for (j = 1; j < wMaxSupportChannel; j++)
			{
				if (byMainImageSize != m_channelInfo[i*wMaxSupportChannel+j].stMainVideoEncOpt[0].byImageSize)
				{
					MessageBox(ConvertString(" DSP is synchronization, demand channel of DSP have the same sampling rate!"), ConvertString("Prompt"));
					return FALSE;
				}
				
			}

			if (bDoubleFrame)
			{
				BYTE byAssiImageSize = m_channelInfo[i*wMaxSupportChannel].stAssiVideoEncOpt[0].byImageSize;
				for (j = 1; j < wMaxSupportChannel; j++)
				{
					if (byAssiImageSize != m_channelInfo[i*wMaxSupportChannel+j].stAssiVideoEncOpt[0].byImageSize)
					{
						MessageBox(ConvertString(" DSP is synchronization, demand channel of DSP have the same sampling rate!"), ConvertString("Prompt"));
						return FALSE;
					}
					
				}
			}
			
		}
	}
	
	// Check it exceeds the highest encode capacity 
	for (int i = 0; i < m_nChannelCount/wMaxSupportChannel; i++)
	{
		DWORD dwCurEncodePower = 0;
		for (int j = 0; j < wMaxSupportChannel; j++)
		{
			//	When there is only main stream, system adds up encode capacity without chekcing video has been enabled or not. When there is extra stream, system needs to check video has benn enabled or not when adding up the encode capacity.
			if (bDoubleFrame)
			{
				BYTE byImageSize = m_channelInfo[i*wMaxSupportChannel+j].stMainVideoEncOpt[0].byImageSize;
				BYTE byFramesPerSec = m_channelInfo[i*wMaxSupportChannel+j].stMainVideoEncOpt[0].byFramesPerSec;
				
				if (m_channelInfo[i*wMaxSupportChannel+j].stMainVideoEncOpt[0].byVideoEnable)
				{
					dwCurEncodePower += GetFramePerSec(byFramesPerSec) * GetFenbianLv(byImageSize);
				}

				byImageSize = m_channelInfo[i*wMaxSupportChannel+j].stAssiVideoEncOpt[0].byImageSize;
				byFramesPerSec = m_channelInfo[i*wMaxSupportChannel+j].stAssiVideoEncOpt[0].byFramesPerSec;
				
				if (m_channelInfo[i*wMaxSupportChannel+j].stAssiVideoEncOpt[0].byVideoEnable)
				{
					dwCurEncodePower += GetFramePerSec(byFramesPerSec) * GetFenbianLv(byImageSize);
				}
			}
			else
			{
				BYTE byImageSize = m_channelInfo[i*wMaxSupportChannel+j].stMainVideoEncOpt[0].byImageSize;
				BYTE byFramesPerSec = m_channelInfo[i*wMaxSupportChannel+j].stMainVideoEncOpt[0].byFramesPerSec;
				dwCurEncodePower += GetFramePerSec(byFramesPerSec) * GetFenbianLv(byImageSize);
			}
		}

		if (dwCurEncodePower > dwMaxEncodePower)
		{
			MessageBox(ConvertString("Sampling rate and frame rate too high to out of the bigest code ability of DSP!"), ConvertString("Prompt"));
			return FALSE;
		}
	}

	return TRUE;
}

DWORD CConfigChannel::GetFenbianLv(WORD wImageSize)
{
	int width = 1;
	int height = 1;
	if(m_nVideoStandard == 0)  //PAL
	{
		switch (wImageSize)
		{
			case 0: 			width = 704;	height = 576;		break;//D1
			case 1:				width = 352;	height = 576;		break;//HD1
			case 2:				width = 704;	height = 288;		break;//BCIF
			case 3:				width = 352;	height = 288;		break;//CIF
			case 4:				width = 176;	height = 144;		break;//QCIF
			case 5:				width = 640;	height = 480;		break;//VGA
			case 6:				width = 320;	height = 240;		break;//QVGA
			case 7:				width = 480;	height = 480;		break;//SVCD
			case 8:				width = 160;	height = 128;		break;//QQVGA
			case 9:				width = 1600;   height = 1200;		break;//UXGA
			case 10:			width = 1920;	height = 1200;		break;//WUXGA
			default:	ASSERT(FALSE);				break;
		}
	}		
	else
	{
		switch (wImageSize)
		{
			case 0:				width = 704;	height = 480;		break;//D1
			case 1:				width = 352;	height = 480;		break;//HD1
			case 2:				width = 704;	height = 240;		break;//BCIF
			case 3:				width = 352;	height = 240;		break;//CIF
			case 4:				width = 176;	height = 120;		break;//QCIF
			case 5:				width = 640;	height = 480;		break;//VGA
			case 6:				width = 320;	height = 240;		break;//QVGA
			case 7:				width = 480;	height = 480;		break;//SVCD
			case 8:				width = 160;	height = 128;		break;//QQVGA
			case 9:				width = 1600;   height = 1200;		break;//UXGA
			case 10:			width = 1920;	height = 1200;		break;//WUXGA
			default:	ASSERT(FALSE);				break;
		}
	}
	return width*height;
}

DWORD CConfigChannel::GetFramePerSec(WORD wFramePerSec)
{
	DWORD dwRet = 1;

	if (m_nProtoVer >= 5)
	{
		dwRet = wFramePerSec;
	}
	else
	{
		if (0 == m_nVideoStandard)		//PAL
		{
			switch(wFramePerSec)
			{
				case 0:
				case 1:
				case 2:			dwRet = wFramePerSec + 1;			break;
				case 3:			dwRet = 6;							break;
				case 4:			dwRet = 12;							break;
				case 5:			dwRet = 25;							break;
				default:							break;
			}
		}
		else if (1 == m_nVideoStandard)	//NTSC
		{
			switch(wFramePerSec)
			{
				case 0:
				case 1:			dwRet = wFramePerSec + 1;			break;
				case 2:			dwRet = 4;							break;
				case 3:			dwRet = 7;							break;
				case 4:			dwRet = 15;							break;
				case 5:			dwRet = 20;							break;
				case 6:			dwRet = 30;							break;
				default:							break;
			}
		}
	}

	return dwRet;
}

BOOL CConfigChannel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	ConvertComboBox(m_ctlImageQuality);
	ConvertComboBox(m_ctlBRCtl);
	ConvertComboBox(m_ctlOSD);
	ConvertComboBox(m_ctlBlindType);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigChannel::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}








































































































































































































































































































































































































































































































































