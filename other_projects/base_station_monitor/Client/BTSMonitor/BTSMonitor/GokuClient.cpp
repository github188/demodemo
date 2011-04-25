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
	if ( !socket->SendCmdAndRecvMsg(cmd,sMsg) )
		return -1; 

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
	//buffer.Empty();
	CString sCmd = "cmd>login?user=";
	sCmd+=user;
	sCmd+="&password=";
	sCmd+=password;
	
	/*
	sCmd.Append("cmd>login?user=");
	sCmd.Append(user);
	sCmd.Append("&password=");
	sCmd.Append(password);
	*/

	int ret = execute_command(sCmd);
	
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
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		return;

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
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		return;

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
	sCmd.Append("type=");
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
	sCmd.Append("&status=");
	sCmd.Append(type);
	sCmd.Append("&level=");
	sCmd.Append(level);
	sCmd.Append("&limit=");
	sCmd.Append(limit);
	sCmd.Append("&offset=");
	sCmd.Append(offset);
	sCmd.Append("\n");

	CString sMsg;
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		return;

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
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg))
		return;

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
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		return false;

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
bool GokuClient::getTaskList(CString& sTaskList)
{
	
	CString sCmd; //="cmd>list_task?sid=";
	if (!m_nSid)
		sCmd.Format("cmd>list_task?sid=%d", m_nSid);
	else
		sCmd=="cmd>list_task?sid=";

	sCmd.Append("\n");

	CString sMsg;
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		return false;

	if (sMsg.IsEmpty())
		return false;

	CString temp;
	//util::split_next(sMsg, temp, '$', 0);
	util::split_next(sMsg, temp, ':', 0);
	int retval=util::str2int(temp);
	///------by-liang--------------------------------------
	if(retval==0)
	{
		sTaskList = sMsg;
		return true;
	}

	return false;
}
bool GokuClient::saveTaskInfo(CString sTaskID, 
		CString sName, 
		CString sUUID, 
		CString sCh, 
		CString sWindowID, 
		CString sWeek, 
		CString sStartDate,
		CString sEndDate, 
		CString sStartTime,
		CString sEndTime,
		CString sMinShowTime,
		CString sShowOrder)

