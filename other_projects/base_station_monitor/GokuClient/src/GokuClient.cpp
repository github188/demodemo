//============================================================================
// Name        : GokuClient.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <sys/socket.h>
using namespace std;

//int split_next(string &src, string &des, char ch, int start);

static int split_next(const string &src, string &des, char ch, int start){
	int pos = 0;
	pos = src.find('$', start);
	des.clear();
	if(pos < start){
		pos = src.length();
	}
	if(pos > start){
		des.assign(src, start, pos - start);
	}
	return pos;
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
	string primary_server;
	string secondary_server;
public:
	SimpleSocket(string &primary_server, string &secondary_server){
	};
	~SimpleSocket(){
	}
	virtual int write(string data);
	virtual string readline();
	virtual int read_buffer(char *buffer, int size);
};

class LinuxSimpleSocket: public SimpleSocket{
	int sockfd;
	int init_socket(){
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    //if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
	    //    error("ERROR connecting");
	}

	void error(string msg);

public:
	int write(string data);
	virtual string readline();
	virtual int read_buffer(char *buffer, int size);
};

/*
class WinSimpleSocket: public SimpleSocket{


};
*/

class GokuClient{
	SimpleSocket *socket;
public:
	GokuClient(string &primary_server, string &secondary_server){
		//socket = new LinuxSimpleSocket(primary_server, secondary_server);
	}

	~GokuClient(){
	}

	list<BaseStation> station_list;
	list<BaseStation> alarm_list;
	/**
	 * login to master server;
	 */
	int login(string user, string password);
	int logout();
	/**
	 *
	 */
	int list_basestation();
	/**
	 *
	 */
	int list_alarm();
	int real_play(string &uuid, DataCallBack callback, int session=0);
	int replay(string &uuid, DataCallBack callback, int session=0);
};


