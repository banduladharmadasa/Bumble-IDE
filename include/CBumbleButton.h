#pragma once

// CBumbleButton

class CBumbleButton : public CMFCButton
{
	DECLARE_DYNAMIC(CBumbleButton)

public:
	CBumbleButton();
	virtual ~CBumbleButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
