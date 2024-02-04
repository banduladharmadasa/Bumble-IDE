// GenaricWebView.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "GenaricWebView.h"


// CGenaricWebView

IMPLEMENT_DYNCREATE(CGenaricWebView, CBumbleEditView)

CGenaricWebView::CGenaricWebView()
{

}

CGenaricWebView::~CGenaricWebView()
{
}

BEGIN_MESSAGE_MAP(CGenaricWebView, CBumbleEditView)
END_MESSAGE_MAP()


// CGenaricWebView drawing

void CGenaricWebView::OnDraw(CDC* pDC)
{
	CBumbleEditDoc* pDoc = GetDocument();
	// TODO: add draw code here
}


// CGenaricWebView diagnostics

#ifdef _DEBUG
void CGenaricWebView::AssertValid() const
{
	CBumbleEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CGenaricWebView::Dump(CDumpContext& dc) const
{
	CBumbleEditView::Dump(dc);
}

#endif
#endif //_DEBUG


