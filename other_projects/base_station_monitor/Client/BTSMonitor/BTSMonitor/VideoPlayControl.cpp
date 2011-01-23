#include "stdafx.h"
#include "VideoPlayControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void VideoPlayControl::real_play(CString &uuid)
{
	CString cmd="video>real?uuid=";
	cmd.Append(uuid);
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

UINT video_read_thread(LPVOID param)
{
	VideoPlayControl *control = (VideoPlayControl *)param;
	int bufferLen = 4096 + 100;
	char* buffer = new char[bufferLen];
	int ret = 0;
	memset(buffer, 0, bufferLen);

	CString ack("video>ack\n");

	CLogFile::WriteLog("start video read thread");

	if (control->socket->connect_server() > 0)
	{
		control->status = 1;
		while(1){
			for(unsigned int i = 0; i < control->vedio_command_list.size(); i++){
					control->socket->write_wstring(control->vedio_command_list[i]);
			}
			//for(unsigned int i = 0; i < control->vedio_command_list.size(); i++){
			//	delete control->vedio_command_list[i]; //crash
			//}
			control->vedio_command_list.clear();

			ret = control->socket->read_buffer(buffer, bufferLen);
			if(ret > 0){
				control->callback(control->sessionId, buffer, ret);
				control->socket->write_wstring(ack);
			}else {
				control->status = -1;
				break;
			}
		}
	}
	CLogFile::WriteLog("end video read thread.");

	delete[] buffer;

	control->status = -1;
	control->close();

	return 0;
}