// ConfigCover.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "ConfigCover.h"
#include "ClientDemo5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigCover dialog


CConfigCover::CConfigCover(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigCover::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigCover)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nChannelCount = 0;
	m_nCurChannel = -1;
	m_bNewProtocol = FALSE;
}


void CConfigCover::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigCover)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ctlCoverType);
	DDX_Control(pDX, IDC_LIST_COVERS, m_ctlCoverLst);
	DDX_Control(pDX, IDC_COMBO_CHANNO, m_ctlChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigCover, CDialog)
	//{{AFX_MSG_MAP(CConfigCover)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_READALL, OnButtonReadall)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNO, OnSelchangeComboChanno)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COVERS, OnItemchangedListCovers)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigCover message handlers

BOOL CConfigCover::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	InitListView();
	ConvertComboBox(m_ctlCoverType);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigCover::InitListView()
{
	m_ctlCoverLst.SetExtendedStyle(m_ctlCoverLst.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctlCoverLst.InsertColumn(0, ConvertString("area"),LVCFMT_LEFT,60,-1);
	m_ctlCoverLst.InsertColumn(1, ConvertString("Margin-left"),LVCFMT_LEFT,90,0);
	m_ctlCoverLst.InsertColumn(2, ConvertString("Margin-top"),LVCFMT_LEFT,90,1);
	m_ctlCoverLst.InsertColumn(3, ConvertString("Margin-right"),LVCFMT_LEFT,90,2);
	m_ctlCoverLst.InsertColumn(4, ConvertString("Margin-bottom"),LVCFMT_LEFT,90,3);
	m_ctlCoverLst.InsertColumn(5, ConvertString("Overlay Color"),LVCFMT_LEFT,80,4);
	m_ctlCoverLst.InsertColumn(6, ConvertString("Overlay Mode"),LVCFMT_LEFT,80,5);
}

void CConfigCover::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_ctlCoverLst.GetItemCount();
	if (nCount >= m_channelCoverInfo[m_nCurChannel].bTotalBlocks) 
	{
		MessageBox(ConvertString("More than total blocks!"), ConvertString("Prompt"));
		return;
	}
	CString strID;
	strID.Format("%d", nCount+1);
	
	LONG lLeft = 0;
	LONG lRight = 0;
	LONG lTop = 0;
	LONG lBotton = 0;

	CString strLeft;
	int nTemp = GetDlgItemInt(IDC_EDIT_LEFT);
	if (nTemp<0 || nTemp>8192)
	{
		MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
		return;
	}
	strLeft.Format("%d", nTemp);
	lLeft = nTemp;

	CString strTop;
	nTemp = GetDlgItemInt(IDC_EDIT_TOP);
	if (nTemp<0 || nTemp>8192)
	{
		MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
		return;
	}
	strTop.Format("%d", nTemp);
	lTop = nTemp;

	CString strRight;
	nTemp = GetDlgItemInt(IDC_EDIT_RIGHT);
	if (nTemp<0 || nTemp>8192)
	{
		MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
		return;
	}
	strRight.Format("%d", nTemp);
	lRight = nTemp;

	CString strBotton;
	nTemp = GetDlgItemInt(IDC_EDIT_BUTTON);
	if (nTemp<0 || nTemp>8192)
	{
		MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
		return;
	}
	strBotton.Format("%d", nTemp);
	lBotton = nTemp;

	if (lRight < lLeft || lBotton < lTop)
	{
		MessageBox(ConvertString("Coordinate error!"), ConvertString("Prompt"));
		return;
	}
	
	CString strColor;
	nTemp = GetDlgItemInt(IDC_EDIT_COLOR);
	if (nTemp<0 || nTemp>255)
	{
		MessageBox(ConvertString("Color range£º0~255!"), ConvertString("Prompt"));
		return;
	}
	strColor.Format("%d", nTemp);

	CString strType;
	if (0 == m_ctlCoverType.GetCurSel())
	{
		strType = ConvertString("Black block");
	}
	else if (1 == m_ctlCoverType.GetCurSel())
	{
		strType = ConvertString("Mosaic");
	}
	else
	{
		MessageBox(ConvertString("You have not select overlay mode!"), ConvertString("Prompt"));
		return;
	}
	
	InsertListView(strID, strLeft, strTop, strRight, strBotton, strColor, strType);
}

