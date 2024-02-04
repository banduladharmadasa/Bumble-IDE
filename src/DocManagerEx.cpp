#include "stdafx.h"
#include "DocManagerEx.h"
#include "BumbleEditDoc.h"
#include "resource.h"
#include "ChildFrm.h"
#include "MultiDocTemplateEx.h"

CDocManagerEx::CDocManagerEx()
{
}


CDocManagerEx::~CDocManagerEx()
{
}

void CDocManagerEx::AddNewTextDoc()
{
	if (m_templateList.IsEmpty())
	{
		TRACE(traceAppMsg, 0, "Error: no document templates registered with CWinApp.\n");
		BumbleMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}

	CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetHead();

	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	pTemplate->OpenDocumentFile(NULL);
	// if returns NULL, the user has already been alerted
	
}
