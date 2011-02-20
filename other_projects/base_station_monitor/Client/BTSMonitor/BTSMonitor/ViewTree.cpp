// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

#include "stdafx.h"
#include "BTSMonitor.h"
#include "const.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree ��Ϣ�������

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
    UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	ScreenToClient(&curPoint);
	HTREEITEM htItem = HitTest(curPoint, &nFlags);
	
	if ( ItemHasChildren(htItem) )
		return;

	//CString strText = GetItemText(htItem);
	CString strText, sUUID, sInfo, sChannelID;
	HTREEITEM hParentItem = GetParentItem(htItem);
	if (hParentItem)
	{
		strText = GetItemText(hParentItem);
		CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
		sUUID = pApp->pgkclient->btsmanager.GetCameraUUID(strText);
		sChannelID = GetItemText(htItem);
		sChannelID = sChannelID.Left(sChannelID.Find(':'));

		sInfo = sUUID + '$' + sChannelID;

	}

	HTREEITEM itemChild = GetChildItem(htItem);
	if ( !itemChild ) //juge wheth the item is leaf or not.
	{
		//Open the related Video stream..On the current selected windows.
		CView *pView = ( (CBTSMonitorApp*)AfxGetApp() )->GetBaseView();
		if (pView && pView->m_hWnd)
			::SendMessage(pView->m_hWnd,WM_PLAYVIEW_SELECTED,MSG_SELECT_CAMERA_DEVICE,((LPARAM)sInfo.GetBuffer(sInfo.GetLength())));
	}

	*pResult = 0;
}
