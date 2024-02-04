#pragma once


// CCaptionButton

class CCaptionButton : public CMFCButton
{
	DECLARE_DYNAMIC(CCaptionButton)

public:
	CCaptionButton();
	virtual ~CCaptionButton();


protected:
	DECLARE_MESSAGE_MAP()
public:

	virtual void OnDraw(CDC* pDC, const CRect& rect, UINT uiState);

protected:
	CImage m_image;
	CImage m_maxImage;


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
};


