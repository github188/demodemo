// WarningMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarningMgr.h"
#include "include/iPlay.h"
#include "const.h"
#include "util.h"
#include "DlgImage.h"
#include "MonitorImage.h"
// CWarningMgr dialog

IMPLEMENT_DYNAMIC(CWarningMgr, CDialog)

CWarningMgr::CWarningMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningMgr::IDD, pParent)
	, m_sFilter(_T(""))
	, m_nCurItem(0)
	, m_menuPt(0)
	, m_bPopMenu(false)
{
	VERIFY(pDlgImage = new CDlgImage());
}

CWarningMgr::~CWarningMgr()
{
	if (pDlgImage)
		delete pDlgImage;

	//Need free Memory...
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	//pApp->pgkclient->StopAlamVideoPlay();
	{
		pApp->pgkclient->Stop_Play(cnWARNING_VEDIO);
		BOOL bPlay = PLAY_Stop(cnWARNING_VEDIO);		
		BOOL bOpenRet = PLAY_CloseStream(cnWARNING_VEDIO);			
	}

}

void CWarningMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEVICE_FIND, m_sFilter);
	DDX_Control(pDX, IDC_TREE_WARN_MGR_DEVICE, m_treeWarnMgr);
	DDX_Control(pDX, IDC_LST_TARGET_WARNING, m_lstFindWarnResult);
	DDX_Control(pDX, IDC_CBO_WARN_TYPE, m_cboWarnType);
	DDX_Control(pDX, IDC_CBO_WARN_ACK_TYPE, m_cboWarnAckType);
	DDX_Control(pDX, IDC_DATE_BEGIN, m_dateBegin);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_Control(pDX, IDC_CBO_START_HOUR, m_cboStartHour);
	DDX_Control(pDX, IDC_CBO_END_HOUR, m_cboEndHour);
	DDX_Control(pDX, IDC_CBO_START_MINUTE, m_cboStartMin);
	DDX_Control(pDX, IDC_CBO_END_MINUTE, m_cboEndMin);
	DDX_Control(pDX, IDC_CBO_START_SECOND, m_cboStartSec);
	DDX_Control(pDX, IDC_CBO_END_SECOND, m_cboEndSec);
	DDX_Control(pDX, IDC_WINDOW_VEDIO, m_wndWarnVedio);
}


BEGIN_MESSAGE_MAP(CWarningMgr, CDialog)
	ON_BN_CLICKED(IDC_BTN_WARN_MGR_DEVICE_FIND, &CWarningMgr::OnBnClickedBtnWarnMgrDeviceFind)
	ON_BN_CLICKED(IDC_FIND_TARGET_WARNING, &CWarningMgr::OnBnClickedFindTargetWarning)
	ON_NOTIFY(NM_CLICK, IDC_TREE_WARN_MGR_DEVICE, &CWarningMgr::OnNMClickTreeWarnMgrDevice)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CWarningMgr::OnHdnItemclickLstTargetWarning)
	ON_NOTIFY(NM_CLICK, IDC_LST_TARGET_WARNING, &CWarningMgr::OnNMClickLstTargetWarning)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_TARGET_WARNING, &CWarningMgr::OnNMDblclkLstTargetWarning)
	ON_NOTIFY(NM_RCLICK, IDC_LST_TARGET_WARNING, &CWarningMgr::OnNMRClickLstTargetWarning)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WARNINGMGR_EXPORT, &CWarningMgr::OnWarningmgrExport)
	ON_UPDATE_COMMAND_UI(ID_WARNINGMGR_EXPORT, &CWarningMgr::OnUpdateWarningmgrExport)
	ON_COMMAND(ID_WARNINGMGR_SAVEAS, &CWarningMgr::OnWarningmgrSaveas)
	ON_UPDATE_COMMAND_UI(ID_WARNINGMGR_SAVEAS, &CWarningMgr::OnUpdateWarningmgrSaveas)
END_MESSAGE_MAP()


// CWarningMgr message handlers

