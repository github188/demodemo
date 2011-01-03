// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// robbinui3Doc.cpp : Crobbinui3Doc 类的实现
//

#include "stdafx.h"
#include "robbinui3.h"

#include "robbinui3Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Crobbinui3Doc

IMPLEMENT_DYNCREATE(Crobbinui3Doc, CDocument)

BEGIN_MESSAGE_MAP(Crobbinui3Doc, CDocument)
END_MESSAGE_MAP()


// Crobbinui3Doc 构造/析构

Crobbinui3Doc::Crobbinui3Doc()
{
	// TODO: 在此添加一次性构造代码

}

Crobbinui3Doc::~Crobbinui3Doc()
{
}

BOOL Crobbinui3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Crobbinui3Doc 序列化

void Crobbinui3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// Crobbinui3Doc 诊断

#ifdef _DEBUG
void Crobbinui3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Crobbinui3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Crobbinui3Doc 命令

void Crobbinui3Doc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument::SetTitle(_T("视频监控系统"));
}
