// CBumblePropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CBumblePropertyPage.h"


// CBumblePropertyPage

IMPLEMENT_DYNAMIC(CBumblePropertyPage, CMFCPropertyPage)

CBumblePropertyPage::CBumblePropertyPage(UINT nResID) : CMFCPropertyPage(nResID)
{
	m_pEditBkBrush = new CBrush(RGB(43, 43, 43));
}

CBumblePropertyPage::~CBumblePropertyPage()
{
}


BEGIN_MESSAGE_MAP(CBumblePropertyPage, CMFCPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CBumblePropertyPage message handlers




HBRUSH CBumblePropertyPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CMFCPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return (HBRUSH)(GetStockObject(WHITE_BRUSH));
		pDC->SetTextColor(RGB(0, 0, 0));
		break;


	default:
		pDC->SetBkColor(RGB(43, 43, 43));
		pDC->SetTextColor(RGB(255, 255, 255));
		break;
	}




	return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
}


void CBumblePropertyPage::OnDestroy()
{
	CMFCPropertyPage::OnDestroy();

	delete m_pEditBkBrush;
}
