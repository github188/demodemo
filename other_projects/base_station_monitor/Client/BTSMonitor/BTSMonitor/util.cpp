#include "stdafx.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int util::split_next(const CString &src, CString &des, char ch, int start)
{
	int pos = 0;
	pos = src.Find(ch, start);
	des.Empty();
	if(pos < start){
		pos = src.GetLength();
	}
	if(pos > start){
		des=src.Mid(start, pos-start);
	}
	return pos;
}

int util::str2int(const CString src){
	int ret=_tstoi(src);
	return ret;
}

void util::int2str(CString &buffer, int i){
	
	//buffer.Format("%d",i);
	CString sAppend;
	sAppend.Format("%d", i);
	buffer += sAppend;
}

/*
将CString转为unicode
*/
void util::str2widechar(CString &src, WCHAR **des){
	LPTSTR lpStr = src.GetBuffer( src.GetLength() );
	int nLen = MultiByteToWideChar(CP_ACP, 0,lpStr, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, lpStr, -1, *des, nLen);
}

void util::widechar2str(WCHAR *src, CString &des)
{
	DWORD dwNum=::WideCharToMultiByte(CP_OEMCP, NULL, src, -1, NULL, 0, NULL, FALSE);
	char *psText=new char[dwNum];
	WideCharToMultiByte (CP_OEMCP,NULL,src,-1,psText,dwNum,NULL,FALSE);
	des=psText;
}

//<<<<<<< .mine
//int util::widechar2str(WCHAR *src, char **des)
//=======
DWORD util::widechar2str(WCHAR *src, char **des)
//>>>>>>> .r647
{
	memset(*des, 0, 4*1024);
	DWORD dwNum= WideCharToMultiByte(CP_OEMCP, NULL, src, -1, NULL, 0,    NULL, FALSE);
	int nBytes = WideCharToMultiByte(CP_OEMCP, NULL, src, -1, *des, dwNum,NULL, FALSE);

	DWORD dwError = GetLastError();
	if ( dwError == ERROR_INSUFFICIENT_BUFFER || dwError==ERROR_INVALID_FLAGS || dwError==ERROR_INVALID_PARAMETER)
	{
		AfxMessageBox("数据转换出错!");
		return 0;
	}

	return nBytes;
//	return dwNum;
}

void util::nettolocal(WCHAR *dstlocal, char *netbuffer, int netlen)
{
	u_short *ps=(u_short *)&netbuffer[2];
	u_short pconvs;
	for(int i=0;i<netlen/2;i++)
	{
		pconvs=ntohs(*ps);
		ps++;
		memcpy(&dstlocal[i], &pconvs, sizeof(pconvs));
	}
}
