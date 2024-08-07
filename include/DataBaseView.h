#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif

// CDataBaseView html view

class CDataBaseView : public CHtmlView
{
	DECLARE_DYNCREATE(CDataBaseView)

protected:
	CDataBaseView(); // protected constructor used by dynamic creation
	virtual ~CDataBaseView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
