
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BumbleEdit.h"

#include "MainFrm.h"
#include "VisualManagerEx.h"
#include <memory>
#include "ZoomSlider.h"
#include "ChildFrm.h"
#include "BumbleEditView.h"
#include "BumbleEditDoc.h"
#include "MFCPopupMenuEx.h"
#include "CNewFolderNameDlg.h"
#include "CRenameDialog.h"

TCHAR FOLDER_DELETE_TEXT[] = L"Do you really need to delete the folder, %s, and it's contents?";
TCHAR FILE_DELETE_TEXT[] = L"Do you really need to delete the file, %s?";
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int BORDERWIDTH = 7; // width of extra non-client area

#define ID_TOOL_FIRST 0x7C00
#define ID_TOOL_LAST 0x7FFF

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
ON_WM_CREATE()
ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
ON_UPDATE_COMMAND_UI(ID_SYSTEM_MINIMIZE, &CMainFrame::OnUpdateSystemMinimize)
ON_UPDATE_COMMAND_UI(ID_SYSTEM_RESTORE, &CMainFrame::OnUpdateSystemRestore)
ON_COMMAND(ID_STATUSBAR_ZOOMSLIDER, OnZoomSlider)
ON_COMMAND(ID_STATUSBAR_ZOOM, OnZoom)
ON_UPDATE_COMMAND_UI(ID_STATUSBAR_ZOOM, OnZoomSliderUpdate)
ON_UPDATE_COMMAND_UI(ID_CURSORPOS, OnUpdateCursorPos)

ON_WM_ACTIVATE()

ON_COMMAND(ID_OPTIONS_OPEN, &CMainFrame::OnOptionsOpen)
ON_WM_TIMER()
ON_WM_DESTROY()
ON_WM_NCPAINT()
ON_WM_NCACTIVATE()
ON_WM_NCCALCSIZE()
ON_COMMAND(ID_OTHERWINDOWS_OUTPUTWINDOW, &CMainFrame::OnOtherwindowsOutputwindow)
ON_COMMAND(ID_FILE_CLOSEALLBUTTHIS, &CMainFrame::OnFileCloseAllButThis)
ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEALLBUTTHIS, &CMainFrame::OnUpdateFileCloseAllButThis)
ON_COMMAND(ID_FILEBROWSERCTX_OPEN, &CMainFrame::OnFileBrowserCtxOpen)
ON_COMMAND(ID_FILEBROWSERCTX_OPENWITHDEFAULTAPPLICATION, &CMainFrame::OnFileBrowserCtxOpenWithDefaultApplication)
ON_COMMAND(ID_FILEBROWSERCTX_NEWFOLDER, &CMainFrame::OnFileBrowserCtxNewfolder)
ON_COMMAND(ID_FILEBROWSERCTX_PRINT, &CMainFrame::OnFileBrowserCtxPrint)
ON_COMMAND(ID_FILEBROWSERCTX_RENAME, &CMainFrame::OnFileBrowserCtxRename)
ON_COMMAND(ID_FILEBROWSERCTX_DELETE, &CMainFrame::OnFileBrowserCtxDelete)
ON_COMMAND(ID_FILEBROWSERCTX_NEWFILE, &CMainFrame::OnFileBrowserCtxNewFile)
ON_COMMAND_RANGE(ID_TOOL_FIRST, ID_TOOL_LAST, OnTool)

ON_REGISTERED_MESSAGE(AFX_WM_ON_GET_TAB_TOOLTIP, &CMainFrame::OnTabToolTipNeeded)
ON_COMMAND(ID_FILE_NEWPROJECT, &CMainFrame::OnFileNewproject)
ON_COMMAND(ID_OTHERWINDOWS_FILEBROWSER, &CMainFrame::OnOtherwindowsFilebrowser)
ON_COMMAND(ID_WINDOW_NEW, &CMainFrame::OnWindowNew)
ON_COMMAND(ID_EDIT_CAPTURESCREEN, &CMainFrame::OnEditCapturescreen)
ON_COMMAND(ID_UITHREADTOCAPTURE_SENDSTOPMESSAGE, &CMainFrame::OnUIThreadToCaptureSendStopMessage)
ON_COMMAND(ID_TOOLS_TUTORIALMODE, &CMainFrame::OnToolsTutorialMode)
ON_UPDATE_COMMAND_UI(ID_TOOLS_TUTORIALMODE, &CMainFrame::OnUpdateToolsTutorialMode)
ON_WM_COPYDATA()

ON_COMMAND(ID_RUN_CONFIGURE, &CMainFrame::OnRunConfigure)
ON_COMMAND(ID_TOOLS_FUNCTIONS, &CMainFrame::OnToolsFunctions)
END_MESSAGE_MAP()

