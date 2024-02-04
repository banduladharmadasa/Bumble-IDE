// CInsertAtPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CInsertAtPropSheet.h"


// CInsertAtPropSheet

IMPLEMENT_DYNAMIC(CInsertAtPropSheet, CBumblePropertySheet)

CInsertAtPropSheet::CInsertAtPropSheet()
{
	SetLook(PropSheetLook_OneNoteTabs);
	
	AddPage(&m_textPropSheet);
	AddPage(&m_numberPropSheet);
	SetTitle(L"Multi Line Text Insertion");

	

}

CInsertAtPropSheet::~CInsertAtPropSheet()
{
}


BEGIN_MESSAGE_MAP(CInsertAtPropSheet, CBumblePropertySheet)
	ON_COMMAND(IDOK, &CInsertAtPropSheet::OnIdok)
	
END_MESSAGE_MAP()



// CInsertAtPropSheet message handlers




void CInsertAtPropSheet::OnIdok()
{
	GetActivePage()->OnOK();
}



