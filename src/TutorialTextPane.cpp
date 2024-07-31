// TutorialTextPane.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "TutorialTextPane.h"
#define IsBitSet(val, bit) ((val) & (1 << (bit)))

HHOOK CCommandHistoryPane::m_kbdHook;
HHOOK CCommandHistoryPane::m_mouseHook;

// CString CCommandHistoryPane::m_displayString;
std::vector<CString> CCommandHistoryPane::m_messageQueue;
CCommandHistoryPane *CCommandHistoryPane::m_pTutorialTextPane;

// CCommandHistoryPane

IMPLEMENT_DYNAMIC(CCommandHistoryPane, CDockablePane)

CCommandHistoryPane::CCommandHistoryPane()
{
}

CCommandHistoryPane::~CCommandHistoryPane()
{
}

BEGIN_MESSAGE_MAP(CCommandHistoryPane, CDockablePane)
ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCommandHistoryPane message handlers

using namespace Gdiplus;

void CCommandHistoryPane::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	RectF _rc;
	CString str;
	GetClientRect(&rc);
	Gdiplus::Font bigFont(L"Arial", 16);
	Gdiplus::Font smallFont(L"Arial", 12);
	Graphics graphics(dc);
	StringFormat fmt;
	fmt.SetAlignment(StringAlignmentCenter);

	_rc = RectF(rc.left, rc.top, rc.Width(), rc.Height());
	graphics.FillRectangle(&SolidBrush(Color::LightYellow), _rc);

	int size = m_messageQueue.size();
	for (int i = m_messageQueue.size() - 1; i > 0; i--)
	{
		str = m_messageQueue.at(i);

		_rc = RectF(rc.left, rc.top + (size - i) * 32, rc.Width(), rc.Height());

		if (i == (size - 1))
		{
			graphics.DrawString(str.GetString(), str.GetLength(), &bigFont, _rc, &fmt, &SolidBrush(Color::Black));
		}
		else
		{
			graphics.DrawString(str.GetString(), str.GetLength(), &smallFont, _rc, &fmt, &SolidBrush(Color::Black));
		}
	}

	while (m_messageQueue.size() > 5)
	{
		m_messageQueue.erase(m_messageQueue.begin());
	}
}

int CCommandHistoryPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_kbdHook = SetWindowsHook(WH_KEYBOARD, &CCommandHistoryPane::KeyBoardHookProc);
	ASSERT(NULL != m_kbdHook);

	m_mouseHook = SetWindowsHook(WH_MOUSE, &CCommandHistoryPane::MouseHookProc);
	ASSERT(NULL != m_mouseHook);

	m_pTutorialTextPane = this;

	return 0;
}

LRESULT CCommandHistoryPane::KeyBoardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || nCode != HC_ACTION) // do not process message
		return ::CallNextHookEx(m_kbdHook, nCode, wParam, lParam);

	if (isprint(wParam) && !IsBitSet(lParam, 30) && !IsBitSet(lParam, 31))
	{
		CString str;
		BOOL bUse = FALSE;
		str.Format(L"[%c]", (TCHAR)wParam);

		if (GetAsyncKeyState(VK_MENU) < 0)
		{
			str.Insert(0, L"[ALT] + ");
			bUse = TRUE;
		}

		if (GetAsyncKeyState(VK_SHIFT) < 0)
		{
			str.Insert(0, L"[SHIFT] + ");
			bUse = TRUE;
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			str.Insert(0, L"[CTRL] + ");
			bUse = TRUE;
		}

		if (bUse)
		{
			m_messageQueue.push_back(str);
			m_pTutorialTextPane->Invalidate();
		}
	}
	else
	{
		CString str;
		switch (wParam)
		{
		case VK_TAB:
			str = L"[TAB]";
			break;

		case VK_SPACE:
			str = L"[SPACE]";
			break;

		case VK_CAPITAL:
		{
			if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
			{
				str = (L"[CAPS-LOCK ON]");
			}
			else
			{
				str = (L"[CAPS-LOCK OFF]");
			}
		}
		break;

		case VK_LEFT:
			str = L"[LEFT-ARROW]";
			break;

		case VK_UP:
			str = L"[UP-ARROW]";
			break;

		case VK_RIGHT:
			str = L"[RIGHT-ARROW]";
			break;

		case VK_DOWN:
			str = L"[DOWN-ARROW]";
			break;

		case VK_ESCAPE:
			str = L"[ESCAPE]";
			break;

		case VK_RETURN:
			str = L"[RETURN]";
			break;

		case VK_BACK:
			str = L"[BACK-SPACE]";
			break;

		case VK_DELETE:
			str = L"[DELETE]";
			break;

		default:
			break;
		}

		if (!str.IsEmpty())
		{

			if (GetAsyncKeyState(VK_MENU) < 0)
			{
				str.Insert(0, L"[ALT] + ");
			}

			if (GetAsyncKeyState(VK_SHIFT) < 0)
			{
				str.Insert(0, L"[SHIFT] + ");
			}

			if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				str.Insert(0, L"[CTRL] + ");
			}

			m_messageQueue.push_back(str);
			m_pTutorialTextPane->Invalidate();
		}
	}

	return ::CallNextHookEx(m_kbdHook, nCode, wParam, lParam);
}

