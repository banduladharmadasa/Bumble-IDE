// CBumbleButton.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CBumbleButton.h"


// CBumbleButton

IMPLEMENT_DYNAMIC(CBumbleButton, CMFCButton)

CBumbleButton::CBumbleButton()
{
	
	SetFaceColor(RGB(255, 224, 136), true);
	SetTextColor(RGB(0, 0, 0));
	this->SetTextHotColor(RGB(255, 100, 0));
	m_nFlatStyle = FlatStyle::BUTTONSTYLE_SEMIFLAT;

	this->m_bDrawFocus = FALSE;
	
	
}

CBumbleButton::~CBumbleButton()
{
}


BEGIN_MESSAGE_MAP(CBumbleButton, CMFCButton)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CBumbleButton message handlers




void CBumbleButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	/*CMFCButton::OnDrawItem(nIDCtl, lpDrawItemStruct);*/
	//if (nIDCtl == IDC_BUTTON_COLOR)
	//{
	/*CDC dc;


	dc.Attach(lpDrawItemStruct->hDC);
	RECT rect;
	rect = lpDrawItemStruct->rcItem;


	UINT state = lpDrawItemStruct->itemState;

	if ((state & ODS_SELECTED))
	{
		dc.FillSolidRect(&rect, RGB(0xdd, 0xdd, 0x44));

	}
	else if (state & ODS_HOTLIGHT)
	{
		dc.FillSolidRect(&rect, RGB(0xff, 0xff, 0x88));
	}
	else
	{
		dc.FillSolidRect(&rect, RGB(0xff, 0xe0, 0x88));
	}

	dc.SetBkColor(RGB(100, 100, 255));
	dc.SetTextColor(RGB(0, 0, 0));
	dc.SetBkMode(TRANSPARENT);

	TCHAR buffer[MAX_PATH];
	ZeroMemory(buffer, MAX_PATH);
	::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
	dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc.Detach();*/
	//}


	CMFCButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
