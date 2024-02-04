#pragma once


// CInsertTextAt

class CInsertTextAt : public CBumblePropertyPage
{
	DECLARE_DYNCREATE(CInsertTextAt)

public:
	CInsertTextAt();
	virtual ~CInsertTextAt();

	enum{ IDD = IDD_INSERT_TEXT_AT_PAGE	};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DoDataExchange(CDataExchange* pDX);
	CBumbleButton m_radFront;
	CBumbleButton m_radBack;
	CEdit m_edtText;
	virtual BOOL OnSetActive();
	virtual void OnOK();
};