{
	//save_task?taskID=1&name=test&uuid=1004&channel=1&windowID=1&weeks=1,2,3,4&startTime=18:11&endTime=20:11&minShowTime=10&showOrder=10
	CString sCmd;
	sCmd.Format("save_task?taskID=%s&name=%s&uuid=%s&channel=%s&windowID=%s&weeks=%s&startDate=%s&endDate=%s&startTime=%s&endTime=%s&minShowTime=%s&showOrder=%s",
		sTaskID,
		sName,
		sUUID,
		sCh,
		sWindowID,
		sWeek,
		sStartDate,
		sEndDate,
		sStartTime,
		sEndTime,
		sMinShowTime,
		sShowOrder);
	sCmd.Append("\n");

	CString sMsg;
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		return false;

	if (sMsg.IsEmpty())
		return false;

	CString temp;
	util::split_next(sMsg, temp, '$', 0);
	int retval=util::str2int(temp);
	///------by-liang--------------------------------------
	if(retval==0)
		return true;
	return false;
}
//VideoPlayControl* GokuClient::real_play(CString &uuid, CString &channel, DataCallBack callback, int session)
bool GokuClient::real_play(CString &uuid, CString &channel, DataCallBack callback, int session)
{
	VideoPlayControl *control;
	BTSInfo* bsinfo = this->btsmanager.btsmap[util::str2int(uuid)];
	if(bsinfo != NULL){
		//CSimpleSocket *masterSocket = new CSimpleSocket(bsinfo->route, bsinfo->route);
		CSimpleSocketImpl *masterSocket = new CSimpleSocketImpl(bsinfo->route, bsinfo->route);

		control = new VideoPlayControl(masterSocket, callback, session);
		
		if (control==NULL)
			return false;
		
		m_pArrVideoCtrl[session] = control;

		if ( control->socket->connect_server() > 0 )
		{
			control->real_play(uuid, channel);
			
			//Socket Detach 
			if ( control->socket->SocketDetach() )
				//CWinThread *playThread=AfxBeginThread(video_read_thread, control);
				m_pPlayThread[session] = AfxBeginThread(video_read_thread, control);
			else
			{
				//Close socket,free memory, return false.
				//CString strError;
				//strError.Format("����ʧ��%d",session);
				//AfxMessageBox(strError);
				delete m_pArrVideoCtrl[session];
				m_pArrVideoCtrl[session] = NULL;
				return false;
			}
		}
		else
			return false;
	}

	//return control;
	return true;
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
UINT Command_SendAndReceiveTimer(LPVOID param)
{
	GokuSocket *socket = (GokuSocket *)param;

	if (!socket)		return 0;

	bool bAutoWaiting = false;

	bool bRuning = true;
	while(bRuning)
	{
		DWORD dwRet = ::WaitForMultipleObjects(2, socket->m_hEvent, false, socket->GetWaitTime());
		switch(dwRet)
		{
		case WAIT_OBJECT_0: //SetAutoWait....that mean's begin to counting...
			{
				bAutoWaiting = true;
			}
			break;
		case WAIT_OBJECT_0+1:
			{
				bAutoWaiting = false;
			}
			break;
		case WAIT_OBJECT_0+2:
			bRuning = false;
			break;
		case WAIT_ABANDONED_0:
			break;
		case WAIT_ABANDONED_0+1:
			break;
		case WAIT_ABANDONED_0+2:
			break;
		case WAIT_TIMEOUT:
			{
				socket->CancelSocket();
			}
			break;
		case WAIT_FAILED:
			{
				DWORD dwError = GetLastError();
				bRuning = false;

			}
			break;
		}
	}//...while...

	return 0x11;
}

bool GokuClient::Stop_Play(int nVideoID)
{
	if (nVideoID<0 || nVideoID>cnMAX_VV-1)
	{
		CString sErrInfo;
		sErrInfo.Format("����Ӵ����ڷ�Χ��:%d", nVideoID);
		AfxMessageBox(sErrInfo);
		return false;
	}

	if (m_pArrVideoCtrl[nVideoID]==NULL) //Unknown , needn't do futher operation...
		return true;

	if ( m_pArrVideoCtrl[nVideoID]->status != 1) //Thread is exit....
	{
		delete m_pArrVideoCtrl[nVideoID];
		m_pArrVideoCtrl[nVideoID]=NULL;
		
		return true;
	}


	m_pArrVideoCtrl[nVideoID]->bIsBlocking = true; //
	m_pArrVideoCtrl[nVideoID]->status = 0; //Thread is runing...

	DWORD dwRet = ::WaitForSingleObject(m_pPlayThread[nVideoID]->m_hThread,2000);
	if (dwRet == WAIT_TIMEOUT)
	{
		if (m_pArrVideoCtrl[nVideoID]->bIsBlocking)
			m_pArrVideoCtrl[nVideoID]->socket->CancelSocket();

		//Continue wait another 2s
		dwRet = ::WaitForSingleObject(m_pPlayThread[nVideoID]->m_hThread,3000);
		if (dwRet == WAIT_TIMEOUT)
		{
			if (m_pPlayThread[nVideoID] != NULL && m_pPlayThread[nVideoID]->m_hThread > 0) 
				::AfxTermThread((HINSTANCE__*)(m_pPlayThread[nVideoID]->m_hThread));
		}
	}
	else
	{
		if (m_pArrVideoCtrl[nVideoID]->status != -1) //thread is still not exit...
		{

			if (m_pPlayThread[nVideoID] != NULL && m_pPlayThread[nVideoID]->m_hThread > 0) 
				::AfxTermThread((HINSTANCE__*)(m_pPlayThread[nVideoID]->m_hThread));
		}
	}

	//::CloseHandle(m_pPlayThread[nVideoID]->m_hThread);
	//m_pPlayThread[nVideoID]->m_hThread = NULL;
	m_pPlayThread[nVideoID] = NULL;

#ifdef _DEBUG
	if (m_pArrVideoCtrl[nVideoID]->status==-1)
		TRACE("\r\nTERMINATE thread window:%d!!!", nVideoID);
#endif

	delete m_pArrVideoCtrl[nVideoID];
	m_pArrVideoCtrl[nVideoID]=NULL;

	 	 
	return true;
}

MonitorImage* GokuClient::getRealImagebyBase64(BTSInfo *binfo, int *errno)
{
	//CString ip;
	//int pos=util::split_next(binfo->route, ip, ':',0);
	//ip.Append(":");
	//ip.Append(IMAGE_PORT);

	GokuSocket *imageSock=new GokuSocket(binfo->route, binfo->route);
	if(imageSock->connect_server()>0)
	{
		MonitorImage *pImage=new MonitorImage();
		pImage->mSock=imageSock;
		CString sCmd;
		sCmd.Append("img>real_image?");
		sCmd.Append("baseStation=");
		sCmd.Append(binfo->uuid);
		sCmd.Append("&");
		sCmd.Append("channel=");
		sCmd.Append(binfo->channelInfo);
		sCmd.Append("\n");
		
		CString sMsg;
		if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg) )
		{
			delete pImage;
			return NULL;
		}
		CString line;
		int ileft=0, iright=0, ipos=0;
		ipos=sMsg.Find('\n', ileft);
		line=sMsg.Mid(ileft, ipos-ileft+1);
		//get the session.
		pImage->getSessionFromLine(line);
		*errno=util::str2int(pImage->errcode);
		if(*errno!=0)
		{
			delete pImage;
			return NULL;
		}
		ileft=ipos;
		ipos=sMsg.Find('\n', ileft+1);
		line=sMsg.Mid(ileft+1, ipos-ileft);
		pImage->getImageText(line);
		line=sMsg.Mid(ipos+1, sMsg.GetLength()-ipos);
		pImage->decodeImageData(line);
		return pImage;
	}
	return NULL;
}

