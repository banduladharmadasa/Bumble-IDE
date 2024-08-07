#pragma once
#include "BumbleEditView.h"

// CGenaricWebView view

class CGenaricWebView : public CBumbleEditView
{
	DECLARE_DYNCREATE(CGenaricWebView)

protected:
	CGenaricWebView(); // protected constructor used by dynamic creation
	virtual ~CGenaricWebView();

public:
	virtual void OnDraw(CDC *pDC); // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext &dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};