BOOL CWarningMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//Extend Style
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_lstFindWarnResult.m_hWnd, styles, styles );

	CString strHeader[] = 
	{
		"�澯����", //level
		"λ��",		//place
		"�˾�����", //BTSType
		"�澯����", //alarmCode :1:��ǰʵʱ�澯, ���ڷ����ĸ澯;��2:��ʷ�澯��¼
		"��վ",		//BTSID
		"ͨ����",	//ChannelID
		"��ʼʱ��", //start tiem
		"����ʱ��", //end   time
		"����״̬", //status
		"ͼ������", //Category 1.��Ƶ 2.ͼƬ, 3.��
		""
	};

	int nCnt = sizeof(strHeader)/sizeof(strHeader[0]);
	int nWidth[] = {68,160,68,68,68,60,180,180,68,50,0};
	int i=0;
	for (; i<nCnt-1; i++)
		m_lstFindWarnResult.InsertColumn(i,strHeader[i],LVCFMT_CENTER, nWidth[i]);
	m_lstFindWarnResult.InsertColumn(i,strHeader[i],LVCFMT_CENTER, 0);
	

	//List Warning 
	m_imagelist.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
	m_lstFindWarnResult.SetImageList(&m_imagelist,LVSIL_SMALL);
	
	CBTSMonitorApp *pApp = (CBTSMonitorApp*)AfxGetApp();
	HICON hIcon = pApp->LoadIconA(IDI_LEVEL_1);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_2);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_3);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_4);
	if (hIcon)		m_imagelist.Add(hIcon);
	hIcon = pApp->LoadIconA(IDI_LEVEL_5);
	if (hIcon)		m_imagelist.Add(hIcon);

	//Tree View 
	m_treeImages.Create(IDB_CAMERA_VIEW, 16, 0, RGB(255, 0, 255));
	m_treeWarnMgr.SetImageList(&m_treeImages, TVSIL_NORMAL);

	//Init combo 
	InitVedioDeviceTree();
	
	//Init Warning Type
	m_cboWarnType.ResetContent();
	m_cboWarnType.AddString("���и澯"); //all
	m_cboWarnType.AddString("�ⲿ����"); //1001 
	m_cboWarnType.AddString("��Ƶ��ʧ"); //1002 
	m_cboWarnType.AddString("��̬���"); //1003 
	m_cboWarnType.AddString("Ӳ�̴���"); //1004
	m_cboWarnType.AddString("���ӳ�ʱ"); //2001
	m_cboWarnType.AddString("ͼƬ�澯"); //5001

	//Init Ack type
	m_cboWarnAckType.ResetContent();
	m_cboWarnAckType.AddString("��������"); //all
	m_cboWarnAckType.AddString("δȷ��");  //1
	m_cboWarnAckType.AddString("�澯��ʱ�Զ�ȷ��"); //2
	m_cboWarnAckType.AddString("�ֶ�ȷ��"); //3

	//Init Time
	m_cboStartHour.ResetContent();
	m_cboEndHour.ResetContent();
	m_cboStartMin.ResetContent();
	m_cboStartSec.ResetContent();
	m_cboEndMin.ResetContent();
	m_cboEndSec.ResetContent();
	i=0;
	CString strTime;
	for(; i<24; i++) //Hour
	{
		strTime.Format("%d",i);
		m_cboStartHour.AddString(strTime);
		m_cboEndHour.AddString(strTime);
	}

	i=0;
	for(; i<60; i++) //minute
	{
		strTime.Format("%d",i);
		m_cboStartMin.AddString(strTime);
		m_cboEndMin.AddString(strTime);

		m_cboStartSec.AddString(strTime);
		m_cboEndSec.AddString(strTime);
	}

	bool bDebug=false;
	int nCount = 0;
	if (bDebug)
	{
		//"�澯״̬", //level
		m_lstFindWarnResult.InsertItem(nCount,"",0);
		m_lstFindWarnResult.SetItem(nCount,0,LVIF_IMAGE,"",1,0,0,0);

		//"UUID",		//UUID
		//m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);
		
		//"λ��",		//place
		CString sLocation = "λ��"; //m_btsMgr.GetCameraPlace(pAlarmInfo->uuid);
		m_lstFindWarnResult.SetItem(nCount,1,LVIF_TEXT,sLocation,0,0,0,0);

		//�˾�����
		m_lstFindWarnResult.SetItem(nCount,2,LVIF_TEXT,"�˾�����",0,0,0,0);

		//"�澯����", //alarmCode :1:��ǰʵʱ�澯, ���ڷ����ĸ澯;2:��ʷ�澯��¼
		m_lstFindWarnResult.SetItem(nCount,3,LVIF_TEXT,"1001",0,0,0,0);
		
		//"��վ",		//BTSID
		m_lstFindWarnResult.SetItem(nCount,4,LVIF_TEXT,"btsid",0,0,0,0);
		//"��ʼʱ��", //start tiem
		m_lstFindWarnResult.SetItem(nCount,5,LVIF_TEXT,"2011-03-08 15:30:00",0,0,0,0);
		//"����ʱ��", //end   time
		m_lstFindWarnResult.SetItem(nCount,6,LVIF_TEXT,"2011-03-08 15:30:00",0,0,0,0);
		//"����״̬" //status
		CString sTemp = "�ֶ�ȷ��";
		m_lstFindWarnResult.SetItem(nCount,7,LVIF_TEXT,sTemp,0,0,0,0);	

		m_lstFindWarnResult.SetItem(nCount,8,LVIF_TEXT,"ͼ������",0,0,0,0);	

		m_lstFindWarnResult.SetItem(nCount,9,LVIF_TEXT,"uuid",0,0,0,0);	
	}

	CRect rc, rect, rectTree;
	GetClientRect(&rect);
	m_treeWarnMgr.GetWindowRect(rectTree);
	//m_wndWarnVedio.GetClientRect(&rc);
	m_wndWarnVedio.GetWindowRect(&rc);
	if (pDlgImage)
	{
		m_rcVedio.top    = rect.top;
		m_rcVedio.bottom = rectTree.bottom-12;
		m_rcVedio.left   = rectTree.right;
		m_rcVedio.right  = rect.right;

		pDlgImage->Create(IDD_DLG_IMAGE, this);
		pDlgImage->MoveWindow(&m_rcVedio);
		pDlgImage->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//Find Device...
void CWarningMgr::OnBnClickedBtnWarnMgrDeviceFind()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_DEVICE_FIND,str);

	if ( str.IsEmpty() ) return;

	BOOL bFind = FindNewTarget( str );
	if (!bFind)
		AfxMessageBox("û���ҵ�!");

}
//Find target warning...
void CWarningMgr::OnBnClickedFindTargetWarning()
{
	// TODO: Add your control notification handler code here
	//Clear old find Result
	m_lstFindWarnResult.DeleteAllItems();

	//int nWarnLevel = m_cboWarnType.GetCurSel();
	int nCategory = m_cboWarnType.GetCurSel();
	CString sCategory = (nCategory == 0) ? "all" :
						(nCategory == 1) ? "1001":
						(nCategory == 2) ? "1002":
						(nCategory == 3) ? "1003":
						(nCategory == 4) ? "1004":
						(nCategory == 5) ? "2001":
						(nCategory == 6) ? "5001":"all";


	int nAckType   = m_cboWarnAckType.GetCurSel();
	CString sAckType =  (nAckType == 0) ? "all" :
						(nAckType == 1) ? "1":
					    (nAckType == 2) ? "2":"3";
	// get as a CTime
	CTime timeStartDate,timeEndDate ;
	CString strStartDate,strEndDate;
	DWORD dwResult = m_dateBegin.GetTime(timeStartDate);
	if (dwResult == GDT_VALID)
		strStartDate = timeStartDate.Format(_T("%Y-%m-%d"));
	else
	{
		AfxMessageBox(_T("Start Time not set!"));
		return;
	}

	dwResult = m_dateEnd.GetTime(timeEndDate);
	if (dwResult == GDT_VALID)
		strEndDate = timeEndDate.Format(_T("%Y-%m-%d"));
	else
	{
		AfxMessageBox(_T("Start Time not set!"));
		return;
	}
	
	CString strStartTime, strEndTime;
	
	int nStartHour = m_cboStartHour.GetCurSel();
	if (nStartHour == -1) nStartHour = 0;

	int nEndHour = m_cboEndHour.GetCurSel();
	if (nEndHour == -1)	nEndHour=0;

	int nStartMin = m_cboStartMin.GetCurSel();
	if (nStartMin == -1) nStartMin = 0;

	int nEndMin = m_cboEndMin.GetCurSel();
	if (nEndMin == -1)	nEndMin = 0;

	int nStartSec = m_cboStartSec.GetCurSel();
	if (nStartSec == -1) nStartSec = 0;

	int nEndSec    = m_cboEndSec.GetCurSel();
	if (nEndSec == -1)	nEndSec = 0;
	
	strStartTime.Format("%02d:%02d:%02d",nStartHour,nStartMin,nStartSec);
	strEndTime.Format("%02d:%02d:%02d",nEndHour,nEndMin,nEndSec);

	//Get Current Selected Item UUID	
	CString sLevel, sLimit, sOffset, sQAlarmStr;
	sLimit.Empty();
	sOffset.Empty();
	sQAlarmStr.Empty();

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	//pApp->pgkclient->queryAlarmInfo(category,uuid,startDate,starttime,type,level,limit,offset,qalarmStr);
	//m_btsMgr.buildbtsTree(pApp->btsTotalStr, &m_treeWarnMgr);
	CString sNameItem = m_treeWarnMgr.GetItemText(m_hItemCurFind);
	
	CString sUUID; // = m_btsMgr.GetCameraUUID(sNameItem); 
	CString sCh;

	if (sNameItem.Find(':') !=-1) //Channel...
	{
		util::split_next(sNameItem, sCh,':',0);
		HTREEITEM hBtsItem = m_treeWarnMgr.GetParentItem(m_hItemCurFind);
		CString strText = m_treeWarnMgr.GetItemText(hBtsItem);
		if ( strText.Find('_') != -1 )
			util::split_next(strText,sUUID,'_',0);
		else
		{
			AfxMessageBox("��վ������ʽ����,���ܼ�����ѯ!");
			return;
		}

	}
	else if (sNameItem.Find('_') != -1 ) //Bts
	{
		util::split_next(sNameItem,sUUID,'_',0);
	}
	else
	{
		AfxMessageBox("��ѡ���վ����ͨ�����в�ѯ!");
		return ;
	}

	pApp->pgkclient->queryAlarmInfo(sCategory,sUUID, sCh, strStartDate,strStartTime,sAckType,sLevel,sLimit,sOffset,sQAlarmStr);
	if (sQAlarmStr.IsEmpty())
	{
		AfxMessageBox("û����ظ澯!");
		return;
	}

	//��ʾ��ظ澯��Ϣ���б���
	m_alarmMgr.getalarmList(sQAlarmStr,true);
	CString sLocation, sTemp;
	int nCount = 0;
	AlarmInfo* pAlarmInfo = NULL;	
	POSITION pos = m_alarmMgr.alarmList.GetHeadPosition();
	while( pos!=NULL )
	{
		pAlarmInfo = m_alarmMgr.alarmList.GetNext(pos);
		if (pAlarmInfo)
		{
			//"�澯״̬", //level
			m_lstFindWarnResult.InsertItem(nCount,"",0);
			m_lstFindWarnResult.SetItem(nCount,0,LVIF_IMAGE,"",atoi(pAlarmInfo->level),0,0,0);

			//"UUID",		//UUID
			//m_lstRuntimeWarning.SetItem(m_alarmIndex,1,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);
			
			//"λ��",		//place
			sLocation = m_btsMgr.GetCameraPlace(pAlarmInfo->BTSID);
			m_lstFindWarnResult.SetItem(nCount,1,LVIF_TEXT,sLocation,0,0,0,0);

			//�˾�����
			m_lstFindWarnResult.SetItem(nCount,2,LVIF_TEXT,pAlarmInfo->BTSType,0,0,0,0);

			//"�澯����", //alarmCode :1:��ǰʵʱ�澯, ���ڷ����ĸ澯;2:��ʷ�澯��¼
			m_lstFindWarnResult.SetItem(nCount,3,LVIF_TEXT,pAlarmInfo->alarmCode,0,0,0,0);
			
			//"��վ",		//BTSID
			m_lstFindWarnResult.SetItem(nCount,4,LVIF_TEXT,pAlarmInfo->BTSID,0,0,0,0);

			//Channel,		
			m_lstFindWarnResult.SetItem(nCount,5,LVIF_TEXT,pAlarmInfo->ChannelID,0,0,0,0);

			//"��ʼʱ��", //start tiem
			m_lstFindWarnResult.SetItem(nCount,6,LVIF_TEXT,pAlarmInfo->startTime,0,0,0,0);
			//"����ʱ��", //end   time
			m_lstFindWarnResult.SetItem(nCount,7,LVIF_TEXT,pAlarmInfo->endTime,0,0,0,0);
			//"����״̬" //status
			sTemp = pAlarmInfo->status==1 ? "δ����":
				pAlarmInfo->status==2 ? "��ʱ�Զ�����":"�ֶ�ȷ��";
			m_lstFindWarnResult.SetItem(nCount,8,LVIF_TEXT,sTemp,0,0,0,0);	

			int nCategory = atoi(pAlarmInfo->category);
			sTemp = nCategory == 1 ?  "��Ƶ":
					nCategory == 2 ?  "ͼƬ": "��";
			m_lstFindWarnResult.SetItem(nCount,9,LVIF_TEXT,sTemp,0,0,0,0);	

			m_lstFindWarnResult.SetItem(nCount,10,LVIF_TEXT,pAlarmInfo->uuid,0,0,0,0);	

			nCount++;

		}// ArmList
	}	
}

