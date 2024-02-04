// CMessageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CMessageWnd.h"


// CMessageWnd

IMPLEMENT_DYNAMIC(CMessageWnd, CRichEditCtrl)

CMessageWnd::CMessageWnd()
{

}

CMessageWnd::~CMessageWnd()
{
}

void CMessageWnd::AddMessage(const CString& message, AppMessageType type)
{
	CHARFORMAT cf = { 0 };
	int txtLen =  GetTextLength();


		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = ~CFE_AUTOCOLOR;

	
	switch (type)
	{
	case AppMessageType_Info:
		cf.crTextColor = RGB(0, 0, 0);
		break;
	case AppMessageType_Error:
		cf.crTextColor = RGB(255, 0, 0);
		break;
	case AppMessageType_Warning:
		cf.crTextColor = RGB(255, 255, 0);
		break;
	case AppMessageType_Character:
		cf.crTextColor = RGB(0, 0, 0);
		break;
	case AppMessageTpe_ShellOutput:
		cf.crTextColor = RGB(0, 0, 255);
		break;
	default:
		break;
	}




	 SetSel(txtLen, -1);
	 SetSelectionCharFormat(cf);
	 ReplaceSel(message);
	 int currentLength = GetTextLength();

	 SetSel(currentLength, -1);
	 SetFocus();

	 ((CDockablePane*)GetParent())->ShowPane(TRUE, 0, TRUE);

	 if (((CDockablePane*)GetParent())->IsAutoHideMode()) {
		 ((CDockablePane*)GetParent())->ToggleAutoHide();
	 }

}


BEGIN_MESSAGE_MAP(CMessageWnd, CRichEditCtrl)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CMessageWnd message handlers

void CMessageWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar != VK_BACK) {
		theApp.PostWinMessageToShellThread(WM_CHAR, nChar, nRepCnt);
	}
	else {
		CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	}
}
