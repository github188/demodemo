#include "StdAfx.h"
#include "MakeHtml.h"
#include "util.h"

CMakeHtml::CMakeHtml(CString sPicDir,int nCol, int nWidth, int nHeight)
{
	m_sPictureDir = sPicDir;

	m_nCol		= nCol;
	m_nWidth	= nWidth;
	m_nHeight	= nHeight;

	InitHtmlFile();
}

CMakeHtml::~CMakeHtml(void)
{
}

//****************************************
//功能：
//
//****************************************
void CMakeHtml::InitHtmlFile()
{
	//example for CFile::Open
	CTime t = CTime::GetCurrentTime();
	m_sReportFile.Format("%s\\%.4d%.2d%.2d_%.2d%.2d%.2d.htm",m_sPictureDir, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(),t.GetSecond());

	CFileException e;
	if( !m_fHtml.Open( m_sReportFile, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		AfxMessageBox("Create File Failed!!!");
		return ;
	}

	CString str;
	str.Format("%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
	"<html>",
	"<head>",
	"<title> Warning Images</title>",
	"<META Content-Type: text/html; charset=gb2312>",
	"</head>",
	"<body>");
	
	m_fHtml.Write(str.GetBuffer(0),str.GetLength());
	
}

void CMakeHtml::AddTable(int nType)
{
	CString str;
	if (nType%2)		//#00ff00
	{
		//Add Table
		str.Format("<table border=0 cols=%d cellpadding=0 cellspacing=0 bordercolor=#00ff00 align=center>\r\n", m_nCol);
	}
	else
	{
		str.Format("<table border=0 cols=%d cellpadding=0 cellspacing=0 bordercolor=#0000FF align=center>\r\n", m_nCol);
	}

	m_fHtml.Write(str.GetBuffer(0),str.GetLength());

}

void CMakeHtml::CompleteTable()
{
	CString str;
	str.Format("%s\r\n", "</table>");
	m_fHtml.Write(str.GetBuffer(0),str.GetLength());
}

void CMakeHtml::AddRow(BOOL bHeader)
{
	CString str;
	if (bHeader) //Header
	{
		str.Format("%s\r\n", "<tr bgColor=#d1eafa>");
	}
	else
	{
		str.Format("%s\r\n", "<tr>");
	}

	m_fHtml.Write(str.GetBuffer(0),str.GetLength());

}

void CMakeHtml::CompleteRow()
{
	CString str;
	str.Format("%s\r\n", "</tr>");
	m_fHtml.Write(str.GetBuffer(0),str.GetLength());

}

void CMakeHtml::AddCol(BOOL bFirst, CString sContent)
{
	CString str ;
	if (bFirst)
		str.Format("<td  width=%d height=%d>%s</td>\r\n",m_nWidth, m_nHeight,sContent);
	else
		str.Format("<td  width=150>%s</td>\r\n", sContent);

	m_fHtml.Write(str.GetBuffer(0),str.GetLength());
}
void CMakeHtml::AddColPic(CString sPicture)
{
	CString str;
	str.Format("<td  width=%d height=%d> <IMG src=\"%s\" width=%d height=%d  border=1> </td>\r\n",m_nWidth, m_nHeight ,sPicture,m_nWidth, m_nHeight);

	m_fHtml.Write(str.GetBuffer(0),str.GetLength());
}

void CMakeHtml::MergeCol(int nCol, CString sContent)
{
	CString str;
	str.Format("<td colSpan=%d>%s</td>",nCol, sContent);

	m_fHtml.Write(str.GetBuffer(0),str.GetLength());
}

void CMakeHtml::CompleteHtml()
{
	CString str;
	str.Format("%s\r\n%s\r\n",
				"</body>",
				"</html>");

	m_fHtml.Write(str.GetBuffer(0),str.GetLength());

	m_fHtml.Close();
}

void CMakeHtml::AddBlankRow(int nLines)
{
	CString str;
	for (int i=0; i<nLines; i++)
	{
		str = "&nbsp\r\n";
		m_fHtml.Write(str.GetBuffer(0),str.GetLength());
	}
		
}


