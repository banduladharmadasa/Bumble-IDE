// CFileBrowserDockPane.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CFileBrowserDockPane.h"


// CFileBrowserDockPane

IMPLEMENT_DYNAMIC(CFileBrowserDockPane, CDockablePane)

CFileBrowserDockPane::CFileBrowserDockPane()
{
	m_strSelectedFilePath = L"";
}

CFileBrowserDockPane::~CFileBrowserDockPane()
{
}

CFileBrowserOptionWnd* CFileBrowserDockPane::GetOptWnd()
{
	return &m_wndOptions;
}

void CFileBrowserDockPane::SetFilters(const CString& filters)
{
	m_treeCtrl.SetFilters(filters);
}

CString CFileBrowserDockPane::GetSelectedPath()
{
	
	return m_strSelectedFilePath;
}

CEdit* CFileBrowserDockPane::PrepareItemForRename()
{
	m_treeCtrl.ModifyStyle(0, TVS_EDITLABELS);
	m_treeCtrl.SelectItem(m_hSelectedItem);
	m_treeCtrl.SetFocus();
	CEdit* pmyEdit = m_treeCtrl.EditLabel(m_hSelectedItem);


	

	return pmyEdit;
}



void CFileBrowserDockPane::AddNewFileItem(const CString& name)
{
	HTREEITEM item = m_treeCtrl.AddItem(m_hSelectedItem, name);
	m_treeCtrl.Expand(m_hSelectedItem, TVE_EXPAND);
	m_treeCtrl.Expand(m_hSelectedItem, TVE_COLLAPSE);
	m_treeCtrl.Expand(m_hSelectedItem, TVE_EXPAND);
	
}

void CFileBrowserDockPane::DeleteItemFromTree()
{
	m_treeCtrl.DeleteItem(m_hSelectedItem);
}


BEGIN_MESSAGE_MAP(CFileBrowserDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	

	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FILEBROWSERCTX_OPEN, &CFileBrowserDockPane::OnFileOpen)
END_MESSAGE_MAP()



// CFileBrowserDockPane message handlers




int CFileBrowserDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	wchar_t system_buffer[MAX_PATH];
	system_buffer[0] = 0;


	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_treeCtrl.Create(WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_EDITLABELS | TVS_HASBUTTONS | TVS_TRACKSELECT, CRect(0,0, 200, 200), this, IDC_FILE_TREE)) {

		return -1;
	}

	m_treeCtrl.DisplayTree(NULL /*Display all*/, TRUE /* TRUE = Display Files*/);

	m_treeCtrl.ShowWindow(SW_SHOW);

	if (FAILED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, system_buffer))) {

		return 0;
	}

	

	CString str(system_buffer);

	str.Append(L"\\Bumble Projects");
	

	//
	if (CreateDirectory(str, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		//
		//m_treeCtrl.DisplayTree(str, TRUE);
		//m_treeCtrl.SetSelPath(str);
	}

	

	if (!m_wndOptions.Create(IDD_FILE_BROWSER_OPT, this)) {

		return 0;
	}

	m_wndOptions.ShowWindow(SW_SHOW);

	return 0;
}


void CFileBrowserDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_treeCtrl.GetSafeHwnd()) {
		m_treeCtrl.MoveWindow(0, 40, cx, cy - 40);
	}

	if (m_wndOptions.GetSafeHwnd()) {
		m_wndOptions.SetWindowPos(nullptr, 0, 0, cx, 40, SWP_SHOWWINDOW);
	}


}


void CFileBrowserDockPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	//Notify user that the color is in the clipboard
	
	CPoint ptMouse;
	POINT sMouse;
	UINT uFlags;
	::GetCursorPos(&sMouse);
	ptMouse =  sMouse;
	m_treeCtrl.ScreenToClient(&ptMouse);
	m_hSelectedItem = m_treeCtrl.HitTest(ptMouse, &uFlags);

	if (m_hSelectedItem == NULL)
		return;



	m_strSelectedFilePath = m_treeCtrl.GetFullPath(m_hSelectedItem);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_FILE_BROWSER_CTX));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (pPopup)
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
		pPopupMenu->SetAnimationType(CMFCPopupMenu::SLIDE);
		pPopupMenu->SetForceShadow(true);
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach());
	}
	
	m_treeCtrl.SelectItem(m_hSelectedItem);
}


void CFileBrowserDockPane::OnFileOpen()
{
	// TODO: Add your command handler code here
}
