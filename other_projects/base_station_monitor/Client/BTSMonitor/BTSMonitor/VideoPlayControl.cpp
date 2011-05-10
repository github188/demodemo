#include "stdafx.h"
#include "VideoPlayControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void VideoPlayControl::real_play(CString &uuid, CString &channel)
{
	CString cmd="video>real?uuid=";
	cmd.Append(uuid);
	cmd.Append("&ch=");
	cmd.Append(channel);
	cmd.Append("\n");
	vedio_command_list.push_back(cmd);
}

void VideoPlayControl::replay(CString &uuid)
{
	CString cmd="video>replay?uuid=";
	cmd.Append(uuid);
	cmd.Append("\n");
	vedio_command_list.push_back(cmd);
}

bool VideoPlayControl::SaveWarnVideo()
{
	CFile fVideo;
	CFileException error;
	if (fVideo.Open(m_sFileName,CFile::modeCreate|CFile::modeWrite, &error))
	{
		char   szCause[255];
		CString strFormatted;
		error.GetErrorMessage(szCause, 255);
		// (in real life, it's probably more
		// appropriate to read this from
		//  a string resource so it would be easy to
		// localize)
		strFormatted = _T("The data file could not be opened because of this error: ");
		strFormatted += szCause;
		AfxMessageBox(strFormatted);
		return false;
	}


	int bufferLen = 4096 + 100;
	char* buffer = new char[bufferLen];
	int ret = 0;
	memset(buffer, 0, bufferLen);
	CString ack("video>ack\n");
	CLogFile::WriteLog("start warning video save thread");

	CString slog;
	status = 1;
	//while(1){
	while(status == 1){
		bIsBlocking = false;
		for(unsigned int i = 0; i < vedio_command_list.size(); i++){
				socket->write_wstring(vedio_command_list[i]);
		}

		vedio_command_list.clear();

		ret = socket->read_buffer(buffer, bufferLen);
		if(ret > 0){
			fVideo.Write(buffer,ret);
			//callback(control->sessionId, buffer, ret);
			socket->write_wstring(ack);			
		}else {
			
			//Need to reconnect the socket.
			//control->socket->connect_server();

			status = -1;

			CString sErrorInfor;
			sErrorInfor.Format("Abnormally exit warn save vedio thread , SessionID = %d", sessionId);
			CLogFile::WriteLog(sErrorInfor);
			
			break;
		}
	}
	
	CString strLog;
	strLog.Format("Normally exit vedio thread, SessionID = %d", sessionId);
	CLogFile::WriteLog(strLog);

	delete[] buffer;

	status = -1;
	close();
}
//static UINT nRead = 0;

UINT video_read_thread(LPVOID param)
{

	VideoPlayControl *control = (VideoPlayControl *)param;
	
	if (!control) return 0x11;
	CFile fVideo;
	if (control->m_bSave)
	{
		CFileException error;
		if (fVideo.Open(control->m_sFileName,CFile::modeCreate|CFile::modeWrite, &error))
		{
			char   szCause[255];
			CString strFormatted;
			error.GetErrorMessage(szCause, 255);
			// (in real life, it's probably more
			// appropriate to read this from
			//  a string resource so it would be easy to
			// localize)
			strFormatted = _T("The data file could not be opened because of this error: ");
			strFormatted += szCause;
			AfxMessageBox(strFormatted);
			return 0x11;
		}
	}

	int bufferLen = 4096 + 100;
	char* buffer = new char[bufferLen];
	int ret = 0;
	memset(buffer, 0, bufferLen);

	CString ack("video>ack\n");

	CLogFile::WriteLog("start video read thread");

	CString slog;
	//if (control->socket->connect_server() > 0)
	if (control->socket->SocketAttach())
	{
		control->status = 1;
		//while(1){
		while(control->status == 1){
			control->bIsBlocking = false;
			for(unsigned int i = 0; i < control->vedio_command_list.size(); i++){
					control->socket->write_wstring(control->vedio_command_list[i]);
			}
			//for(unsigned int i = 0; i < control->vedio_command_list.size(); i++){
			//	delete control->vedio_command_list[i]; //crash
			//}
			control->vedio_command_list.clear();

			ret = control->socket->read_buffer(buffer, bufferLen);
			if(ret > 0){

				if (control->m_bSave)
					fVideo.Write(buffer,ret); //Save Video
				else
					control->callback(control->sessionId, buffer, ret); //Play Video

				control->socket->write_wstring(ack);
				
			}else {
				
				//Need to reconnect the socket.
				//control->socket->connect_server();

				control->status = -1;

				CString sErrorInfor;
				sErrorInfor.Format("Abnormally exit vedio thread , SessionID = %d", control->sessionId);
				CLogFile::WriteLog(sErrorInfor);
				
				break;
			}
		}
	}

	CString strLog;
	strLog.Format("Normally exit vedio thread, SessionID = %d", control->sessionId);
	CLogFile::WriteLog(strLog);

	delete[] buffer;

	control->status = -1;
	control->close();

	fVideo.Close();

	return 0;
}
