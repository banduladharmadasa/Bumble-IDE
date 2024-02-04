// TutorialBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TutorialBrowser.h"

#define FIRST_HISTORY_COMMAND	0x1000
#define HISTORY_LEN	10
// CTutorialBrowser dialog

IMPLEMENT_DYNCREATE(CTutorialBrowser, CDHtmlDialog)

CTutorialBrowser::CTutorialBrowser(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_TUTORIALBROWSER, 356, pParent)
{

}

CTutorialBrowser::~CTutorialBrowser()
{
}

void CTutorialBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CTutorialBrowser::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR);
	Navigate(L"http://hexonianlabs.com/bumble_tutorials", navNoReadFromCache | navNoHistory);

	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME));

	
	SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(WM_SETICON, ICON_BIG, (LPARAM)hIcon);


	UINT uiToolbarHotID = IDB_HOTTOOLBAR;
	UINT uiToolbarColdID = IDB_COLDTOOLBAR;
	UINT uiMenuID = IDB_MENU_IMAGES;
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_BROWSERTOOLBAR, uiToolbarColdID, uiMenuID,
			FALSE /* Not locked */, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1; // fail to create
	}
	m_wndToolBar.SetWindowText(_T("Standard"));
	m_wndToolBar.SetBorders();
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle()
		& ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_ANY));

	CSize   sizeToolBar = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	m_wndToolBar.SetWindowPos(NULL, 0, 10, sizeToolBar.cx, sizeToolBar.cy,
		SWP_NOACTIVATE | SWP_NOZORDER);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

STDMETHODIMP_(HRESULT __stdcall) CTutorialBrowser::ShowContextMenu(DWORD dwID, POINT* ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved)
{

	return S_OK;
}

void CTutorialBrowser::OnGoBack()
{
	//Eval(L"window.history.back();");
	CHistoryObj* pHistoryObj = GoBack();
	if (pHistoryObj != NULL)
	{
		Navigate(pHistoryObj->GetURL(), 0, NULL);
	}
}

void CTutorialBrowser::OnGoForward()
{
	//Eval(L"window.history.forward();");
	CHistoryObj* pHistoryObj = GoForward();
	if (pHistoryObj != NULL)
	{
		Navigate(pHistoryObj->GetURL(), 0, NULL);
	}
}

void CTutorialBrowser::OnGoStartPage()
{
	Navigate(L"http://hexonianlabs.com/bumble_tutorials", navNoReadFromCache | navNoHistory);
}

void CTutorialBrowser::OnViewRefresh()
{
	Eval(L"location.reload();");
}

BEGIN_MESSAGE_MAP(CTutorialBrowser, CDHtmlDialog)
	ON_WM_SIZE()
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)	
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)	
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTutorialBrowser)

END_DHTML_EVENT_MAP()



// CTutorialBrowser message handlers



CComVariant CTutorialBrowser::Eval(const CString& exp)
{
	CComPtr<IHTMLDocument2> pIDoc2;
	BOOL bRes = FALSE;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2)))  //Uses CDHtmlDialog as 'this'
	{
		CStringArray arrArgs;
		arrArgs.Add(exp);
		CComVariant varRes = 0;
		CComVariant _command = L"eval";

		if (CallClientScript(_command.bstrVal, &arrArgs, &varRes)) {
			return varRes;
		}
	}

	return bRes;
}


BOOL CTutorialBrowser::CallClientScript(LPCTSTR pStrFuncName, CStringArray* pArrFuncArgs, CComVariant* pOutVarRes)
{
	BOOL bRes = FALSE;
	CComVariant vaResult;
	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2)))  //Uses CDHtmlDialog as 'this'
	{
		//Getting IDispatch for Java Script objects
		CComPtr<IDispatch> spScript;
		if (SUCCEEDED(pIDoc2->get_Script(&spScript)))
		{
			//Find dispid for given function in the object
			CComBSTR bstrMember(pStrFuncName);
			DISPID dispid = NULL;
			if (SUCCEEDED(spScript->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_USER_DEFAULT, &dispid)))
			{
				const int arraySize = pArrFuncArgs ? pArrFuncArgs->GetSize() : 0;

				//Putting parameters  
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
				UINT nArgErr = (UINT)-1;  // initialize to invalid arg

				//Call JavaScript function         
				if (SUCCEEDED(spScript->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr)))
				{
					//Done!
					bRes = TRUE;
				}
				//Free mem
				delete[] dispparams.rgvarg;
			}
		}
	}

	if (pOutVarRes)
		*pOutVarRes = vaResult;

	return bRes;
}


void CTutorialBrowser::EnableButton(int iCmd, BOOL bStatus)
{
	// iCmd is not the index, but the command ID - this way the
	// buttons can be re-arranged and this routine still works
	int idx = m_wndToolBar.CommandToIndex(iCmd);
	if (idx == -1) return;  // not in this toolbar
	UINT iStyle = m_wndToolBar.GetButtonStyle(idx);
	CMFCToolBarButton* Button = m_wndToolBar.GetButton(idx);
	if (bStatus) { // enable
		m_wndToolBar.SetButtonStyle(idx, iStyle & !TBBS_DISABLED);
	}
	else {
		m_wndToolBar.SetButtonStyle(idx, iStyle | TBBS_DISABLED);
	}
	Button->EnableWindow(bStatus);
}

void CTutorialBrowser::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);

	// TODO: Add your specialized code here and/or call the base class
	
	CComVariant var = Eval(L"document.title");
	AddURLToHistory(var.bstrVal, szUrl);
	EnableButton(ID_GO_BACK, IsBackAvailable());
	EnableButton(ID_GO_FORWARD, IsFrwdAvailable());
}


void CTutorialBrowser::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);
	

	if (m_hWnd) {
		CRect clientRect;
		CRect toolbarRect;
		GetClientRect(clientRect);
		m_wndToolBar.GetWindowRect(toolbarRect);
		m_wndBrowser.SetWindowPos(NULL, clientRect.left, toolbarRect.Height() + 20, clientRect.Width(), clientRect.Height() - toolbarRect.Height(), SWP_SHOWWINDOW);
	}



}

void CTutorialBrowser::GetBackList(_T_HistotyList& lst) const
{
	lst.RemoveAll();
	for (int i = m_iHistoryOffset + 1; i < m_arHistory.GetSize(); i++)
	{
		lst.AddTail(m_arHistory[i]);
	}
}

void CTutorialBrowser::GetFrwdList(_T_HistotyList& lst) const
{
	lst.RemoveAll();
	for (int i = m_iHistoryOffset - 1; i >= 0; i--)
	{
		ASSERT(i < m_arHistory.GetSize());
		lst.AddTail(m_arHistory[i]);
	}
}

CHistoryObj* CTutorialBrowser::AddURLToHistory(const CString& strTitle, const CString& strURL)
{
	ASSERT(m_arHistory.GetSize() <= HISTORY_LEN);

	for (int i = 0; i < m_arHistory.GetSize(); i++)
	{
		CHistoryObj* pObj = m_arHistory[i];
		ASSERT(pObj != NULL);

		if (pObj && pObj->GetTitle() == strTitle && pObj->GetURL() == strURL)
		{
			return pObj;
		}
	}

	if (m_arHistory.GetSize() == HISTORY_LEN)
	{
		delete m_arHistory[0];
		m_arHistory.RemoveAt(0);
	}

	CHistoryObj* pObj = new CHistoryObj(strTitle, strURL,
		FIRST_HISTORY_COMMAND + (UINT)m_arHistory.GetSize());
	m_arHistory.InsertAt(0, pObj);

	m_iHistoryOffset = 0;
	return pObj;
}


