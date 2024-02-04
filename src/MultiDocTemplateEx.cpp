#include "stdafx.h"
#include "MultiDocTemplateEx.h"


IMPLEMENT_DYNAMIC(CMultiDocTemplateEx, CMultiDocTemplate)

CMultiDocTemplateEx::CMultiDocTemplateEx(UINT nIDResource, CRuntimeClass * pDocClass, CRuntimeClass * pFrameClass, CRuntimeClass * pViewClass)
	: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{

}

CMultiDocTemplateEx::~CMultiDocTemplateEx()
{
}

BOOL CMultiDocTemplateEx::GetDocString(CString & rString, DocStringIndex index) const
{
	CString strTemp, strLeft, strRight;
	int nFindPos;
	AfxExtractSubString(strTemp, m_strDocStrings, (int)index, L'\n');

	if (index == CDocTemplate::filterExt) {
		nFindPos = strTemp.Find(L';');
		if (-1 != nFindPos) {
			//string contains two extensions
			strLeft = strTemp.Left(nFindPos + 1);
			strRight = L"*" + strTemp.Right(lstrlen((const WCHAR*)strTemp) - nFindPos - 1);
			strTemp = strLeft + strRight;
		}
	}

	rString = strTemp;
	if ((int)index == 4) {
		rString.Replace(L"*", L"");
	}
	
	return TRUE;
}


CDocTemplate::Confidence CMultiDocTemplateEx::MatchDocType(const
	WCHAR* pszPathName, CDocument*& rpDocMatch)
{
	ASSERT(pszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (pDoc->GetPathName() == pszPathName) {
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}  // end while

	   // see if it matches either suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
		!strFilterExt.IsEmpty())
	{
		// see if extension matches
		ASSERT(strFilterExt[0] == L'.');
		CString ext1, ext2;
		int nDot = CString(pszPathName).ReverseFind(L'.');
		const WCHAR* pszDot = nDot < 0 ? NULL : pszPathName + nDot;

		int nSemi = strFilterExt.Find(L';');
		if (-1 != nSemi) {
			// string contains two extensions
			ext1 = strFilterExt.Left(nSemi);
			ext2 = strFilterExt.Mid(nSemi + 2);
			// check for a match against either extension
			if (nDot >= 0 && (lstrcmpi(pszPathName + nDot, ext1) == 0
				|| lstrcmpi(pszPathName + nDot, ext2) == 0))
				return yesAttemptNative; // extension matches
		}
		else
		{ // string contains a single extension
			if (nDot >= 0 && (lstrcmpi(pszPathName + nDot,
				strFilterExt) == 0))
				return yesAttemptNative;  // extension matches
		}
	}
	return yesAttemptForeign; //unknown document type
}