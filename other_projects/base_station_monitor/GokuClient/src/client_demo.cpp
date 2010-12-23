
#include <iostream>
#include "GokuClient.cpp"

using namespace std;

void output(wstring &src){
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	int writeLen = wcstombs(buffer, src.c_str(), src.length());
	cout << buffer;
}

int call_back(int sessionId, char *pBuffer, int len){
	cout << "call_back session " << sessionId << " size:" << len << "\n";
	sleep(3);
}

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!


	GokuClient *client; //("127.0.0.1");

	wstring host(L"127.0.0.1:8000");

	client = new GokuClient(host, host);

	write_log("start client...");
	//string user = ""
	int code = client->login(L"test1", L"pass");
	//write_log("login status:")
	cout << "login status:" << code << "\n";
	int count = client->list_basestation();
	cout << "list station:" << count << "\n";

	if(count > 0){
		for (int i = 0; i < count; i++){
			cout << "uuid:";
			output(client->station_list[i]->uuid);
			cout << "\n" << endl;

			cout << "route:";
			output(client->station_list[i]->route);
			cout << "\n" << endl;
		}
	}

	host = L"10020002";
	client->replay(host, &call_back, 1);

	cout << "waiting replay video....";
	sleep(10);

	delete client;


	/*
	BaseStation bs(*new string("123$1$2"));

	cout << "uuid:" << bs.uuid << "\n";
	cout << "devType:" << bs.devType << "\n";
	cout << "devType2:" << bs.idevType << "\n";
	cout << "route:" << bs.route << "\n";
	cout << "status:" << bs.status << "\n";

	*/
	return 0;
}
