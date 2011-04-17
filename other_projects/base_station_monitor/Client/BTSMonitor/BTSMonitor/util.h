#ifndef __UTIL_H_
#define __UTIL_H_

extern char __TempBuffer[_MAX_PATH];
extern char __MainAppPath[_MAX_PATH];
extern char __MainAppFileName[_MAX_PATH];

class util
{
public:
	static int split_next(const CString &src, CString &des, char ch, int start);
	static int str2int(const CString src);
	static void int2str(CString &buffer, int i);
	static void str2widechar(CString &src, WCHAR **des);
	static void widechar2str(WCHAR *src, CString &des);
	static DWORD widechar2str(WCHAR *src, char **des);
	static void nettolocal(WCHAR *dstlocal, char *netbuffer, int netlen);
	static void TrimSpecialChar(CString& src, char ch);

	//Common function
	static void InitApp();
	static char *GetAppPath();
	static char *GetAppFileName();
	static char *GetApplicationFullPath();
	static char *GetApplicationFileName();
	static char *ExtractFileExt(const char *filename,char *outstr=__TempBuffer);
	static char *ExtractFileName(const char *filename,char *outstr=__TempBuffer);
	static char *ExtractFilePath(const char *filename,char *outstr=__TempBuffer);
	static char *ChangeFileExt(const char *filename,const char *ext,char *outstr=__TempBuffer);
	static char *ExtractFileNameHead(const char *filename,char *outstr=__TempBuffer);
	static BOOL FileExists(const char *filename);
	static BOOL CharInString(char ch,const char *s);
	static int  LastDelimiter(const char *Delimiters,const char *s);

};

#endif