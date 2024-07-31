// CGotoLine.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CGotoLine.h"
#include "BumbleEdit.h"
#include "CodeEditor.h"

// CGotoLine dialog

IMPLEMENT_DYNAMIC(CGotoLine, CBumbleDialog)

CGotoLine::CGotoLine(CWnd *pParent /*=nullptr*/)
	: CBumbleDialog(CGotoLine::IDD, pParent)
{
}

CGotoLine::~CGotoLine()
{
}

void CGotoLine::DoDataExchange(CDataExchange *pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GOTOLINE_LABEL, m_label);
	DDX_Control(pDX, IDC_GOTO_LINE_EDIT, m_edtGototLine);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_GOTOLINE_COL_EDIT, m_edtCol);
}

BEGIN_MESSAGE_MAP(CGotoLine, CBumbleDialog)
ON_BN_CLICKED(IDOK, &CGotoLine::OnBnClickedOk)
END_MESSAGE_MAP()

// CGotoLine message handlers

void CGotoLine::OnBnClickedOk()
{
	CString temp;
	m_edtGototLine.GetWindowTextW(temp);

	if (temp.IsEmpty())
	{
		return;
	}

	int line = _wtoi(temp);

	m_edtCol.GetWindowTextW(temp);

	if (temp.IsEmpty())
	{
		temp = L"1";
	}
	int col = _wtoi(temp);

	CCodeEditor *pCodeEditor = theApp.GetActiveCodeEditor();
	if (pCodeEditor)
	{
		pCodeEditor->OnGoToLine(line, col);
	}

	CDialog::OnOK();
}

BOOL CGotoLine::OnInitDialog()
{
	CBumbleDialog::OnInitDialog();

	CCodeEditor *pCodeEditor = theApp.GetActiveCodeEditor();

	if (pCodeEditor)
	{

		CString tmp = pCodeEditor->GetGoToLineInfo();
		m_label.SetWindowTextW(tmp);
	}

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}
