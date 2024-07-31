#pragma once

// CTabBuild dialog

class CTabBuild : public CBumbleDialog
{
	DECLARE_DYNAMIC(CTabBuild)

public:
	CTabBuild(CWnd *pParent = nullptr); // standard constructor
	virtual ~CTabBuild();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_TAB_BUILD1
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
