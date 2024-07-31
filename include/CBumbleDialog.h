#pragma once
#include <vector>

enum class SysMenuButtonType
{
	MinMaxButton = 0,
	RestoreButton,
	CloseButton
};

struct SysMenuButton
{
	CRect boundingRect;
	SYSBUTTONSTATES btnState = SBS_NORMAL;
	Gdiplus::Color hotColor = Gdiplus::Color::Orange;
	SysMenuButtonType buttonType;
	Gdiplus::Bitmap *bitmap;
};

// CBumbleDialog dialog

class CBumbleDialog : public CDialog
{
	DECLARE_DYNAMIC(CBumbleDialog)

public:
	CBumbleDialog(UINT nIDTemplate, CWnd *pParentWnd = NULL); // standard constructor
	virtual ~CBumbleDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_CBumbleDialog
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

protected:
	CBrush *m_pEditBkBrush;
	CRect m_rectLogo;

public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL LoadCaptionButtonsIcons(UINT nID);
	virtual BOOL OnInitDialog();

protected:
	std::vector<SysMenuButton> m_sysMenuButtons;

	SysMenuButton m_minMaxButton;
	SysMenuButton m_restoreButton;
	SysMenuButton m_closeButton;

	CImageList m_CaptionButtonIconst;
	CSize m_CaptionButtonSize;

public:
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseLeave();
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void RepositionSysMenuButtons();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
