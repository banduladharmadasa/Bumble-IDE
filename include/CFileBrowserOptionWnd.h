#pragma once

// CFileBrowserOptionWnd dialog

class CFileBrowserOptionWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CFileBrowserOptionWnd)

public:
	CFileBrowserOptionWnd(CWnd *pParent = nullptr); // standard constructor
	virtual ~CFileBrowserOptionWnd();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_FILE_BROWSER_OPT
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChkSetFolder();

private:
	CMenu m_ctxMenu;
	CMFCMenuButton m_btnMenu;
	CEdit m_edtFileFilter;
	CToolTipCtrl m_ToolTip;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
