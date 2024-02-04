#pragma once


// CGotoLine dialog

class CGotoLine : public CBumbleDialog
{
	DECLARE_DYNAMIC(CGotoLine)

public:
	CGotoLine(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CGotoLine();


	enum { IDD = IDD_GOTOLINE };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_label;
	CEdit m_edtGototLine;
	CBumbleButton m_btnOk;
	CBumbleButton m_btnCancel;
	afx_msg void OnBnClickedOk();
	CEdit m_edtCol;
	virtual BOOL OnInitDialog();
};
