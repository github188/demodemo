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

// BTSMonitorView.h : CBTSMonitorView ��Ľӿ�
//


#pragma once

#include "const.h"



typedef struct tagVV_INFO
{
	CView* vv;
	CRect  rc;
	BOOL   bMonitoring;
	CString sUUID;
	CString sCh;
	int		nImageType; //0: unknow, 1 Video, 2 Piture
}VV_INFO;
typedef enum tagVV_COUNT
{
	VV_1 = 1,
	VV_4 = 4,
	VV_6 = 6,
	VV_8 = 8,
	VV_9 = 9,
	VV_16= 16,
	VV_25= 25
}VV_COUNT;
typedef struct tagVV_STATUS
{
	int  activeid;  //acitve vv id
	BOOL bFullwindow;
	VV_COUNT vvcount;  
}VV_STATUS;
typedef struct tagVV_CONTROL
{
	VV_STATUS  vvStatus;
	VV_INFO    vvInfo[cnMAX_VV];
}VV_CONTROL;

/////////////////////////////////////
//int play_video(int  sessionId, char * pBuffer, int  len);

/////////////////////////////////////
class CBTSMonitorView : public CView
{
//protected: // �������л�����
public:
	CBTSMonitorView();
	DECLARE_DYNCREATE(CBTSMonitorView)

// ����
public:
	CBTSMonitorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CBTSMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	//afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPlayviewSelected(WPARAM wParam, LPARAM lParam);

private:
	VV_CONTROL m_vvControl;

	//VV_INFO m_vvInfo[cnMAX_VV];

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SelectVideoView(int nVideoViewID);
//	afx_msg void OnVv1();
	void StartMonitorBTS(CString strBtsInfo);
	void StartMonitorBTS(int nVV, CString sUUID, CString sCh, int nCategory=0);
	void StopMonitorBTS(int nViewIndex, int status=0);

	//Task List
	static void ProcessTask(LPVOID pv);
	void	SaveTaskInfo(int nVV, CString& sUUID, CString& sCh);
	CWinThread *m_pTaskThread;
};

#ifndef _DEBUG  // BTSMonitorView.cpp �еĵ��԰汾
inline CBTSMonitorDoc* CBTSMonitorView::GetDocument() const
   { return reinterpret_cast<CBTSMonitorDoc*>(m_pDocument); }
#endif

