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

// BTSMonitorDoc.cpp : CBTSMonitorDoc 类的实现
//

#include "stdafx.h"
#include "BTSMonitor.h"

#include "BTSMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBTSMonitorDoc

IMPLEMENT_DYNCREATE(CBTSMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CBTSMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CBTSMonitorDoc 构造/析构

CBTSMonitorDoc::CBTSMonitorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CBTSMonitorDoc::~CBTSMonitorDoc()
{
}

BOOL CBTSMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	//this->SetTitle(_T("移动基站视频图像监控系统"));

	return TRUE;
}




// CBTSMonitorDoc 序列化

void CBTSMonitorDoc::Serialize(CArchive& ar)
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


// CBTSMonitorDoc 诊断

#ifdef _DEBUG
void CBTSMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBTSMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBTSMonitorDoc 命令
