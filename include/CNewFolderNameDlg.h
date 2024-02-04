#pragma once


// CNewFolderNameDlg dialog

class CNewFolderNameDlg : public CBumbleDialog
{
	DECLARE_DYNAMIC(CNewFolderNameDlg)

public:
	CNewFolderNameDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNewFolderNameDlg();

// Dialog Data

	enum { IDD = IDD_NEW_FOLDER_NAME };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CBumbleButton m_btnOK;
	CBumbleButton m_btnCancel;
};
