#pragma once


// CRenameDialog dialog

class CRenameDialog : public CBumbleDialog
{
	DECLARE_DYNAMIC(CRenameDialog)

public:
	CRenameDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CRenameDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RENAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBumbleButton m_btnOK;
	CBumbleButton m_btnCancel;
	virtual BOOL OnInitDialog();
	CEdit m_edtRename;
	virtual void OnOK();
};
