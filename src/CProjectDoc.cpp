// CProjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CProjectDoc.h"


// CProjectDoc

IMPLEMENT_DYNCREATE(CProjectDoc, CDocument)

CProjectDoc::CProjectDoc()
{
}

BOOL CProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CProjectDoc::~CProjectDoc()
{
}


BEGIN_MESSAGE_MAP(CProjectDoc, CDocument)
END_MESSAGE_MAP()


// CProjectDoc diagnostics

#ifdef _DEBUG
void CProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CProjectDoc serialization

void CProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CProjectDoc commands
