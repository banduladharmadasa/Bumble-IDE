// DataBaseDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AceEditor.h"
#include "DataBaseDoc.h"

// CDataBaseDoc

IMPLEMENT_DYNCREATE(CDataBaseDoc, CDocument)

CDataBaseDoc::CDataBaseDoc()
{
}

BOOL CDataBaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDataBaseDoc::~CDataBaseDoc()
{
}

BEGIN_MESSAGE_MAP(CDataBaseDoc, CDocument)
END_MESSAGE_MAP()

// CDataBaseDoc diagnostics

#ifdef _DEBUG
void CDataBaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CDataBaseDoc::Dump(CDumpContext &dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CDataBaseDoc serialization

void CDataBaseDoc::Serialize(CArchive &ar)
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

// CDataBaseDoc commands