LRESULT CMainFrame::OnTabToolTipNeeded(WPARAM wParam, LPARAM lParam)
{
	CMFCTabToolTipInfo *pInfo = (CMFCTabToolTipInfo *)lParam;
	ASSERT(pInfo != NULL);

	if (!pInfo)
	{
		return 0;
	}
	ASSERT_VALID(pInfo->m_pTabWnd);
	if (!pInfo->m_pTabWnd->IsMDITab())
	{
		return 0;
	}
	CBumbleEditView *pcView = GetActiveView();
	if (pcView->GetSafeHwnd())
	{
		pInfo->m_strText = pcView->GetDocument()->GetPathName();
	}
	/// <summary>
	///
	/// </summary>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	return 0;
}

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
	m_timer = 0;
	m_pWndCursorPos = nullptr;
	pSlider = nullptr;
	m_pScreenCaptureThread = nullptr;
	m_bTutorialMode = FALSE;
	m_nColours = 0;
	// Allow user-defined toolbars operations:
	// InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	EnableWindowsDialog(ID_WINDOW_MANAGER, _T("&Windows..."), TRUE);
	// EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."), ID_VIEW_TOOLBARS, FALSE, TRUE);
	// EnableFullScreenMode(ID_VIEW_FULLSCREEN);
}

BOOL CMainFrame::WasMaximized()
{
	return m_bWasMaximized;
}

void CMainFrame::CloseMenu()
{
	m_wndMenuBar.SendMessage(WM_CANCELMODE);
}

/// <summary>
/// Update the color edit box
/// </summary>

void CMainFrame::OnColorPick(COLORREF cr)
{
	/*if(m_wndToolBar.GetSafeHwnd())
	m_wndToolBar.UpdateButton(ID_COLOREDITBOX);*/

	m_wndToolBar.Invalidate();
}

void CMainFrame::OnDrawMenuLogo(CDC *pDC, CMFCPopupMenu *pMenu, const CRect &rectLogo)
{
	if (pMenu->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuEx)))
	{
		COLORREF color = CMFCColorMenuButton::GetColorByCmdID(ID_COLORPICKER);
		pDC->FillSolidRect(rectLogo, color);
	}
}

void CMainFrame::ReconfigAllEditors()
{
	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate)
	{
		CDocTemplate *pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);

			CBumbleEditDoc *_pDoc = (CBumbleEditDoc *)pDoc;

			POSITION posView = pDoc->GetFirstViewPosition();
			((CBumbleEditView *)pDoc->GetNextView(posView))->SendMessage(WM_COMMAND, RECONFIG_ALL_EDITORS, (LPARAM)0);
		}
	}
}

void CMainFrame::CloseAllFiles(BOOL bKeepCurrent)
{
	const CObList *tabList = &m_wndClientArea.GetMDITabGroups();
	CMFCTabCtrl *pTabWnd = (CMFCTabCtrl *)tabList->GetHead();

	if (pTabWnd != NULL)
	{
		int iActiveTab = bKeepCurrent ? pTabWnd->GetActiveTab() : -1;

		ASSERT_VALID(pTabWnd);

		for (int i = pTabWnd->GetTabsNum() - 1; i >= 0; i--)
		{
			CMDIChildWndEx *pNextWnd = DYNAMIC_DOWNCAST(CMDIChildWndEx, pTabWnd->GetTabWnd(i));
			if ((pNextWnd != NULL) && (iActiveTab != i))
			{
				ASSERT_VALID(pNextWnd);
				pNextWnd->SendMessage(WM_CLOSE, (WPARAM)0, (LPARAM)0);
			}
		}
	}
}

void CMainFrame::SetNewFileOrFolderName(const CString &name)
{
	m_strNewName = name;
}

CMainFrame::~CMainFrame()
{
	DestroyCaptureThread();
}

BOOL CMainFrame::AddStatusBar()
{
	if (!m_wndStatusBar.CreateEx(this))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE; // fail to create
	}

	m_pWndCursorPos = new CMFCRibbonStatusBarPane(ID_CURSORPOS, _T(""));
	m_pWndCursorPos->SetAlmostLargeText(L"999999999999999999");
	m_pWndCursorPos->SetTextAlign(TA_CENTER);

	m_wndStatusBar.AddElement(m_pWndCursorPos, _T("Formatted Page Number"));

	m_wndStatusBar.AddSeparator();

	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_ZOOM, _T("100%"), FALSE, NULL, _T("1000%")), _T("Zoom"));

	pSlider = new CZoomSlider(ID_STATUSBAR_ZOOMSLIDER);
	pSlider->SetZoomButtons();
	pSlider->SetRange(0, 200);
	pSlider->SetPos(100);

	m_wndStatusBar.AddExtendedElement(pSlider, _T("Zoom Slider"));

	return TRUE;
}

void CMainFrame::OnZoom()
{
	// MessageBox(_T("Zoom dialog box..."));
}

