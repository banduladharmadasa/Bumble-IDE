#pragma once

// CFileDialogEx

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
				  LPCTSTR lpszDefExt = nullptr,
				  LPCTSTR lpszFileName = nullptr,
				  DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				  LPCTSTR lpszFilter = nullptr,
				  CWnd *pParentWnd = nullptr);
	virtual ~CFileDialogEx();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOpenNetworkLocationClicked();

	virtual void OnButtonClicked(DWORD dwIDCtl);
};
