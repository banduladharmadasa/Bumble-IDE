// MyVisualManager.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "VisualManagerEx.h"

#define TAB_BK RGB(130, 130, 130)

// CVisualManagerEx
IMPLEMENT_DYNCREATE(CVisualManagerEx, CMFCVisualManagerOffice2007)

CVisualManagerEx::CVisualManagerEx()
{
	m_pBackBrush = new CBrush(RGB(43, 43, 43));
	m_pFaceBrush = new CBrush(RGB(0, 255, 255));
}

CVisualManagerEx::~CVisualManagerEx()
{
	delete m_pBackBrush;
	delete m_pFaceBrush;
}

void CVisualManagerEx::GetTabFrameColors(const CMFCBaseTabCtrl *pTabWnd, COLORREF &clrDark, COLORREF &clrBlack, COLORREF &clrHighlight, COLORREF &clrFace, COLORREF &clrDarkShadow, COLORREF &clrLight, CBrush *&pbrFace, CBrush *&pbrBlack)
{
	clrDark = TAB_BK;
	clrBlack = TAB_BK; // RGB(0, 0, 0);
	clrHighlight = RGB(0, 0, 0);
	clrFace = RGB(0, 0, 0);
	clrDarkShadow = RGB(0, 0, 0);
	clrLight = RGB(0, 0, 0);
	pbrFace = m_pBackBrush;
	pbrBlack = m_pFaceBrush;
}

void CVisualManagerEx::OnEraseTabsArea(CDC *pDC, CRect rect, const CMFCBaseTabCtrl *pTabWnd)
{
	static_cast<void>(pTabWnd);
	pDC->FillSolidRect(rect, TAB_BK);
}

BOOL CVisualManagerEx::OnEraseTabsFrame(CDC *pDC, CRect rect, const CMFCBaseTabCtrl *pTabWnd)
{

	pDC->FillSolidRect(rect, TAB_BK);
	return TRUE;
}

// void CVisualManagerEx::OnDrawTabCloseButton(CDC * pDC, CRect rect, const CMFCBaseTabCtrl * pTabWnd, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
//{
//
//	pDC->FillSolidRect(rect, RGB(163, 63, 63));
//
// }
//
// void CVisualManagerEx::OnEraseTabsButton(CDC * pDC, CRect rect, CMFCButton * pButton, CMFCBaseTabCtrl * pWndTab)
//{
//	pDC->FillSolidRect(rect, RGB(163, 63, 63));
//
// }

COLORREF CVisualManagerEx::OnDrawRibbonStatusBarPane(CDC *pDC, CMFCRibbonStatusBar *pBar, CMFCRibbonStatusBarPane *pPane)
{

	CRect rc = pPane->GetRect();

	pDC->FillSolidRect(rc, RGB(63, 63, 63));

	return RGB(255, 255, 255);
}

void CVisualManagerEx::OnDrawRibbonSliderZoomButton(CDC *pDC, CMFCRibbonSlider *pSlider, CRect rect, BOOL bIsZoomOut, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	CPen pen;
	if (!pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0)))
		return;

	HPEN oldPen = (HPEN)pDC->SelectObject(pen);

	if (bIsHighlighted)
		VerticalGradient(pDC->m_hDC, rect, HILITE_GRAD_TOP, HILITE_GRAD_BOTTOM);
	else
		VerticalGradient(pDC->m_hDC, rect, RGB(255, 221, 126), RGB(255, 242, 195));

	pDC->MoveTo(rect.left + 5, (rect.top + rect.bottom) / 2);
	pDC->LineTo(rect.right - 5, (rect.top + rect.bottom) / 2);

	if (!bIsZoomOut)
	{
		pDC->MoveTo((rect.left + rect.right) / 2, rect.top + 5);
		pDC->LineTo((rect.left + rect.right) / 2, rect.bottom - 5);
	}

	pDC->SelectObject(oldPen);

	DeleteObject(pen);
}

void CVisualManagerEx::OnDrawRibbonSliderChannel(CDC *pDC, CMFCRibbonSlider *pSlider, CRect rect)
{
	pDC->FillSolidRect(rect, RGB(127, 127, 127));
}

void CVisualManagerEx::OnDrawRibbonSliderThumb(CDC *pDC, CMFCRibbonSlider *pSlider, CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	CRect rc = rect;
	rc.DeflateRect(0, 2, 0, 2);

	if (bIsHighlighted)
		VerticalGradient(pDC->m_hDC, rc, HILITE_GRAD_TOP, HILITE_GRAD_BOTTOM);
	else
		VerticalGradient(pDC->m_hDC, rc, RGB(255, 221, 126), RGB(255, 242, 195));
}

