// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// documentTestView.h : CdocumentTestView ��Ľӿ�
//


#pragma once
#include "PlayWnd.h"

class CdocumentTestView : public CView
{
protected: // �������л�����
	CdocumentTestView();
	DECLARE_DYNCREATE(CdocumentTestView)

// ����
public:
	CdocumentTestDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CdocumentTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int frameWidth;
	CList<CWnd*, CWnd*> playwndList;
	CRect mRect;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // documentTestView.cpp �еĵ��԰汾
inline CdocumentTestDoc* CdocumentTestView::GetDocument() const
   { return reinterpret_cast<CdocumentTestDoc*>(m_pDocument); }
#endif

