// SettingsDoc.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "SettingsDoc.h"


// CSettingsDoc

IMPLEMENT_DYNCREATE(CSettingsDoc, CDocument)

CSettingsDoc::CSettingsDoc()
{
	SetTitle(L"Options");
}

BOOL CSettingsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSettingsDoc::~CSettingsDoc()
{
}


BEGIN_MESSAGE_MAP(CSettingsDoc, CDocument)
END_MESSAGE_MAP()


// CSettingsDoc diagnostics

#ifdef _DEBUG
void CSettingsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSettingsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSettingsDoc serialization

void CSettingsDoc::Serialize(CArchive& ar)
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


// CSettingsDoc commands
