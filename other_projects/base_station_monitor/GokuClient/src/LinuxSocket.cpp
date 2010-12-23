#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

class LinuxSimpleSocket: public SimpleSocket{
public:
	LinuxSimpleSocket(wstring &ps, wstring &ss): SimpleSocket(ps, ss) {
		socketOk = 0;
	};

	int connect_server(){
		socketOk = 0;
		initServerAddr();
		struct sockaddr_in serv_addr;
		struct hostent *server;
		char buffer[1024];
		sockfd = socket(AF_INET, SOCK_STREAM, 0);

	    if (sockfd < 0) error(*new string("ERROR opening socket"));

	    memset(buffer, 0, sizeof(buffer));
	    int writeLen = wcstombs(buffer, ipaddr.c_str(), ipaddr.length());

	    server = gethostbyname(buffer);
	    if (server == NULL) {
	    	errorMsg = L"No such host:" + ipaddr;
	    	error(errorMsg);
	    }else {
			memset((char *) &serv_addr, 0, sizeof(serv_addr));

			serv_addr.sin_family = AF_INET;
			bcopy((char *)server->h_addr,
				  (char *)&serv_addr.sin_addr.s_addr,
				  server->h_length);

			serv_addr.sin_port = htons(port);
			//cout << "connecting...!";
			if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
				errorMsg = L"ERROR connecting";
				error(errorMsg);
			}else{
				write_log(L"connect to " + ipaddr + L" ok!");
				socketOk = 1;
			}
	    }
	}

	int read_buffer(char *buffer, int size){
		cout << "read_buffer at linux socket..." <<endl;
		int len = -1;
		if(socketOk){
			memset((char *)buffer, 0, size);
			len = read(sockfd, buffer, size);
			cout << "read len:" << len << " size:"<< size <<endl;
			string temp;
			temp.assign(buffer, len);
			//cout << "read:" << temp << endl;
		}else {
			errorMsg = L"Not connected.";
			error(errorMsg);
		}
		return len;
	};
	void close(){};

private:
	int sockfd;
	wstring errorMsg;
	int socketOk;
	void error(wstring &msg){
		int writeLen = wcstombs(buffer, msg.c_str(), msg.length());
		buffer[writeLen] = 0;
		cout << "error:" <<buffer << endl;
	}
	void error(string &msg){
		cout << "error:" <<msg << endl;
	}

protected:
	virtual int write_data(const char *buff, int len){
		cout << "write_data in linux socket.. size:" << len <<endl;
		if(socketOk){
			int n = write(sockfd, buff, len);
			if (n < 0){
				errorMsg = L"ERROR writing to socket";
				error(errorMsg);
			}
		}else {
			errorMsg = L"Not connected.";
			error(errorMsg);
		}
	};
};
