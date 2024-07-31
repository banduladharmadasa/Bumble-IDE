// CaptionButton.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CaptionButton.h"
#include "MainFrm.h"

// CCaptionButton

IMPLEMENT_DYNAMIC(CCaptionButton, CMFCButton)

CCaptionButton::CCaptionButton()
{
}

CCaptionButton::~CCaptionButton()
{
}

BEGIN_MESSAGE_MAP(CCaptionButton, CMFCButton)
ON_WM_CREATE()
END_MESSAGE_MAP()

void CCaptionButton::OnDraw(CDC *pDC, const CRect &rect, UINT uiState)
{
	if (GetDlgCtrlID() == ID_APPLICATION)
	{

		HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

		pDC->FillSolidRect(rect, RGB(0, 0, 0));
		pDC->DrawIcon(2, 2, hIcon);
		return;
	}

	if (!m_image)
		return;

	if (!this->IsHighlighted())
	{
		if (this->GetDlgCtrlID() == ID_SYSTEM_RESTORE)
		{
			if (((CMainFrame *)theApp.GetMainWnd())->WasMaximized())
				m_image.Draw(pDC->m_hDC, 0, 0);

			else
				m_maxImage.Draw(pDC->m_hDC, 0, 0);
		}
		else
		{
			m_image.Draw(pDC->m_hDC, 0, 0);
		}
		return;
	}

	CPaintDC dc(this);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap *m_bmpPLMain = nullptr;

	if (this->GetDlgCtrlID() == ID_SYSTEM_RESTORE)
	{

		if (((CMainFrame *)theApp.GetMainWnd())->WasMaximized())
			m_bmpPLMain = CBitmap::FromHandle(m_image);

		else
			m_bmpPLMain = CBitmap::FromHandle(m_maxImage);
	}
	else
	{

		m_bmpPLMain = CBitmap::FromHandle(m_image);
	}

	CBitmap *pOldBmp = (CBitmap *)dcMem.SelectObject(m_bmpPLMain);

	pDC->BitBlt(0, 0, 26, 26, &dcMem, 0, 0, SRCINVERT);

	dcMem.SelectObject(pOldBmp);

	DeleteObject(dcMem);
}

int CCaptionButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	switch (GetDlgCtrlID())
	{

	case ID_APP_EXIT:
		m_image.LoadFromResource(theApp.m_hInstance, IDB_CLOSE);
		break;

	case ID_SYSTEM_MINIMIZE:
		m_image.LoadFromResource(theApp.m_hInstance, IDB_MINIMIZE);
		break;

	case ID_SYSTEM_RESTORE:
		m_image.LoadFromResource(theApp.m_hInstance, IDB_RESTORE);
		m_maxImage.LoadFromResource(theApp.m_hInstance, IDB_MAXIMIZE);

		break;

	case ID_APPLICATION:
	{
		HICON icon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		SetIcon(icon);
	}

	break;
	}

	m_bDrawFocus = FALSE;

	return 0;
}

void CCaptionButton::OnDrawBorder(CDC *pDC, CRect &rectClient, UINT uiState)
{
}