void CMainFrame::OnZoomSlider()
{
	CMFCRibbonSlider *pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndStatusBar.FindElement(ID_STATUSBAR_ZOOMSLIDER));
	ASSERT_VALID(pSlider);

	int nPos = pSlider->GetPos();

	CMFCRibbonStatusBarPane *pZoom = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_ZOOM));
	ASSERT_VALID(pZoom);

	CString strZoom;
	strZoom.Format(_T("%d%%"), nPos);

	pZoom->SetText(strZoom);
	pZoom->Redraw();

	CView *view = GetActiveView();
	if (view->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
	{
		((CBumbleEditView *)GetActiveView())->SetZoomLevel(nPos);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// theApp.LoadPlugins();

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_VS2005;
	mdiTabParams.m_bActiveTabCloseButton = TRUE;
	mdiTabParams.m_bTabIcons = FALSE;
	mdiTabParams.m_bAutoColor = FALSE;
	mdiTabParams.m_bTabCustomTooltips = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{

		return -1;
	}

	if (!AddToolBar())
	{
		TRACE0("Failed to create or initialize ribbon bar\n");

		return -1;
	}

	m_wndMenuBar.SetPaneStyle(CBRS_ORIENT_VERT | CBRS_SIZE_DYNAMIC);

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockPane(&m_wndMenuBar);

	if (!AddStatusBar())
		return -1;

	if (!m_wndOptionDockPane.Create(L"Options", this, CRect(0, 0, 300, 500), TRUE, IDD_OPTIONGRID, WS_CHILD | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{

		return -1;
	}

	m_wndOptionDockPane.SetPaneStyle(CBRS_ORIENT_VERT | CBRS_SIZE_DYNAMIC);
	m_wndOptionDockPane.EnableDocking(CBRS_ALIGN_RIGHT);

	DockPane(&m_wndOptionDockPane, AFX_IDW_DOCKBAR_RIGHT);
	m_wndOptionDockPane.ShowWindow(SW_SHOW);
	m_wndOptionDockPane.ShowPane(TRUE, FALSE, TRUE);

	CDockingManager::SetDockingMode(DT_SMART);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CVisualManagerEx /*CMFCVisualManagerOffice2007*/));
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);
	ModifyStyle(0, FWS_PREFIXTITLE);

	// Output Window
	if (!m_wndOutputDockPane.Create(L"Output Window", this, CRect(0, 0, 100, 100), TRUE, IDD_OUTPUT_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}
	m_wndOutputDockPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutputDockPane);

	// File Browser Dockable pane
	if (!m_wndFileBrowserDockPane.Create(L"File Browser", this, CRect(0, 0, 300, 500), TRUE, IDD_FILE_BROWSER_DOCK_PANE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File Browser window\n");
		return -1;
	}

	m_wndFileBrowserDockPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileBrowserDockPane);

	// Tutorial Text Pane
	// if (!m_wndCmdHistory.Create(_T("Command History"), this, CRect(),
	//	TRUE,
	//	IDD_TUTORIAL_WND,
	//	WS_CHILD | WS_VISIBLE,
	//	AFX_CBRS_REGULAR_TABS,
	//	AFX_CBRS_CLOSE | AFX_CBRS_FLOAT | AFX_CBRS_AUTO_ROLLUP))
	//{
	//	TRACE0("Failed to create infor bar bar\n");
	//	return -1;      // fail to create
	// }
	//
	// m_wndCmdHistory.EnableDocking(0);
	// m_wndCmdHistory.FloatPane(CRect(0, 0, 400, 300));

	RecalcLayout();

	return 0;
}