MonitorImage* GokuClient::getAlarmImagebyBase64(CString alarmID, int *errno)
{
	CString ip;
	int pos=util::split_next(host, ip, ':',0);
	ip.Append(":");
	ip.Append(IMAGE_PORT);
	//ip.Append("8002");

	CSimpleSocket *imageSock=new CSimpleSocketImpl(ip, ip);
	if(imageSock->connect_server()>0)
	{
		MonitorImage *pImage=new MonitorImage();
		pImage->mSock=imageSock;
		CString sCmd;
		sCmd.Append("img>alarm_image?");
		sCmd.Append("alarmId=");
		sCmd.Append(alarmID);
		sCmd.Append("\n");
		//sCmd.Append("&");
		//sCmd.Append("encode=raw\n");
		CString sMsg;
		if ( !imageSock->SendCmdAndRecvMsg(sCmd,sMsg) )
		{
			delete pImage;
			return NULL;
		}
		
		CString line;
		int ileft=0, iright=0, ipos=0;
		ipos=sMsg.Find('\n', ileft);
		line=sMsg.Mid(ileft, ipos-ileft+1);
		//get the session.
		pImage->getSessionFromLine(line);
		*errno=util::str2int(pImage->errcode);
		if(*errno!=0)
		{
			delete pImage;
			return NULL;
		}
		ileft=ipos;
		ipos=sMsg.Find('\n', ileft+1);
		line=sMsg.Mid(ileft+1, ipos-ileft);
		pImage->getImageText(line);
		line=sMsg.Mid(ipos+1, sMsg.GetLength()-ipos);
		pImage->decodeImageData(line);
		//CString filename="1.jpg";
		//pImage->savedata(filename);

		//pImage->getNextImage();
		//filename="2.jpg";
		//pImage->savedata(filename);
		return pImage;
	}
	return NULL;
}

int GokuClient::stop_Alarm(CString btsid, CString timeout)
{
	CString sCmd;
	sCmd.Empty();
	sCmd.Append("cmd>stopalarm?baseStation=");
	sCmd.Append(btsid);
	sCmd.Append("&timeout=");
	sCmd.Append(timeout);
	sCmd.Append("\n");

	CString sMsg;
	if ( !socket->SendCmdAndRecvMsg(sCmd,sMsg))
		return -1;

	if (sMsg.IsEmpty())
		return -3;

	CString temp;
	int pos=util::split_next(sMsg, temp, '$', 0);
	int retval=util::str2int(temp);
	
	return retval;
}
