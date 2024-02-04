#include "stdafx.h"
#include "MFCToolBarEditBoxButtonEx.h"
#include "Resource.h"
#include "helper.h"
#include "AvailEdit.h"

IMPLEMENT_DYNCREATE(CMFCToolBarEditBoxButtonEx, CMFCToolBarEditBoxButton)

CMFCToolBarEditBoxButtonEx::CMFCToolBarEditBoxButtonEx()
{

}

CMFCToolBarEditBoxButtonEx::CMFCToolBarEditBoxButtonEx(UINT uiID, int iImage, DWORD dwStyle, int iWidth)
	: CMFCToolBarEditBoxButton(uiID, iImage, dwStyle, iWidth)
{

}


CMFCToolBarEditBoxButtonEx::~CMFCToolBarEditBoxButtonEx()
{
}

void CMFCToolBarEditBoxButtonEx::OnDraw(CDC * pDC, const CRect & rect, CMFCToolBarImages * pImages, BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight, BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{

	COLORREF color = CMFCColorMenuButton::GetColorByCmdID(ID_COLORPICKER);
	m_pWndEdit->SetWindowText(GetColorString(color));

	//pDC->FillSolidRect(rect, color);
}

