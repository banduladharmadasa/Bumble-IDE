// FileDialogEx.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "FileDialogEx.h"
#include "resource.h"

// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
	DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
	CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	//m_bVistaStyle = TRUE;

	//AddPushButton(ID_FILE_OPENNETWORKLOCATION, L"Open URL");

	//CNetAddressCtrl *ctrl = new CNetAddressCtrl();
	////AddEditBox(1, L"URL");

	//AddControlItem(0, IDC_NETADDRESS1, L"OK")
	//	;
}

CFileDialogEx::~CFileDialogEx()
{
}

BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	ON_COMMAND(1, OnOpenNetworkLocationClicked)
END_MESSAGE_MAP()



// CFileDialogEx message handlers


void CFileDialogEx::OnOpenNetworkLocationClicked()
{
	
}

void CFileDialogEx::OnButtonClicked(DWORD dwIDCtl)
{
	/*if (dwIDCtl == ID_FILE_OPENNETWORKLOCATION) {

		int i = 0;
	}*/
}


