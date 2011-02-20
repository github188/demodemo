#ifndef VIDEO_PLAY_CONTROL_H_
#define VIDEO_PLAY_CONTROL_H_

#include "SimpleSocket.h"
#include "logfile.h"
#include <vector>
using namespace std;

typedef int(*DataCallBack)(int sessionId, char *pBuffer, int len);

UINT video_read_thread(LPVOID param);

class VideoPlayControl{
public:
	DataCallBack callback;
	CSimpleSocket *socket;
	int sessionId;
	int status;  //0 not start, -1 end, 1 running, -2 start error

	//不能在界面线程调用socket，发送和接受方法。其他线程把需要发送的命令保存到list,由socket线程统一发送。
	vector<CString> vedio_command_list;

	VideoPlayControl(CSimpleSocket *sc, DataCallBack handle, int sid){
		socket = sc;
		callback = handle;
		sessionId = sid;
		status = 0;
	}

	~VideoPlayControl(){
		delete socket;
	}

	void real_play(CString &uuid, CString &channel);

	void replay(CString &uuid);

	void seek(int pos){
	}

	void close(){
		socket->close();
	}
};


/*
static UINT video_read_thread(LPVOID param){
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
*/
#endif