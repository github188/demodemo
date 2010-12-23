//============================================================================
// Name        : GokuClient.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <stdlib.h>

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
}

static int write_log(const char *src){
	ofstream out_log("client_log.txt", ios::app);
	out_log << src << endl;
	out_log.close();
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
	int port;
	SimpleSocket(wstring &pserver, wstring &sserver){
		primary = 0;
		primary_server = pserver;
		secondary_server = sserver;
	};
	~SimpleSocket(){
	};

	virtual int connect_server(){};
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

#include "LinuxSocket.cpp"
#define CSimpleSocket LinuxSimpleSocket
/*
class WinSimpleSocket: public SimpleSocket{
};
*/

class VideoPlayControl{
public:
	DataCallBack callback;
	SimpleSocket *socket;
	int sessionId;
	int status;  //0 not start, -1 end, 1 running
	VideoPlayControl(SimpleSocket *sc, DataCallBack handle, int sid){
		socket = sc;
		callback = handle;
		sessionId = sid;
		status = 0;
	}

	void seek(wstring &uuid){

	}

	void close(wstring &uuid){

	}
};


void *video_read_thread(void *param){
	VideoPlayControl *control = (VideoPlayControl *)param;
	char buffer[1024 * 4];
	int ret = 0;

	control->status = 1;

	write_log("start video read thread");
	while(1){
		ret = control->socket->read_buffer(buffer, sizeof(buffer));
		if(ret > 0){
			control->callback(control->sessionId, buffer, ret);
		}else {
			control->status = -1;
			break;
		}
	}
	write_log("end video read thread.");

	control->status = -1;
}



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
		socket->write_wstring(buffer);
		socket->readline(buffer);
		wstring code;
		int pos = split_next(buffer, code, ':', 0);
		wstringstream ss(code);
		ss >> pos;
		return pos;
	}
	/**
	 *
	 */
	int list_basestation(){
		for(int i = 0; i < station_list.size(); i++){
			delete station_list[i];
		}
		station_list.clear();

		buffer = L"cmd>list_bs";
		int ret = execute_command(buffer);

		int pos = split_next(cmd_msg, buffer, '$', 0);
		split_next(cmd_msg, buffer, '$', pos);

		if(ret == 0){
			BaseStation *bs;
			for(int i = str2int(buffer); i > 0; i++){
				socket->readline(buffer);
				if(buffer.length() == 0)break;
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
	VideoPlayControl* real_play(wstring &uuid, DataCallBack callback, int session=0);
	VideoPlayControl* replay(wstring &uuid, DataCallBack callback, int session=0){
		wstring master_server = socket->ipaddr + L":";
		int2str(master_server, socket->port);
		SimpleSocket *masterSocket = new CSimpleSocket(master_server, master_server);

		VideoPlayControl *control = (VideoPlayControl *)NULL;
		if (masterSocket->connect_server() > 0){
			control = new VideoPlayControl(masterSocket, callback, session);
			buffer = L"video>replay?uuid=" + uuid + L"\n";
			masterSocket->write_wstring(buffer);
			start_new_thread(video_read_thread, control);
		}

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


