// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes

#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars



#include <afxrich.h>            // MFC rich edit classes
#include <afxdhtml.h>
#include <afxwin.h>
#include <afxext.h>

#include <afxdlgs.h>

#include <afx.h>
#include <afxhtml.h>


#include <afxhtml.h>

#include <gdiplus.h>

#include "helper.h"
#include "CBumbleButton.h"
#include "CBumbleDialog.h"
#include "CBumbleMessageBox.h"
#include "CBumblePropertySheet.h"
#include "CBumblePropertyPage.h"
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afx.h>
#include <afxdhtml.h>
#include <afxdhtml.h>
#include <afxdhtml.h>
#include <afxdhtml.h>


#ifndef VERTICAL_GRADIENT
#define VERTICAL_GRADIENT

#define HILITE_GRAD_TOP RGB(0xdd, 0xcc, 0x66)
#define HILITE_GRAD_BOTTOM RGB(0xff, 0xe0, 0x88)

void VerticalGradient(HDC hDC, const RECT& GradientFill, COLORREF rgbTop = HILITE_GRAD_TOP, COLORREF rgbBottom = HILITE_GRAD_BOTTOM, int deflation = 0);
#endif // !1




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


int BumbleMessageBox(LPCWSTR caption, UINT nType = 0, UINT nIDHelp = 0);
int BumbleMessageBox(UINT nIDPrompt, UINT nType = 0, UINT nIDHelp = 0);