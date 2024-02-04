// CInsertNumberAt.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CInsertAtNumber.h"
#include "BumbleEdit.h"
#include "CodeEditor.h"

// CInsertNumberAt

IMPLEMENT_DYNCREATE(CInsertNumberAt, CBumblePropertyPage)

CInsertNumberAt::CInsertNumberAt()
	:CBumblePropertyPage(CInsertNumberAt::IDD)
{

}

CInsertNumberAt::~CInsertNumberAt()
{
}


BEGIN_MESSAGE_MAP(CInsertNumberAt, CBumblePropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CInsertNumberAt message handlers




int CInsertNumberAt::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBumblePropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CInsertNumberAt::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_START_NUMBER, m_edtInitNumber);
	DDX_Control(pDX, IDC_INCREMENT_BY, m_edtInc);
	DDX_Control(pDX, IDC_LEADING_ZEROS, m_edtLeadingZeros);
	
}


BOOL CInsertNumberAt::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBumblePropertyPage::OnSetActive();
}


void CInsertNumberAt::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	int start;
	int inc;
	int pad;
	CString fmt = L"";

	CString temp;
	m_edtInitNumber.GetWindowText(temp);
	start = _wtoi(temp);

	m_edtInc.GetWindowText(temp);
	inc = _wtoi(temp);

	m_edtLeadingZeros.GetWindowTextW(temp);
	pad = _wtoi(temp);

	

	theApp.GetActiveCodeEditor()->InsertMultiLineNumber(start, inc, pad, fmt);

	CBumblePropertyPage::OnOK();
}
