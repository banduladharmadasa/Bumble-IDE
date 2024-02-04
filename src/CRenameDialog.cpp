// CRenameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CRenameDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CRenameDialog dialog

IMPLEMENT_DYNAMIC(CRenameDialog, CBumbleDialog)

CRenameDialog::CRenameDialog(CWnd* pParent /*=nullptr*/)
	: CBumbleDialog(IDD_RENAME, pParent)
{

}

CRenameDialog::~CRenameDialog()
{
}

void CRenameDialog::DoDataExchange(CDataExchange* pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RENAME_EDIT, m_edtRename);
}


BEGIN_MESSAGE_MAP(CRenameDialog, CBumbleDialog)
END_MESSAGE_MAP()


// CRenameDialog message handlers


BOOL CRenameDialog::OnInitDialog()
{
	CBumbleDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_edtRename.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CRenameDialog::OnOK()
{
	CString str;
	m_edtRename.GetWindowTextW(str);

	if (str.IsEmpty()) {

		BumbleMessageBox(L"New file/folder name is empty!");
		return;
	}


	((CMainFrame*)theApp.GetMainWnd())->SetNewFileOrFolderName(str);

	CBumbleDialog::OnOK();
}
