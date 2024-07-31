#pragma once

#include "resource.h"
// CFindResultWnd dialog

class CFindResultWnd : public CDHtmlDialog
{

public:
	CFindResultWnd(CWnd *pParent = nullptr); // standard constructor
	virtual ~CFindResultWnd();
	// Overrides

	HRESULT OnClickAnchorTag(IHTMLElement *pElement);

	CComVariant Eval(const CString &exp);

	BOOL CallClientScript(LPCTSTR pStrFuncName, CStringArray *pArrFuncArgs, CComVariant *pOutVarRes);
	void copyContent();

	// Dialog Data
	// #ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_FIND_RESULT
	};
	// #endif

	void AddResults(LPCWSTR args);
	void ClearResult();

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
	virtual BOOL OnInitDialog();
	/*HRESULT OnMouseOverElement(IHTMLElement* phtmlElement);
	HRESULT OnMouseOutElement(IHTMLElement* phtmlElement);*/

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	STDMETHOD(ShowContextMenu)
	(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

private:
	CComPtr<IHTMLElement> m_spCurrElement;

public:
};
