#ifndef __LOGFILE_H_
#define __LOGFILE_H_

#include   <sys/timeb.h>

//The logfile may have problem while using in Mut-thread project.
//add CRITICAL_SECTION, need to call InitCriticalSection() first.

class CLogFile  
{
public:
    CLogFile();
    virtual ~CLogFile();
    static CString GetFileName();
    static CString GetFilePath();
    static BOOL WriteLog(CString LogText, bool bsmall=false);
	static CRITICAL_SECTION cs;
	static void InitCriticalSection();
};

#endif