void CWarningMgr::InitVedioDeviceTree(void)
{
	bool bTest = false;
	if (bTest)
	{
		enum {euHZ, euYH};
		char sCitys[2][10] = {"����","�ຼ"};
		char sHZCityDistrict[6][10] = {"�ϳ���", "�³���","������","������","��ɳ��","������"};
		//char sHZ_ShangCQ[6][10] = {"ShangCQ1","ShangCQ2","ShangCQ3","ShangCQ4","ShangCQ5","ShangCQ6"};
		char sHZ_CQ_CAMERA[6][10] = {"ShangCQ","XiaCQ","XiHQ","JiangGQ","XiaSQ","BinJQ"};

		char sYHCityDistrict[6][10] = {"��һ��", "�����","������","������","������","������"};
		char sYH_CQ_CAMERA[6][10] = {"YuYQ","YuEQ","YuSQ","YuSQ","YuWQ","YuLQ"};

		//char *pDistrict[2][]={sHZCityDistrict, sYHCityDistrict};

		HTREEITEM m_hRootItem = m_treeWarnMgr.InsertItem("BTS List", 0, 0);
		m_treeWarnMgr.SetItemState(m_hRootItem, TVIS_BOLD, TVIS_BOLD);

		int nCitys = 2;
		int nDistrict = 6;
		int nCamera	  = 6;
		CString sCity, sDistrict, sCamera;
		for (int nC=0; nC<nCitys; nC++)
		{
			sCity = sCitys[nC];
			HTREEITEM hCity = m_treeWarnMgr.InsertItem(sCity, 1, 1, m_hRootItem);
			switch(nC)
			{
			case euHZ:
				{
					for (int nDS = 0; nDS<nDistrict; nDS++)
					{
		
						sDistrict = sHZCityDistrict[nDS];
						HTREEITEM hDistrict = m_treeWarnMgr.InsertItem(sDistrict, 1,1, hCity);
						for(int nCM=0; nCM<nCamera; nCM++) //Each District have six camera
						{
								sCamera.Format("%s_Camera_%d",sHZ_CQ_CAMERA[nDS], nCM);
								m_treeWarnMgr.InsertItem(sCamera,2,2, hDistrict);
						}
					}
				}
				break;
			case euYH:
				{
					for (int nDS = 0; nDS<nDistrict; nDS++)
					{
		
						sDistrict = sYHCityDistrict[nDS];
						HTREEITEM hDistrict = m_treeWarnMgr.InsertItem(sDistrict,1,1, hCity);
						for(int nCM=0; nCM<nCamera; nCM++) //Each District have six camera
						{
								sCamera.Format("%s_Camera_%d",sYH_CQ_CAMERA[nDS], nCM);
								m_treeWarnMgr.InsertItem(sCamera,2,2, hDistrict);
						}
					}
				}
				break;
			default:
				break;
			}
		}

		return;
	}

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	m_btsMgr.buildbtsTree(pApp->btsTotalStr, &m_treeWarnMgr);
	

	/*
	POSITION pos = pApp->pgkclient->btsmanager.btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		pApp->pgkclient->btsmanager.btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo)
		{			
			if(pBTSInfo->parentuuid=="0")
			{
				HTREEITEM htitem=pBTSInfo->insertRootIntoTree(&m_treeWarnMgr);
			}
			else
			{
				BTSInfo *pParentInfo=pApp->pgkclient->btsmanager.btsmap[util::str2int(pBTSInfo->parentuuid)];
				pBTSInfo->insertIntoTree(&m_treeWarnMgr, pParentInfo->getPosInTree());
			}

		}
	}
	*/

	
	//buildbtsTree(pApp->btsTotalStr, &m_treeWarnMgr);
}
HTREEITEM CWarningMgr::FindTarget(HTREEITEM  hItem, CString sName, bool bFindNext)
{
	if (hItem == NULL)
		hItem = m_treeWarnMgr.GetRootItem();

	if (hItem == NULL)
		return NULL;

	CString szString = m_treeWarnMgr.GetItemText(hItem);
	if (szString.Find(sName) > -1)
	{
		m_treeWarnMgr.SelectSetFirstVisible(hItem);
		m_treeWarnMgr.SelectItem(hItem);
		return hItem;
	}


	if (m_treeWarnMgr.ItemHasChildren(hItem) == FALSE)
		return NULL;

	//�ݹ����hItem�������ӽڵ�
	HTREEITEM hRes = NULL;
	HTREEITEM hItemChild = m_treeWarnMgr.GetChildItem(hItem);
	while (hItemChild)
	{
		hRes = FindTarget(hItemChild, sName); //����hItemΪ����֦
		if (hRes != NULL)					  //�������hItemΪ����֦���ҵ������ؽ��
			return hRes;
		else								  //���򣬲�����hItemͬ������һ��֦
			hItemChild = m_treeWarnMgr.GetNextSiblingItem(hItemChild);
	} // end of while(hItem != NULL, has next item)

	return NULL; 
}

