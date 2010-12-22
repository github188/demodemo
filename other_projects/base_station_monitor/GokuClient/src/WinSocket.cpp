
class WinSimpleSocket: public SimpleSocket{

public:
	WinSimpleSocket(string &ps, string &ss): SimpleSocket(ps, ss) {
	};

	int connect_server(){
		//const char *host = ipaddr.c_str();
		//const char *Lhost = ipaddr.c_str();
		CString host("127.0.0.1");
		initServerAddr();
		if(cs.Create() == FALSE){
			MessageBox(NULL,_T("Error create"), _T("Error create"),MB_OK);
			return -1;
		}
		if(cs.Connect(_T("127.0.0.1"), 8000)==FALSE)
		{
			MessageBox(NULL,_T("Error"), _T("Error"),MB_OK);
			return -1;
		}
		return 0;
	}
	int read_buffer(char *buffer, int size){
		return cs.Receive(buffer, size);
	}

protected:
	virtual int write_data(const char *buff, int len){
		char sendcmd_startch[200];
		//wcstombs(sendcmd_startch, buff, len);
		return cs.Send(sendcmd_startch, len);
	}

private:
	CSocket cs; //=new CSocket();

};