void CConfigCover::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_ctlCoverLst.GetItemCount())
	{
		if(0 < m_ctlCoverLst.GetSelectedCount())
		{
			int nIndex = m_ctlCoverLst.GetSelectionMark();
			if(-1 != nIndex)
			{
				m_ctlCoverLst.DeleteItem(nIndex);
			}
		}
	}
}

void CConfigCover::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_ctlCoverLst.GetItemCount())
	{
		if(0 < m_ctlCoverLst.GetSelectedCount())
		{
			int nIndex = m_ctlCoverLst.GetSelectionMark();
			if(-1 != nIndex)
			{
				LONG lLeft = 0;
				LONG lRight = 0;
				LONG lTop = 0;
				LONG lBotton = 0;
				
				CString strLeft;
				int nTemp = GetDlgItemInt(IDC_EDIT_LEFT);
				if (nTemp<0 || nTemp>8192)
				{
					MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
					return;
				}
				strLeft.Format("%d", nTemp);
				lLeft = nTemp;
				
				CString strTop;
				nTemp = GetDlgItemInt(IDC_EDIT_TOP);
				if (nTemp<0 || nTemp>8192)
				{
					MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
					return;
				}
				strTop.Format("%d", nTemp);
				lTop = nTemp;
				
				CString strRight;
				nTemp = GetDlgItemInt(IDC_EDIT_RIGHT);
				if (nTemp<0 || nTemp>8192)
				{
					MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
					return;
				}
				strRight.Format("%d", nTemp);
				lRight = nTemp;
				
				CString strBotton;
				nTemp = GetDlgItemInt(IDC_EDIT_BUTTON);
				if (nTemp<0 || nTemp>8192)
				{
					MessageBox(ConvertString("Margin range£º0~8192!"), ConvertString("Prompt"));
					return;
				}
				strBotton.Format("%d", nTemp);
				lBotton = nTemp;
				
				if (lRight < lLeft || lBotton < lTop)
				{
					MessageBox(ConvertString("Coordinate error!"), ConvertString("Prompt"));
					return;
				}
				
				CString strColor;
				nTemp = GetDlgItemInt(IDC_EDIT_COLOR);
				if (nTemp<0 || nTemp>255)
				{
					MessageBox(ConvertString("Color range£º0~255!"), ConvertString("Prompt"));
					return;
				}
				strColor.Format("%d", nTemp);
				
				CString strType;
				if (0 == m_ctlCoverType.GetCurSel())
				{
					strType = ConvertString("Black block");
				}
				else if (1 == m_ctlCoverType.GetCurSel())
				{
					strType = ConvertString("Mosaic");
				}
				else
				{
					MessageBox(ConvertString("You have not select overlay mode!"), ConvertString("Prompt"));
					return;
				}

				m_ctlCoverLst.SetItemText(nIndex, 1, strLeft);
				m_ctlCoverLst.SetItemText(nIndex, 2, strTop);
				m_ctlCoverLst.SetItemText(nIndex, 3, strRight);
				m_ctlCoverLst.SetItemText(nIndex, 4, strBotton);
				m_ctlCoverLst.SetItemText(nIndex, 5, strColor);
				m_ctlCoverLst.SetItemText(nIndex, 6, strType);
			}
		}
	}
}

void CConfigCover::InitDlgInfo(DHDEV_VIDEOCOVER_CFG *pCoverInfo, BYTE nLength, BOOL bNewProtocol)
{
	m_ctlChannel.ResetContent();
	m_nChannelCount = nLength>32?32:nLength;
	m_bNewProtocol = bNewProtocol;
	CString str;
	int nIndex = 0;
	//Channel dropdown menu setup
	for(BYTE i=0;i<nLength;i++)
	{
		str.Format("%d",i);
		nIndex = m_ctlChannel.AddString(str);
		m_ctlChannel.SetItemData(nIndex,i);
		//Save channel video information 
		m_channelCoverInfo[i] = pCoverInfo[i];
	}

	//Select channel 0
	if(0 < m_ctlChannel.GetCount())
	{
		m_ctlChannel.SetCurSel(0);
		OnSelchangeComboChanno();
	}
}