CBumbleEditView *CMainFrame::GetActiveView() const
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();

	if (!pFrame->GetSafeHwnd())
	{
		return nullptr;
	}

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();

	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

	// Get the active view attached to the active MDI child window.
	if (!pChild)
	{
		return nullptr;
	}

	CView *pView = pChild->GetActiveView();
	if (!pView)
	{

		return nullptr;
	}

	if (pView->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
	{
		return (CBumbleEditView *)pView;
	}

	return nullptr;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext &dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

void ScreenCaptureProc(LPVOID pParam)
{
}

void CMainFrame::DestroyCaptureThread()
{
	OnUIThreadToCaptureSendStopMessage();
}
void CMainFrame::StartScreenCapture(ScreenCaptureType captureType)
{

	if (!m_pScreenCaptureThread)
	{

		m_pScreenCaptureThread = (CWinThread *)AfxBeginThread(RUNTIME_CLASS(CScreenCaptureThread));
	}

	CRect rc;
	switch (captureType)
	{
	case ScreenCaptureType::Desktop:
	{
		int height = GetSystemMetrics(SM_CYSCREEN);
		int width = GetSystemMetrics(SM_CXSCREEN);

		rc = CRect(0, 0, width, height);
	}
	break;
	case ScreenCaptureType::Application:
	{
		GetWindowRect(rc);
	}
	break;
	case ScreenCaptureType::Document:
	{
		CBumbleEditView *pView = GetActiveView();
		if (!pView)
		{
			return;
		}

		pView->GetClientRect(rc);
		pView->ClientToScreen(rc);
	}
	break;
	default:
		break;
	}
}

void CMainFrame::StopScreenCapture()
{
	if (m_pScreenCaptureThread)
	{
		m_pScreenCaptureThread->PostThreadMessageW(ID_UITHREADTOCAPTURE_SENDSTOPMESSAGE, 0, 0);
	}
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

BOOL CMainFrame::AddToolBar()
{
	// The this pointer points to a CMainFrame class which extends the CFrameWndEx class.
	/*if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, 0, -1, TRUE))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}*/

	DWORD dwCtrlStyle = TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | CBRS_SIZE_DYNAMIC /*| TBSTYLE_TRANSPARENT*/;
	DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_ALIGN_TOP | CBRS_FLOATING;
	CMFCToolBarInfo tbi, tbiA;
	const CRect r1(1, 1, 1, 1);

	if (!m_wndToolBar.CreateEx(this, dwCtrlStyle, dwStyle, r1, IDR_MAINFRAME))
	{
		return -1; // fail to create
	}

	if (!m_wndToolBar.LoadToolBar(IDR_TOOLBAR2, IDB_TOOLBAR, 0, TRUE, IDB_TOOLBAR_DISABLED, 0U, IDB_TOOLBAR))
	{
		return -1;
	}

	CSize szImage, szButton;
	szImage = m_wndToolBar.GetImageSize();
	szButton.cx = szImage.cx + 6; // button size must be at least image size + 6
	szButton.cy = szImage.cy + 6;
	m_wndToolBar.SetMenuSizes(szButton, szImage);
	m_wndToolBar.EnableToolTips(FALSE);

	dwStyle = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | dwStyle);
	m_wndToolBar.AdjustLayout();
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);

	m_wndToolBar.AdjustSizeImmediate();

	AddColorButton();
	AddFunctionWindow();

	m_timer = SetTimer(1, 500, 0);

	return TRUE;
}

/// <summary>
/// Add the color button to the tool bar
/// </summary>
void CMainFrame::AddColorButton()
{
	CMFCColorMenuButton *pColorButton = CreateColorButton();
	pColorButton->SetMessageWnd(this);
	m_wndToolBar.ReplaceButton(ID_COLORPICKER, *pColorButton, TRUE);
	delete pColorButton;
}

void CMainFrame::AddFunctionWindow()
{

	CMFCToolBarMenuButton *pBorderTypeButton = CreateFunctionButton();
	m_wndToolBar.ReplaceButton(ID_TOOLS_FUNCTIONS, *pBorderTypeButton, TRUE);
	delete pBorderTypeButton;
}

CMFCColorMenuButton *CMainFrame::CreateColorButton()
{
	if (m_palColorPicker.GetSafeHandle() == NULL)
	{
		m_nColours = sizeof(crColours) / sizeof(ColourTableEntry);
		ASSERT(m_nColours <= MAX_COLOURS);
		if (m_nColours > MAX_COLOURS)
			m_nColours = MAX_COLOURS;

		// Create the palette
		struct
		{
			LOGPALETTE LogPalette;
			PALETTEENTRY PalEntry[MAX_COLOURS];
		} pal;

		LOGPALETTE *pLogPalette = (LOGPALETTE *)&pal;
		pLogPalette->palVersion = 0x300;
		pLogPalette->palNumEntries = (WORD)m_nColours;

		for (int i = 0; i < m_nColours; i++)
		{
			pLogPalette->palPalEntry[i].peRed = GetRValue(crColours[i].crColour);
			pLogPalette->palPalEntry[i].peGreen = GetGValue(crColours[i].crColour);
			pLogPalette->palPalEntry[i].peBlue = GetBValue(crColours[i].crColour);
			pLogPalette->palPalEntry[i].peFlags = 0;
		}

		m_palColorPicker.CreatePalette(pLogPalette);
	}

	CMFCColorMenuButton *pColorButton = new CMFCColorMenuButton(ID_COLORPICKER, L"", &m_palColorPicker);
	pColorButton->SetImage(20);
	pColorButton->m_bText = FALSE;
	pColorButton->m_bImage = TRUE;

	pColorButton->EnableOtherButton(_T("More Colors..."));
	// pColorButton->EnableDocumentColors(_T("Document's Colors"));
	pColorButton->EnableTearOff(ID_COLORTEAROFF, 5, 2);
	pColorButton->SetColumnsNumber(10);
	pColorButton->SetColor(RGB(0, 0, 255));

	// Initialize color names:
	for (int i = 0; i < m_nColours; i++)
	{
		CMFCColorMenuButton::SetColorName(crColours[i].crColour, crColours[i].szName);
	}

	return pColorButton;
}

