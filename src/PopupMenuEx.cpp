#include "stdafx.h"
#include "PopupMenuEx.h"

IMPLEMENT_SERIAL(CPopupMenuEx, CMFCPopupMenu, 1)
CPopupMenuEx::CPopupMenuEx()
{
}

CPopupMenuEx::~CPopupMenuEx()
{
}
BEGIN_MESSAGE_MAP(CPopupMenuEx, CMFCPopupMenu)
ON_WM_PAINT()
ON_WM_CREATE()
END_MESSAGE_MAP()

void CPopupMenuEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
	// Do not call CMFCPopupMenu::OnPaint() for painting messages

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(rect);
	int height = this->m_iLogoWidth;

	rect.DeflateRect(0, 0, 0, 0);

	dc.FillSolidRect(rect, RGB(43, 43, 43));
	// VerticalGradient(dc, rect, HILITE_GRAD_TOP, HILITE_GRAD_BOTTOM, 1);
}

int CPopupMenuEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCPopupMenu::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	EnableMenuLogo(20, CMFCPopupMenu::MENU_LOGO_TOP);

	CRect rect;
	GetWindowRect(&rect);
	int w = rect.Width();
	int h = rect.Height();

	CRgn rgn1;
	CRgn rgn2;

	CPaintDC dc(this);
	CreateCompatibleDC(dc);
	dc.BeginPath();

	dc.MoveTo(16, 0);
	dc.LineTo(32, 16);
	dc.LineTo(0, 16);

	dc.EndPath();

	rgn1.CreateFromPath(&dc);

	rgn2.CreateRectRgn(1, 16, w, h);
	CombineRgn(rgn1, rgn1, rgn2, RGN_OR);

	SetWindowRgn(static_cast<HRGN>(rgn1.GetSafeHandle()), TRUE);
	rgn1.Detach();
	rgn2.Detach();

	ReleaseDC(&dc);

	return 0;
}
