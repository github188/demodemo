#ifndef __UTIL_H_
#define __UTIL_H_

class util
{
public:
	static int split_next(const CString &src, CString &des, char ch, int start);
	static int str2int(const CString src);
	static void int2str(CString &buffer, int i);
	static void str2widechar(CString &src, WCHAR **des);
	static void widechar2str(WCHAR *src, CString &des);
//<<<<<<< .mine
//	static int  widechar2str(WCHAR *src, char **des);
//=======
	static DWORD widechar2str(WCHAR *src, char **des);
//>>>>>>> .r647
	static void nettolocal(WCHAR *dstlocal, char *netbuffer, int netlen);
	static void TrimSpecialChar(CString& src, char ch);
};

#endif