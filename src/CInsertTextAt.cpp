// CInsertTextAt.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CInsertTextAt.h"
#include "BumbleEdit.h"
#include "CodeEditor.h"

// CInsertTextAt

IMPLEMENT_DYNCREATE(CInsertTextAt, CBumblePropertyPage)

CInsertTextAt::CInsertTextAt() :CBumblePropertyPage(CInsertTextAt::IDD)
{

}

CInsertTextAt::~CInsertTextAt()
{
}


BEGIN_MESSAGE_MAP(CInsertTextAt, CBumblePropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CInsertTextAt message handlers




int CInsertTextAt::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBumblePropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	// TODO:  Add your specialized creation code here
	m_radBack.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_radBack.SetImage(IDB_RADIO_OFF);
	m_radBack.SetCheckedImage(IDB_RADIO_ON);

	m_radFront.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_radFront.SetImage(IDB_RADIO_OFF);
	m_radFront.SetCheckedImage(IDB_RADIO_ON);
	
	return 0;
}

void CInsertTextAt::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_RAD_INSERTATFRONT, m_radFront);
	DDX_Control(pDX, IDC_RAD_INSERTATBACK, m_radBack);
	DDX_Control(pDX, IDC_INSER_TEXT, m_edtText);
}


BOOL CInsertTextAt::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class


	m_radFront.SetCheck(TRUE);

	return CBumblePropertyPage::OnSetActive();
}


void CInsertTextAt::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CString text;
	m_edtText.GetWindowTextW(text);

	text.Replace(L"\\", L"\\\\");
	text.Replace(L"'", L"\\'");

	theApp.GetActiveCodeEditor()->InsertMultiLineText(text, m_radFront.GetCheck());


	CBumblePropertyPage::OnOK();
}