BOOL CWarningMgr::FindNewTarget(CString sFindStr)
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

BOOL CWarningMgr::FindNextTarget(void)
{
	if (m_sFindStr.IsEmpty())
		return FALSE;

	BOOL bRet = FALSE;

	HTREEITEM hItemFind = NULL;
	//Child's Item need to find.
	HTREEITEM hItemChild = m_treeWarnMgr.GetChildItem(m_hItemCurFind);
	//if (hItemChild)
	//	hItemFind = FindTarget(hItemChild, m_sFindStr);
	if(hItemChild)
	{
		while(hItemChild)
		{
			hItemFind = FindTarget(hItemChild, m_sFindStr);

			if (hItemFind)
			{
				m_hItemCurFind = hItemFind;

				m_treeWarnMgr.SelectItem(m_hItemCurFind);
				
				return TRUE;
			}

			hItemChild = m_treeWarnMgr.GetNextSiblingItem(hItemChild);
		}
	}


	//Brother's Item need to find.
	HTREEITEM hItemBrother=m_treeWarnMgr.GetNextSiblingItem(m_hItemCurFind); //ignor itself
	while(hItemBrother)
	{
		hItemFind = FindTarget(hItemBrother, m_sFindStr);
		if (hItemFind)
		{
			m_hItemCurFind = hItemFind;
			return TRUE;
		}
		hItemBrother=m_treeWarnMgr.GetNextSiblingItem(hItemBrother);
	}

	//Father's Item Need to find
	HTREEITEM hItemFather = m_treeWarnMgr.GetParentItem(m_hItemCurFind);
	while(hItemFather)
	{
		CString szString = m_treeWarnMgr.GetItemText(hItemFather); //ignor itself
		hItemBrother = hItemFather; //polling all item of Father's brother
		do //all of the brother...
		{
			hItemBrother = m_treeWarnMgr.GetNextSiblingItem(hItemBrother);
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

		hItemFather = m_treeWarnMgr.GetParentItem(hItemFather); //polling father's father
	}
	
	//-----------------------

	return bRet;
}
BOOL CWarningMgr::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message >= WM_KEYFIRST &&	pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_F3: //Find Next
			{
				if (pMsg->lParam>0)
				{
					//m_bFindNext=TRUE;
					BOOL bFind = FindNextTarget( );
					if (!bFind)
						AfxMessageBox("û���ҵ�!");
				}
			}
			break;				
		default:
			break;
		};
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CWarningMgr::OnNMClickTreeWarnMgrDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
    UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_treeWarnMgr.ScreenToClient(&curPoint);
	HTREEITEM htItem = m_treeWarnMgr.HitTest(curPoint, &nFlags);

	if ( htItem && (TVHT_ONITEM & nFlags) )
	{
		//CString str = m_treeWarnMgr.GetItemText(htItem);
		m_hItemCurFind = htItem;
	}

	*pResult = 0;
}

