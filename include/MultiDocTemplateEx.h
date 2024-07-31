#pragma once
#include "afxwin.h"
class CMultiDocTemplateEx : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CMultiDocTemplateEx)
public:
	CMultiDocTemplateEx(UINT nIDResource, CRuntimeClass *pDocClass, CRuntimeClass *pFrameClass, CRuntimeClass *pViewClass);
	~CMultiDocTemplateEx();

	virtual BOOL GetDocString(CString &rString, enum DocStringIndex index) const override;
	CDocTemplate::Confidence MatchDocType(const WCHAR *pszPathName, CDocument *&rpDocMatch) override;
};
