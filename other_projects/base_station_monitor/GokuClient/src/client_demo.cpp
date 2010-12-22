
#include <iostream>
#include "GokuClient.cpp"

using namespace std;


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!


	GokuClient *client; //("127.0.0.1");

	string host("127.0.0.1:8000");

	client = new GokuClient(host, host);

	//string user = ""
	int code = client->login("test1", "pass");
	cout << "login status:" << code << "\n";
	int count = client->list_basestation();
	cout << "list station:" << count << "\n";

	if(count > 0){
		for (int i = 0; i < count; i++){
			cout << "uuid:" << client->station_list[i]->uuid << "\n" << endl;
			cout << "route:" << client->station_list[i]->route << "\n" << endl;
		}
	}

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
