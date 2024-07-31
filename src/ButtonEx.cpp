// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ButtonEx.h"

// CButtonEx

IMPLEMENT_DYNAMIC(CButtonEx, CMFCButton)

CButtonEx::CButtonEx()
{
}

CButtonEx::~CButtonEx()
{
}

BEGIN_MESSAGE_MAP(CButtonEx, CMFCButton)
ON_WM_CREATE()
END_MESSAGE_MAP()

// CButtonEx message handlers

int CButtonEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	this->SetTextColor(RGB(255, 255, 255));
	this->SetFaceColor(RGB(51, 122, 183));

	return 0;
}

void CButtonEx::OnDraw(CDC *pDC, const CRect &rect, UINT uiState)
{
	if (this->IsHighlighted())
	{
		VerticalGradient(pDC->m_hDC, rect);
	}

	return CMFCButton::OnDraw(pDC, rect, uiState);
}
