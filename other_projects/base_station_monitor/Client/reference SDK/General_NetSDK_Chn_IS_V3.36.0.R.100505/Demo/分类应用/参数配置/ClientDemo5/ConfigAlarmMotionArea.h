#if !defined(AFX_CONFIGALARMMOTIONAREA_H__3DA25876_BE0B_4847_9843_4F9E62F60406__INCLUDED_)
#define AFX_CONFIGALARMMOTIONAREA_H__3DA25876_BE0B_4847_9843_4F9E62F60406__INCLUDED_

#include "dhnetsdk.h"
#include "ConfigAlarmMotionAreaBlock.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigAlarmMotionArea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigAlarmMotionArea dialog

class CConfigAlarmMotionArea : public CDialog
{
// Construction
public:
	CConfigAlarmMotionArea(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigAlarmMotionArea)
	enum { IDD = IDD_CONFIG_ALARM_MOTION_AREA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigAlarmMotionArea)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigAlarmMotionArea)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetArea(BYTE *area, WORD wRows, WORD wCols);
	BYTE *GetArea(){return (BYTE*)m_myArea;}
	void RecordPoint(BYTE x, BYTE y);
	void ReleasePoint();
	void MovePoint(DWORD dwParm, BYTE x, BYTE y);
private:
	CConfigAlarmMotionAreaBlock m_block[DH_MOTION_ROW][DH_MOTION_COL];
	BYTE			m_myArea[DH_MOTION_ROW][DH_MOTION_COL];
	CRect			m_rect;
	int				m_blckWid;
	int				m_blckHght;
	
	BOOL			m_bDrawing;
	POINT			m_stpoint;
	POINT			m_lastpoint;

	WORD			m_wRows;
	WORD			m_wCols;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGALARMMOTIONAREA_H__3DA25876_BE0B_4847_9843_4F9E62F60406__INCLUDED_)
