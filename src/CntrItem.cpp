
// CntrItem.cpp : implementation of the CTextAppCntrItem class
//

#include "stdafx.h"
#include "BumbleEdit.h"

#include "BumbleEditDoc.h"
#include "BumbleEditView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextAppCntrItem implementation

IMPLEMENT_SERIAL(CTextAppCntrItem, CRichEditCntrItem, 0)

CTextAppCntrItem::CTextAppCntrItem(REOBJECT* preo, CBumbleEditDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CTextAppCntrItem::~CTextAppCntrItem()
{
	// TODO: add cleanup code here
}


// CTextAppCntrItem diagnostics

#ifdef _DEBUG
void CTextAppCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CTextAppCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

