
#include "stdafx.h"
#include "resource.h"

void VerticalGradient(HDC hDC, const RECT& GradientFill, COLORREF rgbTop, COLORREF rgbBottom, int deflation)
{

	GRADIENT_RECT gradientRect = { 0, 1 };
	TRIVERTEX triVertext[2] = {
		GradientFill.left,
		GradientFill.top,
		GetRValue(rgbTop) << 8,
		GetGValue(rgbTop) << 8,
		GetBValue(rgbTop) << 8,
		0x0000,
		GradientFill.right,
		GradientFill.bottom,
		GetRValue(rgbBottom) << 8,
		GetGValue(rgbBottom) << 8,
		GetBValue(rgbBottom) << 8,
		0
	};
	::GradientFill(hDC, triVertext, 2, &gradientRect, 1, GRADIENT_FILL_RECT_V);
}

int BumbleMessageBox(LPCWSTR caption, UINT nType, UINT nIDHelp)
{
	CBumbleMessageBox* dlg = new CBumbleMessageBox(caption, NULL);
	
	int result = dlg->DoModal();
	delete dlg;

	return result;
}

int BumbleMessageBox(UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
	return AfxMessageBox(nIDPrompt, nType, nIDHelp);
}
