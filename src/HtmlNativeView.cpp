// HtmlNativeView.cpp : implementation file
//

#include "stdafx.h"
#include "AvailEdit.h"
#include "HtmlNativeView.h"
#include "AvailEditDoc.h"

// CHtmlNativeView

IMPLEMENT_DYNCREATE(CHtmlNativeView, CGenaricWebView)

CHtmlNativeView::CHtmlNativeView()
{
}

CHtmlNativeView::~CHtmlNativeView()
{
}

BEGIN_MESSAGE_MAP(CHtmlNativeView, CGenaricWebView)
ON_COMMAND(ID_DEBUG_RUN, &CHtmlNativeView::OnDebugRun)
ON_COMMAND(ID_DEBUG_REFRESH, &CHtmlNativeView::OnDebugRefresh)
END_MESSAGE_MAP()

// CHtmlNativeView drawing

void CHtmlNativeView::OnDraw(CDC *pDC)
{
	CAvailEditDoc *pDoc = GetDocument();
	// TODO: add draw code here
}

// CHtmlNativeView diagnostics

#ifdef _DEBUG
void CHtmlNativeView::AssertValid() const
{
	CGenaricWebView::AssertValid();
}

#ifndef _WIN32_WCE
void CHtmlNativeView::Dump(CDumpContext &dc) const
{
	CGenaricWebView::Dump(dc);
}
#endif
#endif //_DEBUG

// CHtmlNativeView message handlers

void CHtmlNativeView::OnDebugRun()
{
	CString pathName = GetDocument()->GetPathName();

	if (!GetDocument()->DoFileSave())
	{
		return;
	}

	pathName = GetDocument()->GetPathName();

	m_codeEditor.Eval(L" var logWindow = window.open()");

	theApp.SetUpDebugEnviorenment(IDR_TYPE_HTML, pathName);
	CString fileName = pathName.Mid(pathName.ReverseFind('\\') + 1);

	fileName.Insert(0, L"logWindow.location.href='http://127.0.0.1:8085/");
	fileName.Append(L"'");

	m_codeEditor.Eval(fileName);
}

void CHtmlNativeView::OnDebugRefresh()
{
	CString pathName = GetDocument()->GetPathName();

	if (!GetDocument()->DoFileSave())
	{
		return;
	}

	pathName = GetDocument()->GetPathName();
	theApp.SetUpDebugEnviorenment(IDR_TYPE_HTML, pathName);
	CString fileName = pathName.Mid(pathName.ReverseFind('\\') + 1);

	fileName.Insert(0, L"logWindow.location.href='http://127.0.0.1:8085/");
	fileName.Append(L"'");

	m_codeEditor.Eval(fileName);
}
