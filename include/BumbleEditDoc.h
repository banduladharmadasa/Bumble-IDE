#pragma once
#include "CXmlConfigManager.h"

class CTextAppCntrItem;
class CBumbleEditDoc : public CRichEditDoc
{
	DECLARE_DYNCREATE(CBumbleEditDoc)

public:
	CBumbleEditDoc();
	virtual ~CBumbleEditDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive &ar); // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext &dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	UINT GetUniqueDocID();
	void Reload();

	virtual CRichEditCntrItem *CreateClientItem(REOBJECT *preo = NULL) const override;
	void OnFileSaveCopyAs();

	BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace) override;

	void AddExtraMenu(const CString &language);

	void LoadXmlConfigFile(const CString &langMode);

protected:
	virtual HMENU GetDefaultMenu();
	HMENU m_hDefaultMenu;

private:
	UINT m_docID;

	CLanguageSpecificConfigManager m_xmlConfigManager;
};

// Make a unique ID for each document instance to help
// Plugin DLLs to recognize correct document to deliver the
// processed content. The value of this identifier is copied
// in each instance at the constructor of the CDocument derived class
static UINT docId = 0;