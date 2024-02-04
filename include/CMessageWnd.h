#pragma once
#include "BumbleEdit.h"

// CMessageWnd

class CMessageWnd : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CMessageWnd)

public:
	CMessageWnd();
	virtual ~CMessageWnd();

	void AddMessage(const CString& message, AppMessageType type);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


