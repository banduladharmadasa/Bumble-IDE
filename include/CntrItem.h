
// CntrItem.h : interface of the CTextAppCntrItem class
//

#pragma once

class CBumbleEditDoc;
class CTextAppView;

class CTextAppCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CTextAppCntrItem)

	// Constructors
public:
	CTextAppCntrItem(REOBJECT *preo = NULL, CBumbleEditDoc *pContainer = NULL);
	// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
	//  IMPLEMENT_SERIALIZE requires the class have a constructor with
	//  zero arguments.  Normally, OLE items are constructed with a
	//  non-NULL document pointer

	// Attributes
public:
	CBumbleEditDoc *GetDocument()
	{
		return reinterpret_cast<CBumbleEditDoc *>(CRichEditCntrItem::GetDocument());
	}
	CTextAppView *GetActiveView()
	{
		return reinterpret_cast<CTextAppView *>(CRichEditCntrItem::GetActiveView());
	}

	// Implementation
public:
	~CTextAppCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif
};
