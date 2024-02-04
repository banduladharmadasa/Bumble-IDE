// CStaticEx.cpp : implementation file
//

#include "stdafx.h"
#include "CStaticEx.h"

using namespace Gdiplus;

// CStaticEx

IMPLEMENT_DYNAMIC(CStaticEx, CStatic)

CStaticEx::CStaticEx()
{

}

CStaticEx::~CStaticEx()
{
}


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStaticEx message handlers




void CStaticEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages

	Graphics graphics(dc);
	CString string;
	GetWindowText(string);

	FontFamily   fontFamily(L"MS Shell Dlg");
	Gdiplus::Font         font(&fontFamily, 9, FontStyleRegular, UnitPoint);
	CRect rc;
	GetClientRect(rc);
	RectF        rectF(rc.left, rc.top, rc.Width(), rc.Height());
	StringFormat stringFormat;
	SolidBrush   solidBrush(Color(255, 255, 255, 255));


	//stringFormat.SetAlignment(StringAlignmentCenter);


	stringFormat.SetLineAlignment(StringAlignmentCenter);

	graphics.DrawString(string.GetBuffer(), -1, &font, rectF, &stringFormat, &solidBrush);

	
}