void CWarningMgr::OnHdnItemclickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CWarningMgr::OnNMClickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_nCurItem = pNMItemActivate->iItem;

	*pResult = 0;
}

//Play the target warning vedio
void CWarningMgr::OnNMDblclkLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMItemActivate->iItem == -1)
	{
		*pResult = 0;
		return;
	}	

	m_nCurItem = pNMItemActivate->iItem;

	CString sBTSID		= m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 4);			
	CString sCh			= m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 5);			
	CString sStartTime  = m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 6); 
	CString sEndTime    = m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 7); 
	CString sCategory	= m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 9);
	CString AlarmID		= m_lstFindWarnResult.GetItemText(pNMItemActivate->iItem, 10);


	BOOL bDebug = FALSE;
	if (bDebug)
	{
		PLAY_CloseFile(cnWARNING_VEDIO);

		CString sVVFile;
		CString path="F:\\Projects\\Video\\BTSMonitor\\test\\";
		CString sVideo[] = {"test01.h264","test02.h264","test03.h264","test04.h264","test05.h264","test06.h264","test07.h264"};

		
		sVVFile = path + sVideo[1];
		
		BOOL bPlayFile = TRUE;
		if (bPlayFile)
		{
			PLAY_OpenFile(cnWARNING_VEDIO, sVVFile.GetBuffer());
		
			PLAY_Play(cnWARNING_VEDIO, m_wndWarnVedio.m_hWnd);
		}

		return;

	}

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if (sCategory == "��Ƶ")
	{
		ShowButton(TRUE);			
		pDlgImage->ShowWindow(SW_HIDE);

		//Stop Vedio Thread...
		//pApp->pgkclient->StopAlamVideoPlay();
		pApp->pgkclient->Stop_Play(cnWARNING_VEDIO);
		BOOL bPlay = PLAY_Stop(cnWARNING_VEDIO);		
		BOOL bOpenRet = PLAY_CloseStream(cnWARNING_VEDIO);			


		//Open New ---------------------------------------------
		PLAY_SetStreamOpenMode(cnWARNING_VEDIO, STREAME_REALTIME);
		bOpenRet = PLAY_OpenStream(cnWARNING_VEDIO,0,0,1024*900);
		if(bOpenRet)
		{
			PLAY_Play(cnWARNING_VEDIO, m_wndWarnVedio.m_hWnd);
			//Play Remote Vedio runatime			
			pApp->pgkclient->replay(AlarmID, play_video, cnWARNING_VEDIO);

			//Reload first, then play the video.????
		}

	}
	else if (sCategory == "ͼƬ")
	{
		ShowButton(FALSE);			
		pDlgImage->ShowWindow(SW_SHOW);

		int  err;
		CString sDateTime, sBts, sCh;
		CString sRoute		= pApp->pgkclient->btsmanager.GetRouteByUUID(sBTSID);
		//MonitorImage *pMoImage = pApp->pgkclient->getAlarmImagebyBase64(sUUID, &err);
		MonitorImage *pMoImage = pApp->pgkclient->getAlarmImagebyBase64(sBTSID, "", sRoute, AlarmID,&err);
		if (pMoImage)
		{

			//Save Image to local Directory
			CString sJpgName;
			util::InitApp();
			char *pAlarmImageDir = util::GetAppPath();
			strcat(pAlarmImageDir,"AlarmImage");

			//Remove all file of this directory...
			//util::DeleteAllFile(pAlarmImageDir);

			//Save current alarm image to the directory.
			CString sLine, str;
			int pos=0;
			int err=0;
			while ( pMoImage->getNextImage(&err))
			{
				//pos = util::split_next(pMoImage->datetime,str,'-',0);
				//sDateTime+=str;//YY
				pos = util::split_next(pMoImage->datetime,str,'-',pos+1);
				sDateTime+=str;//YY
				pos = util::split_next(pMoImage->datetime,str,'-',pos+1);
				sDateTime+=str;//MM
				pos = util::split_next(pMoImage->datetime,str,' ',pos+1);
				sDateTime+=str;//DD
				pos = util::split_next(pMoImage->datetime,str,'-',pos+1);
				sDateTime+=str;//hh
				pos = util::split_next(pMoImage->datetime,str,'-',pos+1);
				sDateTime+=str;//mm
				//pos = util::split_next(pMoImage->datetime,str,'-',pos+1);
				str = pMoImage->datetime.Mid(pos+1);
				sDateTime+=str;//ss

				pos = util::split_next(pMoImage->bts,str,':',0);
				//pos = util::split_next(pMoImage->bts,str,':',pos+1);
				pMoImage->bts.Mid(pos+1);
				sBts+=str;

				pos = util::split_next(pMoImage->channel,str,':',0);
				//pos = util::split_next(pMoImage->channel,str,':',pos+1);
				pMoImage->channel.Mid(pos+1);
				sCh+=str;

				sJpgName.Format("%s%s%s_%s_%s.jpg", pAlarmImageDir,"\\", sBts, sCh, sDateTime);
				pMoImage->savedata(sJpgName);
			}


			pDlgImage->Initialize(pAlarmImageDir,2,m_rcVedio.Width()/2-1, m_rcVedio.Height()/2-1);
		}

	}
	else
	{
		AfxMessageBox("�޼����Ϣ��¼!");
	}

	*pResult = 0;
}

