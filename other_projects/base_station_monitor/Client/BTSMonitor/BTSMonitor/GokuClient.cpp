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

	/*//----------------------------------------------
	socket->write_wstring(cmd);
	socket->readline(cmd_msg);
	util::split_next(cmd_msg, code, ':', 0);
	*///------------Modify above code----------------
	CString sMsg;
	socket->SendCmdAndRecvMsg(cmd,sMsg);
	if (sMsg.IsEmpty())
		return -1; //?????
	util::split_next(sMsg, code, ':', 0);
	///----by-liang----------------------------------
	
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
	/*//----------------------------------------------
	buffer.Empty();
	buffer.Append("cmd>list_bs_tree");
	buffer.Append("\n");
	socket->write_wstring(buffer);
	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	util::split_next(cmd_msg, temp, '$', pos+1);
	int line=util::str2int(temp);
	for(int i=0;i<line;i++)
	{
		socket->readline(cmd_msg);
		str.Append(cmd_msg);
		str.Append("\n");
	}
	*///------------------------------------------------
	CString sCmd;
	sCmd.Empty();
	sCmd.Append("cmd>list_bs_tree");
	sCmd.Append("\n");
	CString sMsg;
	socket->SendCmdAndRecvMsg(sCmd,sMsg);
	if (sMsg.IsEmpty())
		return;

	CString temp;
	int pos=util::split_next(sMsg, temp, '$',   0);
	pos = util::split_next(sMsg, temp, '\n', pos+1);
	int line=util::str2int(temp);
	//pos++;
	for(int i=0;i<line;i++)
	{
		pos=util::split_next(sMsg, temp, '\n', pos+1);
		if (temp.IsEmpty())
			break;

		str.Append(temp);
		str.Append("\n");
	}

}

