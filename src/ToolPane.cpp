// ToolPane.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ToolPane.h"

// CToolPane

IMPLEMENT_DYNAMIC(CToolPane, CDockablePane)

CToolPane::CToolPane()
{
}

CToolPane::~CToolPane()
{
}

BEGIN_MESSAGE_MAP(CToolPane, CDockablePane)
ON_WM_CREATE()
ON_WM_SIZE()
END_MESSAGE_MAP()

// CToolPane message handlers

int CToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
}
