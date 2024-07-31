#pragma once

class CProjectView;
// CProjectTabCtrl

class CProjectTabCtrl : public CTabCtrl
{
	//   Construction/destruction
public:
	CProjectTabCtrl(CProjectView *pParent);
	virtual ~CProjectTabCtrl();

public:
public:
	void SetColours(COLORREF bSelColour, COLORREF bUnselColour);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();

protected:
	COLORREF m_crSelColour, m_crUnselColour;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

	CProjectView *m_pParent;

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};
