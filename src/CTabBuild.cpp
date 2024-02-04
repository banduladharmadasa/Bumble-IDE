// CTabBuild.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CTabBuild.h"
#include "afxdialogex.h"


// CTabBuild dialog

IMPLEMENT_DYNAMIC(CTabBuild, CBumbleDialog)

CTabBuild::CTabBuild(CWnd* pParent /*=nullptr*/)
	: CBumbleDialog(IDD_TAB_BUILD, pParent)
{

}

CTabBuild::~CTabBuild()
{
}

void CTabBuild::DoDataExchange(CDataExchange* pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabBuild, CBumbleDialog)
END_MESSAGE_MAP()


// CTabBuild message handlers
