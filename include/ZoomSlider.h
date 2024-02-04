#pragma once

// CZoomSlider command target

class CZoomSlider : public CMFCRibbonSlider
{
public:
	CZoomSlider();
	CZoomSlider(UINT nID, int nWidth = 100);
	virtual ~CZoomSlider();

	virtual void OnDraw(CDC* pDC);
};


