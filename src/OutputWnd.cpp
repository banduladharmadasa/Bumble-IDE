
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd() noexcept
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, &COutputWnd::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &COutputWnd::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, &COutputWnd::OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &COutputWnd::OnUpdateEditClear)
	ON_COMMAND(32887, &COutputWnd::OnHide)
	ON_UPDATE_COMMAND_UI(32887, &COutputWnd::OnUpdateHide)
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_3D, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndMessage.Create(dwStyle | ES_WANTRETURN | ES_MULTILINE | ES_READONLY, rectDummy, &m_wndTabs, IDD_MESSAGE_WND)||
		!m_wndFindResults.Create(IDD_FIND_RESULT, &m_wndTabs))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	UpdateFonts();

	CString strTabName;
	


	strTabName = L"Console";
	m_wndTabs.AddTab(&m_wndMessage, strTabName, (UINT)0);
	strTabName = L"Find Results";
	m_wndTabs.AddTab(&m_wndFindResults, strTabName, (UINT)2);
	m_wndFindResults.SetParent(this);
	m_wndMessage.SetParent(this);

	

	

	return 0;
}

void COutputWnd::SetActiveTab(int index)
{
	m_wndTabs.SetActiveTab(index);
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AddMessage(const CString & message, AppMessageType type)
{
 m_wndMessage.AddMessage(message, type);

}

void COutputWnd::ClearMessages()
{
	m_wndMessage.SetWindowTextW(L"");
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}




void COutputWnd::UpdateFonts()
{
	m_wndMessage.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	m_wndFindResults.SetFont(&afxGlobalData.fontRegular);
}

CFindResultWnd* COutputWnd::GetFindResultWnd()
{
	return &m_wndFindResults;
}

HTREEITEM COutputWnd::AddItemToFindFileResults(const CString& str, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	return HTREEITEM();
}







void COutputWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUTPOPUP);
	CMenu* popup = menu.GetSubMenu(0);
	if (popup)
	{
		CContextMenuManager* manager = theApp.GetContextMenuManager();
		if (manager)
			//for CDialogEx:
			manager->ShowPopupMenu(popup->Detach(), point.x, point.y, this, TRUE, TRUE, FALSE);
		//for CDialog:
		//manager->ShowPopupMenu(popup->Detach(), p.x, p.y, this, FALSE, TRUE, FALSE);
	}
}


void COutputWnd::OnEditCopy()
{
	int index = this->m_wndTabs.GetActiveTab();
	if (index == 0) {
		
		m_wndMessage.Copy();
	}
	else if (index == 1) {
		m_wndFindResults.copyContent();
	}
}


void COutputWnd::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}


void COutputWnd::OnEditClear()
{
	int index = this->m_wndTabs.GetActiveTab();
	if (index == 0) {
		m_wndMessage.SetWindowTextW(L"");
	}
	else if (index == 1) {
		m_wndFindResults.Eval(L"var target = document.getElementById('find-results'); \n target.innerHTML='';");
	}
}


void COutputWnd::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}


void COutputWnd::OnHide()
{
	this->SetAutoHideMode(TRUE, CBRS_ALIGN_BOTTOM);
	
}


void COutputWnd::OnUpdateHide(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
