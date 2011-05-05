#include "stdafx.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char __TempBuffer[_MAX_PATH];
char __MainAppPath[_MAX_PATH];
char __MainAppFileName[_MAX_PATH];


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

DWORD util::widechar2str(WCHAR *src, char **des)
{
	DWORD dwNum= WideCharToMultiByte(CP_ACP/*CP_OEMCP*/, NULL, src, -1, NULL, 0,    NULL, FALSE);
	int nBytes = WideCharToMultiByte(CP_ACP/*CP_OEMCP*/, NULL, src, -1, *des, dwNum,NULL, FALSE);

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
	//u_short *ps=(u_short *)&netbuffer[2];
	int nLen = netlen/2;
	u_short *ps=(u_short *)&netbuffer[0];
	u_short uHeader = ntohs(*ps);
	if ( (uHeader) == 0xFEFF)
	{
		ps=(u_short *)&netbuffer[2];
		nLen = nLen-1;
	}

	u_short pconvs;
	//for(int i=0;i<netlen/2;i++)
	for(int i=0;i<nLen;i++)
	{
		pconvs=ntohs(*ps);
		ps++;
		memcpy(&dstlocal[i], &pconvs, sizeof(pconvs));
	}
}

void util::TrimSpecialChar(CString& src, char ch)
{
	CString des;
	int i=0;
	int len = src.GetLength();
	for (i=0; i<len; i++)
	{
		if (src[i] == ch)
			continue;

		des += src[i];
	}

	src.Empty();
	src = des;
}

void util::InitApp()
{
	GetModuleFileName(NULL,__MainAppFileName, _MAX_PATH);
	strcpy_s(__MainAppPath ,sizeof(__MainAppPath), ExtractFilePath(__MainAppFileName));
}

char* util::GetAppPath()
{
	return (char *)__MainAppPath;
}


char* util::GetAppFileName()
{
	return (char *)__MainAppFileName;
}

////////////////////////////////////////////////////////////////
char* util::GetApplicationFullPath()
{
	static char tmpBuffer[_MAX_PATH];
	GetModuleFileName(NULL,tmpBuffer,_MAX_PATH);
	strcpy_s(tmpBuffer, _MAX_PATH, ExtractFilePath(tmpBuffer));
	
	
	return (char *)tmpBuffer;
}

char* util::GetApplicationFileName()
{
	static char tmpBuffer[_MAX_PATH];
	GetModuleFileName(NULL,tmpBuffer,_MAX_PATH);
	return (char *)tmpBuffer;
}

BOOL  util::CharInString(char ch,const char *s)
{
	int len = strlen(s);
	for(int i=0; i<len; i++)
	{
		if(ch==s[i]) 
			return TRUE;
	}
	return FALSE;
}

int   util::LastDelimiter(const char *Delimiters,const char *s)
{
  char ch;
  int i;
  for (i=strlen(s)-1 ; i>=0 ; i--)
  {
	  ch=s[i];
	  if(CharInString(ch,Delimiters)) break;
  }
  return i;
}

char *util::ExtractFileExt(const char *filename,char *outstring)
{
  int i = LastDelimiter(".\\:", filename);
  int len = strlen(filename) - i;
  if ((i >= 0) && (filename[i] == '.'))
  {
	  memcpy(outstring,&filename[i],len);
	  outstring[len]=0;
  }
  else
  {
	  outstring[0]=0;
  }
  return outstring;
}

char *util::ExtractFileName(const char *filename,char *outstring)
{
  int I = LastDelimiter("\\:", filename)+1;
  int len=strlen(filename) - I;
  memcpy(outstring,&filename[I],len);
  outstring[len]=0;
  return outstring;
}

char *util::ExtractFilePath(const char *filename,char *outstring)
{
  int I = LastDelimiter("\\:", filename) +1;
  memcpy(outstring,filename,I);
  outstring[I]=0;
  return outstring;
}

char *util::ChangeFileExt(const char *filename,const char *ext,char *outstring)
{
  int I = LastDelimiter(".\\:",filename);
  if ((I == -1) || (filename[I] != '.'))
	  I = strlen(filename);
  memcpy(outstring,filename,I);
  memcpy(&outstring[I],ext,strlen(ext));
  outstring[I+strlen(ext)]=0 ;
  return outstring;
}

char *util::ExtractFileNameHead(const char *filename,char *outstring)
{
	ExtractFileName(filename,outstring);//jxl change
	for(int i=0; outstring[i]!=0 ;i++)
	{
		if(outstring[i]=='.')
		{
			outstring[i] = 0;
			break;
		}
	}
	return outstring;
}
BOOL util::FileExists(const char *filename)
{
   CFileFind finder;
   BOOL bWorking = finder.FindFile(filename);
   while (bWorking)
   {
      bWorking = finder.FindNextFile();
	  if(!finder.IsDirectory())
		  return TRUE;
   }
   return FALSE;
}

BOOL util::DeleteAllFile(CString sDirecotory)
{
	if(PathFileExists(sDirecotory))     
		DeleteDirectory((LPSTR)(LPCTSTR)sDirecotory);
	return 1;
}

BOOL util::DeleteDirectory(char* sDirName) 
{ 
    CFileFind tempFind; 
    char sTempFileFind[200] ;
    
    sprintf_s(sTempFileFind,200,"%s\\*.*",sDirName); 
    BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
    while (IsFinded) 
    { 
        IsFinded = tempFind.FindNextFile(); 
        
        if (!tempFind.IsDots()) 
        { 
            char sFoundFileName[200]; 
            strcpy_s(sFoundFileName,200,tempFind.GetFileName().GetBuffer(200)); 
            
            if (tempFind.IsDirectory()) 
            { 
                char sTempDir[200]; 
                sprintf_s(sTempDir,200,"%s\\%s",sDirName,sFoundFileName); 
                DeleteDirectory(sTempDir); 
            } 
            else 
            { 
                char sTempFileName[200]; 
                sprintf_s(sTempFileName,200,"%s\\%s",sDirName,sFoundFileName); 
                DeleteFile(sTempFileName); 
            } 
        } 
    } 
    tempFind.Close(); 
    if(!RemoveDirectory(sDirName)) 
    { 
        return FALSE; 
    } 
    return TRUE; 
} 