void CConfigCover::CleanAll()
{
	m_ctlChannel.SetCurSel(-1);
	m_ctlCoverType.SetCurSel(-1);
	m_ctlCoverLst.DeleteAllItems();

	SetDlgItemText(IDC_EDIT_CHANNAME,"");
	SetDlgItemText(IDC_EDIT_BLOCKCOUNT,"");
	
	SetDlgItemText(IDC_EDIT_LEFT, "");
	SetDlgItemText(IDC_EDIT_TOP, "");
	SetDlgItemText(IDC_EDIT_RIGHT, "");
	SetDlgItemText(IDC_EDIT_BUTTON, "");
	SetDlgItemText(IDC_EDIT_COLOR, "");
	
	m_nChannelCount = 0;
	m_nCurChannel = -1;
}

void CConfigCover::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if(-1 != m_ctlChannel.GetCurSel())
	{
		SaveChannelInfo(m_nCurChannel);
		//Call father window to save data 
		if (m_bNewProtocol == FALSE)
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->SetCoverConfigInfo(m_channelCoverInfo,m_nChannelCount);
		}
		else
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->SetOneCoverConfigInfo(&m_channelCoverInfo[m_nCurChannel],m_nCurChannel);
		}	
	}
	else
	{
		MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
	}
}

void CConfigCover::OnButtonReadall() 
{
	// TODO: Add your control notification handler code here
	//Call father window to read data 
	
	//First clear
	CleanAll();
	((CClientDemo5Dlg*)AfxGetMainWnd())->GetCoverConfigInfo();	
}

void CConfigCover::SaveChannelInfo(int nNum)
{
	if(-1 < nNum)
	{
		DHDEV_VIDEOCOVER_CFG *pInfo = m_channelCoverInfo + nNum;
		if(NULL != pInfo)
		{
			//Channel name
			CString str = "";
			GetDlgItemText(IDC_EDIT_CHANNAME,str);
			strcpy(pInfo->szChannelName,str);
			//Privacy mask amout supported 
			pInfo->bTotalBlocks = GetDlgItemInt(IDC_EDIT_BLOCKCOUNT);
			
			//Privacy mask zone
			int nCount = m_ctlCoverLst.GetItemCount();
			nCount = nCount>DH_MAX_VIDEO_COVER_NUM ? DH_MAX_VIDEO_COVER_NUM : nCount;
			pInfo->bCoverCount = nCount;
			CString strTemp;
			for (int i=0; i<nCount; i++)
			{
				strTemp = m_ctlCoverLst.GetItemText(i, 1);
				pInfo->CoverBlock[i].rcBlock.left = atol(strTemp);

				strTemp = m_ctlCoverLst.GetItemText(i, 2);
				pInfo->CoverBlock[i].rcBlock.top = atol(strTemp);

				strTemp = m_ctlCoverLst.GetItemText(i, 3);
				pInfo->CoverBlock[i].rcBlock.right = atol(strTemp);

				strTemp = m_ctlCoverLst.GetItemText(i, 4);
				pInfo->CoverBlock[i].rcBlock.bottom = atol(strTemp);

				strTemp = m_ctlCoverLst.GetItemText(i, 5);
				pInfo->CoverBlock[i].nColor = atoi(strTemp);

				strTemp = m_ctlCoverLst.GetItemText(i, 6);
				if (strTemp == "Black block" || strTemp == ConvertString("Black block"))
				{
					pInfo->CoverBlock[i].bBlockType = 0;
				} 
				else if (strTemp == "Mosaic" || strTemp == ConvertString("Mosaic"))
				{
					pInfo->CoverBlock[i].bBlockType = 1;
				}

				pInfo->CoverBlock[i].bEncode = 1;
				pInfo->CoverBlock[i].bPriview = 1;
			}
		}
	}
}

