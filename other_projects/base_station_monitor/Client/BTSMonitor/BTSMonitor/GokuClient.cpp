#include "stdafx.h"
#include "util.h"
#include "GokuClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int GokuClient::execute_command(CString &cmd)
{
	CString code;
	cmd.Append("\n");
	socket->write_wstring(cmd);
	socket->readline(cmd_msg);
	util::split_next(cmd_msg, code, ':', 0);
	return util::str2int(code);
}

int GokuClient::login(const TCHAR *username, const TCHAR *password)
{
	CString user(username);
	CString pass(password);
	return login(user, pass);
}

int GokuClient::login(CString &user, CString &password)
{
	buffer.Empty();
	buffer.Append("cmd>login?user=");
	buffer.Append(user);
	buffer.Append("&password=");
	buffer.Append(password);
	int ret = execute_command(buffer);
	
	if (ret == 0)
	{
		m_sUserName = user;
		m_sPassword = password;
	}

	return ret;
}

int GokuClient::logout()
{
	buffer = L"cmd>logout";
	int ret = execute_command(buffer);
	return ret;
}

bool GokuClient::GetUserInfo(CString& sUserName, CString& sPassword, int& nSid)
{
	if (m_sUserName.IsEmpty() && m_sPassword.IsEmpty())
		return false;

	sUserName = m_sUserName;
	sPassword = m_sPassword;
	nSid	  = m_nSid;
	return true;
}

void GokuClient::listbtstree(CString &str)
{
	buffer.Empty();
	buffer.Append("cmd>list_bs_tree");
	buffer.Append("\n");
	socket->write_wstring(buffer);
	//do{
	//	socket->readline(cmd_msg);
	//	cmd_msg.Append("\n");
	//	printf(cmd_msg);
	//}while(cmd_msg!="\n" && cmd_msg!="\r");

	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	util::split_next(cmd_msg, temp, '$', pos+1);
	int line=util::str2int(temp);
	for(int i=0;i<line;i++)
	{
		socket->readline(cmd_msg);
		str.Append(cmd_msg);
		//if(i<line-1)
		//{
		//	str.Append("\n");
		//}
		str.Append("\n");
	}
}

void GokuClient::getAlarmStr(CString &alarmStr)
{
	alarmStr.Empty();
	buffer.Empty();
	buffer.Append("cmd>list_al");
	buffer.Append("\n");
	socket->write_wstring(buffer);

	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	util::split_next(cmd_msg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	for(int i=0;i<linenum;i++)
	{
		socket->readline(cmd_msg);
		alarmStr.Append(cmd_msg);
		alarmStr.Append("\n");
	}
}

void GokuClient::queryAlarmInfo(CString category, CString uuid, CString startDate, CString startTime,
				CString type, CString level, CString limit, CString offset, CString &qalarmStr)
{
	qalarmStr.Empty();
	buffer.Empty();
	buffer.Append("cmd>list_al?");
	buffer.Append("c=");
	buffer.Append(category);
	buffer.Append("&uuid=");
	buffer.Append(uuid);
	buffer.Append("&startTime=");
	buffer.Append(startDate);
	if(startDate!="")
	{
		buffer.Append(" ");
		buffer.Append(startTime);
	}
	buffer.Append("&type=");
	buffer.Append(type);
	buffer.Append("&level=");
	buffer.Append(level);
	buffer.Append("&limit=");
	buffer.Append(limit);
	buffer.Append("&offset=");
	buffer.Append(offset);
	buffer.Append("\n");
	socket->write_wstring(buffer);

	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	util::split_next(cmd_msg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	for(int i=0;i<linenum;i++)
	{
		socket->readline(cmd_msg);
		qalarmStr.Append(cmd_msg);
		qalarmStr.Append("\n");
	}
}

void GokuClient::getRealTimeAlarmStr(CString &alarmStr)
{
	alarmStr.Empty();
	buffer.Empty();
	buffer.Append("cmd>list_al?c=1");
	buffer.Append("\n");
	socket->write_wstring(buffer);

	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	util::split_next(cmd_msg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	for(int i=0;i<linenum;i++)
	{
		socket->readline(cmd_msg);
		alarmStr.Append(cmd_msg);
		alarmStr.Append("\n");
	}
}

bool GokuClient::confirmAlarm(CString uuid)
{
	buffer.Empty();
	buffer.Append("cmd>alarm_action?uuid=");
	buffer.Append(uuid);
	buffer.Append("&status=3");
	buffer.Append("\n");
	socket->write_wstring(buffer);

	socket->readline(cmd_msg);
	CString temp;
	util::split_next(cmd_msg, temp, '$', 0);
	int retval=util::str2int(temp);
	if(retval==0)
		return true;
	else
		return false;
}

VideoPlayControl* GokuClient::real_play(CString &uuid, CString &channel, DataCallBack callback, int session)
{
	VideoPlayControl *control;
	BTSInfo* bsinfo = this->btsmanager.btsmap[util::str2int(uuid)];
	if(bsinfo != NULL){
		//CSimpleSocket *masterSocket = new CSimpleSocket(bsinfo->route, bsinfo->route);
		CSimpleSocketImpl *masterSocket = new CSimpleSocketImpl(bsinfo->route, bsinfo->route);

		control = new VideoPlayControl(masterSocket, callback, session);
		control->real_play(uuid, channel);
		CWinThread *playThread=AfxBeginThread(video_read_thread, control);
	}
	return control;
}

VideoPlayControl* GokuClient::replay(CString &videoId, DataCallBack callback, int session)
{
	CString master_server = socket->ipaddr + ":";
	util::int2str(master_server, socket->port);
	
	//CSimpleSocket *masterSocket = new CSimpleSocket(master_server, master_server);
	CSimpleSocket * masterSocket = new CSimpleSocketImpl(master_server, master_server);

	VideoPlayControl *control = new VideoPlayControl(masterSocket, callback, session);
	control->replay(videoId);
	CWinThread *playThread=AfxBeginThread(video_read_thread, control);

	return control;
}