// This method changes all bar colors, including the status bar and the menus
void CVisualManagerEx::OnFillBarBackground(CDC *pDC, CBasePane *pBar, CRect rectClient, CRect rectClip, BOOL bNCArea)
{
	if (pBar->IsKindOf(RUNTIME_CLASS(CMFCColorBar)))
	{

		pDC->FillSolidRect(rectClient, RGB(255, 255, 255));
	}
	else
	{
		pDC->FillSolidRect(rectClient, RGB(43, 43, 43));
	}
}

void CVisualManagerEx::OnDrawMenuBorder(CDC *pDC, CMFCPopupMenu *pMenu, CRect rect)
{
	pDC->FillSolidRect(rect, RGB(0, 0, 0));
}

void CVisualManagerEx::OnDrawTabsButtonBorder(CDC *pDC, CRect &rect, CMFCButton *pButton, UINT uiState, CMFCBaseTabCtrl *pWndTab)
{
	// Document List menu
	// pDC->FillSolidRect(rect, RGB(250,150,0));
	VerticalGradient(pDC->m_hDC, rect, RGB(255, 221, 126), RGB(255, 242, 195));
}

void CVisualManagerEx::OnDrawMenuCheck(CDC *pDC, CMFCToolBarMenuButton *pButton, CRect rect, BOOL bHighlight, BOOL bIsRadio)
{
	if (!bHighlight)
	{
		pDC->FillSolidRect(rect, RGB(43, 43, 43));
	}

	CRect _rect(0, 0, 16, 16);
	int difX = rect.CenterPoint().x - _rect.CenterPoint().x;
	int difY = rect.CenterPoint().y - _rect.CenterPoint().y;

	_rect.MoveToXY(difX, difY);

	CImage image;
	image.LoadFromResource(theApp.m_hInstance, IDB_CHECKBOX);

	if (image)
		image.Draw(pDC->m_hDC, _rect);
}

// void CVisualManagerEx::OnDrawMenuItemButton(CDC * pDC, CMFCToolBarMenuButton * pButton, CRect rectButton, BOOL bHighlight, BOOL bDisabled)
//{
//
//	pDC->FillSolidRect(rectButton, RGB(255, 0 ,255));
//
// }

void CVisualManagerEx::OnDrawSeparator(CDC *pDC, CBasePane *pBar, CRect rect, BOOL bIsHoriz)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN oldPen = (HPEN)pDC->SelectObject(pen);

	if (!bIsHoriz)
	{
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right, rect.top);
	}
	else
	{
		pDC->MoveTo(rect.left + 4, rect.top);
		pDC->LineTo(rect.left + 4, rect.bottom);
	}

	pDC->SelectObject(oldPen);

	DeleteObject(pen);
}

