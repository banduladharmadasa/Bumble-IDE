// PhpView.cpp : implementation file
//

#include "stdafx.h"
#include "AvailEdit.h"
#include "PhpView.h"
#include "AvailEditDoc.h"

// CPhpView

IMPLEMENT_DYNCREATE(CPhpView, CGenaricWebView)

CPhpView::CPhpView()
{

}

CPhpView::~CPhpView()
{
}

BEGIN_MESSAGE_MAP(CPhpView, CGenaricWebView)
	ON_COMMAND(ID_SERVERTYPE_BUILTIN, &CPhpView::OnServerTypeBuiltIn)
	ON_COMMAND(ID_SERVERTYPE_PHP, &CPhpView::OnServerTypePhp)
	ON_COMMAND(ID_SERVERTYPE_EXTERNAL, &CPhpView::OnServerTypeExternal)
	ON_COMMAND(ID_DEBUG_RUN, &CPhpView::OnDebugRun)
	ON_COMMAND(ID_DEBUG_REFRESH, &CPhpView::OnDebugRefresh)
END_MESSAGE_MAP()


// CPhpView drawing

void CPhpView::OnDraw(CDC* pDC)
{
	CAvailEditDoc* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPhpView diagnostics

#ifdef _DEBUG
void CPhpView::AssertValid() const
{
	CGenaricWebView::AssertValid();
}

#ifndef _WIN32_WCE
void CPhpView::Dump(CDumpContext& dc) const
{
	CGenaricWebView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPhpView message handlers


void CPhpView::OnServerTypeBuiltIn()
{
	// TODO: Add your command handler code here
}


void CPhpView::OnServerTypePhp()
{
	// TODO: Add your command handler code here
}


void CPhpView::OnServerTypeExternal()
{
	// TODO: Add your command handler code here
}

void CPhpView::OnDebugRun()
{
	CString pathName = GetDocument()->GetPathName();

	if (!GetDocument()->DoFileSave()) {
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

void CPhpView::OnDebugRefresh()
{
	CString pathName = GetDocument()->GetPathName();

	if (!GetDocument()->DoFileSave()) {
		return;
	}

	pathName = GetDocument()->GetPathName();
	theApp.SetUpDebugEnviorenment(IDR_TYPE_HTML, pathName);
	CString fileName = pathName.Mid(pathName.ReverseFind('\\') + 1);

	fileName.Insert(0, L"logWindow.location.href='http://127.0.0.1:8085/");
	fileName.Append(L"'");

	m_codeEditor.Eval(fileName);

}

