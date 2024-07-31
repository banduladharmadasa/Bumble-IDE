#pragma once

// CStatusBarEx

class CStatusBarEx : public CMFCRibbonStatusBar
{
	DECLARE_DYNAMIC(CStatusBarEx)

public:
	CStatusBarEx();
	virtual ~CStatusBarEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
