#include "stdafx.h"
#include "ImageSocket.h"

int ImageSocket::read_buffer(char *buffer, int size)
{
	StartAutoWait(5); //5 second

	//char szBuffer[BUFSIZE];   
	//ZeroMemory(szBuffer,sizeof(szBuffer)); 
	
	CString xx;
	CWaitCursor wait;	


	//memset(buffer,0,size);
	//int len = cs.Receive(buffer, size);
	//int len = cs.Receive(szBuffer,BUFSIZE);

	///Receive all data ---------------------------------
	int nTotalRead=0;
	int  nRead=0;
	BOOL bRead=TRUE;
	int  i,nTrimLF, nSpace=0;
	i = nTrimLF = nSpace = 0;
	
	//trim off LF
	char chLF[2];
	while(bRead)
	{
		nRead = cs.Receive(chLF, 2);
		if (nRead==0 || SOCKET_ERROR == nRead)
		{
			CString sError;
			sError.Format("���ݽ���ʧ�ܣ����͵����ݣ�����������ܳ�������,�������:%d",  GetLastError());
			//AfxMessageBox(sError);
			CLogFile::WriteLog(sError);
			bRead = FALSE;

			StopAutoWait();
			return 0;
		}

		if ( chLF[1] == cnLF)
			continue;

		break;
	}

	buffer[0]=chLF[0];
	buffer[1]=chLF[1];
	nTotalRead+=nRead;
	
	while(bRead)
	{
		nSpace = BUFSIZE-nTotalRead;
		nRead = cs.Receive(buffer+nTotalRead, nSpace);
		if (nRead==0 || SOCKET_ERROR == nRead)
		{
			CString sError;
			sError.Format("���ݽ���ʧ�ܣ����͵����ݣ�����������ܳ�������,�������:%d",  GetLastError());
			//AfxMessageBox(sError);
			CLogFile::WriteLog(sError);
			bRead = FALSE;

			StopAutoWait();
			return 0;
		}

		nTotalRead+=nRead;
		if (nRead>0 && nTotalRead>=4)
		{
			//------------------------------------liangjl 2011-04-20--------------------------
			//some time , we will read the end character which is not be read from the pre cmd.
			/*/so ,here we should throw these messages...
			if (( *(buffer+1) == cnLF) && ( *(buffer+3) == cnLF) )
			{
				for (i=1; i<nTotalRead; i+=2)
				{
					if ( *(buffer+i) == cnLF)
						continue;
					
					break;
				}

				nTrimLF = i-1; //trim LF in front of the sMsg.

				if (nTrimLF==nTotalRead) // this mean, we read nothing..., we should continue read ...
					continue; //while...
			}
			*///---------------------------------------------------------------------------------


			if ( ( *(buffer+nTotalRead-1) == cnLF) && ( *(buffer+nTotalRead-3) == cnCR)
					|| (( *(buffer+nTotalRead-1) == cnLF) && ( *(buffer+nTotalRead-3) == cnLF)))
			{
				//ofstream ofs("1.data", ios::binary|ios::out);
				//ofs.write(buffer, nTotalRead);
				//ofs.close();

				bRead = FALSE;
				break;
			}

			continue;
		}
		else
		{
			//AfxMessageBox("Socket���ݶ�ȡ������,������ȡ����!");
			continue;
			//bRead = FALSE;
			//break;
		}
	}

	StopAutoWait();

	if (nTotalRead >= BUFSIZE)
		AfxMessageBox("���ݻ���������!");
	///--------------------------------------------------------

	int len = nTotalRead;
	//int len = nTotalRead - nTrimLF;

	//if (len<0)		return 0; //if socket failed to logon, here will be returned -1, add this to viod crash.
	if (nTotalRead<0)	return 0;
	
	int nNum=len;

	//write to Log File.
	//util::int2str(xx, len);
	util::int2str(xx,nNum);
	CLogFile::WriteLog("read buffer:" + xx);
	if(len > 0 && len < 1024){
		buffer[len] = 0;
		CLogFile::WriteLog((const char *)buffer);
	}

	wait.Restore();         

	//return len;
	return nNum;
}