#include "stdafx.h"
//============================================================================
// Name        : GokuClient.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

static int write_log(const wstring &src){
	ofstream out_log("client_log.txt", ios::app);
	char buffer[1024];
	//memset(buffer, 0, sizeof(buffer));
	int writeLen = wcstombs(buffer, src.c_str(), src.length());
	buffer[writeLen] = 0;

	//write_log(buffer);
	cout << buffer << endl;
	out_log << buffer << endl;
	out_log.close();

	return 0;
}

static int write_log(const char *src){
	ofstream out_log("client_log.txt", ios::app);
	out_log << src << endl;
	out_log.close();

	return 0;
}

static int split_next(const wstring &src, wstring &des, char ch, int start){
	int pos = 0;
	pos = src.find(ch, start);
	des.clear();
	if(pos < start){
		pos = src.length();
	}
	if(pos > start){
		des.assign(src, start, pos - start);
	}
	return pos;
}

static int str2int(const wstring src){
	wstringstream ss(src);
	int ret;
	ss >> ret;
	return ret;
}

static void int2str(wstring &buffer, int i){
	wstringstream out;
	out << i;
	buffer.append(out.str());
}

class BaseStation{
private:
	void _build(wstring &info){
		int pos = 0;
		pos = split_next(info, uuid, '$', 0);
		pos = split_next(info, devType, '$', pos + 1);
		pos = split_next(info, route, '$', pos + 1);
		pos = split_next(info, status, '$', pos + 1);

		if(devType.length() > 0){
			wstringstream ss(devType);
			ss >> idevType;
		}
	}

public:
	wstring uuid;
	wstring devType;
	wstring route;
	wstring status;
	int idevType;
	BaseStation(wstring &info){
		_build(info);
	}

};

class AlarmRecord{
private:
	void _build(wstring &info){
		int pos = 0;
		pos = split_next(info, uuid, '$', 0);
	}

public:
	wstring uuid;
	wstring baseStation;
	wstring alarmType;
	wstring alarmStatus;
	wstring user;
	wstring startTime;
	wstring endTime;
	wstring level;
	AlarmRecord(wstring &info){
		_build(info);
	}
};

typedef int(*DataCallBack)(int sessionId, char *pBuffer, int len);

class SimpleSocket{
public:
	wstring ipaddr;
	unsigned int port;
	SimpleSocket(wstring &pserver, wstring &sserver){
		primary = 0;
		primary_server = pserver;
		secondary_server = sserver;
	};
	~SimpleSocket(){
	};

	virtual int connect_server(){return 0;};
	virtual void close(){};
	virtual int read_buffer(char *buffer, int size){
		write_log("read_buffer in SimpleSocket");
		return -1;
	};

	int write_wstring(wstring &data){
		//data.append("\n");
		//cout << "write:" << data << "len:" << data.length() << endl;
		//memset((char *) &serv_addr, 0, sizeof(serv_addr));
		write_log(L"write_wstring:" + data);
		int writeLen = wcstombs(buffer, data.c_str(), data.length());
		int len = write_data(buffer, writeLen);
	    bufferLimit = bufferPos = 0;
	    return len;
	};

	int readline(wstring &des, long timeout=3){
		des.clear();
		char ch = 0;
		if(bufferPos >= bufferLimit){
			bufferLimit = read_buffer(buffer, sizeof(buffer));
			bufferPos = 0;
		}
		for(;bufferPos < bufferLimit && timeout > 0;){
			ch = buffer[bufferPos++];
			if(ch == '\n'){
				break;
			}else{
				des.append(1, ch);
			}
			if(bufferPos >= bufferLimit){
				//sleep(1);
				bufferLimit = read_buffer(buffer, sizeof(buffer));
				bufferPos = 0;
				timeout--;
			}
		}
		write_log(L"readline:" + des);
		return des.length();
	};

protected:
	wstring primary_server;
	wstring secondary_server;
	int primary;

