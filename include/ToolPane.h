#pragma once

// CToolPane

class CToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(CToolPane)

public:
	CToolPane();
	virtual ~CToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


