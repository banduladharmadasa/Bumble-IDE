#pragma once

// CSettingsFormView view

class CSettingsFormView : public CFormView
{
	DECLARE_DYNCREATE(CSettingsFormView)

protected:
	CSettingsFormView(); // protected constructor used by dynamic creation
	virtual ~CSettingsFormView();

public:
	enum
	{
		IDD = IDD_SETTINGSVIEW
	};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext &dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
protected:
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
