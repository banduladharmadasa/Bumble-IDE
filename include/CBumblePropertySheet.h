#pragma once

// CBumblePropertySheet

class CBumblePropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(CBumblePropertySheet)

public:
	CBumblePropertySheet();
	virtual ~CBumblePropertySheet();

protected:
	DECLARE_MESSAGE_MAP()

	CBrush *m_pEditBkBrush;
	CRect m_rectLogo;

public:
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL LoadCaptionButtonsIcons(UINT nID);
	BOOL OnInitDialog();
	void ReplacePushButtons(UINT nID);
	void OnNcMouseMove(UINT nHitTest, CPoint point);
	void OnNcMouseLeave();
	void OnNcLButtonUp(UINT nHitTest, CPoint point);
	void OnNcLButtonDown(UINT nHitTest, CPoint point);
	void OnSize(UINT nType, int cx, int cy);
	void RepositionSysMenuButtons();
	LRESULT OnNcHitTest(CPoint point);

protected:
	std::vector<SysMenuButton> m_sysMenuButtons;

	SysMenuButton m_minMaxButton;
	SysMenuButton m_restoreButton;
	SysMenuButton m_closeButton;

	CImageList m_CaptionButtonIconst;
	CSize m_CaptionButtonSize;
};

bool LoadBitmapFromPNG(UINT uResourceID, Gdiplus::Bitmap **ppBitmapOut, HINSTANCE hInstance);