CMFCToolBarMenuButton *CMainFrame::CreateFunctionButton()
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_TYPE_BUMBLEEDIT));

	CMFCToolBarMenuButton *pBorderType = NULL;
	CMenu *pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	if (pPopup != NULL)
	{
		pBorderType = new CMFCToolBarMenuButton(ID_TOOLS_FUNCTIONS, pPopup->GetSafeHmenu(), 21, 0, TRUE);
		pBorderType->SetMenuPaletteMode(0, 0);
		// pBorderType->SetTearOff(ID_FUNCTION_TEAROFF);
	}

	return pBorderType;
}

void CMainFrame::OnZoomSliderUpdate(CCmdUI *pCmdUI)
{
	CView *view = GetActiveView();
	if (!view->GetSafeHwnd())
		return;

	if (!view->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
		return;

	pCmdUI->Enable();
	int zoomLevel = ((CBumbleEditView *)GetActiveView())->GetEditorZoomLevel();
	if (zoomLevel > 200)
	{
		zoomLevel = 200;
		((CBumbleEditView *)GetActiveView())->SetZoomLevel(zoomLevel);
	}
	pSlider->SetPos(zoomLevel);
}

void CMainFrame::OnUpdateCursorPos(CCmdUI *pCmdUI)
{
}

BOOL CMainFrame::OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bTabDrop)
{
	if (!bTabDrop)
	{
		CMenu menu;
		menu.LoadMenu(IDR_TAB_CLOSE_METHOD);

		CMenu *pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
		{
			CMFCPopupMenu *pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return TRUE;

			((CMDIFrameWndEx *)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
	}

	return 0;
}

void CMainFrame::OnUpdateSystemMinimize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CMainFrame::OnUpdateSystemRestore(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CMainFrame::OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized)
{
	CMDIFrameWndEx::OnActivate(nState, pWndOther, bMinimized);

	SendMessage(WM_NCPAINT);
}

CMDIChildWndEx *CMainFrame::CreateDocumentWindow(LPCTSTR lpcszDocName, CObject *pObj)
{
	//// TODO: Add your specialized code here and/or call the base class

	return CMDIFrameWndEx::CreateDocumentWindow(lpcszDocName, pObj);
}

void CMainFrame::OnOptionsOpen()
{
	m_wndOptionDockPane.ToggleAutoHide();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	CView *view = GetActiveView();
	if (!view->GetSafeHwnd())
		return;

	if (nIDEvent == m_timer)
	{
		if (!view->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
			return;

		// update the cursor position display on the status bar
		CString str = ((CBumbleEditView *)GetActiveView())->GetCursorPos();
		m_pWndCursorPos->SetText(str);
		m_pWndCursorPos->Redraw();
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy()
{
	CMDIFrameWndEx::OnDestroy();

	KillTimer(m_timer);
}

void CMainFrame::OnNcPaint()
{
	CFrameWnd::OnNcPaint(); // do default thing (important!)

	// get client rectangle
	CRect rc;
	GetClientRect(rc);

	// convert to window coordinates
	CRect rcWin;
	GetWindowRect(rcWin);				 // window rect (in screen coords)..
	ScreenToClient(rcWin);				 // ..convert to client coords...
	rc -= CPoint(rcWin.left, rcWin.top); // ..convert to window coords

	// rc now has client rect in window coords
	CWindowDC dc(this);
	dc.ExcludeClipRect(rc);					  // don't paint in client area
	rc.InflateRect(BORDERWIDTH, BORDERWIDTH); // expand client rect by border width

	// Now draw the rectangle. Since i've clipped the client area, this only
	// draws the border (expanded rectangle minus client rectangle).

	dc.FillSolidRect(rc, RGB(43, 43, 43));
}

BOOL CMainFrame::OnNcActivate(BOOL bActive)
{
	SendMessage(WM_NCPAINT);

	return CMDIFrameWndEx::OnNcActivate(bActive);
}

void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp)
{
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWndEx::OnNcCalcSize(bCalcValidRects, lpncsp);
	((CRect &)lpncsp->rgrc[0]).DeflateRect(BORDERWIDTH, BORDERWIDTH);
}

void CMainFrame::AddMessage(const CString &message, AppMessageType type)
{
	m_wndOutputDockPane.AddMessage(message, type);
}

void CMainFrame::ClearMessages()
{
	m_wndOutputDockPane.ClearMessages();
}

void CMainFrame::OnOtherwindowsOutputwindow()
{
	m_wndOutputDockPane.ShowWindow(SW_SHOW);
}

void CMainFrame::OnFileCloseAllButThis()
{
	CloseAllFiles(TRUE);
}

void CMainFrame::OnUpdateFileCloseAllButThis(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::ProcessShellExecErrors(int code)
{
	if (code == 0)
	{
		BumbleMessageBox(L"The operating system is out of memory or resources");
	}

	else if (code == ERROR_FILE_NOT_FOUND)
	{
		BumbleMessageBox(L"The specified file was not found");
	}
	else if (code == ERROR_PATH_NOT_FOUND)
	{
		BumbleMessageBox(L"The specified path was not found");
	}

	else if (code == ERROR_BAD_FORMAT)
	{
		BumbleMessageBox(L"The .exe file is invalid (non-Win32 .exe or error in .exe image)");
	}

	else if (code == SE_ERR_ACCESSDENIED)
	{
		BumbleMessageBox(L"The operating system denied access to the specified file");
	}

	else if (code == SE_ERR_ASSOCINCOMPLETE)
	{
		BumbleMessageBox(L"The file name association is incomplete or invalid");
	}

	else if (code == SE_ERR_DDEBUSY)
	{
		BumbleMessageBox(L"The DDE transaction could not be completed because other DDE transactions were being processed");
	}

	else if (code == SE_ERR_DDEFAIL)
	{
		BumbleMessageBox(L"The DDE transaction failed");
	}

	else if (code == SE_ERR_DDETIMEOUT)
	{
		BumbleMessageBox(L"The DDE transaction could not be completed because the request timed out");
	}

	else if (code == SE_ERR_DLLNOTFOUND)
	{
		BumbleMessageBox(L"The specified DLL was not found");
	}

	else if (code == SE_ERR_FNF)
	{
		BumbleMessageBox(L"The specified file was not found");
	}

	else if (code == SE_ERR_NOASSOC)
	{
		BumbleMessageBox(L"There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable");
	}

	else if (code == SE_ERR_OOM)
	{
		BumbleMessageBox(L"There was not enough memory to complete the operation");
	}

	else if (code == SE_ERR_PNF)
	{
		BumbleMessageBox(L"The specified path was not found");
	}

	else if (code == SE_ERR_SHARE)
	{
		BumbleMessageBox(L"A sharing violation occurred");
	}
}

/// <summary>
/// Invoke from file browser context menu
/// </summary>
void CMainFrame::OnFileBrowserCtxOpen()
{
	// TODO: Add your command handler code here
	CString str = m_wndFileBrowserDockPane.GetSelectedPath();

	if (!PathIsDirectory(str))
	{
		theApp.OpenDocumentFile(str);
	}
	else
	{
		int code = (int)ShellExecute(theApp.GetMainWnd()->m_hWnd, L"open", str, NULL, NULL, SW_SHOWNORMAL);
		ProcessShellExecErrors(code);
	}
}

/// <summary>
/// Invoke from file browser context menu
/// </summary>
void CMainFrame::OnFileBrowserCtxOpenWithDefaultApplication()
{
	CString str = m_wndFileBrowserDockPane.GetSelectedPath();
	int code = (int)ShellExecute(theApp.GetMainWnd()->m_hWnd, L"open", str, NULL, NULL, SW_SHOWNORMAL);
	ProcessShellExecErrors(code);
}

void CMainFrame::OnFileBrowserCtxNewfolder()
{
	CString str = m_wndFileBrowserDockPane.GetSelectedPath();

	if (!PathIsDirectory(str))
	{
		BumbleMessageBox(L"Invalid Operation!");
		return;
	}

	CNewFolderNameDlg *pDlg = new CNewFolderNameDlg(this);

	if (pDlg->DoModal() == IDOK)
	{

		str.Append(L"\\");

		str.Append(m_strNewName);

		if (!CreateDirectory(str, NULL))
		{
			theApp.ThrowLastError();
		}
		else
		{
			m_wndFileBrowserDockPane.AddNewFileItem(str);
		}
	}

	delete pDlg;
}

void CMainFrame::OnFileBrowserCtxPrint()
{
	CString str = m_wndFileBrowserDockPane.GetSelectedPath();
	if (!PathIsDirectory(str))
	{

		int code = (int)ShellExecute(theApp.GetMainWnd()->m_hWnd, L"print", str, NULL, NULL, SW_SHOWNORMAL);

		ProcessShellExecErrors(code);
	}
}

void CMainFrame::OnFileBrowserCtxRename()
{
	m_wndFileBrowserDockPane.PrepareItemForRename();
}

//-----------------------------------------------------------------------------
HRESULT CreateAndInitializeFileOperation(REFIID riid, void **ppv)
{
	*ppv = NULL;
	// Create the IFileOperation object
	IFileOperation *pfo;
	HRESULT hr = CoCreateInstance(__uuidof(FileOperation), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfo));
	if (SUCCEEDED(hr))
	{
		// Set the operation flags.  Turn off  all UI
		// from being shown to the user during the
		// operation.  This includes error, confirmation
		// and progress dialogs.
		hr = pfo->SetOperationFlags(FOF_NO_UI);
		if (SUCCEEDED(hr))
		{
			hr = pfo->QueryInterface(riid, ppv);
		}
		pfo->Release();
	}
	return hr;
}

HRESULT DeleteFileFromTree(IShellItem *psiSrc)
{
	IFileOperation *pfo;
	HRESULT hr = CreateAndInitializeFileOperation(IID_PPV_ARGS(&pfo));
	if (SUCCEEDED(hr))
	{
		hr = pfo->DeleteItem(psiSrc, NULL);
		if (SUCCEEDED(hr))
		{

			hr = pfo->PerformOperations();
		}
		pfo->Release();
	}
	return hr;
}
//-------------------------------------------------------------------------------------

void CMainFrame::OnFileBrowserCtxDelete()
{
	CString str = m_wndFileBrowserDockPane.GetSelectedPath();

	CString message;
	if (PathIsDirectory(str))
	{
		message.Format(FOLDER_DELETE_TEXT, str);
	}
	else
	{

		message.Format(FILE_DELETE_TEXT, str);
	}

	if (IDCANCEL == BumbleMessageBox(message))
	{
		return;
	}

	IShellItem *item;

	SHCreateItemFromParsingName(str, NULL, IID_PPV_ARGS(&item));

	if (S_OK == DeleteFileFromTree(item))
	{
		m_wndFileBrowserDockPane.DeleteItemFromTree();
	}

	item->Release();
}

#define BUFSIZE 4096
void CMainFrame::OnFileBrowserCtxNewFile()
{
	CString path = m_wndFileBrowserDockPane.GetSelectedPath();

	if (!PathIsDirectory(path))
	{

		CString tmp = CString(PathFindFileName(path));
		path.Replace(tmp, L"");
	}

	CString fullPath;
	int index = 1;
	while (true)
	{
		CString strFmt = L"%s\\New File%d.txt";
		fullPath = CString();
		fullPath.Format(strFmt, path, index);
		if (!PathFileExists(fullPath))
		{
			break;
		}

		index++;
	}

	CString fileName = PathFindFileName(fullPath);

	if (path.IsEmpty())
	{
		BumbleMessageBox(L"File creation error!");
		return;
	}

	// theApp.m_pDocManager->OnFileNew();

	if (INVALID_HANDLE_VALUE == CreateFile(fullPath,
										   GENERIC_READ | GENERIC_WRITE,
										   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
										   NULL,
										   CREATE_NEW,
										   FILE_ATTRIBUTE_NORMAL,
										   NULL))
	{

		theApp.ThrowLastError();
	}
	else
	{
		m_wndFileBrowserDockPane.AddNewFileItem(fullPath);
	}
}

void CMainFrame::OnTool(UINT nID)
{

	theApp.GetPluginManger()->OnCommand(nID);
}

BOOL CMainFrame::OnShowPopupMenu(CMFCPopupMenu *pMenuPopup)
{
	// TODO: Add your specialized code here and/or call the base class

	// return CMDIFrameWndEx::OnShowPopupMenu(pMenuPopup);

	// ------------------------------------------------ -------- -
	// Replace the id_view_toolbars menu item toolbar list :
	// ------------------------------------------------ -------- -
	CMDIFrameWndEx::OnShowPopupMenu(pMenuPopup);

	if (!pMenuPopup)
	{
		return TRUE;
	}

	CString caption;
	if (pMenuPopup->GetMenuBar()->CommandToIndex(ID_TOOLS_TOOLCONTAINER, 0) < 0)
	{
		return TRUE;
	}

	pMenuPopup->RemoveAllItems();

	CMenu *menu = new CMenu();
	menu->CreateMenu();

	if (theApp.GetPluginManger()->m_lstPlugins.size() == 0)
	{
		return FALSE;
	}

	CPluginManager *pPlgMan = theApp.GetPluginManger();
	for (auto &it : pPlgMan->m_pMenuList)
	{
	}

	for (int i = 0; i < pPlgMan->m_lstPlugins.size(); i++)
	{
		CString name = pPlgMan->m_lstPlugins.at(i)->GetName();
		if (name.IsEmpty())
		{
			name = L"Extension need a name??";
		}

		if (pPlgMan->m_pMenuList.at(i)->GetSubMenu(0))
		{
			menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)(pPlgMan->m_pMenuList.at(i)->GetSubMenu(0)->m_hMenu), name);
		}
		else
		{
			menu->InsertMenuW(i, MF_STRING, pPlgMan->m_pMenuList.at(i)->GetMenuItemID(0), name);
		}
	}

	pMenuPopup->GetMenuBar()->ImportFromMenu(menu->m_hMenu, 0);

	return TRUE;
}

void CMainFrame::OnFileNewproject()
{
	theApp.OnFileNewProject();
}

void CMainFrame::OnOtherwindowsFilebrowser()
{
	m_wndFileBrowserDockPane.ShowPane(TRUE, 0, TRUE);
}

void CMainFrame::OnWindowNew()
{
	theApp.AddNewTextDoc();
}

void CMainFrame::OnEditCapturescreen()
{
	ScreenCaptureType captureType = ScreenCaptureType::Document;

	StartScreenCapture(captureType);
}

void CMainFrame::OnUIThreadToCaptureSendStopMessage()
{
	if (m_pScreenCaptureThread)
	{
		m_pScreenCaptureThread->PostThreadMessageW(WM_COMMAND, ID_ACTION_STOP, 0);

		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pScreenCaptureThread->m_hThread, 5000))
		{

			m_pScreenCaptureThread->ExitInstance();
		}

		delete m_pScreenCaptureThread;
		m_pScreenCaptureThread = nullptr;
	}
}

