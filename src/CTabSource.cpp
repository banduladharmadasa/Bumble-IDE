// CTabSource.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CTabSource.h"
#include "afxdialogex.h"

// CTabSource dialog

IMPLEMENT_DYNAMIC(CTabSource, CBumbleDialog)

CTabSource::CTabSource(CWnd *pParent /*=nullptr*/)
	: CBumbleDialog(CTabSource::IDD, pParent)
{
}

CTabSource::~CTabSource()
{
}

void CTabSource::AddFile(const CString &file)
{
	m_treeCtrl.AddFile(file);
}

void CTabSource::DoDataExchange(CDataExchange *pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTabSource, CBumbleDialog)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_CONTEXTMENU()
ON_COMMAND(IDOK, &CTabSource::OnIdok)
ON_COMMAND(IDOK2, &CTabSource::OnIdok2)
ON_COMMAND(IDCANCEL, &CTabSource::OnIdcancel)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CTabSource message handlers

int CTabSource::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBumbleDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_treeCtrl.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER |
							   TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES |
							   TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS | TVS_EDITLABELS,
						   CRect(10, 10, 300, 100), this, IDC_PROJECT_TREE))
	{

		return -1;
	}

	return 0;
}

void CTabSource::OnSize(UINT nType, int cx, int cy)
{
	CBumbleDialog::OnSize(nType, cx, cy);

	if (m_treeCtrl.GetSafeHwnd())
	{
		m_treeCtrl.MoveWindow(0, 0, 400, cy);
	}
}

void CTabSource::OnContextMenu(CWnd *pWnd, CPoint point)
{

	CPoint ptMouse;
	POINT sMouse;
	UINT uFlags;
	::GetCursorPos(&sMouse);
	ptMouse = sMouse;
	ScreenToClient(&ptMouse);
	HTREEITEM m_hSelectedItem = m_treeCtrl.HitTest(ptMouse, &uFlags);

	if (m_hSelectedItem == NULL)
		return;

	CString m_strSelectedFilePath = L"";

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_PROJECT_SOURCE_MENU));

	CMenu *pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (pPopup)
	{
		this->SetForegroundWindow();
		CMFCPopupMenu *pPopupMenu = new CMFCPopupMenu;
		pPopupMenu->SetAnimationType(CMFCPopupMenu::SLIDE);
		pPopupMenu->SetForceShadow(true);
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach());
	}

	m_treeCtrl.SelectItem(m_hSelectedItem);
}

void CTabSource::OnIdok()
{
	// TODO: Add your command handler code here
}

void CTabSource::OnIdok2()
{
	// TODO: Add your command handler code here
}

void CTabSource::OnIdcancel()
{
	// TODO: Add your command handler code here
}

void CTabSource::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CBumbleDialog::OnLButtonDown(nFlags, point);
}