	char buffer[4 * 1024];
	int bufferPos;
	int bufferLimit;

	virtual int write_data(const char *buffer, int size){
		write_log(L"write_data in SimpleSocket...");
		return -1;
	};

	void initServerAddr(){
		wstring cur;
		if(primary == 0){
			cur = primary_server;
			primary = 1;
		}else {
			cur = secondary_server;
			primary = 0;
		}
		int pos = split_next(cur, ipaddr, ':', 0);

		wstring tempPort;

		split_next(cur, tempPort, ':', pos + 1);
		wstringstream ss(tempPort);
		ss >> port;
	}
};

//#include "LinuxSocket.cpp"
//#define CSimpleSocket LinuxSimpleSocket

class WinSimpleSocket: public SimpleSocket{

public:
	WinSimpleSocket(wstring &ps, wstring &ss): SimpleSocket(ps, ss) {
	};

	int connect_server(){
		//const char *host = ipaddr.c_str();
		//const char *Lhost = ipaddr.c_str();
		//host.l
		initServerAddr();
		//CString host(ipaddr.c_str());
		if(cs.Create() == FALSE){
			MessageBox(NULL,_T("Error create"), _T("Error create"),MB_OK);
			return -1;
		}
		CString host(ipaddr.c_str());
		//MessageBox(NULL,_T("Error 2"), host, MB_OK);
		if(cs.Connect(host, port)==FALSE)
		{
			MessageBox(NULL,_T("Error 3"), _T("Error connect"),MB_OK);
			return -1;
		}
		return 1;
	}
	int read_buffer(char *buffer, int size){
		wstring xx;
		int len = cs.Receive(buffer, size);
		//buffer[len] = 0;
		int2str(xx, len);
		write_log(L"read buffer:" + xx);
		if(len > 0 && len < 1024){
			buffer[len] = 0;
			write_log((const char *)buffer);
		}

		return len;
	}

protected:
	virtual int write_data(const char *buff, int len){
		//char sendcmd_startch[200];
		//wcstombs(sendcmd_startch, buff, len);
		return cs.Send(buff, len);
	}

private:
	CSocket cs; //=new CSocket();

};

static int start_new_thread(AFX_THREADPROC fun, LPVOID param){
	CWinThread *mythread;
	//Need delete the thread object when the thead is stopped?
	mythread = AfxBeginThread(fun, param);
	return 1;
};


/*
class WinSimpleSocket: public SimpleSocket{
};
*/

class VideoPlayControl{
public:
	DataCallBack callback;
	SimpleSocket *socket;
	
	/*
	不能在界面线程调用socket，发送和接受方法。其他线成把需要发送的命令保存到list,由socket现成统一发送。
	*/
	vector<wstring*> vedio_command_list;
	int sessionId;
	int status;  //0 not start, -1 end, 1 running, -2 start error
	VideoPlayControl(SimpleSocket *sc, DataCallBack handle, int sid){
		socket = sc;
		callback = handle;
		sessionId = sid;
		status = 0;
	}

	~VideoPlayControl(){
		delete socket;
	}

	void real_play(wstring &uuid){
		 vedio_command_list.push_back(new wstring(L"video>real?uuid=" + uuid + L"\n"));
	}

	void replay(wstring &uuid){
		vedio_command_list.push_back(new wstring(L"video>replay?uuid=" + uuid + L"\n"));
	}

	void seek(int pos){
		wstring *cmd = new wstring(L"video>seek?pos=");
		int2str(*cmd, pos);
		vedio_command_list.push_back(cmd);
		//cmd.append(L"\n");
		//socket->write_wstring(cmd);
	}

	void close(){
		socket->close();
	}
};


