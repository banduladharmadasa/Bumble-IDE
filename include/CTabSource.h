#pragma once
#include "resource.h"
#include "ProjectTree.h"
#include "SourceFolderList.h"
// CTabSource dialog
typedef struct {
	LPTSTR pszFolder;
	BOOL bRecurse;
	BOOL bIncludeOnly;
} ITEMDATA, * PITEMDATA;


class CTabSource : public CBumbleDialog
{
	DECLARE_DYNAMIC(CTabSource)

public:
	CTabSource(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTabSource();

	void AddFile(const CString& file);


	enum { IDD = IDD_TAB_SOURCE };

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CProjectTree m_treeCtrl;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);



	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnIdok();
	afx_msg void OnIdok2();
	afx_msg void OnIdcancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