void CWarningMgr::OnNMRClickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
    UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_menuPt = curPoint;
	m_lstFindWarnResult.ScreenToClient(&curPoint);
	int nItem = m_lstFindWarnResult.HitTest(curPoint, &nFlags);

	m_nCurItem = pNMItemActivate->iItem;

//	if ( nItem && (TVHT_ONITEM & nFlags) )
//	{
//		m_hItemCurFind = htItem;
//	}

	CMenu myMenu ;
	myMenu.LoadMenu( IDR_POPMENU_WARNMGR ) ;
	CMenu *pMenu = myMenu.GetSubMenu(0) ;
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, m_menuPt.x, m_menuPt.y, this ) ;

	m_bPopMenu = true;

	*pResult = 0;
}

void CWarningMgr::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	//if (m_bPopMenu)
	//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPMENU_WARNMGR, m_menuPt.x, m_menuPt.y, this, TRUE);	

	//m_bPopMenu = false;
}

void CWarningMgr::OnWarningmgrExport()
{
	// TODO: Add your command handler code here
	CFileDialog FileDlg(TRUE,_T(""),"WarningInfomation.txt",OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,NULL,NULL);
	FileDlg.m_ofn.lpstrTitle = "";
	char sDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH,sDir);
	FileDlg.m_ofn.lpstrInitialDir = sDir;
	CString strMapPath;
	if(FileDlg.DoModal()==IDOK)
	{
		strMapPath = FileDlg.GetPathName();
		//if(!FileExists(m_strMapPath))
		//	return;
	}
	else
		return;

	//save listview info to the file
	CFileException error;
	CFile fVideoInfo;//(strMapPath,
	if ( !fVideoInfo.Open(strMapPath,CFile::modeCreate|CFile::modeWrite, &error) )
	{
		char   szCause[255];
		CString strFormatted;
		error.GetErrorMessage(szCause, 255);
		// (in real life, it's probably more
		// appropriate to read this from
		//  a string resource so it would be easy to
		// localize)
		strFormatted = _T("The data file could not be opened because of this error: ");
		strFormatted += szCause;
		AfxMessageBox(strFormatted);
		return;
	}
	
	//File Title
	char lpBuf[MAX_PATH];
	memset(lpBuf,0,MAX_PATH);
	char *pTitle = "�澯����\tλ��\t�˾�����\t�澯����\t��վ\tͨ����\t��ʼʱ��\t����ʱ��\t����״̬\tͼ������";
	sprintf_s(lpBuf,MAX_PATH, "%s\r\n", pTitle);
	fVideoInfo.Write(lpBuf,strlen(lpBuf));
	
	LVITEM lvItem;
	CString strTemp;
	int nCnt = m_lstFindWarnResult.GetItemCount();
	for (int i=0; i<nCnt; i++)
	{
		//"�澯״̬", //level
		lvItem.mask = LVIF_IMAGE;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		m_lstFindWarnResult.GetItem(&lvItem);
		_itoa_s(lvItem.iImage,lpBuf,sizeof(lpBuf), 10);
		fVideoInfo.Write(lpBuf, strlen(lpBuf));
		fVideoInfo.Write("\t", 1);
		
		//"λ��",		//place
		strTemp = m_lstFindWarnResult.GetItemText(i,1) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//�˾����� .BTSType
		strTemp = m_lstFindWarnResult.GetItemText(i,2) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"�澯����", //alarmCode :1:��ǰʵʱ�澯, ���ڷ����ĸ澯;2:��ʷ�澯��¼
		strTemp = m_lstFindWarnResult.GetItemText(i,3) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());
		
		//"��վ",		//BTSID
		strTemp = m_lstFindWarnResult.GetItemText(i,4) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//Channel,		
		strTemp = m_lstFindWarnResult.GetItemText(i,5) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"��ʼʱ��", //start tiem
		strTemp = m_lstFindWarnResult.GetItemText(i,6) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"����ʱ��", //end   time
		strTemp = m_lstFindWarnResult.GetItemText(i,7) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//"����״̬" //status	sTemp = pAlarmInfo->status==1 ? "δ����":pAlarmInfo->status==2 ? "��ʱ�Զ�����":"�ֶ�ȷ��";	
		strTemp = m_lstFindWarnResult.GetItemText(i,8) + "\t";
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		//ͼ������
		strTemp = m_lstFindWarnResult.GetItemText(i,9) + "\r\n";	
		fVideoInfo.Write(strTemp, strTemp.GetLength());

		strTemp = m_lstFindWarnResult.GetItemText(i,10) + "\r\n";	
		fVideoInfo.Write(strTemp, strTemp.GetLength());
	}

	//..
	fVideoInfo.Close();
}

