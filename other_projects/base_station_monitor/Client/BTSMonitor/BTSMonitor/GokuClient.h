#pragma once

#include "GokuSocket.h"

class GokuClient{
	GokuSocket *socket;
	CString buffer;
	CString cmd_msg; //last command status.
public:
	GokuClient(CString &primary_server, CString &secondary_server)
	{
		socket = new GokuSocket(primary_server, secondary_server);
		socket->connect_server();
	}

	~GokuClient(){}

	/**
	 * login to master server;
	 */
	int login(const TCHAR *username, const TCHAR *password);
	int login(CString &user, CString &password);

	int logout();

	void listbtstree(CString &str);

protected:
	int execute_command(CString &cmd);
};