void CMainFrame::OnToolsTutorialMode()
{
	/*BOOL bAutoHideMode = m_wndCmdHistory.IsAutoHideMode();
	BOOL bTabbed = m_wndCmdHistory.IsTabbed();

	ShowPane(&m_wndCmdHistory,
		!m_wndCmdHistory.IsVisible(),
		FALSE, bAutoHideMode || !bTabbed);
	RecalcLayout();*/
}

void CMainFrame::OnUpdateToolsTutorialMode(CCmdUI *pCmdUI)
{
	/*m_bTutorialMode = m_wndCmdHistory.IsVisible();
	pCmdUI->SetCheck(m_bTutorialMode);*/
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
	// TODO: Add your specialized code here and/or call the base class

	if (!CMDIFrameWndEx::PreCreateWindow(cs))
	{
		return FALSE;
	}

	WNDCLASS wndcls;
	if (!::GetClassInfo(AfxGetInstanceHandle(), theApp.GetMainWindowClassName(), &wndcls))
	{
		// Get the current requested window class
		VERIFY(GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls));

		// We want to register this info with our name
		wndcls.lpszClassName = theApp.GetMainWindowClassName();

		// Need to preset the icon otherwise the function GetIconWndClass
		// calling us will overwrite our class.
		LPCTSTR pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
		HINSTANCE hInst = AfxFindResourceHandle(pszIcon, ATL_RT_GROUP_ICON);
		_ASSERTE(hInst != NULL);
		wndcls.hIcon = ::LoadIcon(hInst, pszIcon);

		// Register our class now and check the outcome
		if (!::RegisterClass(&wndcls))
		{
			_ASSERTE(!__FUNCTION__ "RegisterClass failed");
			return FALSE;
		}
	}

	cs.lpszClass = theApp.GetMainWindowClassName();
	return TRUE;
}

