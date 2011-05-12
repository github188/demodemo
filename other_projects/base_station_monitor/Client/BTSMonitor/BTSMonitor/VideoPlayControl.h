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
	int status;  //0 not start, -1 end, 1 running, -2 start error, 2 jump to the play position.
	bool bIsBlocking; //just for judge whether the socket blocked or not
	bool m_bSave;
	CString m_sFileName;

	//�����ڽ����̵߳���socket�����ͺͽ��ܷ����������̰߳���Ҫ���͵�����浽list,��socket�߳�ͳһ���͡�
	vector<CString> vedio_command_list;

	VideoPlayControl(CSimpleSocket *sc, DataCallBack handle, int sid, CString sFile="", bool bSave=false){
		socket = sc;
		callback = handle;
		sessionId = sid;
		status = 0;
		bIsBlocking = true;

		m_sFileName = sFile;

		m_bSave = bSave;
	}

	~VideoPlayControl(){
		delete socket;
	}

	void real_play(CString &uuid, CString &channel);
	void replay(CString &uuid);
	bool VideoPlayControl::SaveWarnVideo();

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