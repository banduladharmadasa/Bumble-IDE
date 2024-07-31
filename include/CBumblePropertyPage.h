#pragma once

// CBumblePropertyPage

class CBumblePropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CBumblePropertyPage)

public:
	CBumblePropertyPage(UINT nResID);
	virtual ~CBumblePropertyPage();

protected:
	DECLARE_MESSAGE_MAP()
	CBrush *m_pEditBkBrush;
	CRect m_rectLogo;

public:
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
};