void CConfigCover::OnSelchangeComboChanno() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = (int)m_ctlChannel.GetItemData(nIndex);

		//First save channel nformaiton
		SaveChannelInfo(m_nCurChannel);

		if (m_bNewProtocol == TRUE)
		{
			((CClientDemo5Dlg*)AfxGetMainWnd())->GetOneCoverConfigInfo(&m_channelCoverInfo[nNum], nNum);
		}
		
		//Display channel corresponding information 
		DHDEV_VIDEOCOVER_CFG *pCoverInfo = m_channelCoverInfo + nNum;
		//Set channel name
		SetDlgItemText(IDC_EDIT_CHANNAME,pCoverInfo->szChannelName);
		//Privacy mask amount supported 
		SetDlgItemInt(IDC_EDIT_BLOCKCOUNT, pCoverInfo->bTotalBlocks);

		//Privacy mask zone
		m_ctlCoverLst.DeleteAllItems();
		int nRowCount = pCoverInfo->bCoverCount>DH_MAX_VIDEO_COVER_NUM ? DH_MAX_VIDEO_COVER_NUM : pCoverInfo->bCoverCount;
		for (int i=0; i<nRowCount; i++)
		{
			VIDEO_COVER_ATTR *pBlockInfo = pCoverInfo->CoverBlock + i;
			CString strID;
			strID.Format("%d", i+1);
			CString strLeft;
			strLeft.Format("%d", pBlockInfo->rcBlock.left);
			CString strTop;
			strTop.Format("%d", pBlockInfo->rcBlock.top);
			CString strRight;
			strRight.Format("%d", pBlockInfo->rcBlock.right);
			CString strButton;
			strButton.Format("%d", pBlockInfo->rcBlock.bottom);
			CString strColor;
			strColor.Format("%d", pBlockInfo->nColor);
			CString strType;
			if (0 == pBlockInfo->bBlockType)
			{
				strType = ConvertString("Black block");
			}
			else
			{
				strType = ConvertString("Mosaic");
			}
			InsertListView(strID, strLeft, strTop, strRight, strButton, strColor, strType);
		}
		
		m_nCurChannel = nNum;
	}
}

void CConfigCover::InsertListView(CString strID, CString strLeft, CString strTop, CString strRight, CString strButton, CString strColor, CString strType)
{
	int nIndex = m_ctlCoverLst.GetItemCount();
	m_ctlCoverLst.InsertItem(LVIF_TEXT|LVIF_STATE,nIndex,strID,0,LVIS_SELECTED,0,0);

	m_ctlCoverLst.SetItemText(nIndex,1,strLeft);
	m_ctlCoverLst.SetItemText(nIndex,2,strTop);
	m_ctlCoverLst.SetItemText(nIndex,3,strRight);
	m_ctlCoverLst.SetItemText(nIndex,4,strButton);
	m_ctlCoverLst.SetItemText(nIndex,5,strColor);
	m_ctlCoverLst.SetItemText(nIndex,6,strType);
}


void CConfigCover::OnItemchangedListCovers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMListView ==NULL)
	{
		return;
	}

	if (pNMListView->uNewState == 3/* && pNMListView->uOldState == 0*/)
	{
		int nIndex = pNMListView->iItem;
		if(-1 != nIndex)
		{
			CString strTemp;
			strTemp = m_ctlCoverLst.GetItemText(nIndex, 1);
			SetDlgItemText(IDC_EDIT_LEFT, strTemp);
			strTemp = m_ctlCoverLst.GetItemText(nIndex, 2);
			SetDlgItemText(IDC_EDIT_TOP, strTemp);
			strTemp = m_ctlCoverLst.GetItemText(nIndex, 3);
			SetDlgItemText(IDC_EDIT_RIGHT, strTemp);
			strTemp = m_ctlCoverLst.GetItemText(nIndex, 4);
			SetDlgItemText(IDC_EDIT_BUTTON, strTemp);
			strTemp = m_ctlCoverLst.GetItemText(nIndex, 5);
			SetDlgItemText(IDC_EDIT_COLOR, strTemp);
			strTemp = m_ctlCoverLst.GetItemText(nIndex, 6);
			if (strTemp == "Black block" || strTemp == ConvertString("Black block"))
			{
				m_ctlCoverType.SetCurSel(0);
			} 
			else if (strTemp == "Mosaic" || strTemp ==ConvertString("Mosaic"))
			{
				m_ctlCoverType.SetCurSel(1);
			}
		}
	}
	
	*pResult = 0;
}

void CConfigCover::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}








































































































































































































































































































































































































































































































































