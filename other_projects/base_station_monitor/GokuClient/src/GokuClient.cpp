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

//#include <stdio.h>
using namespace std;

//int split_next(string &src, string &des, char ch, int start);

static int split_next(const string &src, string &des, char ch, int start){
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

static int str2int(const string src){
	stringstream ss(src);
	int ret;
	ss >> ret;
	return ret;
}

class BaseStation{
private:
	void _build(string &info){
		int pos = 0;
		pos = split_next(info, uuid, '$', 0);
		pos = split_next(info, devType, '$', pos + 1);
		pos = split_next(info, route, '$', pos + 1);
		pos = split_next(info, status, '$', pos + 1);

		if(devType.length() > 0){
			stringstream ss(devType);
			ss >> idevType;
		}
	}

public:
	string uuid;
	string devType;
	string route;
	string status;
	int idevType;
	BaseStation(string &info){
		_build(info);
	}

};

class AlarmRecord{
private:
	void _build(string &info){
		int pos = 0;
		pos = split_next(info, uuid, '$', 0);
	}

public:
	string uuid;
	string baseStation;
	string alarmType;
	string alarmStatus;
	string user;
	string startTime;
	string endTime;
	string level;
	AlarmRecord(string &info){
		_build(info);
	}
};

typedef int(*DataCallBack)(int sessionId, char *pBuffer, int len);

class SimpleSocket{
public:
	string ipaddr;
	int port;
	SimpleSocket(string &pserver, string &sserver){
		primary = 0;
		primary_server = pserver;
		secondary_server = sserver;
	};
	~SimpleSocket(){
	};

	virtual int connect_server(){};
	virtual void close(){};
	virtual int read_buffer(char *buffer, int size){
		cout << "read_buffer in SimpleSocket";
		return 0;
	};

	int write_string(string &data){
		//data.append("\n");
		//cout << "write:" << data << "len:" << data.length() << endl;
		int len = write_data(data.c_str(), data.length());
	    bufferLimit = bufferPos = 0;
	    return len;
	};

	int readline(string &des, long timeout=3){
		des.clear();
		//cout << "readline buffer len:" << sizeof(buffer) <<endl;
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
				sleep(1);
				bufferLimit = read_buffer(buffer, sizeof(buffer));
				bufferPos = 0;
				timeout--;
			}
		}
		cout << "readline:" << des << endl;
		return des.length();
	};

protected:
	string primary_server;
	string secondary_server;
	int primary;

	char buffer[4 * 1024];
	int bufferPos;
	int bufferLimit;

	virtual int write_data(const char *buffer, int size){
		cout << "write_data..." <<endl;
	};

	void initServerAddr(){
		string cur;
		if(primary == 0){
			cur = primary_server;
			primary = 1;
		}else {
			cur = secondary_server;
			primary = 0;
		}
		int pos = split_next(cur, ipaddr, ':', 0);

		string tempPort;

		split_next(cur, tempPort, ':', pos + 1);
		stringstream ss(tempPort);
		ss >> port;
	}
};


/*
class WinSimpleSocket: public SimpleSocket{


};
*/

class GokuClient{
	LinuxSimpleSocket *socket;
	string buffer;
	string cmd_msg; //last command status.
public:
	GokuClient(string &primary_server, string &secondary_server){
		socket = new LinuxSimpleSocket(primary_server, secondary_server);
		socket->connect_server();
	}

	~GokuClient(){
	}

	vector<BaseStation*> station_list;
	//list<BaseStation> alarm_list;
	/**
	 * login to master server;
	 */
	int login(const char *username, const char *password){
		string user(username);
		string pass(password);
		return login(user, pass);
	}

	int login(string &user, string &password){
		buffer = "cmd>login?user=" + user + "&password=" + password;
		int ret = execute_command(buffer);
		return ret;
	}

	int logout(){
		buffer = "cmd>logout";
		socket->write_string(buffer);
		socket->readline(buffer);
		string code;
		int pos = split_next(buffer, code, ':', 0);
		stringstream ss(code);
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

		buffer = "cmd>list_bs";
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
	int real_play(string &uuid, DataCallBack callback, int session=0);
	int replay(string &uuid, DataCallBack callback, int session=0);

protected:
	int execute_command(string &cmd){
		cout << "execute_command:" << cmd << "\n" <<endl;
		string code;
		cmd.append("\n");
		socket->write_string(cmd);
		socket->readline(cmd_msg);
		split_next(cmd_msg, code, ':', 0);
		return str2int(code);
	}
};