LRESULT CCommandHistoryPane::MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) // do not process message
		return ::CallNextHookEx(m_mouseHook, nCode, wParam, lParam);

	CString str;
	BOOL bDetected = FALSE;

	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		str = L"[LEFT BUTTON DOWN]";
		bDetected = TRUE;
		break;

	case WM_LBUTTONUP:
		str = L"[LEFT BUTTON UP]";
		bDetected = TRUE;
		break;

	case WM_LBUTTONDBLCLK:
		str = L"[DOUBLE CLICK]";
		bDetected = TRUE;
		break;

	case WM_RBUTTONDOWN:
		str = L"[RIGHT BUTTON DOWN]";
		bDetected = TRUE;
		break;

	case WM_RBUTTONUP:
		str = L"[RIGHT BUTTON UP]";
		bDetected = TRUE;
		break;

	case WM_MOUSEWHEEL:
	{
		MOUSEHOOKSTRUCTEX *info = reinterpret_cast<MOUSEHOOKSTRUCTEX *>(lParam);
		if (static_cast<std::make_signed_t<WORD>>(HIWORD(info->mouseData)) > 0)
		{
			str = L"[MOUSE WHEEL FORWARD]";
		}
		else
		{
			str = L"[MOUSE WHEEL BACKWARD]";
		}

		bDetected = TRUE;
	}

	break;
	case WM_MOUSEMOVE:
	{
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			str = L"[MOUSE DRAG]";
			bDetected = TRUE;
		}
	}
	break;
	default:
		break;
	}

	if (bDetected)
	{
		SHORT ctrl = GetKeyState(VK_CONTROL);
		SHORT shift = GetKeyState(VK_SHIFT);
		SHORT alt = GetKeyState(VK_MENU);
		SHORT ret = GetKeyState(VK_RETURN);

		CString strChar((TCHAR)wParam);

		if (ctrl < 0)
		{
			str.Insert(0, L"[CTRL] + ");
		}

		if (shift < 0)
		{
			str.Insert(0, L"[SHIFT] + ");
		}

		if (alt < 0)
		{
			str.Insert(0, L"[ALT] + ");
		}

		if (ret < 0)
		{
			str.Insert(0, L"[RETURN]");
		}

		m_messageQueue.push_back(str);
		m_pTutorialTextPane->Invalidate();
	}

	return ::CallNextHookEx(m_mouseHook, nCode, wParam, lParam);
}

void CCommandHistoryPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	if (NULL != m_kbdHook)
	{
		::UnhookWindowsHookEx(m_kbdHook);
	}

	if (NULL != m_mouseHook)
	{
		::UnhookWindowsHookEx(m_mouseHook);
	}
}
