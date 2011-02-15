// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "CameraView.h"
#include "BTSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CCameraView

IMPLEMENT_DYNAMIC(CCameraView, CDockablePane)

CCameraView::CCameraView()
: m_sFindStr(_T(""))
{
	VERIFY ( m_pBtnFind = new CMFCButton() );
	VERIFY ( m_pEdtFind = new CEdit() );
	VERIFY ( m_pStcFind = new CStatic() );

	VERIFY (m_hBrush = ::CreateSolidBrush(RGB(208,212,221)) );
}

CCameraView::~CCameraView()
{
	if (m_pBtnFind)
		delete m_pBtnFind;

	if (m_pEdtFind)
		delete m_pEdtFind;
	
	if (m_pStcFind)
		delete m_pStcFind;

}


BEGIN_MESSAGE_MAP(CCameraView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_FIND, &CCameraView::OnFindBTS)
	ON_UPDATE_COMMAND_UI(ID_BTN_FIND, &CCameraView::OnUpdateFindBTS)

END_MESSAGE_MAP()



// CCameraView message handlers



int CCameraView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_ctrlCameraTree.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	//
	HFONT hFont;   
	hFont =  CreateFont(12,0,0,0,400,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH || FF_DONTCARE,"宋体");
	m_pStcFind->Create("查询设备",WS_CHILD|WS_VISIBLE, CRect(20,3,70,20), this);
	m_pStcFind->SendMessage(WM_SETFONT,(DWORD)hFont,TRUE);

	//m_wndEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER |   WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN, CRect(10, 10, 400, 300), this, IDC_EDIT1);
	//m_wndEdit.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_wndEdit.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/ |  WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN, CRect(10, 10, 400, 300), this, IDC_EDIT1);
	m_pEdtFind->Create(WS_CHILD | WS_VISIBLE,CRect(75,3,170,20), this ,ID_EBOX_FIND);
	m_pEdtFind->ModifyStyleEx(0,   WS_EX_CLIENTEDGE,   SWP_DRAWFRAME);
	m_pEdtFind->SendMessage(WM_SETFONT,(DWORD)hFont,TRUE);

	//
	m_pBtnFind->Create("查找",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(175,3,230,20), this ,ID_BTN_FIND);
	m_pBtnFind->EnableWindow();
	m_pBtnFind->SendMessage(WM_SETFONT,(DWORD)hFont,TRUE);


	// 加载视图图像:
	m_lstImages.Create(IDB_CAMERA_VIEW, 16, 0, RGB(255, 0, 255));
	m_ctrlCameraTree.SetImageList(&m_lstImages, TVSIL_NORMAL);


	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	//InitCameraView();
	fillinData();
	AdjustLayout();

	return 0;
}

void CCameraView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

void CCameraView::fillinData()
{
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	pApp->pgkclient->btsmanager.buildbtsTree(pApp->btsTotalStr, &m_ctrlCameraTree);
}

void CCameraView::InitCameraView(void)
{

	enum {euHZ, euYH};
	char sCitys[2][10] = {"杭州","余杭"};
	char sHZCityDistrict[6][10] = {"上城区", "下城区","西湖区","江干区","下沙区","滨江区"};
	//char sHZ_ShangCQ[6][10] = {"ShangCQ1","ShangCQ2","ShangCQ3","ShangCQ4","ShangCQ5","ShangCQ6"};
	char sHZ_CQ_CAMERA[6][10] = {"ShangCQ","XiaCQ","XiHQ","JiangGQ","XiaSQ","BinJQ"};

	char sYHCityDistrict[6][10] = {"余一区", "余二区","余三区","余四区","余五区","余六区"};
	char sYH_CQ_CAMERA[6][10] = {"YuYQ","YuEQ","YuSQ","YuSQ","YuWQ","YuLQ"};

	//char *pDistrict[2][]={sHZCityDistrict, sYHCityDistrict};

	m_hRootItem = m_ctrlCameraTree.InsertItem("BTS List", 0, 0);
	m_ctrlCameraTree.SetItemState(m_hRootItem, TVIS_BOLD, TVIS_BOLD);

	int nCitys = 2;
	int nDistrict = 6;
	int nCamera	  = 6;
	CString sCity, sDistrict, sCamera;
	for (int nC=0; nC<nCitys; nC++)
	{
		sCity = sCitys[nC];
		HTREEITEM hCity = m_ctrlCameraTree.InsertItem(sCity, 1, 1, m_hRootItem);
		switch(nC)
		{
		case euHZ:
			{
				for (int nDS = 0; nDS<nDistrict; nDS++)
				{
	
					sDistrict = sHZCityDistrict[nDS];
					HTREEITEM hDistrict = m_ctrlCameraTree.InsertItem(sDistrict, 1,1, hCity);
					for(int nCM=0; nCM<nCamera; nCM++) //Each District have six camera
					{
							sCamera.Format("%s_Camera_%d",sHZ_CQ_CAMERA[nDS], nCM);
							m_ctrlCameraTree.InsertItem(sCamera,2,2, hDistrict);
					}
				}
			}
			break;
		case euYH:
			{
				for (int nDS = 0; nDS<nDistrict; nDS++)
				{
	
					sDistrict = sYHCityDistrict[nDS];
					HTREEITEM hDistrict = m_ctrlCameraTree.InsertItem(sDistrict,1,1, hCity);
					for(int nCM=0; nCM<nCamera; nCM++) //Each District have six camera
					{
							sCamera.Format("%s_Camera_%d",sYH_CQ_CAMERA[nDS], nCM);
							m_ctrlCameraTree.InsertItem(sCamera,2,2, hDistrict);
					}
				}
			}
			break;
		default:
			break;
		}
	}

}

