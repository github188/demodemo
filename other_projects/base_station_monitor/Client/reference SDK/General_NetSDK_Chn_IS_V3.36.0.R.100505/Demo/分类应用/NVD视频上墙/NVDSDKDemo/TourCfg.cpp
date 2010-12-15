// TourCfg.cpp : implementation file
//

#include "stdafx.h"
#include "nvdsdkdemo.h"
#include "TourCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTourCfg dialog


CTourCfg::CTourCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CTourCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTourCfg)
	m_nInterval = 0;
	m_strIP = _T("");
	m_nPort = 0;
	m_strPwd = _T("");
	m_strUserName = _T("");
	m_nCtrlType = 0;
	//}}AFX_DATA_INIT

	m_hCurLoginID = 0;
	m_nTourNum = 0;
	m_nDecChannel = 0;
	memset(&m_stuTourCfgInfo, 0, sizeof(DHDEV_DECODER_TOUR_CFG));
}


void CTourCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTourCfg)
	DDX_Control(pDX, IDC_LIST_TOUR_INFO, m_ctlTourCfgInfo);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ctlType);
	DDX_Control(pDX, IDC_COMBO_TOURNUM, m_ctlTourNo);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ctlChannelNo);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nInterval);
	DDV_MinMaxUInt(pDX, m_nInterval, 0, 120);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDV_MaxChars(pDX, m_strIP, 128);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDV_MaxChars(pDX, m_strPwd, 32);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, 32);
	DDX_Radio(pDX, IDC_RADIO1, m_nCtrlType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTourCfg, CDialog)
	//{{AFX_MSG_MAP(CTourCfg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_SAVE, OnSave)
	ON_BN_CLICKED(ID_QUERY, OnQuery)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TOUR_INFO, OnClickListTourInfo)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_DELETE_ALL, OnDeleteAll)
	ON_BN_CLICKED(IDC_CONTROL, OnControl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTourCfg message handlers

BOOL CTourCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	
	// TODO: Add extra initialization here
	
	m_ctlTourCfgInfo.SetExtendedStyle(m_ctlTourCfgInfo.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctlTourCfgInfo.InsertColumn(0, ConvertString("Tour Num"), LVCFMT_LEFT, 80, -1);
	m_ctlTourCfgInfo.InsertColumn(1, ConvertString("IP"), LVCFMT_LEFT, 80, 0);
	m_ctlTourCfgInfo.InsertColumn(2, ConvertString("Port"), LVCFMT_LEFT, 60, 1);
	m_ctlTourCfgInfo.InsertColumn(3, ConvertString("Channel No."), LVCFMT_LEFT, 60, 2);
	m_ctlTourCfgInfo.InsertColumn(4, ConvertString("Type"), LVCFMT_LEFT, 80, 3);
	m_ctlTourCfgInfo.InsertColumn(5, ConvertString("User Name"), LVCFMT_LEFT, 80, 4);
	m_ctlTourCfgInfo.InsertColumn(6, ConvertString("Password"), LVCFMT_LEFT, 60, 5);
	m_ctlTourCfgInfo.InsertColumn(7, ConvertString("Interval"), LVCFMT_LEFT, 80, 6);

	m_ctlTourNo.ResetContent();
	m_ctlType.ResetContent();
	m_ctlChannelNo.ResetContent();

	CString strText = "";
	int nIndex = 0;
	for (int i = 0; i < m_nTourNum; i++)
	{
		strText.Format("%d", i);
		nIndex = m_ctlTourNo.AddString(strText);
		m_ctlTourNo.SetItemData(nIndex, i);
	}
	m_ctlTourNo.SetCurSel(0);

	for (i = 0; i < 16; i++)
	{
		strText.Format("%d", i+1);
		nIndex = m_ctlChannelNo.AddString(strText);
		m_ctlChannelNo.SetItemData(nIndex, i+1);
	}
	m_ctlChannelNo.SetCurSel(0);

	nIndex = m_ctlType.AddString(ConvertString("main"));
	m_ctlType.SetItemData(nIndex, 0);
	nIndex = m_ctlType.AddString(ConvertString("sub"));
	m_ctlType.SetItemData(nIndex, 1);
	m_ctlType.SetCurSel(0);

	OnQuery(); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTourCfg::SetDlgParam(LONG lLoginID, int nTourNum, int nDecChannel)
{
	m_hCurLoginID = lLoginID;
	m_nTourNum = nTourNum;
 	m_nDecChannel = nDecChannel;
}

void CTourCfg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CTourCfg::OnSave() 
{
    SaveChannelInfo();
	
	int nRet = CLIENT_SetDevConfig(m_hCurLoginID, DH_DEV_DECODER_TOUR_CFG, m_nDecChannel, &m_stuTourCfgInfo, sizeof(DHDEV_DECODER_TOUR_CFG), 2000);
	if (nRet) 
	{
		OnQuery();
	}
	else
	{
		MessageBox(ConvertString("Failed to set!"), ConvertString("Prompt"));		
	}	
}

void CTourCfg::OnQuery() 
{
	int nRet = 0;
	DWORD dwRetLen = 0;
	
	nRet = CLIENT_GetDevConfig(m_hCurLoginID, DH_DEV_DECODER_TOUR_CFG, m_nDecChannel, &m_stuTourCfgInfo, sizeof(DHDEV_DECODER_TOUR_CFG), &dwRetLen, 2000);
	if (nRet && dwRetLen == sizeof(DHDEV_DECODER_TOUR_CFG))
	{
		ShowChannelInfo();	
	}
	else
	{
		MessageBox(ConvertString("Failed to query!"), ConvertString("Prompt"));
	}
}


void CTourCfg::SaveChannelInfo()
{
	UpdateData(TRUE);

	int nNum = m_ctlTourCfgInfo.GetItemCount();

	if (nNum > m_nTourNum || m_nTourNum > 64)
	{
		MessageBox(ConvertString("Beyond the tour support's numbers!"), ConvertString("Prompt"));
		return;
	}

	memset(&m_stuTourCfgInfo, 0, sizeof(DHDEV_DECODER_TOUR_CFG));
	m_stuTourCfgInfo.nCfgNum = nNum;
	
	for (int i = 0; i < nNum; i++)
	{
		DHDEV_DECODER_TOUR_SINGLE_CFG *pInfo = &m_stuTourCfgInfo.tourCfg[i];
		if(NULL != pInfo)
		{
			//ip
			CString strIp = m_ctlTourCfgInfo.GetItemText(i, 1);
			strcpy(pInfo->szIP, strIp.GetBuffer(0));
					
			//port
			pInfo->nPort = atoi(m_ctlTourCfgInfo.GetItemText(i, 2).GetBuffer(0));

			pInfo->nPlayChn = atoi(m_ctlTourCfgInfo.GetItemText(i, 3).GetBuffer(0));

			CString strType = m_ctlTourCfgInfo.GetItemText(i, 4);
			if (strType == "sub")
			{
				pInfo->nPlayType = 1;
			}
			else
			{
				pInfo->nPlayType = 0;
			}

			CString strUserName = m_ctlTourCfgInfo.GetItemText(i, 5);
			strcpy(pInfo->szUserName, strUserName.GetBuffer(0));

			CString strPwd = m_ctlTourCfgInfo.GetItemText(i, 6);
			strcpy(pInfo->szPassword, strPwd.GetBuffer(0));
			pInfo->nInterval = atoi(m_ctlTourCfgInfo.GetItemText(i, 7).GetBuffer(0));		
		}
	}
}

void CTourCfg::ShowChannelInfo()
{
	m_ctlTourCfgInfo.DeleteAllItems();
	for (int k = 0; k < m_stuTourCfgInfo.nCfgNum; k++)
	{
		DHDEV_DECODER_TOUR_SINGLE_CFG stuTourInfo = m_stuTourCfgInfo.tourCfg[k];
		CString strNum;
		strNum.Format("%d", k+1);
		CString strIP;
		CString strPort;
		CString strChannelNo;
		CString strType;
		CString strUserName;
		CString strInterval;
		CString strPassword;
		
		strIP = stuTourInfo.szIP;
		strPort.Format("%d", stuTourInfo.nPort);
		strChannelNo.Format("%d", stuTourInfo.nPlayChn);
		strType = stuTourInfo.nPlayType==0?"main":"sub";
		strUserName = stuTourInfo.szUserName;
		strPassword = stuTourInfo.szPassword;
		strInterval.Format("%d", stuTourInfo.nInterval);
		
		
		int nIndex = m_ctlTourCfgInfo.GetItemCount();
		m_ctlTourCfgInfo.InsertItem(LVIF_TEXT|LVIF_STATE, nIndex, strNum, 0, LVIS_SELECTED, 0, 0);
		
		m_ctlTourCfgInfo.SetItemText(nIndex,1,strIP);
		m_ctlTourCfgInfo.SetItemText(nIndex,2,strPort);
		m_ctlTourCfgInfo.SetItemText(nIndex,3,strChannelNo);
		m_ctlTourCfgInfo.SetItemText(nIndex,4,strType);
		m_ctlTourCfgInfo.SetItemText(nIndex,5,strUserName);
		m_ctlTourCfgInfo.SetItemText(nIndex,6,strPassword);
		m_ctlTourCfgInfo.SetItemText(nIndex,7,strInterval);
		
	}
}

void CTourCfg::OnClickListTourInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CTourCfg::OnAdd() 
{
	UpdateData(TRUE);

	int nIndex = m_ctlTourCfgInfo.GetItemCount();
	if (nIndex >= m_nTourNum)
	{
		MessageBox(ConvertString("Beyond the tour support's numbers!"), ConvertString("Prompt"));
		return;
	}

	int nTypeIndex = m_ctlType.GetCurSel();
	int nChannelIndex = m_ctlChannelNo.GetCurSel();	
	CString strNum;
	strNum.Format("%d", nIndex + 1);
	CString strIP;
	CString strPort;
	CString strChannelNo;
	CString strType;
	CString strUserName;
	CString strInterval;
	CString strPassword;
	int nChannelNo = m_ctlChannelNo.GetItemData(nChannelIndex);
	
	strIP = m_strIP;
	strPort.Format("%d", m_nPort);
	strChannelNo.Format("%d", nChannelNo);
	strType = m_ctlType.GetItemData(nTypeIndex)==0?"main":"sub";
	strUserName = m_strUserName;
	strPassword = m_strPwd;
	strInterval.Format("%d", m_nInterval);

	if (strIP == "" || m_nInterval < 0 || m_nInterval > 120 || m_nPort < 0 || m_nPort > 65535)
	{
		MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
		return;
	}

	m_ctlTourCfgInfo.InsertItem(LVIF_TEXT|LVIF_STATE, nIndex, strNum, 0, LVIS_SELECTED, 0, 0);
	
	m_ctlTourCfgInfo.SetItemText(nIndex,1,strIP);
	m_ctlTourCfgInfo.SetItemText(nIndex,2,strPort);
	m_ctlTourCfgInfo.SetItemText(nIndex,3,strChannelNo);
	m_ctlTourCfgInfo.SetItemText(nIndex,4,strType);
	m_ctlTourCfgInfo.SetItemText(nIndex,5,strUserName);
	m_ctlTourCfgInfo.SetItemText(nIndex,6,strPassword);
	m_ctlTourCfgInfo.SetItemText(nIndex,7,strInterval);

	m_nInterval = 0;
	m_strIP = _T("");
	m_nPort = 0;
	m_strPwd = _T("");
	m_strUserName = _T("");	

	UpdateData(FALSE);
}

void CTourCfg::OnDelete() 
{
	if ( m_ctlTourCfgInfo.GetItemCount() <= 0 )
	{
		MessageBox(ConvertString("No item in the list!"), ConvertString("ERROR"));
		return;
	}
	
	//得到当前list选项
	int nSelect = m_ctlTourCfgInfo.GetSelectionMark();
	if( nSelect < 0 )	
	{
		MessageBox(ConvertString("Please select a item!"), ConvertString("ERROR"));
		return;
	}

	if (MessageBox(ConvertString("Really to delete this item?"), ConvertString("WARING"), MB_YESNO|MB_ICONQUESTION)==IDNO) 
	{
		return;
	}
	
	m_ctlTourCfgInfo.DeleteItem(nSelect);

	UpdateData(FALSE);
}

void CTourCfg::OnDeleteAll() 
{
	if ( m_ctlTourCfgInfo.GetItemCount() <= 0 )
	{
		MessageBox(ConvertString("No item in the list!"), ConvertString("ERROR"));
		return;
	}

	if (MessageBox(ConvertString("Really to delete these items?"), ConvertString("WARING"), MB_YESNO|MB_ICONQUESTION)==IDNO) 
	{
		return;
	}
	
	m_ctlTourCfgInfo.DeleteAllItems();	
}

void CTourCfg::OnControl() 
{
	UpdateData(TRUE);

	DEC_CTRL_TOUR_TYPE emActionParam = (DEC_CTRL_TOUR_TYPE)m_nCtrlType;

	int nRet = CLIENT_CtrlDecoderTour(m_hCurLoginID, m_nDecChannel, emActionParam, 1500);
	if (nRet == FALSE)
	{
		MessageBox(ConvertString("Failed to control decoder tour."),  ConvertString("Prompt"));
	}
}
