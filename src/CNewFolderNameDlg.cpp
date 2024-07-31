// CNewFolderNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CNewFolderNameDlg.h"
#include "MainFrm.h"
#include "BumbleEdit.h"

// CNewFolderNameDlg dialog

IMPLEMENT_DYNAMIC(CNewFolderNameDlg, CBumbleDialog)

CNewFolderNameDlg::CNewFolderNameDlg(CWnd *pParent /*=nullptr*/)
	: CBumbleDialog(CNewFolderNameDlg::IDD, pParent)
{
}

CNewFolderNameDlg::~CNewFolderNameDlg()
{
}

void CNewFolderNameDlg::DoDataExchange(CDataExchange *pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CNewFolderNameDlg, CBumbleDialog)
ON_BN_CLICKED(IDOK, &CNewFolderNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CNewFolderNameDlg message handlers

BOOL CNewFolderNameDlg::OnInitDialog()
{
	CBumbleDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	GetDlgItem(IDC_NEW_FOLDER_NAME)->SetWindowTextW(L"New Folder");
	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewFolderNameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CString str;
	GetDlgItem(IDC_NEW_FOLDER_NAME)->GetWindowTextW(str);

	if (str.IsEmpty())
	{

		BumbleMessageBox(L"New folder name is empty!");
		return;
	}

	((CMainFrame *)theApp.GetMainWnd())->SetNewFileOrFolderName(str);
	CBumbleDialog::OnOK();
}
