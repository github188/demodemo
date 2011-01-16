#include "stdafx.h"
#include "util.h"
#include "GokuClient.h"

int GokuClient::execute_command(CString &cmd)
{
	CString code;
	cmd.Append("\n");
	socket->write_wstring(cmd);
	socket->readline(cmd_msg);
	util::split_next(cmd_msg, code, ':', 0);
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
	buffer.Empty();
	buffer.Append("cmd>login?user=");
	buffer.Append(user);
	buffer.Append("&password=");
	buffer.Append(password);
	int ret = execute_command(buffer);
	return ret;
}

int GokuClient::logout()
{
	buffer = L"cmd>logout";
	int ret = execute_command(buffer);
	return ret;
}

void GokuClient::listbtstree()
{
	buffer.Empty();
	buffer.Append("cmd>list_bs_tree");
	buffer.Append("\n");
	socket->write_wstring(buffer);
	//do{
	//	socket->readline(cmd_msg);
	//	cmd_msg.Append("\n");
	//	printf(cmd_msg);
	//}while(cmd_msg!="\n" && cmd_msg!="\r");

	socket->readline(cmd_msg);
	CString temp;
	int pos=util::split_next(cmd_msg, temp, '$', 0);
	util::split_next(cmd_msg, temp, '$', pos+1);
	int line=util::str2int(temp);
	for(int i=0;i<line;i++)
	{
		socket->readline(cmd_msg);
		printf("%s\n",cmd_msg);
	}
}