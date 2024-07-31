// CFindResultWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CFindResultWnd.h"
#include "resource.h"
#include "MainFrm.h"
#include "BumbleEditView.h"
#include "BumbleEditDoc.h"

// CFindResultWnd dialog

CFindResultWnd::CFindResultWnd(CWnd *pParent /*=nullptr*/)
	: CDHtmlDialog(CFindResultWnd::IDD, 0, pParent)
{
}

CFindResultWnd::~CFindResultWnd()
{
}

void CFindResultWnd::AddResults(LPCWSTR args)
{
	CString _str(args);
	_str.Replace(L"'", L"\\'");
	CString str;

	str.Format(L"var v = document.getElementById('find-results');\nv.innerHTML +='%s';", _str);
	Eval(str);

	((CDockablePane *)GetParent())->ShowPane(TRUE, 0, TRUE);

	if (((CDockablePane *)GetParent())->IsAutoHideMode())
	{
		((CDockablePane *)GetParent())->ToggleAutoHide();
	}
}

void CFindResultWnd::ClearResult()
{
	CString str(L"var v = document.getElementById('find-results');\nv.innerHTML ='';");
	Eval(str);
}

void CFindResultWnd::DoDataExchange(CDataExchange *pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CFindResultWnd::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	LoadFromResource(IDR_HTML_CFINDRESULTWND);
	return TRUE; // return TRUE  unless you set the focus to a control
}

// HRESULT CFindResultWnd::OnMouseOverElement(IHTMLElement* phtmlElement)
//{
//	IHTMLStyle* phtmlStyle;
//	phtmlElement->get_style(&phtmlStyle);
//	if (phtmlStyle)
//	{
//		VARIANT varColor;
//		varColor.vt = VT_I4;
//		varColor.lVal = 0xff0000;
//		phtmlStyle->put_textDecorationUnderline(VARIANT_TRUE);
//		phtmlStyle->put_color(varColor);
//		phtmlStyle->put_cursor(L"hand");
//		phtmlStyle->Release();
//	}
//	m_spCurrElement = phtmlElement;
//	return S_OK;
// }
//
// HRESULT CFindResultWnd::OnMouseOutElement(IHTMLElement* phtmlElement)
//{
//	IHTMLStyle* phtmlStyle;
//	phtmlElement->get_style(&phtmlStyle);
//	if (phtmlStyle)
//	{
//		VARIANT varColor;
//		varColor.vt = VT_I4;
//		varColor.lVal = 0x000000;
//		phtmlStyle->put_textDecorationUnderline(VARIANT_FALSE);
//		phtmlStyle->put_color(varColor);
//		phtmlStyle->Release();
//	}
//	m_spCurrElement = NULL;
//	return S_OK;
// }

STDMETHODIMP_(HRESULT __stdcall)
CFindResultWnd::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	GetParent()->PostMessage(WM_CONTEXTMENU, (WPARAM)this->m_hWnd, MAKELPARAM(ppt->x, ppt->y));

	return S_OK;
}

BEGIN_MESSAGE_MAP(CFindResultWnd, CDHtmlDialog)
ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CFindResultWnd)
DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONCLICK, _T("find-result-anchor"), OnClickAnchorTag)
DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONCLICK, _T("hyperace-match"), OnClickAnchorTag)
END_DHTML_EVENT_MAP()

// CFindResultWnd message handlers

