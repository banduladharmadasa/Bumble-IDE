#pragma once

// CInsertNumberAt

class CInsertNumberAt : public CBumblePropertyPage
{
	DECLARE_DYNCREATE(CInsertNumberAt)

public:
	CInsertNumberAt();
	virtual ~CInsertNumberAt();

	enum
	{
		IDD = IDD_INSERT_NUMBER_AT_PAGE
	};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DoDataExchange(CDataExchange *pDX);
	CEdit m_edtInitNumber;
	CEdit m_edtInc;
	CEdit m_edtLeadingZeros;

	virtual BOOL OnSetActive();
	virtual void OnOK();
};
