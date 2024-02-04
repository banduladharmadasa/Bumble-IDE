// ZoomSlider.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ZoomSlider.h"


// CZoomSlider

CZoomSlider::CZoomSlider()
{
}

CZoomSlider::CZoomSlider(UINT nID, int nWidth) : CMFCRibbonSlider(nID, nWidth)
{
	this->m_nWidth = 150;
	
}

CZoomSlider::~CZoomSlider()
{
}

void CZoomSlider::OnDraw(CDC * pDC)
{
	
	pDC->FillSolidRect(m_rect, RGB(0,0,0));

	CMFCRibbonSlider::OnDraw(pDC);
}



