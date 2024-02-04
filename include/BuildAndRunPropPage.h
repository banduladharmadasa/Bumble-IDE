#pragma once


// CBuildAndRunPropPage dialog

class CBuildAndRunPropPage : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(CBuildAndRunPropPage)

	// Construction
public:
	CBuildAndRunPropPage();
	~CBuildAndRunPropPage();

	// Dialog Data
	enum { IDD = IDD_BUILDANDRUNPROPPAGE };

	// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

