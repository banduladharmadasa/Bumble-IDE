// StatusBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "StatusBarEx.h"

// CStatusBarEx

IMPLEMENT_DYNAMIC(CStatusBarEx, CMFCRibbonStatusBar)

CStatusBarEx::CStatusBarEx()
{
}

CStatusBarEx::~CStatusBarEx()
{
}

BEGIN_MESSAGE_MAP(CStatusBarEx, CMFCRibbonStatusBar)
ON_WM_CREATE()
END_MESSAGE_MAP()

// CStatusBarEx message handlers

int CStatusBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCRibbonStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