void GokuClient::getAlarmStr(CString &alarmStr)
{
	alarmStr.Empty();

	/*//----------------------------------------------
	buffer.Empty();
	buffer.Append("cmd>list_al");
	buffer.Append("\n");
	socket->write_wstring(buffer);
	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	pos=util::split_next(cmd_msg, temp, '$', pos+1);
	pos=util::split_next(cmd_msg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	for(int i=0;i<linenum;i++)
	{
		socket->readline(cmd_msg);
		alarmStr.Append(cmd_msg);
		alarmStr.Append("\n");
	}
	*///------------------------------------------------
	
	CString sCmd;
	sCmd.Append("cmd>list_al");
	sCmd.Append("\n");

	CString sMsg;
	socket->SendCmdAndRecvMsg(sCmd,sMsg);
	if (sMsg.IsEmpty())
		return;
	CString temp;
	int pos=util::split_next(sMsg, temp, '$', 0);
	pos=util::split_next(sMsg, temp, '$', pos+1);
	pos=util::split_next(sMsg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	pos++; //emit \n
	for(int i=0;i<linenum;i++)
	{
		pos=util::split_next(sMsg, temp, '\n', pos+1);
		if (temp.IsEmpty())
			break;

		alarmStr.Append(temp);
		alarmStr.Append("\n");
	}

	//alarmStr = sMsg.Mid(pos+2,sMsg.GetLength()-pos-2-4); //emit \n

	///------------------------------------------------
}

void GokuClient::queryAlarmInfo(CString category, CString uuid, CString startDate, CString startTime,
				CString type, CString level, CString limit, CString offset, CString &qalarmStr)
{
	/*//----------------------------------------------
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
 	pos=util::split_next(cmd_msg, temp, '$', pos+1);
	pos=util::split_next(cmd_msg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	for(int i=0;i<linenum;i++)
	{
		socket->readline(cmd_msg);
		qalarmStr.Append(cmd_msg);
		qalarmStr.Append("\n");
	}
	*///------------------------------------------------
	qalarmStr.Empty();
	CString sCmd;
	sCmd.Append("cmd>list_al?");
	sCmd.Append("c=");
	sCmd.Append(category);
	sCmd.Append("&uuid=");
	sCmd.Append(uuid);
	sCmd.Append("&startTime=");
	sCmd.Append(startDate);
	if(startDate!="")
	{
		sCmd.Append(" ");
		sCmd.Append(startTime);
	}
	sCmd.Append("&type=");
	sCmd.Append(type);
	sCmd.Append("&level=");
	sCmd.Append(level);
	sCmd.Append("&limit=");
	sCmd.Append(limit);
	sCmd.Append("&offset=");
	sCmd.Append(offset);
	sCmd.Append("\n");

	CString sMsg;
	socket->SendCmdAndRecvMsg(sCmd,sMsg);
	if (sMsg.IsEmpty())
		return;

	CString temp;
	int pos=util::split_next(sMsg, temp, '$', 0);
 	pos=util::split_next(sMsg, temp, '$', pos+1);
	pos=util::split_next(sMsg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	pos++; //emit \n
	for(int i=0;i<linenum;i++)
	{

		pos=util::split_next(sMsg, temp, '\n', pos+1);
		if (temp.IsEmpty())
			break;

		qalarmStr.Append(temp);
		qalarmStr.Append("\n");
	}
	/// Modify by liang...----------------------------------

	CLogFile::WriteLog("queryAlarmInfo");
}

void GokuClient::getRealTimeAlarmStr(CString &alarmStr)
{
	alarmStr.Empty();
	/*//----------------------------------------------
	buffer.Empty();
	buffer.Append("cmd>list_al?c=1");
	buffer.Append("\n");
	socket->write_wstring(buffer);

	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
 	pos=util::split_next(cmd_msg, temp, '$', pos+1);
	pos=util::split_next(cmd_msg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	for(int i=0;i<linenum;i++)
	{
		socket->readline(cmd_msg);
		alarmStr.Append(cmd_msg);
		alarmStr.Append("\n");
	}
	*///------------------------------------------------
	CString sCmd;
	sCmd.Empty();
	sCmd.Append("cmd>list_al?c=1");
	sCmd.Append("\n");

	CString sMsg;
	socket->SendCmdAndRecvMsg(sCmd,sMsg);
	if (sMsg.IsEmpty())
		return;

	CString temp;
	int pos=util::split_next(sMsg, temp, '$', 0);
 	pos=util::split_next(sMsg, temp, '$', pos+1);
	pos=util::split_next(sMsg, temp, '$', pos+1);
	int linenum=util::str2int(temp);
	pos++; //emit \n
	for(int i=0;i<linenum;i++)
	{
		pos=util::split_next(sMsg, temp, '\n', pos+1);
		if (temp.IsEmpty())
			break;

		alarmStr.Append(temp);
		alarmStr.Append("\n");
	}
	/// Modify by liang...----------------------------------
	CString logstr="getRealTimeAlarmStr: ";
	logstr.Append(alarmStr);
	CLogFile::WriteLog(logstr);
}

bool GokuClient::confirmAlarm(CString uuid)
{
	/*//----------------------------------------------
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
	*///------------------------------------------------
	CString sCmd;
	sCmd.Append("cmd>alarm_action?uuid=");
	sCmd.Append(uuid);
	sCmd.Append("&status=3");
	sCmd.Append("\n");

	CString sMsg;
	socket->SendCmdAndRecvMsg(sCmd,sMsg);
	if (sMsg.IsEmpty())
		return false;

	CString temp;
	util::split_next(sMsg, temp, '$', 0);
	int retval=util::str2int(temp);
	///------by-liang--------------------------------------
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
	playThread=AfxBeginThread(video_read_thread, control);

	return control;
}

UINT alarm_getRealAlarm_thread(LPVOID param)
{
	//get the socket from parameter
	//use the socket to get the alarm
	//the alarm should be get from the UI.

	return 0;
}