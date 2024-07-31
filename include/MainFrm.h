// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "StatusBarEx.h"
#include "OptionDock.h"
#include "OutputWnd.h"
#include "ToolPane.h"
#include "CFileBrowserDockPane.h"
#include "ColorPalette.h"
#include "ScreenCaptureThread.h"
#include "TutorialTextPane.h"

class CBumbleEditView;
class CZoomSlider;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	afx_msg LRESULT OnTabToolTipNeeded(WPARAM wParam, LPARAM lParam);
	CMainFrame() noexcept;
	BOOL WasMaximized();
	void CloseMenu();

	void OnColorPick(COLORREF cr);
	virtual void OnDrawMenuLogo(CDC *pDC, CMFCPopupMenu *pMenu, const CRect &rectLogo) override;
	void ReconfigAllEditors();
	void CloseAllFiles(BOOL bKeepCurrent = FALSE);
	void SetNewFileOrFolderName(const CString &name);

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

	/// <summary>
	///
	/// </summary>
	void DestroyCaptureThread();
	/// <summary>
	///
	/// </summary>
	/// <param name="captureType"></param>
	void StartScreenCapture(ScreenCaptureType captureType);
	void StopScreenCapture();

	CWinThread *m_pScreenCaptureThread;

protected: // control bar embedded members
	CStatusBarEx m_wndStatusBar;

	CMFCToolBar m_wndToolBar;
	CMFCMenuBar m_wndMenuBar;

	CZoomSlider *pSlider;
	COutputWnd m_wndOutputDockPane;
	CFileBrowserDockPane m_wndFileBrowserDockPane;

	CMFCRibbonStatusBarPane *m_pWndCursorPos;
	// CCommandHistoryPane m_wndCmdHistory;

	UINT_PTR m_timer;
	CString m_strNewName;
	BOOL m_bTutorialMode;

	BOOL AddStatusBar();

	afx_msg void OnZoom();

	afx_msg void OnZoomSlider();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnWindowManager();
	DECLARE_MESSAGE_MAP()

	BOOL AddToolBar();
	void AddColorButton();
	void AddFunctionWindow();
	CMFCColorMenuButton *CreateColorButton();
	CMFCToolBarMenuButton *CreateFunctionButton();
	afx_msg void OnZoomSliderUpdate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCursorPos(CCmdUI *pCmdUI);
	virtual BOOL OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bTabDrop) override;

public:
	COptionDock m_wndOptionDockPane;

	CBumbleEditView *GetActiveView() const;

	afx_msg void OnUpdateSystemMinimize(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSystemRestore(CCmdUI *pCmdUI);

	afx_msg void OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized);

private:
	CPalette m_palColorPicker; // Palette for color picker
	int m_nColours;

public:
	virtual CMDIChildWndEx *CreateDocumentWindow(LPCTSTR lpcszDocName, CObject *pObj);

	afx_msg void OnOptionsOpen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp);
	void AddMessage(const CString &message, AppMessageType type);
	void ClearMessages();
	afx_msg void OnOtherwindowsOutputwindow();

	afx_msg void OnFileCloseAllButThis();
	afx_msg void OnUpdateFileCloseAllButThis(CCmdUI *pCmdUI);
	void ProcessShellExecErrors(int code);
	afx_msg void OnFileBrowserCtxOpen();
	afx_msg void OnFileBrowserCtxOpenWithDefaultApplication();
	afx_msg void OnFileBrowserCtxNewfolder();
	afx_msg void OnFileBrowserCtxPrint();

	afx_msg void OnFileBrowserCtxRename();
	afx_msg void OnFileBrowserCtxDelete();
	afx_msg void OnFileBrowserCtxNewFile();
	afx_msg void OnTool(UINT nID);
	virtual BOOL OnShowPopupMenu(CMFCPopupMenu *pMenuPopup);
	afx_msg void OnFileNewproject();
	afx_msg void OnOtherwindowsFilebrowser();
	afx_msg void OnWindowNew();
	afx_msg void OnEditCapturescreen();

	afx_msg void OnUIThreadToCaptureSendStopMessage();

	afx_msg void OnToolsTutorialMode();
	afx_msg void OnUpdateToolsTutorialMode(CCmdUI *pCmdUI);

	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	CBumbleEditApp *getBumbleApp();
	afx_msg BOOL OnCopyData(CWnd *pWnd, COPYDATASTRUCT *pCopyDataStruct);

	afx_msg void OnRunConfigure();
	BOOL ActivateTab(INT_PTR nTabGroupIndex, int nTabItemIndex);
	BOOL ActivateTabByView(CBumbleEditView *pView);
	afx_msg void OnToolsFunctions();
};
