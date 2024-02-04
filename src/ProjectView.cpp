// ProjectView.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ProjectView.h"


// CProjectView

IMPLEMENT_DYNCREATE(CProjectView, CFormView)

CProjectView::CProjectView()
	: CFormView(IDD_PROJECT_VIEW)
{
	m_pTabCtrl = nullptr;
}

CProjectView::~CProjectView()
{
	delete m_pTabCtrl;
	m_pTabCtrl = nullptr;
}

void CProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProjectView, CFormView)
	ON_WM_SIZE()
	
	//ON_COMMAND(ID_PROJECTSOURCEFILES_ADDNEWITEM, &CProjectView::OnAddNewItem)
	//ON_COMMAND(ID_PROJECTSOURCEFILES_OPENITEM, &CProjectView::OnOpenItem)
END_MESSAGE_MAP()


// CProjectView diagnostics

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProjectView message handlers


void CProjectView::OnTabChanged()
{
	// TODO: Add your control notification handler code here
	CRect rcTab;
	CRect rcItem;
	GetClientRect(&rcTab);
	m_pTabCtrl->GetItemRect(0, &rcItem);

	int nIndex = m_pTabCtrl->GetCurSel();


	for (int i = 0; i < m_tabPages.size(); i++) {


		if (i == nIndex) {
			m_tabPages[i]->SetWindowPos(NULL, 5, rcItem.bottom + 5, rcTab.Width() - 20, rcTab.Height() - rcItem.bottom - 40, SWP_NOZORDER | SWP_SHOWWINDOW);
			m_tabPages[i]->SetFocus();
		}
		else {
			m_tabPages[i]->ShowWindow(SW_HIDE);
		}
	}
	
}


void CProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle(L"Project Settings");

	m_pTabCtrl = new CProjectTabCtrl(this);
	if (!m_pTabCtrl->Create(TCS_RAGGEDRIGHT | WS_CHILD | WS_VISIBLE, CRect(0,0, 400, 400), this, IDC_PROJECT_TAB_CTRL)) {

		return;
	}

	if (!m_tabSource.Create(IDD_TAB_SOURCE, m_pTabCtrl)) {

		return;
	}



	if (!m_tabBuild.Create(IDD_TAB_SOURCE, m_pTabCtrl)) {

		return;
	}


	

	m_tabPages.push_back(&m_tabSource);
	m_tabPages.push_back(&m_tabBuild);

	RECT r;
	TCITEM tci;

	GetClientRect(&r);

	tci.mask = TCIF_TEXT;
	tci.iImage = -1;

	tci.pszText = L"Source";
	m_pTabCtrl->InsertItem(0, &tci);

	tci.pszText = L"Build & Run";
	m_pTabCtrl->InsertItem(1, &tci);

	OnTabChanged();
}


void CProjectView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);


	if (m_pTabCtrl->GetSafeHwnd() == NULL)
		return; 

	RECT rect;

	
	GetClientRect(&rect);

	m_pTabCtrl->AdjustRect(FALSE, &rect);

	
	m_pTabCtrl->MoveWindow(&rect, TRUE);

	OnTabChanged();
}





void CProjectView::OnAddNewItem()
{
	// TODO: Add your command handler code here
}


void CProjectView::OnOpenItem()
{
	CFileDialog dlgOpenFile(TRUE);
	// add OFN_ALLOWMULTISELECT flag
	dlgOpenFile.GetOFN().Flags |= OFN_ALLOWMULTISELECT;

	try
	{
		// set a buffer to keep at least 100 full path and file names
		const int nBufferSize = 100 * (MAX_PATH + 1) + 1;
		CString strBuffer;
		LPTSTR pBuffer = strBuffer.GetBufferSetLength(nBufferSize);
		dlgOpenFile.GetOFN().lpstrFile = pBuffer;
		dlgOpenFile.GetOFN().nMaxFile = nBufferSize;
		dlgOpenFile.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_SOURCE_FILE_DLG);
		// show modal Open File dialog
		if (IDOK == dlgOpenFile.DoModal())
		{
			// get selected files
			POSITION pos = dlgOpenFile.GetStartPosition();
			CString strLst;
			while (NULL != pos)
			{
				m_tabSource.AddFile(dlgOpenFile.GetNextPathName(pos));
			}
		}
		// release buffer
		strBuffer.ReleaseBuffer();
	}
	catch (CException * e)
	{
		e->ReportError();
		e->Delete();
	}


	CWnd* pParent = GetParent();

	if (!pParent) {
		return;
	}


	
	
	
		
	
}
