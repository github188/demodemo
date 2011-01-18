// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "CameraView.h"


// CCameraView

IMPLEMENT_DYNAMIC(CCameraView, CDockablePane)

CCameraView::CCameraView()
: m_sFindStr(_T(""))
{

}

CCameraView::~CCameraView()
{
}


BEGIN_MESSAGE_MAP(CCameraView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
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

	// 加载视图图像:
	m_lstImages.Create(IDB_CAMERA_VIEW, 16, 0, RGB(255, 0, 255));
	m_ctrlCameraTree.SetImageList(&m_lstImages, TVSIL_NORMAL);


	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	InitCameraView();
	AdjustLayout();

	return 0;
}

void CCameraView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
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

/*

	HTREEITEM hRoot = m_wndFileView.InsertItem(_T(" 文件"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("FakeApp 源文件"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	HTREEITEM hInc = m_wndFileView.InsertItem(_T("FakeApp 头文件"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	HTREEITEM hRes = m_wndFileView.InsertItem(_T("FakeApp 资源文件"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);
*/

}

void CCameraView::AdjustLayout(void)
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	m_ctrlCameraTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);

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
