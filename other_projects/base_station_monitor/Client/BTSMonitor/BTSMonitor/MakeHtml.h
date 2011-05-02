#pragma once

class CMakeHtml
{
public:
	CMakeHtml(CString sPicDir, int nCol, int nWidth, int nHeight);
	~CMakeHtml(void);
public:
	BOOL CreateImageHtmlFile();
	void AddBlankRow(int nLines);
	void CompleteHtml();
	void MergeCol(int nCol, CString sContent);
	void AddCol(BOOL bFirst, CString sContent);
	void AddColPic(CString sPicture);
	void CompleteRow();
	void AddRow(BOOL bHeader=FALSE); //1:Header, 0 non header
	void CompleteTable();
	void AddTable(int nType);
	CString GetHtmlFile() {return m_sReportFile;}
private:
	void InitHtmlFile();

private:
	CString m_sPictureDir;
	CString m_sReportFile;
	CFile   m_fHtml;

	int m_nCol;
	int m_nWidth; 
	int m_nHeight;

};
