
#include <iostream>
#include "GokuClient.cpp"

using namespace std;


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	/*
	GokuClient *client; //("127.0.0.1");

	client = new GokuClient("127.0.0.1:8081");
	int code = clinet->login("test1", "pass");
	cout << "login status:" << code << "\n";
	int count = client->list_basestation();
	cout << "list station:" << count << "\n";
	delete client;
	*/

	BaseStation bs(*new string("123$1$2"));

	cout << "uuid:" << bs.uuid << "\n";
	cout << "devType:" << bs.devType << "\n";
	cout << "devType2:" << bs.idevType << "\n";
	cout << "route:" << bs.route << "\n";
	cout << "status:" << bs.status << "\n";

	return 0;
}
