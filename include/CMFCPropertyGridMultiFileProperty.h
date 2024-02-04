#pragma once
#include <afxpropertygridctrl.h>
class CMFCPropertyGridMultiFileProperty :
	public CMFCPropertyGridFileProperty
{
public:
	CMFCPropertyGridMultiFileProperty(const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);

	virtual void OnClickButton(CPoint point);
};

