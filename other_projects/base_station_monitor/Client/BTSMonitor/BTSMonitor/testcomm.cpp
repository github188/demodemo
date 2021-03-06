// testcomm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "testcomm.h"
#include "util.h"
#include "logfile.h"
#include "BTSInfo.h"
#include "SimpleSocket.h"
#include "GokuClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//http://www.cppblog.com/sandy/archive/2006/06/21/8779.html
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		//CSocket cs;
		AfxSocketInit();

		//test int number
		//CString str="55539";
		//int ival=util::str2int(str);
		//_tprintf("%d\n",ival);			//char...
		//ival+=2;
		//util::int2str(str,ival);
		//_tprintf("%s\n", str);			//char...

		//test chinese
		//str="中文";
		//_tprintf("%s\n", str);			//system locale, code page

		//WCHAR *pwc=new WCHAR[100];
		//util::str2widechar(str, &pwc);
		//wprintf(L"%s\n", pwc);
		
		//pwc=L"测试";
		//wprintf(L"%s\n", pwc);
		//wsprintf(L"%s\n",pwc);
		//DWORD ws;
		//WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE),pwc,wcslen(pwc),&ws,NULL);
		//printf("\n");

		/*WCHAR wsrc[]=L"阿他一";
		CString des;
		util::widechar2str(wsrc, des);
		printf("%s\n",des);

		CLogFile::InitCriticalSection();
		CLogFile::WriteLog("test");
		CLogFile::WriteLog("test2", true);

		BTSInfo binfo("1001$1$44$ooo");
		binfo.printitself();*/

		CLogFile::InitCriticalSection();

		//CSimpleSocket css("127.0.0.1","127.0.0.1");
		//CString host="127.0.0.1:8000";
		//GokuClient *pgkclient=new GokuClient(host, host);
		//CString username="test1";
		//CString password="pass";
		//int ret=pgkclient->login(username, password);
		//printf("%d.........\n",ret);
		//pgkclient->listbtstree();
		//printf("listbtstree\n");

		//CString info="001$3$0$0$0$杭州";
		CString info="002$3$0$0$001$滨江";
		info="003$3$0$0$001$萧山";
		info="10010$1$$01$003$诺西大楼$1:通道1,2:通道2";
		info="10012$1$$01$003$信诚路口$1:通道1,2:通道2";
		BTSInfo bsinfo(info);
		bsinfo.printitself();
	}

	return nRetCode;
}