void CWarningMgr::OnUpdateWarningmgrExport(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CWarningMgr::OnWarningmgrSaveas()
{
	// TODO: Add your command handler code here
	//������Ƶ ����ͼƬ

	//Get Current Item Type...
	if (m_nCurItem<0)  return;

	CString sTime,strTemp,sVideoName, sBTSID, sCh, sStartTime, sEndTime, sCategory, AlarmID;
	sBTSID		= m_lstFindWarnResult.GetItemText(m_nCurItem, 4);			
	sCh			= m_lstFindWarnResult.GetItemText(m_nCurItem, 5);			
	sStartTime  = m_lstFindWarnResult.GetItemText(m_nCurItem, 6); 
	sEndTime    = m_lstFindWarnResult.GetItemText(m_nCurItem, 7); 
	sCategory	= m_lstFindWarnResult.GetItemText(m_nCurItem, 9);
	AlarmID		= m_lstFindWarnResult.GetItemText(m_nCurItem, 10);

	int pos = 0;
	pos = util::split_next(sStartTime,	strTemp,'-',pos);
	sTime+=strTemp;//YY
	pos = util::split_next(sStartTime,		strTemp,'-',pos+1);
	sTime+=strTemp;//MM
	pos = util::split_next(sStartTime,		strTemp,' ',pos+1);
	sTime+=strTemp;//DD
	pos = util::split_next(sStartTime,		strTemp,':',pos+1);
	sTime+=strTemp;//hh
	pos = util::split_next(sStartTime,		strTemp,':',pos+1);
	sTime+=strTemp;//mm
	strTemp = sStartTime.Mid(pos+1);
	sTime+=strTemp;//ss

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if (sCategory=="��Ƶ")
	{
		sVideoName.Format("%s_%s_%s.h264", sBTSID,sCh,sTime);

		CString sWarnVideoName;
		util::InitApp();
		char *pAlarmImageDir = util::GetAppPath();
		strcat(pAlarmImageDir,"AlarmVideo");
		sWarnVideoName.Format("%s%s%s", pAlarmImageDir,"\\", sVideoName);

		pApp->pgkclient->StopAlamVideoPlay();

		pApp->pgkclient->replay(AlarmID, play_video, cnWARNING_VEDIO,sWarnVideoName,true);

	}
	else
		sVideoName.Format("%s_%s_%s.jpg", sBTSID,sCh,sTime);

	/*
	CFileDialog FileDlg(TRUE,_T(""),sVideoName,OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,NULL,NULL);
	FileDlg.m_ofn.lpstrTitle = "";
	char sDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH,sDir);
	FileDlg.m_ofn.lpstrInitialDir = sDir;
	CString strMapPath;
	if(FileDlg.DoModal()==IDOK)
	{
		strMapPath = FileDlg.GetPathName();
		//if(!FileExists(m_strMapPath))
		//	return;
		int i=0;
	}
	else
		return;
	*/

}

void CWarningMgr::OnUpdateWarningmgrSaveas(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CWarningMgr::ShowButton(bool bShow)
{
	if (bShow) //Show
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_BTN_PAUSE);
		if (pBtn) pBtn->ShowWindow(SW_SHOW);
	
		pBtn = (CButton *)GetDlgItem(IDC_BTN_PLAY);
		if (pBtn) pBtn->ShowWindow(SW_SHOW);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_STOP);
		if (pBtn) pBtn->ShowWindow(SW_SHOW);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_FAST_FORWARD);
		if (pBtn) pBtn->ShowWindow(SW_SHOW);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_GOTO_BEGIN);
		if (pBtn) pBtn->ShowWindow(SW_SHOW);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_GOTO_END2);
		if (pBtn) pBtn->ShowWindow(SW_SHOW);

		if ( m_wndWarnVedio.m_hWnd )
			m_wndWarnVedio.ShowWindow(SW_SHOW);


	}
	else
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_BTN_PAUSE);
		if (pBtn) pBtn->ShowWindow(SW_HIDE);
	
		pBtn = (CButton *)GetDlgItem(IDC_BTN_PLAY);
		if (pBtn) pBtn->ShowWindow(SW_HIDE);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_STOP);
		if (pBtn) pBtn->ShowWindow(SW_HIDE);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_FAST_FORWARD);
		if (pBtn) pBtn->ShowWindow(SW_HIDE);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_GOTO_BEGIN);
		if (pBtn) pBtn->ShowWindow(SW_HIDE);

		pBtn = (CButton *)GetDlgItem(IDC_BTN_GOTO_END2);
		if (pBtn) pBtn->ShowWindow(SW_HIDE);

		if ( m_wndWarnVedio.m_hWnd )
			m_wndWarnVedio.ShowWindow(SW_HIDE);

	}

}