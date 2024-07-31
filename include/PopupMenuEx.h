#pragma once
#include <afxpopupmenu.h>
class CPopupMenuEx : public CMFCPopupMenu
{
	DECLARE_SERIAL(CPopupMenuEx)

public:
	CPopupMenuEx();
	~CPopupMenuEx();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
