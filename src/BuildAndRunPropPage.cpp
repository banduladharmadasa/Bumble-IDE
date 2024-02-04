// BuildAndRunPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "BuildAndRunPropPage.h"
#include "afxdialogex.h"

IMPLEMENT_DYNCREATE(CBuildAndRunPropPage, CMFCPropertyPage)

// CBuildAndRunPropPage dialog

CBuildAndRunPropPage::CBuildAndRunPropPage() : CMFCPropertyPage(CBuildAndRunPropPage::IDD)
{
}

CBuildAndRunPropPage::~CBuildAndRunPropPage()
{
}

void CBuildAndRunPropPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBuildAndRunPropPage, CMFCPropertyPage)
END_MESSAGE_MAP()