static UINT video_read_thread(LPVOID param){
	VideoPlayControl *control = (VideoPlayControl *)param;
	int bufferLen = 4096 + 100;
	char* buffer = new char[bufferLen];
	int ret = 0;
	for(int i=0; i < bufferLen; i++){
		buffer[i] = 0;
	}

	wstring ack(L"video>ack\n");

	write_log("start video read thread");
	if (control->socket->connect_server() > 0){
		control->status = 1;
		while(1){
			for(unsigned int i = 0; i < control->vedio_command_list.size(); i++){
				control->socket->write_wstring(*control->vedio_command_list[i]);
			}
			for(unsigned int i = 0; i < control->vedio_command_list.size(); i++){
				delete control->vedio_command_list[i];
			}
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
	write_log("end video read thread.");
	delete buffer;

	control->status = -1;
	control->close();

	return 0;
};

#define CSimpleSocket WinSimpleSocket

class GokuClient{
	SimpleSocket *socket;
	wstring buffer;
	wstring cmd_msg; //last command status.
public:
	GokuClient(wstring &primary_server, wstring &secondary_server){
		socket = new CSimpleSocket(primary_server, secondary_server);
		socket->connect_server();
	}

	~GokuClient(){
	}

	vector<BaseStation*> station_list;
	//list<BaseStation> alarm_list;
	/**
	 * login to master server;
	 */
	int login(const wchar_t *username, const wchar_t *password){
		wstring user(username);
		wstring pass(password);
		return login(user, pass);
	}

	int login(wstring &user, wstring &password){
		buffer = L"cmd>login?user=" + user + L"&password=" + password;
		int ret = execute_command(buffer);
		return ret;
	}

	int logout(){
		buffer = L"cmd>logout";
		int ret = execute_command(buffer);
		return ret;
	}
	/**
	 *
	 */
	int list_basestation(){
		for(unsigned int i = 0; i < station_list.size(); i++){
			delete station_list[i];
		}
		station_list.clear();

		buffer = L"cmd>list_bs";
		int ret = execute_command(buffer);

		int pos = split_next(cmd_msg, buffer, '$', 0);
		split_next(cmd_msg, buffer, '$', pos + 1);
		
		write_log(L"station count:" + buffer);
		if(ret == 0){
			BaseStation *bs;
			for(int i = str2int(buffer); i > 0; i--){
				socket->readline(buffer);
				if(buffer.length() <= 0)break;
				bs = new BaseStation(buffer);
				station_list.push_back(bs);
			}
			ret = station_list.size();
		}else if(ret > 0){
			ret = ret * -1;
		}
		return ret;
	}
	/**
	 *
	 */
	int list_alarm();

	VideoPlayControl* real_play(wstring &uuid, DataCallBack callback, int session=0){
		BaseStation* bs = NULL;
		VideoPlayControl *control = (VideoPlayControl *)NULL;
		for (unsigned int i = 0; i < station_list.size(); i++){
			if(station_list[i]->uuid.compare(uuid) == 0){
				bs = station_list[i];
				break;
			}
		}
		if(bs != NULL){
			SimpleSocket *masterSocket = new CSimpleSocket(bs->route, bs->route);
			control = new VideoPlayControl(masterSocket, callback, session);
			control->real_play(uuid);
		}
		return control;
	}

	VideoPlayControl* replay(wstring &videoId, DataCallBack callback, int session=0){
		wstring master_server = socket->ipaddr + L":";
		int2str(master_server, socket->port);
		SimpleSocket *masterSocket = new CSimpleSocket(master_server, master_server);

		VideoPlayControl *control = new VideoPlayControl(masterSocket, callback, session);
		control->replay(videoId);
		start_new_thread(video_read_thread, control);

		return control;
	}

protected:
	int execute_command(wstring &cmd){
		//write_log(L"execute_command:" + cmd);
		wstring code;
		cmd.append(L"\n");
		socket->write_wstring(cmd);
		socket->readline(cmd_msg);
		split_next(cmd_msg, code, ':', 0);
		return str2int(code);
	}
};


