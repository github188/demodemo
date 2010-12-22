#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class LinuxSimpleSocket: public SimpleSocket{
public:
	LinuxSimpleSocket(string &ps, string &ss): SimpleSocket(ps, ss) {
		socketOk = 0;
	};

	int connect_server(){
		socketOk = 0;
		initServerAddr();
		struct sockaddr_in serv_addr;
		struct hostent *server;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);

	    if (sockfd < 0) error(*new string("ERROR opening socket"));


	    server = gethostbyname(ipaddr.c_str());
	    if (server == NULL) {
	    	errorMsg = "No such host:" + ipaddr;
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
				errorMsg = "ERROR connecting";
				error(errorMsg);
			}else{
				cout << "connect to " << ipaddr << ":" << port << " ok!\n";
				socketOk = 1;
			}
	    }
	}

	int read_buffer(char *buffer, int size){
		cout << "read_buffer..." <<endl;
		int len = -1;
		if(socketOk){
			memset((char *)buffer, 0, size);
			len = read(sockfd, buffer, size);
			cout << "read len:" << len << " size:"<< size <<endl;
			string temp;
			temp.assign(buffer, len);
			cout << "read:" << temp << endl;
		}else {
			errorMsg = "Not connected.";
			error(errorMsg);
		}
		return len;
	};
	void close(){};

private:
	int sockfd;
	string errorMsg;
	int socketOk;
	void error(string &msg){
		cout << "error:" <<msg << endl;
	}

protected:
	virtual int write_data(const char *buff, int len){
		cout << "write_data" <<endl;
		if(socketOk){
			int n = write(sockfd, buff, len);
			if (n < 0)
				errorMsg = "ERROR writing to socket";
				error(errorMsg);
		}else {
			errorMsg = "Not connected.";
			error(errorMsg);
		}
	};
};
