// CBumbleMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CBumbleMessageBox.h"
#include "afxdialogex.h"


// CBumbleMessageBox dialog

IMPLEMENT_DYNAMIC(CBumbleMessageBox, CBumbleDialog)

CBumbleMessageBox::CBumbleMessageBox(const CString& reason, CWnd* pParent /*=nullptr*/)
	: CBumbleDialog(IDD_BUMBLE_MSG_BOX, pParent)
{
	m_strReason = reason;
}

CBumbleMessageBox::~CBumbleMessageBox()
{
}

void CBumbleMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_MSG_REASON, m_staticReason);
	DDX_Control(pDX, IDCANCEL, m_btnCanel);
}




BEGIN_MESSAGE_MAP(CBumbleMessageBox, CBumbleDialog)
END_MESSAGE_MAP()


// CBumbleMessageBox message handlers


BOOL CBumbleMessageBox::OnInitDialog()
{
	CBumbleDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_staticReason.SetWindowTextW(m_strReason);
	return TRUE;
}
