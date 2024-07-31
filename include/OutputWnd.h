
#pragma once
#include "CMessageWnd.h"
#include "CFindResultWnd.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList window

// class COutputList : public CListBox
//{
//// Construction
// public:
//	COutputList() noexcept;
//
//// Implementation
// public:
//	virtual ~COutputList();
//
// protected:
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
//	afx_msg void OnViewOutput();
//
//	DECLARE_MESSAGE_MAP()
// };

class COutputWnd : public CDockablePane
{
	// Construction
public:
	COutputWnd() noexcept;

	void UpdateFonts();
	void SetActiveTab(int index);

	CFindResultWnd *GetFindResultWnd();
	HTREEITEM AddItemToFindFileResults(const CString &str, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);

	// Attributes
protected:
	CMFCTabCtrl m_wndTabs;

	CMessageWnd m_wndMessage;
	// COutputList m_wndOutputDebug;
	CFindResultWnd m_wndFindResults;

protected:
	void AdjustHorzScroll(CListBox &wndListBox);

	// Implementation
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	void AddMessage(const CString &message, AppMessageType type);
	void ClearMessages();

	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnHide();
	afx_msg void OnUpdateHide(CCmdUI *pCmdUI);
};
