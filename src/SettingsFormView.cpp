// SettingsFormView.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "SettingsFormView.h"


// CSettingsFormView

IMPLEMENT_DYNCREATE(CSettingsFormView, CFormView)

CSettingsFormView::CSettingsFormView() :CFormView(IDD_SETTINGSVIEW)
{

}

CSettingsFormView::~CSettingsFormView()
{
}

BEGIN_MESSAGE_MAP(CSettingsFormView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()




// CSettingsFormView diagnostics

#ifdef _DEBUG
void CSettingsFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSettingsFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif
#endif //_DEBUG


// CSettingsFormView message handlers

void CSettingsFormView::DoDataExchange(CDataExchange * pDX)
{
}

int CSettingsFormView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here



	

	return 0;
}


void CSettingsFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here


}





