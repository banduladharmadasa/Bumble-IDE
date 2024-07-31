#pragma once
#include "CTabBuild.h"
#include "CTabSource.h"
#include "ProjectTabCtrl.h"
#include <vector>

// CProjectView form view

class CProjectView : public CFormView
{
	DECLARE_DYNCREATE(CProjectView)

protected:
	CProjectView(); // protected constructor used by dynamic creation
	virtual ~CProjectView();

public:
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_PROJECT_VIEW
	};
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext &dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTabChanged();

	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CProjectTabCtrl *m_pTabCtrl;

	CTabSource m_tabSource;
	CTabBuild m_tabBuild;
	std::vector<CWnd *> m_tabPages;

	afx_msg void OnAddNewItem();
	afx_msg void OnOpenItem();
};