void CCameraView::AdjustLayout(void)
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	m_ctrlCameraTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1+20, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);

}

HTREEITEM CCameraView::FindTarget(HTREEITEM  hItem, CString sName, bool bFindNext)
{
	if (hItem == NULL)
		hItem = m_ctrlCameraTree.GetRootItem();

	if (hItem == NULL)
		return NULL;

	CString szString = m_ctrlCameraTree.GetItemText(hItem);
	if (szString.Find(sName) > -1)
	{
		m_ctrlCameraTree.SelectSetFirstVisible(hItem);
		m_ctrlCameraTree.SelectItem(hItem);
		return hItem;
	}


	if (m_ctrlCameraTree.ItemHasChildren(hItem) == FALSE)
		return NULL;

	//递归查找hItem的所有子节点
	HTREEITEM hRes = NULL;
	HTREEITEM hItemChild = m_ctrlCameraTree.GetChildItem(hItem);
	while (hItemChild)
	{
		hRes = FindTarget(hItemChild, sName); //查以hItem为根的枝
		if (hRes != NULL)					  //如果在以hItem为根的枝里找到，返回结果
			return hRes;
		else								  //否则，查找与hItem同级的下一个枝
			hItemChild = m_ctrlCameraTree.GetNextSiblingItem(hItemChild);
	} // end of while(hItem != NULL, has next item)

	return NULL; 
}

BOOL CCameraView::FindNewTarget(CString sFindStr)
{
	m_hItemCurFind = FindTarget(NULL, sFindStr);

	BOOL bRet = FALSE;
	if (m_hItemCurFind)
	{
		m_sFindStr = sFindStr;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CCameraView::FindNextTarget(void)
{
	if (m_sFindStr.IsEmpty())
		return FALSE;

	BOOL bRet = FALSE;

	HTREEITEM hItemFind = NULL;
	HTREEITEM hItemChild = m_ctrlCameraTree.GetChildItem(m_hItemCurFind);
	if (hItemChild)
		hItemFind = FindTarget(hItemChild, m_sFindStr);

	if (hItemFind)
	{
		m_hItemCurFind = hItemFind;
		return TRUE;
	}


	//Brother's Item need to find.
	HTREEITEM hItemBrother=m_ctrlCameraTree.GetNextSiblingItem(m_hItemCurFind); //ignor itself
	while(hItemBrother)
	{
		hItemFind = FindTarget(hItemBrother, m_sFindStr);
		if (hItemFind)
		{
			m_hItemCurFind = hItemFind;
			return TRUE;
		}
		hItemBrother=m_ctrlCameraTree.GetNextSiblingItem(hItemBrother);
	}

	//Father's Item Need to find
	HTREEITEM hItemFather = m_ctrlCameraTree.GetParentItem(m_hItemCurFind);
	while(hItemFather)
	{
		CString szString = m_ctrlCameraTree.GetItemText(hItemFather); //ignor itself
		hItemBrother = hItemFather; //polling all item of Father's brother
		do //all of the brother...
		{
			hItemBrother = m_ctrlCameraTree.GetNextSiblingItem(hItemBrother);
			if (hItemBrother)
				hItemFind = FindTarget(hItemBrother, m_sFindStr);
			else
				break;

			if (hItemFind)
			{
				m_hItemCurFind = hItemFind;
				return TRUE;
			}
		}
		while(hItemBrother); 

		hItemFather = m_ctrlCameraTree.GetParentItem(hItemFather); //polling father's father
	}
	
	//-----------------------

	return bRet;
}

BOOL CCameraView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	COLORREF   rgbBackGnd   =   RGB(208,212,221); 
	CBrush   brush; 
	brush.CreateSolidBrush(rgbBackGnd); 
	CBrush *pOldbrush = pDC->SelectObject(&brush);
	CRect   rect;     
	GetClientRect(&rect); 
	// pDC->Rectangle(&rect);
	pDC->FillRect(rect,&brush);

	return CDockablePane::OnEraseBkgnd(pDC);
}

HBRUSH CCameraView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDockablePane::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(208,212,221));
		return m_hBrush;
	}
	else if (nCtlColor == CTLCOLOR_BTN)
	{
		//pWnd->EnableWindow();
	}

	HBRUSH hbr = CDockablePane::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
void CCameraView::OnFindBTS()
{
	AfxMessageBox("Dynamic Create Button OK!");
}

void CCameraView::OnUpdateFindBTS(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}
