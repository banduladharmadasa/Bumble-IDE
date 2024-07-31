#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif

#include "HistoryObj.h"
typedef CList<CHistoryObj *, CHistoryObj *> _T_HistotyList;
// CTutorialBrowser dialog

class CTutorialBrowser : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CTutorialBrowser)

public:
	CTutorialBrowser(CWnd *pParent = nullptr); // standard constructor
	virtual ~CTutorialBrowser();
// Overrides

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_TUTORIALBROWSER,
		IDH = 356
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
	virtual BOOL OnInitDialog();
	STDMETHOD(ShowContextMenu)
	(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoStartPage();
	afx_msg void OnViewRefresh();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

	CMFCToolBar m_wndToolBar;

	CComVariant Eval(const CString &exp);
	BOOL CallClientScript(LPCTSTR pStrFuncName, CStringArray *pArrFuncArgs, CComVariant *pOutVarRes);

public:
	void EnableButton(int iCmd, BOOL bStatus);
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CArray<CHistoryObj *, CHistoryObj *> m_arHistory;
	int m_iHistoryOffset;

	void GetBackList(_T_HistotyList &lst) const;
	void GetFrwdList(_T_HistotyList &lst) const;

	BOOL IsBackAvailable() const
	{
		return m_iHistoryOffset < m_arHistory.GetSize() - 1;
	}

	BOOL IsFrwdAvailable()
	{
		return m_iHistoryOffset > 0;
	}

	CHistoryObj *GoBack()
	{
		if (m_iHistoryOffset >= m_arHistory.GetSize())
		{
			ASSERT(FALSE);
			return NULL;
		}

		return m_arHistory[++m_iHistoryOffset];
	}

	CHistoryObj *GoForward()
	{
		if (m_iHistoryOffset < 1)
		{
			ASSERT(FALSE);
			return NULL;
		}

		return m_arHistory[--m_iHistoryOffset];
	}

	CHistoryObj *Go(UINT uiCmd);
	CHistoryObj *AddURLToHistory(const CString &strTitle, const CString &strURL);
};