HRESULT CFindResultWnd::OnClickAnchorTag(IHTMLElement *pElement)
{
	// Get the line number and column number of the target
	// line

	CComVariant attr;
	BSTR _className;
	pElement->get_className(&_className);
	CString className = _className;
	IHTMLElement *pTargetEl = nullptr;
	if (className.Compare(L"hyperace-match") == 0)
	{
		if (S_OK != pElement->get_parentElement(&pTargetEl))
		{

			return S_OK;
		}
	}
	else
	{
		pTargetEl = pElement;
	}

	pTargetEl->getAttribute(L"ln", 0, &attr);
	CString strLn = attr.bstrVal;
	pTargetEl->getAttribute(L"col", 0, &attr);
	CString strCol = attr.bstrVal;
	pTargetEl->getAttribute(L"uniqueDocId", 0, &attr);
	CString strUniqueDocId = attr.bstrVal;

	CView *pView = nullptr; // = ((CMainFrame*)theApp.GetMainWnd())->GetActiveView();

	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate)
	{
		CDocTemplate *pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);
			if (pDoc)
			{
				UINT id = ((CBumbleEditDoc *)pDoc)->GetUniqueDocID();
				CString strID;
				strID.Format(L"%u", id);

				if (strID.Compare(strUniqueDocId) == 0)
				{
					POSITION pos = pDoc->GetFirstViewPosition();
					pView = pDoc->GetNextView(pos);
					break;
				}
			}
		}
	}

	if (pView)
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
		{
			// pView->GetParentFrame()->ActivateFrame();

			CString str;
			str.Format(L"editor.focus();\neditor.navigateFileEnd();\neditor.scrollToLine(50, true, true, function () {}); \n editor.gotoLine(%s, %s, true); ", strLn, strCol);
			((CBumbleEditView *)pView)->Eval(str);
			theApp.GetMainWindow()->ActivateTabByView((CBumbleEditView *)pView);
		}
	}

	return S_OK;
}
CComVariant CFindResultWnd::Eval(const CString &exp)
{
	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2))) // Uses CDHtmlDialog as 'this'
	{
		CStringArray arrArgs;
		arrArgs.Add(exp);
		CComVariant varRes = 0;
		CComVariant _command = L"eval";

		if (CallClientScript(_command.bstrVal, &arrArgs, &varRes))
		{
			return varRes;
		}
	}

	return 0;
}

BOOL CFindResultWnd::CallClientScript(LPCTSTR pStrFuncName, CStringArray *pArrFuncArgs, CComVariant *pOutVarRes)
{
	BOOL bRes = FALSE;
	CComVariant vaResult;
	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2))) // Uses CDHtmlDialog as 'this'
	{
		// Getting IDispatch for Java Script objects
		CComPtr<IDispatch> spScript;
		if (SUCCEEDED(pIDoc2->get_Script(&spScript)))
		{
			// Find dispid for given function in the object
			CComBSTR bstrMember(pStrFuncName);
			DISPID dispid = NULL;
			if (SUCCEEDED(spScript->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_USER_DEFAULT, &dispid)))
			{
				const int arraySize = pArrFuncArgs ? pArrFuncArgs->GetSize() : 0;

				// Putting parameters
				DISPPARAMS dispparams;
				memset(&dispparams, 0, sizeof dispparams);
				dispparams.cArgs = arraySize;
				dispparams.rgvarg = new VARIANT[dispparams.cArgs];
				dispparams.cNamedArgs = 0;

				for (int i = 0; i < arraySize; i++)
				{
					CComBSTR bstr = pArrFuncArgs->GetAt(arraySize - 1 - i); // back reading
					bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
					dispparams.rgvarg[i].vt = VT_BSTR;
				}

				EXCEPINFO excepInfo;
				memset(&excepInfo, 0, sizeof excepInfo);
				UINT nArgErr = (UINT)-1; // initialize to invalid arg

				// Call JavaScript function
				if (SUCCEEDED(spScript->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr)))
				{
					// Done!
					bRes = TRUE;
				}
				// Free mem
				delete[] dispparams.rgvarg;
			}
		}
	}

	if (pOutVarRes)
		*pOutVarRes = vaResult;

	return bRes;
}

void CFindResultWnd::copyContent()
{
	BOOL bRes = FALSE;
	CComVariant vaResult;
	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2))) // Uses CDHtmlDialog as 'this'
	{
		VARIANT v;
		v.bstrVal = L"";
		VARIANT_BOOL b;
		pIDoc2->execCommand(L"copy", true, v, &b);
	}
}
