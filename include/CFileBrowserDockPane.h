#pragma once
#include "DirTreeCtrl.h"
#include "CFileBrowserOptionWnd.h"
// CFileBrowserDockPane

class CFileBrowserDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CFileBrowserDockPane)

public:
	CFileBrowserDockPane();
	virtual ~CFileBrowserDockPane();
	
	CFileBrowserOptionWnd *GetOptWnd();
	void SetFilters(const CString& filters);
	CString GetSelectedPath();

	CEdit* PrepareItemForRename();

	
	void AddNewFileItem(const CString& name);
	void DeleteItemFromTree();
	
protected:
	DECLARE_MESSAGE_MAP()

	//CFileBrowserWnd	m_wndFileBrowser;

	CFileTreeWnd m_treeCtrl;
	CFileBrowserOptionWnd m_wndOptions;
	CMFCMenuButton m_btnMenu;
	CMenu m_ctxMenu;
	CString m_strSelectedFilePath;
	HTREEITEM m_hSelectedItem;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFileOpen();
};


