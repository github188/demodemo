#include "stdafx.h"
#include "LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRITICAL_SECTION CLogFile::cs;

CLogFile::CLogFile()
{
	//InitializeCriticalSection(&cs);	
}

CLogFile::~CLogFile()
{

}
//获取文件名称
CString CLogFile::GetFileName()
{
    CString m_sFileName;

    m_sFileName = CTime::GetCurrentTime().Format("%Y-%m-%d") + ".log";
    
    return m_sFileName;
}
//获取应用程序所在路径
CString CLogFile::GetFilePath()
{   
    CString m_FilePath;

    GetModuleFileName(NULL,m_FilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);

    m_FilePath.ReleaseBuffer();

    int m_iPosIndex;

    m_iPosIndex = m_FilePath.ReverseFind('\\'); 

    m_FilePath = m_FilePath.Left(m_iPosIndex) + "\\Log";

    return m_FilePath;
}

void CLogFile::InitCriticalSection()
{
	InitializeCriticalSection(&cs);
}
void CLogFile::DelCriticalSection()
{
	DeleteCriticalSection(&cs);
}
BOOL CLogFile::WriteLog(CString LogText, bool bsmall)
{
	try
    {
       // CFile m_File;
        CStdioFile m_SFile;
        CFileFind m_FileFind;
        CString m_sErrorMessage;
        CString m_sFileName = GetFileName();
        CString m_sFilePath = GetFilePath();
        CString m_sCurrentTime;
		if(!bsmall){
			m_sCurrentTime= CTime::GetCurrentTime().Format("%Y-%m-%d %X");
		}else
		{
			CTime Tm=CTime::GetCurrentTime(); 
			int nYear,nMonth,nDay,nHour,nMinute,nSecond,nMinSecond; 
			nYear=Tm.GetYear(); 
			nMonth=Tm.GetMonth(); 
			nDay=Tm.GetDay(); 
			nHour=Tm.GetHour(); 
			nMinute=Tm.GetMinute(); 
			nSecond=Tm.GetSecond(); 
			CString mSecond; 
			struct _timeb timebuffer;         
			_ftime(&timebuffer); 
			mSecond.Format( "%d ",timebuffer.millitm); 
			nMinSecond=atoi(mSecond); 
			m_sCurrentTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%3d ",nYear,nMonth,nDay,nHour, 
					nMinute,nSecond,nMinSecond);
		}
        
        m_sErrorMessage = "*******************" + m_sCurrentTime + "*******************" + "\r";
        m_sErrorMessage += LogText + "\r";
        m_sErrorMessage += "*******************" + m_sCurrentTime + "*******************" + "\n";

        if(!m_FileFind.FindFile(m_sFilePath))
        {
            CreateDirectory(m_sFilePath,NULL);
        }
        
		EnterCriticalSection(&cs);
        if(!m_SFile.Open(m_sFilePath + "\\" +m_sFileName,CFile::modeReadWrite))
        {
            m_SFile.Open(m_sFilePath + "\\" + m_sFileName,CFile::modeCreate | CFile::modeReadWrite | CFile::typeText);
        }
        
        m_SFile.SeekToEnd(); 

        //char* m_szMessage;
        //m_szMessage=(LPTSTR)(LPCTSTR)m_sErrorMessage;

        //m_SFile.Write(m_szMessage,lstrlen(m_szMessage));   
		m_SFile.WriteString(m_sErrorMessage);

        m_SFile.Close();
		LeaveCriticalSection(&cs);
    }
    catch(CFileException fileException)
    {
        return false;
    }

    return true;
}