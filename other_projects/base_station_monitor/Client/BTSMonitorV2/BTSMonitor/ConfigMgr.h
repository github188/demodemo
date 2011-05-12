#pragma once

//
class CTaskItem : public CObject
{

public:
	CTaskItem();
	CTaskItem(const CTaskItem& task);
    CTaskItem& operator=(CTaskItem& task);
	~CTaskItem();
public:
	//Base infomation.
	CString sName;
	CString sBTSName;
	CString sUUID;
	CString sCh;
	int		nVV;
	CString sBeginDate;
	CString sEndDate;
	int		nBeginHour;
	int		nBeginMin;
	int		nBeginSec;
	int		nEndHour;
	int		nEndMin;
	int		nEndSec;

	//Task Status
	int status; //0 reserve, 1 waiting, 2 runing

	//Restore old monitoring
	CString sUUID_Old; //UUID$Channel
	CString sCh_Old;



};
// CConfigMgr command target

class CConfigMgr : public CObject
{
public:
	CConfigMgr();
	virtual ~CConfigMgr();

public:
	//Warning Config Mgr...
	void SetPicWinIndex(int index) {m_nPicWinIndex = index;};
	int GetPicWinIndex() {return m_nPicWinIndex;};

	void SetPlayPicInterval(int index) {m_nPlayPicInterval = index;};
	int GetPlayPicInterval() {return m_nPlayPicInterval;};

	void SetMaxPopWarnWin(int index) {m_nMaxPopWarnWin = index;};
	int GetMaxPopWarnWin() {return m_nMaxPopWarnWin;};

	void SetPopWarnSound(CString sPopWarnSound) {m_sPopWarnSound = sPopWarnSound;};
	CString GetPopWarnSound( ) {return m_sPopWarnSound;};

	void SetPopWarnEnable(bool bEnable) { m_bPopWarnEnable = bEnable;};
	bool GetPopWarnEnable() {return m_bPopWarnEnable;};

	//Task Config
	void GetTaskList(CObArray& objArray) {		objArray.Copy( *m_pArrTask);	};

	CObArray* GetTaskList() {	return	m_pArrTask; };
	DWORD WaitForTask()		{	return	::WaitForMultipleObjects(2,m_hCtrlTask,FALSE, 10000); };
	BOOL  SetTaskSignal()	{	return	::SetEvent(m_hCtrlTask[0]); };
	BOOL  SetTaskNoSignal() {	return	::ResetEvent(m_hCtrlTask[0]); };
	BOOL  SetTaskExit()		{	return  ::SetEvent(m_hCtrlTask[1]); };

private:
	//Warning Config Mgr...
	int		m_nPicWinIndex;
	int		m_nPlayPicInterval;
	int		m_nMaxPopWarnWin;
	CString m_sPopWarnSound;
	bool    m_bPopWarnEnable;

	//Task Config Mgr..
	CObArray*  m_pArrTask;	
	HANDLE	   m_hCtrlTask[2];

};

extern CConfigMgr m_gConfigMgr;
