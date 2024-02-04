// CBumbleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CBumbleDialog.h"
#include "afxdialogex.h"

using namespace Gdiplus;



// CBumbleDialog dialog

IMPLEMENT_DYNAMIC(CBumbleDialog, CDialog)

CBumbleDialog::CBumbleDialog(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
	m_pEditBkBrush = new CBrush(RGB(43, 43, 43));
	
}

CBumbleDialog::~CBumbleDialog()
{
}

void CBumbleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBumbleDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCMOUSELEAVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CBumbleDialog message handlers


void CBumbleDialog::OnPaint()
{
	CPaintDC dc(this);

}


HBRUSH CBumbleDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
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


void CBumbleDialog::OnDestroy()
{
	CDialog::OnDestroy();

	delete m_pEditBkBrush;
}

bool LoadBitmapFromPNG(UINT uResourceID, Bitmap** ppBitmapOut, HINSTANCE hInstance /*= NULL*/)
{
	bool bRet = false;

	if (!hInstance)
		hInstance = AfxGetInstanceHandle();

	HRSRC hResourceHandle = ::FindResource(
		hInstance, MAKEINTRESOURCE(uResourceID), L"PNG");
	if (0 == hResourceHandle)
	{
		return bRet;
	}

	DWORD nImageSize = ::SizeofResource(hInstance, hResourceHandle);
	if (0 == nImageSize)
	{
		return bRet;
	}

	HGLOBAL hResourceInstance = ::LoadResource(hInstance, hResourceHandle);
	if (0 == hResourceInstance)
	{
		return bRet;
	}

	const void* pResourceData = ::LockResource(hResourceInstance);
	if (0 == pResourceData)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
	if (0 == hBuffer)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	void* pBuffer = ::GlobalLock(hBuffer);
	if (0 != pBuffer)
	{
		CopyMemory(pBuffer, pResourceData, nImageSize);
		IStream* pStream = 0;
		if (S_OK == ::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream))
		{
			*ppBitmapOut = new Bitmap(pStream);
			pStream->Release();
			bRet = true;
		}
		::GlobalUnlock(hBuffer);
	}
	::GlobalFree(hBuffer);

	UnlockResource(hResourceInstance);
	FreeResource(hResourceInstance);

	return bRet;
}

LRESULT CBumbleDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_NCPAINT:
	case WM_NCACTIVATE:
	{
		

		CWindowDC dc(this);
		

		CRect rcClient, rcWindow;
		GetClientRect(rcClient);
		ClientToScreen(rcClient);
		GetWindowRect(rcWindow);

		rcClient.OffsetRect(-rcWindow.TopLeft());
		rcWindow.OffsetRect(-rcWindow.TopLeft());

		
		dc.ExcludeClipRect(rcClient);

		/*if (m_bMouseOverCloseBtn)
		{
			CRgn rgn;
			rgn.CreateRectRgnIndirect(m_closeBtnRect);
			dc.SelectClipRgn(&rgn);
		}*/

		 
		dc.IntersectClipRect(rcWindow);

		HDC hdcMem = CreateCompatibleDC(dc);
		
		RECT r = {};
		r.right = rcWindow.Width();
		r.bottom = rcWindow.Height();
		
		HBITMAP hBitmap = CreateCompatibleBitmap(dc, rcWindow.Width(), rcWindow.Height());
		SelectObject(hdcMem, hBitmap);



		int width = rcWindow.Width();
		int height = rcWindow.Height();

		Graphics g(hdcMem);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.FillRectangle(&SolidBrush(Color(255, 200, 106)), Rect(rcWindow.left, rcWindow.top, width, height));

		for (auto &it: m_sysMenuButtons) {

			Rect _rc(it.boundingRect.left, it.boundingRect.top, it.boundingRect.Width(), it.boundingRect.Height());
			
			switch (it.btnState)
			{
			case SYSBUTTONSTATES::SBS_NORMAL:
				g.FillRectangle(&SolidBrush(Color::White), _rc);
				break;

			case SYSBUTTONSTATES::SBS_PUSHED:
				g.FillRectangle(&SolidBrush(Color::Gray), _rc);
				break;

			case SYSBUTTONSTATES::SBS_HOT:
				g.FillRectangle(&SolidBrush(it.hotColor), _rc);
				break;

			case SYSBUTTONSTATES::SBS_DISABLED:

				break;
			default:
				break;
			}


			if (it.btnState != SBS_DISABLED) {
				it.btnState = SYSBUTTONSTATES::SBS_NORMAL;
				g.DrawImage(it.bitmap, _rc);
			}
		}


		


		//Draw caption
		Gdiplus::Font font(L"Arial", 10);
		PointF origin(0.0f, 0.0f);
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		CString caption;
		GetWindowText(caption);
		int capHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER)) + 4;
		RectF titleBarRect(30,0, width - 100, capHeight);
		StringFormat fmt;
		fmt.SetAlignment(StringAlignmentNear);
		fmt.SetLineAlignment(StringAlignmentCenter);
		g.DrawString(caption.GetBuffer(), caption.GetLength(), &font, titleBarRect, &fmt, &blackBrush);
		
		HICON hIcon = static_cast<HICON>(::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));

		DrawIconEx(hdcMem, 8, (int)(1.0 + ((capHeight - 16)/2.0)), hIcon, 16, 16,0, NULL, DI_NORMAL);


		

		BitBlt(dc, -1, -1, rcWindow.Width(), rcWindow.Height(), hdcMem, 0, 0, SRCCOPY);

		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
		DeleteDC(dc);

		return 0;

	}

	break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CBumbleDialog::LoadCaptionButtonsIcons(UINT nID)
{
	CBitmap bmp;

	m_CaptionButtonSize = CSize(0, 0);

	if (bmp.LoadBitmap(nID))
	{
		BITMAP bm;
		ZeroMemory(&bm, sizeof(BITMAP));

		bmp.GetBitmap(&bm);

		if (m_CaptionButtonIconst.GetSafeHandle() != NULL &&
			m_CaptionButtonIconst.GetImageCount() > 0)
		{
			m_CaptionButtonIconst.DeleteImageList();
		}

		UINT nFlags = ILC_MASK;

		switch (bm.bmBitsPixel)
		{
		case 4:
			nFlags |= ILC_COLOR4;
			break;
		case 8:
			nFlags |= ILC_COLOR8;
			break;
		case 16:
			nFlags |= ILC_COLOR16;
			break;
		case 24:
			nFlags |= ILC_COLOR24;
			break;
		case 32:
			nFlags |= ILC_COLOR32;
			break;
		default:
			ASSERT(FALSE);
		}

		m_CaptionButtonSize.cx = bm.bmWidth / 12;
		m_CaptionButtonSize.cy = bm.bmHeight;

		m_CaptionButtonIconst.Create(m_CaptionButtonSize.cx,
			m_CaptionButtonSize.cy, nFlags, 0, 3);
		m_CaptionButtonIconst.Add(&bmp, RGB(255, 0, 255));

		return true;
	}

	return false;
}

