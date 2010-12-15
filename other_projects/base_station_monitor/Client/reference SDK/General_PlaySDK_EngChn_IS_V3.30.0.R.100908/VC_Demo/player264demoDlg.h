// player264demoDlg.h : header file
//

#if !defined(AFX_PLAYER264DEMODLG_H__97C5D021_664D_4091_ABAA_7470E715E021__INCLUDED_)
#define AFX_PLAYER264DEMODLG_H__97C5D021_664D_4091_ABAA_7470E715E021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SYSINFO.h"
#include "NiceSlider.h"
#include "WaterCheckDlg.h"
#include "AVIConvertDlg.h"
#include "HoverButton.h"
#include "SetColorDlg.h"
#include "SetPicPath.h"
#include "VideoShowStatic.h"
#include "CaptureSoundDlg.h"


/////////////////////////////////////////////////////////////////////////////
typedef enum __tPLAYSTATE
{
	PlayState_READY = 0,
	PlayState_Playing,
	PlayState_Pause,
	PlayState_Stop,
	PlayState_FAST,
	PlayState_Slow,
	PlayState_StepForward,
	PlayState_StepBackWard
} tPlayState;
/////////////////////////////////////////////////////////////////////////////
// CPlayer264demoDlg dialog

class CPlayer264demoDlg : public CDialog
{
// Construction
public:
	void Refurish();
	CPlayer264demoDlg(CWnd* pParent = NULL);	// standard constructor
	~CPlayer264demoDlg();

	void SetWindowSize();
	void PlayFile(CString csFilePath);
	void PlayStreamFile(CString csFilePath);
	
	CBitmap		m_BMPOverlay;
	CBitmap		m_BMPBlack;
	void		SavePicTmp();
	void		StartDraw();
	WINDOWPLACEMENT m_OldWndplacement;
	BOOL		m_bIsFullScreen;
	void		OnWndFullScreen();
	CRect		GetOnPicRect(CRect rcWnd,CRect rcOnWnd,LONG nPicWidth,LONG nPicHeight);
	CRect		m_rcDraw;
	bool		m_IsPlaying;
	CPoint		m_StartPoint;
	BOOL		m_bStartDraw;
	bool		m_IsRepeatPlay ;
	CRect		m_rcDisplay;
	BOOL		m_bNeedFileClose;
	CMenu* m_pMainMenu ;
	CBitmap m_bitmap ;
	bool m_pause ;
	bool m_playnormal ;
	enum{PORT = 0} ;
	enum{MAX_INDEX_BUF_LEN = 16*60000} ;
	CString m_csfileName ; //ÎÄ¼þÃû

	BYTE *m_fileindexbuf ;
	DWORD m_fileindexlen ;

	int m_iTimerID;

	BOOL m_bFileOpen;
	BOOL m_bPlaySound;

	long m_nWidth;
	long m_nHeight;

	CWaterCheckDlg* m_WaterCheckDlg;
	CAVIConvertDlg* m_AVIConvDlg;
	CSetColorDlg*   m_SetColorDlg;
	CSetPicPath*	m_SetPicPathDlg;
	CaptureSoundDlg* m_CaptureSoundDlg;

	tPlayState m_tPlayState;
	void SetButtonsState(BOOL bEnable);
	void DrawPlayState(CString& csPlayState, CString& csTimeState);
	void GetPlayState(CString& csPlayState);
	CString TimeToString(DWORD dwTime);

	CString m_csPicSavePath;

	int m_iFilePlayMode;
	DWORD m_dwCurFilePos;
	DWORD m_dwFileLen;
	HANDLE m_hPlayThread;
	BOOL  m_bSrcFileChange;
	CFile m_StreamSrcFile;
	void StreamFilePlay_Thread();

// Dialog Data
	//{{AFX_DATA(CPlayer264demoDlg)
	enum { IDD = IDD_PLAYER264DEMO_DIALOG };
	CHoverButton	m_BNPause;
	CNiceSliderCtrl	m_SoundSlider;
	CHoverButton	m_BNSetColor;
	CHoverButton	m_BNFullScreen;
	CHoverButton	m_BNToEnd;
	CHoverButton	m_BNToBegin;
	CHoverButton	m_BNStop;
	CHoverButton	m_BNSound;
	CHoverButton	m_BNSlow;
	CHoverButton	m_BNPicCatch;
	CHoverButton	m_BNStepOne;
	CHoverButton	m_BNBackOne;
	CHoverButton	m_BNFast;
	CHoverButton	m_BNPlay;
	CNiceSliderCtrl	m_sliderplay;
	CVideoShowStatic	m_playregion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayer264demoDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPlayer264demoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPlay();
	afx_msg void OnFileOpen();
	afx_msg void OnPause();
	afx_msg void OnFast();
	afx_msg void OnSlow();
	afx_msg void OnBackone();
	afx_msg void OnOne();
	afx_msg void OnTobegin();
	afx_msg void OnToend();
	afx_msg void OnPiccatch();
	afx_msg void OnStop();
	afx_msg void OnFileClose();
	afx_msg void OnAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLocate();
	afx_msg void OnRepeat();
	afx_msg void PlayMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnResetbuf();
	afx_msg void OnExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();
	afx_msg void OnPartDisplay();
	afx_msg void OnSysinfo();
	afx_msg void OnVerticalSync();
	afx_msg void OnCutfile();
	afx_msg void OnStaticShow();
	afx_msg void OnFullscreen();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCustomdrawSliderPlay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomdrawSliderSound(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDecAudio();
	afx_msg void OnDecVideo();
	afx_msg void OnDecComplex();
	afx_msg void OnDecNone();
	afx_msg void OnLanChinese();
	afx_msg void OnLanEnglish();
	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnPicTypeSet();
	afx_msg void OnWatermarkCheck();
	afx_msg void OnFilePlayMode();
	afx_msg void OnMediaInfo();
	afx_msg void OnAVIConvert();
	afx_msg void OnSound();
	afx_msg void OnButtonSetcolor();
	afx_msg void OnPicPathSet();
	afx_msg void OnCaptureSound();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nPrePlayPosition;

	afx_msg	BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYER264DEMODLG_H__97C5D021_664D_4091_ABAA_7470E715E021__INCLUDED_)
