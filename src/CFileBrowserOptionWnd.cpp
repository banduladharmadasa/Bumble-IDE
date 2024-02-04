// CFileBrowserOptionWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CFileBrowserOptionWnd.h"
#include "afxdialogex.h"
#include "DirTreeCtrl.h"
#include "resource.h"
#include "CFileBrowserDockPane.h"
#include "BumbleEdit.h"
// CFileBrowserOptionWnd dialog

IMPLEMENT_DYNAMIC(CFileBrowserOptionWnd, CDialogEx)

CFileBrowserOptionWnd::CFileBrowserOptionWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_BROWSER_OPT, pParent)
{

}

CFileBrowserOptionWnd::~CFileBrowserOptionWnd()
{
}

void CFileBrowserOptionWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOLDER_OPT_BTN, m_btnMenu);
	DDX_Control(pDX, IDC_FILE_FILTER_EDIT, m_edtFileFilter);
}


BEGIN_MESSAGE_MAP(CFileBrowserOptionWnd, CDialogEx)
	ON_BN_CLICKED(IDC_FOLDER_OPT_BTN, &CFileBrowserOptionWnd::OnBnClickedChkSetFolder)

	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFileBrowserOptionWnd message handlers


void CFileBrowserOptionWnd::OnBnClickedChkSetFolder()
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd = GetParent()->GetDlgItem(IDC_FILE_TREE);

	if (pWnd) {
		((CFileTreeWnd*)pWnd)->SetSelPathCurrent(m_btnMenu.m_nMenuResult, TRUE);
	}


	
}




BOOL CFileBrowserOptionWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ctxMenu.LoadMenu(IDR_PATH_FILTER);
	m_btnMenu.m_hMenu = m_ctxMenu.GetSubMenu(0)->GetSafeHmenu();
	m_btnMenu.SizeToContent();
	m_btnMenu.m_bOSMenu = FALSE;
	//m_btnMenu.ModifyStyle(BS_OWNERDRAW, 0, 0);
	m_btnMenu.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_btnMenu.EnableWindowsTheming(FALSE);
	m_btnMenu.SetFaceColor(RGB(0xff, 0xe0, 0x88));



	m_edtFileFilter.SetWindowTextW(L"*.*");
	m_edtFileFilter.EnableToolTips();
	
	//theApp.CreateToolTip(IDC_FILE_FILTER_EDIT, m_hWnd, L"Hello");

	
	m_ToolTip.Create(this);
	m_ToolTip.SetWindowTheme(L"");
	m_ToolTip.SetTipBkColor(RGB(0, 0, 0));
	m_ToolTip.AddTool(&m_edtFileFilter, L"Comma  separated filter list (e.g. *.cpp, *.h)");
	m_ToolTip.Activate(TRUE);
	
	




	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CFileBrowserOptionWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc, RGB(43, 43, 43));
}


void CFileBrowserOptionWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (m_btnMenu.GetSafeHwnd()) {
		CRect rc;
		m_btnMenu.GetClientRect(rc);
		if (rc.Width() < cx) {
			m_edtFileFilter.MoveWindow(0, 5, cx - rc.Width(), rc.Height());
			m_btnMenu.MoveWindow(cx - rc.Width(), 5, rc.Width(), rc.Height());
		}
		

	}
}


BOOL CFileBrowserOptionWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {

		if (GetFocus() == &m_edtFileFilter) {

			CFileBrowserDockPane* pParent = (CFileBrowserDockPane*)GetParent();
			if (pParent) {
				CString str;
				m_edtFileFilter.GetWindowText(str);
				pParent->SetFilters(str);
			}
		}


		return TRUE;
	}

	if (m_ToolTip.GetSafeHwnd())
		m_ToolTip.RelayEvent(pMsg);


	return CDialogEx::PreTranslateMessage(pMsg);
}