BOOL CBumbleDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	LoadCaptionButtonsIcons(IDB_CAPTION_BTNS);
	
	

	ModifyStyle(WS_SYSMENU, 0);
	
	HRESULT hResult;
	m_minMaxButton.btnState = SBS_DISABLED;
	m_minMaxButton.hotColor = Color::Orange;
	m_minMaxButton.buttonType = SysMenuButtonType::MinMaxButton;
	//HRESULT hResult = LoadBitmapFromPNG(IDB_MAXIMIZE_16_16, &m_minMaxButton.bitmap, NULL);
	m_sysMenuButtons.push_back(m_minMaxButton);
	

	m_restoreButton.btnState = SBS_DISABLED;
	m_restoreButton.hotColor = Color::Orange;
	m_restoreButton.buttonType = SysMenuButtonType::RestoreButton;
	//hResult = LoadBitmapFromPNG(IDB_RESTORE, &m_restoreButton.bitmap, NULL);
	m_sysMenuButtons.push_back(m_restoreButton);

	m_closeButton.btnState = SBS_NORMAL;
	m_closeButton.hotColor = Color::Black;
	m_closeButton.buttonType = SysMenuButtonType::CloseButton;
	hResult = LoadBitmapFromPNG(IDB_CLOSE_16_16, &m_closeButton.bitmap, NULL);
	m_sysMenuButtons.push_back(m_closeButton);

	RepositionSysMenuButtons();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CBumbleDialog::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rcWindow;
	GetWindowRect(&rcWindow);


	CPoint pt(point.x - rcWindow.left, point.y - rcWindow.top);
	for (auto &it : m_sysMenuButtons) {
		
		if (it.boundingRect.PtInRect(pt)) {
			if (it.btnState != SYSBUTTONSTATES::SBS_DISABLED) {
				it.btnState = SBS_HOT;
				SendMessage(WM_NCPAINT);
			}
			
		}
	}


	
	CDialog::OnNcMouseMove(nHitTest, point);
}


void CBumbleDialog::OnNcMouseLeave()
{
	// This feature requires Windows 2000 or greater.
	// The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_NCPAINT);
	CDialog::OnNcMouseLeave();
}


void CBumbleDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnNcLButtonUp(nHitTest, point);
}


void CBumbleDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);


	CPoint pt(point.x - rcWindow.left, point.y - rcWindow.top);
	for (auto& it : m_sysMenuButtons) {

		if (it.boundingRect.PtInRect(pt)) {
			if (it.btnState != SYSBUTTONSTATES::SBS_DISABLED) {
				it.btnState = SBS_PUSHED;
				SendMessage(WM_NCPAINT);
			}

		}
	}

	CDialog::OnNcLButtonDown(nHitTest, point);
}


void CBumbleDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (this->GetSafeHwnd()) {
		RepositionSysMenuButtons();
	}
	
}

void CBumbleDialog::RepositionSysMenuButtons()
{
	CRect rc;
	GetWindowRect(&rc);
	rc.OffsetRect(-rc.TopLeft());


	int i = 3;
	for (auto &it : m_sysMenuButtons) {
		it.boundingRect = CRect(0, 8, 16, 16 + 8);
		it.boundingRect.MoveToX(rc.right - (16 * i) - 8);
		i--;
	}
}


LRESULT CBumbleDialog::OnNcHitTest(CPoint point)
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);


	CPoint pt(point.x - rcWindow.left, point.y - rcWindow.top);
	for (auto& it : m_sysMenuButtons) {

		if (it.boundingRect.PtInRect(pt)) {
			if (((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) && (it.btnState != SBS_DISABLED)) {

				switch(it.buttonType)
				{
					case SysMenuButtonType::CloseButton:
					SendMessage(WM_SYSCOMMAND, SC_CLOSE);
					break;

					case SysMenuButtonType::MinMaxButton:
						SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
						break;


					case SysMenuButtonType::RestoreButton:
						SendMessage(WM_SYSCOMMAND, SC_RESTORE);
						break;
				default:
					break;
				}
			}
			
		}
	}

	

	return CDialog::OnNcHitTest(point);
}
