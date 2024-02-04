// DataBaseView.cpp : implementation file
//

#include "stdafx.h"
#include "AceEditor.h"
#include "DataBaseView.h"


// CDataBaseView

IMPLEMENT_DYNCREATE(CDataBaseView, CHtmlView)

CDataBaseView::CDataBaseView()
{

}

CDataBaseView::~CDataBaseView()
{
}

void CDataBaseView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataBaseView, CHtmlView)
END_MESSAGE_MAP()


// CDataBaseView diagnostics

#ifdef _DEBUG
void CDataBaseView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CDataBaseView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CDataBaseView message handlers