void CVisualManagerEx::OnFillButtonInterior(CDC *pDC, CMFCToolBarButton *pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{

	if (pButton->m_nStyle & TBBS_CHECKED)
	{

		pDC->FillSolidRect(rect, RGB(0, 0, 0));
	}

	switch (state)
	{
	case CMFCVisualManager::ButtonsIsRegular:

		break;
	case CMFCVisualManager::ButtonsIsPressed:

		break;
	case CMFCVisualManager::ButtonsIsHighlighted:
	{
		VerticalGradient(pDC->m_hDC, rect, HILITE_GRAD_TOP, HILITE_GRAD_BOTTOM, 1);
	}

	break;
	default:
		break;
	}
}

// Text  handlings

COLORREF CVisualManagerEx::GetStatusBarPaneTextColor(CMFCStatusBar *pStatusBar, CMFCStatusBarPaneInfo *pPane)
{
	return RGB(255, 255, 255);
}

COLORREF CVisualManagerEx::GetToolbarButtonTextColor(CMFCToolBarButton *pButton, CMFCVisualManager::AFX_BUTTON_STATE state)
{
	COLORREF color = RGB(255, 255, 255);
	switch (state)
	{
	case CMFCVisualManager::ButtonsIsRegular:

		break;
	case CMFCVisualManager::ButtonsIsPressed:
		color = RGB(0, 0, 0);
		break;
	case CMFCVisualManager::ButtonsIsHighlighted:
		color = RGB(0, 0, 0);
		break;
	default:
		break;
	}

	return color;
}

COLORREF CVisualManagerEx::GetMenuItemTextColor(CMFCToolBarMenuButton *pButton, BOOL bHighlighted, BOOL bDisabled)
{
	if (bHighlighted)
	{
		return RGB(0, 0, 0);
	}
	else if (bDisabled)
	{
		return RGB(150, 150, 150);
	}

	return RGB(255, 255, 255);
}

COLORREF CVisualManagerEx::GetHighlightedMenuItemTextColor(CMFCToolBarMenuButton *pButton)
{
	return RGB(0, 0, 0);
}

void CVisualManagerEx::OnHighlightMenuItem(CDC *pDC, CMFCToolBarMenuButton *pButton, CRect rect, COLORREF &clrText)
{
	// pDC->FillSolidRect(rect, RGB(50, 150, 250));
	VerticalGradient(pDC->m_hDC, rect, HILITE_GRAD_TOP, HILITE_GRAD_BOTTOM);
}

COLORREF CVisualManagerEx::GetTabTextColor(const CMFCBaseTabCtrl *pTabWnd, int iTab, BOOL bIsActive)
{
	return bIsActive ? RGB(0, 0, 0) : RGB(255, 255, 255);
}

COLORREF CVisualManagerEx::GetCaptionBarTextColor(CMFCCaptionBar *pBar)
{
	return RGB(255, 255, 255);
}

COLORREF CVisualManagerEx::GetToolbarDisabledTextColor()
{
	return RGB(150, 150, 150);
}

COLORREF CVisualManagerEx::OnDrawRibbonCategoryCaption(CDC *pDC, CMFCRibbonContextCaption *pContextCaption)
{
	return RGB(255, 255, 255);
}

COLORREF CVisualManagerEx::GetRibbonHyperlinkTextColor(CMFCRibbonLinkCtrl *pHyperLink)
{
	return RGB(255, 255, 255);
}

COLORREF CVisualManagerEx::GetRibbonStatusBarTextColor(CMFCRibbonStatusBar *pStatusBar)
{
	return RGB(255, 255, 255);
}

int CVisualManagerEx::GetRibbonPopupBorderSize(const CMFCRibbonPanelMenu *pPopup) const
{
	return 0;
}

void CVisualManagerEx::OnDrawMiniFrameBorder(CDC *pDC, CPaneFrameWnd *pFrameWnd, CRect rectBorder, CRect rectBorderSize)
{
}

void CVisualManagerEx::OnDrawFloatingToolbarBorder(CDC *pDC, CMFCBaseToolBar *pToolBar, CRect rectBorder, CRect rectBorderSize)
{
	// pDC->FillSolidRect(rectBorderSize, RGB(0, 255, 0));
}

COLORREF CVisualManagerEx::OnDrawPaneCaption(CDC *pDC, CDockablePane *pBar, BOOL bActive, CRect rectCaption, CRect rectButtons)
{
	pDC->MoveTo(rectCaption.TopLeft());
	pDC->LineTo(rectCaption.right, rectCaption.top);

	if (bActive)
	{
		pDC->FillSolidRect(rectCaption, RGB(83, 83, 83));
	}

	return RGB(255, 255, 255);
}

void CVisualManagerEx::OnDrawButtonBorder(CDC *pDC, CMFCToolBarButton *pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{
	// pDC->FillSolidRect(rect, RGB(255, 0, 0));
}

void CVisualManagerEx::OnDrawHeaderCtrlBorder(CMFCHeaderCtrl *pCtrl, CDC *pDC, CRect &rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	pDC->FillSolidRect(rect.left, rect.bottom - 2, rect.right, rect.bottom, RGB(43, 43, 43));
}

void CVisualManagerEx::OnDrawRibbonLabel(CDC *pDC, CMFCRibbonLabel *pLabel, CRect rect)
{
	pDC->FillSolidRect(rect, RGB(255, 0, 0));
}

BOOL CVisualManagerEx::GetToolTipInfo(CMFCToolTipInfo &params, UINT nType)
{
	params.m_clrFill = RGB(0, 0, 0);
	params.m_clrText = RGB(255, 255, 255);
	params.m_bDrawIcon = FALSE;
	return TRUE;
}

// Auto-hide buttons:

inline void CVisualManagerEx::OnFillAutoHideButtonBackground(CDC *pDC, CRect rect, CMFCAutoHideButton *pButton)
{
	pDC->FillSolidRect(rect, RGB(0xff, 0xe0, 0x88));
}
