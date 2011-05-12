#include <stdafx.h>
#include "Login.h"
#include "util.h"
#include "GokuSocket.h"

UINT loginProc(LPVOID param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	uinfo* info=(uinfo *)param;
	CString ps;
	ps.Format("%s:%s",info->servIP, info->servPort);
	GokuSocket loginSock(ps,ps);

	if(loginSock.connect_server()==0)
		return 1;

	CString sCmd;
	CString sMsg;
	sCmd.Format("cmd>login?user=%s&password=%s\n", info->uname, info->password);
	if ( !loginSock.SendCmdAndRecvMsg(sCmd,sMsg) )
		return -1;

	CString code;
	CString *session=new CString();
	int pos=util::split_next(sMsg, code, ':', 0);
	pos=util::split_next(sMsg, *session, '$', pos+1);
	pos=util::split_next(sMsg, *session, '\n', pos+1);

	int retcode=util::str2int(code);
	if(retcode==0)
	{
		//login OK
		::PostMessage(info->UIhwnd, MSG_LOGINOK, (WPARAM)session, 0);
	}
	else
	{
		//login NOK
		::PostMessage(info->UIhwnd, MSG_LOGINFAIL, 0,0);
		delete info;
		return 1;
	}

	bool bnend=true;
	while(bnend)
	{
		if(::GetMessage(&msg,0,0,0))
		{
			switch(msg.message)
			{
			case MSG_GETBTSLIST:
				CString sCmd;
				CString *str=new CString();
				sCmd.Empty();
				sCmd.Append("cmd>list_bs_tree");
				sCmd.Append("\n");
				CString sMsg;
				if ( !loginSock.SendCmdAndRecvMsg(sCmd,sMsg) )
				{
					::PostMessage(info->UIhwnd, MSG_GETBTSLISTFAIL, (WPARAM)str, 0);
					delete info;
					return 2;
				}

				if (sMsg.IsEmpty())
				{
					::PostMessage(info->UIhwnd, MSG_GETBTSLISTFAIL, (WPARAM)str, 0);
					delete info;
					return 3;
				}

				CString temp;
				int pos=util::split_next(sMsg, temp, '$',   0);
				pos = util::split_next(sMsg, temp, '\n', pos+1);
				int line=util::str2int(temp);
				for(int i=0;i<line;i++)
				{
					pos=util::split_next(sMsg, temp, '\n', pos+1);
					if (temp.IsEmpty())
						break;

					str->Append(temp);
					str->Append("\n");
				}
				::PostMessage(info->UIhwnd, MSG_GETBTSLISTOK, (WPARAM)str, 0);
				bnend=false;
				break;
			}
		}
	}

	delete info;
	return 0;
}


