#pragma once

#include "CStaticEx.h"
#include "CBumbleDialog.h"
// CBumbleMessageBox dialog

class CBumbleMessageBox : public CBumbleDialog
{
	DECLARE_DYNAMIC(CBumbleMessageBox)

public:
	CBumbleMessageBox(const CString &reason, CWnd *pParent = nullptr); // standard constructor
	virtual ~CBumbleMessageBox();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_BUMBLE_MSG_BOX
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	CBumbleButton m_btnOk;
	CStaticEx m_staticReason;
	CString m_strReason;
	CBumbleButton m_btnCanel;

public:
	virtual BOOL OnInitDialog();
};
