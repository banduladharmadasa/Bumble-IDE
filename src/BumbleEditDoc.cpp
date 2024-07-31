// BumbleEditDoc.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "BumbleEditDoc.h"
#include "BumbleEditView.h"
#include "MainFrm.h"
#include "CntrItem.h"
#include <iostream>
#include <fstream>

// CBumbleEditDoc

IMPLEMENT_DYNCREATE(CBumbleEditDoc, CRichEditDoc)

CBumbleEditDoc::CBumbleEditDoc()
{
	m_docID = ++docId;
}

BOOL CBumbleEditDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	return TRUE;
}

CBumbleEditDoc::~CBumbleEditDoc()
{
}

BEGIN_MESSAGE_MAP(CBumbleEditDoc, CRichEditDoc)
END_MESSAGE_MAP()

// CBumbleEditDoc diagnostics

#ifdef _DEBUG
void CBumbleEditDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CBumbleEditDoc::Dump(CDumpContext &dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CBumbleEditDoc serialization

void CBumbleEditDoc::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}
#endif

// CBumbleEditDoc commands

BOOL CBumbleEditDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	CBumbleEditView *pView = NULL;
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL)
	{
		return FALSE;
	}

	pView = (CBumbleEditView *)GetNextView(pos);
	pView->OpenFile(lpszPathName);

	theApp.AddDirToWatch(lpszPathName);

	return TRUE;
}

std::string to_utf8(const wchar_t *buffer, int len)
{
	int nChars = ::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		NULL,
		0,
		NULL,
		NULL);
	if (nChars == 0)
		return "";

	std::string newbuffer;
	newbuffer.resize(nChars);
	::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		const_cast<char *>(newbuffer.c_str()),
		nChars,
		NULL,
		NULL);

	return newbuffer;
}

std::string to_utf8(const std::wstring &str)
{
	return to_utf8(str.c_str(), (int)str.size());
}

BOOL CBumbleEditDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	theApp.IgnoreNextDirectoryWatch();
	CBumbleEditView *view = ((CBumbleEditView *)m_viewList.GetHead());

	CString content = view->GetContent();
	std::ofstream saveFile;

	saveFile.open(CT2CA(lpszPathName), std::ios::out | std::ios::binary);
	std::string outtext = to_utf8(content.GetBuffer());

	saveFile << outtext;

	saveFile.close();

	view->GetCodeEditor()->SetFileName(lpszPathName);
	return TRUE;
}

void CBumbleEditDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class
	CRichEditDoc::OnCloseDocument();
}

UINT CBumbleEditDoc::GetUniqueDocID()
{
	return m_docID;
}

void CBumbleEditDoc::Reload()
{
	CBumbleEditView *pView = NULL;
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL)
	{
		pView = (CBumbleEditView *)GetNextView(pos);
		pView->GetCodeEditor()->Reload();
	}
}

CRichEditCntrItem *CBumbleEditDoc::CreateClientItem(REOBJECT *preo) const
{
	return new CTextAppCntrItem(preo, const_cast<CBumbleEditDoc *>(this));
}

void CBumbleEditDoc::OnFileSaveCopyAs()
{
	if (!DoSave(NULL, FALSE))
		TRACE(traceAppMsg, 0, "Warning: File save-as failed.\n");
}

// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
BOOL CBumbleEditDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;

	if (newName.IsEmpty())
	{
		CDocTemplate *pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(":/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);
			if (newName.GetLength() > _MAX_FNAME)
				newName.ReleaseBuffer(_MAX_FNAME);

			if (AfxGetApp() && AfxGetApp()->GetDataRecoveryHandler())
			{
				// remove "[recovered]" from the title if it exists
				/*CString strNormalTitle = AfxGetApp()->GetDataRecoveryHandler()->GetNormalDocumentTitle(this);
				if (!strNormalTitle.IsEmpty())
					newName = strNormalTitle;*/
			}

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
				!strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');

				int nSemi;						 // added
				if (nSemi = strExt.Find(';'))	 // added
					strExt = strExt.Left(nSemi); // added

				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
										   bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
										   OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE; // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				e->Delete(); // modified
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
	{
		SetPathName(newName, TRUE);
		OnDocumentEvent(onAfterSaveDocument);
	}

	CBumbleEditView *pView = (CBumbleEditView *)GetView();

	if (pView)
	{

		pView->GetCodeEditor()->SetLanguageByFileName(newName);
	}

	return TRUE; // success
}

HMENU CBumbleEditDoc::GetDefaultMenu()
{
	if (!m_hDefaultMenu)
	{
		m_hDefaultMenu = ::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_TYPE_BUMBLEEDIT));
	}

	return m_hDefaultMenu;
}

///
/// Add an extra menu for the selected language
///
void CBumbleEditDoc::AddExtraMenu(const CString &language)
{
	/*m_hDefaultMenu = ::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_TYPE_BUMBLEEDIT));
	if (m_hDefaultMenu == NULL)
		return;


	if (language.Compare(L"HTML") == 0) {
		CMenu* menu = new CMenu;
		menu->LoadMenuW(IDR_TYPE_HTML);
		::AppendMenu(m_hDefaultMenu, MF_POPUP, (UINT_PTR)menu->GetSubMenu(0)->m_hMenu, L"Debug");
	}

	CMainFrame *frame = theApp.GetMainWindow();
	frame->MDISetMenu(CMenu::FromHandle(m_hDefaultMenu), NULL);
	frame->DrawMenuBar();*/
}

void CBumbleEditDoc::LoadXmlConfigFile(const CString &langMode)
{

	m_xmlConfigManager.Read(langMode);
}