CBumbleEditApp *CMainFrame::getBumbleApp()
{
	return &theApp;
}

/*
This message is used to receive the command line information from the
other instances of the application.
*/
BOOL CMainFrame::OnCopyData(CWnd *pWnd, COPYDATASTRUCT *pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	CString copiedData = (LPCTSTR)(pCopyDataStruct->lpData);
	// AfxMessageBox((LPCTSTR)(pCopyDataStruct->lpData));
	theApp.OpenDocumentFile(copiedData);

	return CMDIFrameWndEx::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnRunConfigure()
{
	CString appPath = theApp.GetApplicationPath();
	if (appPath.IsEmpty())
	{
		BumbleMessageBox(L"Unable to read the configuration file, runtime_config.xml!");
		return;
	}

	CString fileName;
	fileName.Format(L"%s\\xmlconfig\\runtime_config.xml", appPath);
	theApp.OpenFile(fileName);
}

BOOL CMainFrame::ActivateTab(INT_PTR nTabGroupIndex, int nTabItemIndex /*= -1*/)
{
	BOOL bRet = FALSE;
	const CObList &list = GetMDITabGroups();
	POSITION pos = list.FindIndex(nTabGroupIndex);
	if (NULL != pos)
	{
		const CObject *pTabGroup = list.GetAt(pos);
		VERIFY(pTabGroup->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)));
		if (NULL != pTabGroup)
		{
			CMFCTabCtrl *pTabCtrl = (CMFCTabCtrl *)pTabGroup;
			if (pTabCtrl->GetTabsNum() > nTabItemIndex)
			{
				pTabCtrl->ActivateMDITab(nTabItemIndex);
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

BOOL CMainFrame::ActivateTabByView(CBumbleEditView *pView)
{
	::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)pView->GetParentFrame()->GetSafeHwnd(), 0);
	return TRUE;
}

void CMainFrame::OnToolsFunctions()
{
	// TODO: Add your command handler code here
}
