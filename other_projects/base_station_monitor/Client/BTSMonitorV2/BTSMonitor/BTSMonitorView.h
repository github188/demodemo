// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// BTSMonitorView.h : CBTSMonitorView 类的接口
//

#include "const.h"

#pragma once


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
//protected: // 仅从序列化创建
public:
	CBTSMonitorView();
	DECLARE_DYNCREATE(CBTSMonitorView)

// 属性
public:
	CBTSMonitorDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CBTSMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
	void StopMonitorBTS(int nViewIndex);

	//Task List
	static void ProcessTask(LPVOID pv);
	void	SaveTaskInfo(int nVV, CString& sUUID, CString& sCh);
	CWinThread *m_pTaskThread;
};

#ifndef _DEBUG  // BTSMonitorView.cpp 中的调试版本
inline CBTSMonitorDoc* CBTSMonitorView::GetDocument() const
   { return reinterpret_cast<CBTSMonitorDoc*>(m_pDocument); }
#endif

