#include "stdafx.h"
#include "util.h"

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
	int ret=atoi(src);
	return ret;
}

void util::int2str(CString &buffer, int i){
	buffer.Format("%d",i);
}

/*
��CStringתΪunicode
*/
void util::str2widechar(CString &src, WCHAR **des){
	LPTSTR lpStr = src.GetBuffer( src.GetLength() );
	int nLen = MultiByteToWideChar(CP_ACP, 0,lpStr, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, lpStr, -1, *des, nLen);
}