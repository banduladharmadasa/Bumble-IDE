

#include "stdafx.h"
#include "ProjectTabCtrl.h"
#include "ProjectView.h"
// CProjectTabCtrl

CProjectTabCtrl::CProjectTabCtrl(CProjectView* pParent)
	:m_pParent(pParent)
{
	m_crSelColour = RGB(0, 0, 255);
	m_crUnselColour = RGB(50, 50, 50);
}

CProjectTabCtrl::~CProjectTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CProjectTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CProjectTabCtrl) 
	ON_WM_CREATE()
	//}}AFX_MSG_MAP 
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CProjectTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////// 
//   CProjectTabCtrl   message   handlers 

int   CProjectTabCtrl::OnCreate(LPCREATESTRUCT   lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)   return   -1;
	ModifyStyle(0, TCS_OWNERDRAWFIXED);

	return   0;
}

void   CProjectTabCtrl::PreSubclassWindow()
{
	CTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

void   CProjectTabCtrl::DrawItem(LPDRAWITEMSTRUCT   lpDrawItemStruct)
{
	CRect   rect = lpDrawItemStruct->rcItem;
	int   nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0)   return;
	BOOL   bSelected = (nTabIndex == GetCurSel());

	WCHAR   label[64];
	TC_ITEM   tci;
	tci.mask = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText = label;
	tci.cchTextMax = 63;
	if (!GetItem(nTabIndex, &tci))   return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC)   return;
	int   nSavedDC = pDC->SaveDC();

	//   For   some   bizarre   reason   the   rcItem   you   get   extends   above   the   actual 
	//   drawing   area.   We   have   to   workaround   this   "feature ". 
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

	//   Draw   image 
	CImageList* pImageList = GetImageList();

	if (pImageList != NULL && tci.iImage >= 0)
	{

		rect.left += pDC->GetTextExtent(_T("   ")).cx; //   Margin 

		//   Get   height   of   image   so   we   
		IMAGEINFO   info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect   ImageRect(info.rcImage);
		int   nYpos = rect.top;

		pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	if (bSelected) {

		pDC->SetTextColor(m_crSelColour);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
	else {
		pDC->SetTextColor(m_crUnselColour);
		pDC->DrawText(label, rect, DT_SINGLELINE | DT_BOTTOM | DT_CENTER);
	}

	pDC->RestoreDC(nSavedDC);

}

///////////////////////////////////////////////////////////////////////////// 
//   CProjectTabCtrl   operations 

void   CProjectTabCtrl::SetColours(COLORREF   bSelColour, COLORREF   bUnselColour)
{
	m_crSelColour = bSelColour;
	m_crUnselColour = bUnselColour;
	Invalidate();
}






void CProjectTabCtrl::OnPaint()
{
	CPaintDC dc(this);

	dc.SelectObject(GetFont());

	CPen pen, pen_active;
	COLORREF color_off = RGB(94, 94, 94);
	COLORREF color_active = RGB(255, 255, 255);
	CBrush brush_off, brush_active;
	brush_off.CreateSolidBrush(color_off);
	brush_active.CreateSolidBrush(color_active);
	pen.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	pen_active.CreatePen(PS_SOLID, 1, color_active);

	CRect rcitem;
	GetItemRect(0, &rcitem);

	CRect rc;
	GetClientRect(&rc);
	rc.bottom = rcitem.bottom;
	dc.FillSolidRect(&rc, GetSysColor(COLOR_3DFACE));

	GetClientRect(&rc);
	rc.top = rcitem.bottom - 1;
	dc.SelectObject(&pen);
	dc.SelectObject(&brush_active);
	dc.Rectangle(&rc);

	for (int i = 0; i < GetItemCount(); i++)
	{
		dc.SelectObject(&pen);
		if (i == GetCurSel())
		{
			dc.SelectObject(&brush_active);
			dc.SetBkColor(color_active);
		}
		else
		{
			dc.SelectObject(&brush_off);
			dc.SetBkColor(color_off);
		}

		GetItemRect(i, &rcitem);
		rcitem.right++;
		dc.Rectangle(&rcitem);

		if (i == GetCurSel())
		{
			dc.SelectObject(pen_active);
			dc.MoveTo(rcitem.left + 1, rcitem.bottom - 1);
			dc.LineTo(rcitem.right, rcitem.bottom - 1);
		}

		TCITEM item = { 0 };
		wchar_t buf[32];
		item.pszText = buf;
		item.cchTextMax = 32;
		item.mask = TCIF_TEXT;
		GetItem(i, &item);
		dc.DrawText(buf, &rcitem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}


BOOL CProjectTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;// CTabCtrl::OnEraseBkgnd(pDC);
}


void CProjectTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_pParent->OnTabChanged();
}


