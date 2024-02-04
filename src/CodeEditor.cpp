// CodeEditor.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CodeEditor.h"
#include "BumbleEditView.h"
#include "BumbleEditDoc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <map>

#include <io.h>

#include "BumbleEdit.h"
#include "MainFrm.h"
#include "PopupMenuEx.h"
#include "resource.h"
#include "Tokenizer.h"
#include <Gdiplusimaging.h>
#include <shlwapi.h>
// CCodeEditor dialog

TCHAR GOTO_STRING[] = L"editor.resize(true); \neditor.scrollToLine(%d, true, true, function () {}); \neditor.gotoLine(%d, %d, true);";
TCHAR INSERT_MULTILINE_TEXT[] = L"var lines = editor.session.doc.getAllLines().length;\n"
L"var curPos=editor.getCursorPosition().column;\n"
L"for(i= editor.getCursorPosition().row; i < lines; i++){\n"
L"var lineLegth=editor.session.getLine(i).length;"
L"if(lineLegth < curPos){\n"
L"var adjust='';\n"
L"for(k = 0; k < (curPos - lineLegth); k++){adjust += ' ';}\n"
L"editor.session.insert({row:i, column:lineLegth}, adjust)\n"
L"}\n"
L"editor.session.insert({row:i, column:curPos}, '%s');\n"
"}";


TCHAR INSERT_MULTILINE_TEXT_BACK[] = L"var lines = editor.session.doc.getAllLines().length;\n"
L"for(i= editor.getCursorPosition().row; i < lines; i++){\n"
L"var lineLegth=editor.session.getLine(i).length;"
L"editor.session.insert({row:i, column:lineLegth}, '%s');\n"
"}";


TCHAR INSERT_MULTILINE_NUMBER_TEXT[] = L"Number.prototype.pad = function(size) {\n var s = String(this);\n while (s.length < (size || 2)) {s = '0' + s;}\n return s;\n }\n"
L"var curPos = editor.getCursorPosition().column;\n"
L"var start = %d;\n"
L"var inc = %d;\n"
L"\n"
L"var lines = editor.session.doc.getAllLines().length;\n"
L"for(i= editor.getCursorPosition().row; i < lines; i++){\n"
L"var lineLegth=editor.session.getLine(i).length;"
L"if(lineLegth < curPos){\n"
L"var adjust='';\n"
L"for(k = 0; k < (curPos - lineLegth); k++){adjust += ' ';}\n"
L"editor.session.insert({row:i, column:lineLegth}, adjust)\n"
L"}\n"
L"editor.session.insert({row:i, column:curPos}, (start).pad(%d));"
L"start += inc;"
L"}";

TCHAR SET_LANGUAGE_BY_FILE_NAME_TEXT[] = L"function getLangMode(){\n"
L"var modelist=ace.require('ace/ext/modelist');\n"
L"mode=modelist.getModeForPath('%s').mode;\n"
L"editor.session.setMode(mode);\n"
L"return mode;\n}\n"
L"getLangMode();\n";

IMPLEMENT_DYNCREATE(CCodeEditor, CDHtmlDialog)

CCodeEditor::CCodeEditor(CWnd* pParent)
	: CDHtmlDialog(IDD_CODE_EDITOR, NULL, pParent)
{
	//_-----------------------------------------------------------------



	CString strFileName;
	AfxGetModuleFileName(theApp.m_hInstance, strFileName);


	if (!PathRemoveFileSpec(strFileName.GetBuffer())) {
		BumbleMessageBox(L"Unable to extract the application path!");
		return;
	}




	CString str;
	str.Format(L"file:\\\\\\%s\\scripts\\index.html", strFileName);
	this->m_strCurrentUrl = str;







	//m_nHtmlResID = IDR_WEBPAGE;

	m_editingFile = L"";
	m_language = L"Text";
}

CCodeEditor::~CCodeEditor()
{

}

void CCodeEditor::SetEditorFocus()
{
	Eval(L"editor.focus();\n");

}



void CCodeEditor::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);

	CDHtmlDialog::DoDataExchange(pDX);
	DDX_DHtml_ElementInnerHtml(pDX, _T("find-results"), m_myText);
}

BOOL CCodeEditor::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();


	m_pBrowserApp->put_Silent(VARIANT_TRUE);

	RegisterHotKeys();


	EnableAutomation();
	LPDISPATCH pDisp = GetIDispatch(FALSE);
	SetExternalDispatch(pDisp);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCodeEditor::SetFileName(const CString& fileName)
{
	m_editingFile = fileName;
	CString script;
	script.Format(L"filePath = '%s';", fileName);
	script.Replace(L"\\", L"/");
	Eval(script);
}

void CCodeEditor::OpenFile(const CString& file)
{
	m_editingFile = file;

	CString script;
	script.Format(L"filePath = '%s';", file);
	script.Replace(L"\\", L"/");
	Eval(script);
}

void CCodeEditor::Reload()
{
	std::ifstream infile(m_editingFile);

	if (infile) {
		// construct string from iterator range
		std::string fileData = std::string(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
		CString text = CString(fileData.c_str());

		SetContent(text);

	}
	else {
		return;
	}

}

BEGIN_MESSAGE_MAP(CCodeEditor, CDHtmlDialog)

	ON_WM_CONTEXTMENU()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CCodeEditor)

END_DHTML_EVENT_MAP()


BEGIN_DISPATCH_MAP(CCodeEditor, CDHtmlDialog)
	DISP_FUNCTION(CCodeEditor, "OnFindResultReady", OnFindResultReady, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCodeEditor, "OnStartFindAll", OnStartFindAll, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CCodeEditor, "OnStartReplaceAll", OnStartReplaceAll, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()



/*
This is called by Javascript to inform that the searching result is ready  to be processed
*/
void CCodeEditor::OnFindResultReady()
{
	UpdateData(TRUE);

	CWnd* pWnd = (CMainFrame*)theApp.GetMainWnd()->GetDlgItem(IDD_OUTPUT_WND);
	if (pWnd) {
		CFindResultWnd* pWndFindRes = ((COutputWnd*)pWnd)->GetFindResultWnd();

		if (pWndFindRes) {
			pWndFindRes->AddResults(m_myText);

			((COutputWnd*)pWnd)->SetActiveTab(1);
		}

	}

	m_myText = L"";
	UpdateData(FALSE);
}

/*
This is called by Javascript when the user clicked the button
*/
void CCodeEditor::OnStartFindAll(LPCTSTR options)
{
	CWnd* pWnd = (CMainFrame*)theApp.GetMainWnd()->GetDlgItem(IDD_OUTPUT_WND);
	if (pWnd) {
		CFindResultWnd* pWndFindRes = ((COutputWnd*)pWnd)->GetFindResultWnd();

		if (pWndFindRes) {
			pWndFindRes->ClearResult();

			((COutputWnd*)pWnd)->SetActiveTab(1);
		}

	}

	theApp.SearchInAllFiles(options);
}
void CCodeEditor::OnStartReplaceAll(LPCTSTR options)
{
	theApp.ReplaceInAllFiles(options);
}
//-------------------------------------------------------------------------------------------------

void CCodeEditor::OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	// TODO: Add your specialized code here and/or call the base class

	CDHtmlDialog::OnBeforeNavigate(pDisp, szUrl);
}

BOOL CCodeEditor::CallClientScript(LPCTSTR pStrFuncName, CStringArray* pArrFuncArgs, CComVariant* pOutVarRes)
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



/// <summary>
/// To enclose each original script line with double qoutes
/// use Rgular Expressions as below:
/// find for ^(.+)$ and replace
/// with BumbleEdit: "$1\n" or
/// with Norepad++  "\1\\n"
/// </summary>
/// <param name="pDisp"></param>
/// <param name="szUrl"></param>
void CCodeEditor::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);



	CString text;
	if (!m_editingFile.IsEmpty()) {
		FILE* file;
		wchar_t* pszData;
		pszData = nullptr;

		if (_wfopen_s(&file, m_editingFile, L"r"))
		{
			return;
		}

		unsigned int current = ftell(file);
		fseek(file, 0, SEEK_END);
		unsigned int end = ftell(file);
		fseek(file, current, SEEK_SET);
		unsigned int lengthBytes = end - current;
		char* rawBytes = (char*)calloc(lengthBytes + 1, sizeof(char));

		if (rawBytes == nullptr)
		{
			return;
		}

		fread(rawBytes, sizeof(char), lengthBytes, file);
		if (fclose(file))
		{
			return;
		}

		pszData = (wchar_t*)calloc(lengthBytes + 1, sizeof(wchar_t));
		if (pszData == nullptr)
		{
			free(rawBytes);
			return;
		}

		if (MultiByteToWideChar(CP_UTF8, 0, rawBytes, lengthBytes + 1, pszData, lengthBytes + 1) == 0)
		{
			free(pszData);
			free(rawBytes);
			return;
		}


		text = CString(pszData);
		free(pszData);
		free(rawBytes);
	}





	//////////////////////////////////////////////////////////////////////////////////////////////////

	CStringArray arrArgs;
	CString theme = L"ace/theme/";
	theme.Append(theApp.GetConfig()->GetTheme());// .GetEditorThemeName());
	theme = theme.MakeLower();

	//Load main script
	CString script;
	CString _fileName = m_editingFile;
	_fileName.Replace(L"\\", L"/");
	script.Format(theApp.LoadScript(IDR_MAIN_SCRIPT), _fileName, theme);
	Eval(script);
	//Load search extension
	script = theApp.LoadScript(IDR_EXT_SEARCH);
	Eval(script);
	//Load polyfill
	script = theApp.LoadScript(IDR_POLYFILL);
	Eval(script);




	CBumbleEditView* pView = ((CBumbleEditView*)this->GetParent());

	if (pView) {

		CComVariant mode = Eval(L"mode");

		if (mode.vt != VT_BSTR) {

			BumbleMessageBox(L"Javascript error!");
			return;
		}

		CString strMode(mode.bstrVal);
		CString lastPart = strMode.Mid(strMode.ReverseFind('/') + 1);


		if (languageMap.size() == 0) {
			InitLanguageMap();
		}
		for (auto it : languageMap) {
			if (it.second.Compare(lastPart) == 0) {
				m_language = it.first;

				break;
			}
		}


		((CMainFrame*)AfxGetMainWnd())->m_wndOptionDockPane.SetLanguage(m_language);
		pView->ConfigEditor();
		//The xml files are named according to the values of the map
		pView->GetDocument()->LoadXmlConfigFile(lastPart);

	}



	if (!text.IsEmpty()) {
		CStringArray arrArgs;
		arrArgs.Add(text);
		CComVariant varRes;
		CallClientScript(L"setContent", &arrArgs, &varRes);
		Eval(L"editor.getSession().getUndoManager().reset();");
	}


	CString str = pView->GetPathName();
	this->SetFileName(str);


	UINT id = pView->GetDocument()->GetUniqueDocID();
	str.Format(L"uniqueDocId=%d;", id);
	Eval(str);

	Eval(L"editor.focus();");
	
	m_wndBrowser.RedrawWindow();//fix for the frozen area at the begining
}

CComVariant CCodeEditor::Eval(const CString& exp)
{
	CComPtr<IHTMLDocument2> pIDoc2;
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

	return 0;
}

int CCodeEditor::GetZoomLevel()
{
	return 0;
}

void CCodeEditor::SetZoomLevel(int level)
{
	CComVariant vZoom = level;
	m_pBrowserApp->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, &vZoom, NULL);
}

int CCodeEditor::GetEditorZoomLevel()
{
	VARIANT zoom;
	VariantInit(&zoom);
	zoom.vt = VT_I4;
	HRESULT hr = m_pBrowserApp->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, NULL, &zoom);
	if (FAILED(hr))
		return 100;

	return zoom.lVal;
}

void CCodeEditor::ChangeTheme()
{
	CStringArray arrArgs;
	CString theme = L"ace/theme/";
	theme.Append(theApp.GetConfig()->GetTheme()/*.GetEditorThemeName()*/);
	arrArgs.Add(theme.MakeLower());
	CComVariant varRes;
	//arrArgs.Add(_T("1"));//you can add value to CStringArray 
	//arrArgs.Add(_T("2"));//if your javascript function having any arguments
	CallClientScript(L"setTheme", &arrArgs, &varRes);
}

void CCodeEditor::SetContent(const CString str)
{
	CStringArray arrArgs;
	arrArgs.Add(str);
	CComVariant varRes;
	CallClientScript(L"setContent", &arrArgs, &varRes);
}

void CCodeEditor::AppendContent(const CString str)
{
	CString _str;
	_str.Format(L"var session = editor.session;\n session.insert({row: session.getLength(),column : 0}, '%s');", str);
	Eval(_str);
}

//STDMETHODIMP_(HRESULT __stdcall) CCodeEditor::GetExternal(IDispatch** ppDispatch)
//{
//
//	UpdateData(TRUE);
//
//	CWnd* pWnd = (CMainFrame*)theApp.GetMainWnd()->GetDlgItem(IDD_OUTPUT_WND);
//	if (pWnd) {
//		CFindResultWnd* pWndFindRes = ((COutputWnd*)pWnd)->GetFindResultWnd();
//
//		if (pWndFindRes) {
//			pWndFindRes->AddResults(m_myText);
//
//			((COutputWnd*)pWnd)->SetActiveTab(1);
//		}
//
//	}
//
//	m_myText = L"";
//	UpdateData(FALSE);
//	return E_ABORT;
//}

void CCodeEditor::OnGoToLine(int line, int col)
{
	CString str;

	str.Format(GOTO_STRING, line, line, col);
	Eval(str);
}

void CCodeEditor::InsertMultiLineText(const CString& str, BOOL front)
{
	CString temp;

	if (front) {
		temp.Format(INSERT_MULTILINE_TEXT, str);
	}
	else {
		temp.Format(INSERT_MULTILINE_TEXT_BACK, str);
	}




	Eval(temp);

}


void CCodeEditor::InsertMultiLineNumber(int start, int inc, int pad, const CString& format)
{
	CString temp;



	temp.Format(INSERT_MULTILINE_NUMBER_TEXT, start, inc, pad);// , format);
	Eval(temp);

}

CString CCodeEditor::GetGoToLineInfo()
{
	CString str = L"editor.session.doc.getAllLines().length;";

	CComVariant var = Eval(str);

	if (!var.intVal) {
		return CString();
	}

	CString tempStr;
	tempStr.Format(L"%d lines in this document.", var.intVal);
	return tempStr;
}


CString CCodeEditor::GetLanguage() const
{
	return m_language;
}


BOOL CCodeEditor::ExecuteGlobalCommand(const CString& command)
{

	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2)))  //Uses CDHtmlDialog as 'this'
	{
		CStringArray arrArgs;
		arrArgs.Add(L"");
		VARIANT_BOOL varRes;
		VARIANT val;
		val.uintVal = 0;

		ATL::CComBSTR _command = command;

		pIDoc2->execCommand(_command, VARIANT_FALSE, val, &varRes);

		return varRes == VARIANT_TRUE;
	}

	return FALSE;
}

BOOL CCodeEditor::ExecuteEditorCommand(const CString& command, const CStringArray& args)
{
	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2)))  //Uses CDHtmlDialog as 'this'
	{
		CStringArray arrArgs;
		/*arrArgs.Add(text);*/

		for (int i = 0; i < args.GetCount(); i++) {
			arrArgs.Add(args.GetAt(i));
		}

		CComVariant varRes = 0;
		CComVariant _command = command;

		CallClientScript(_command.bstrVal, &arrArgs, &varRes);

		return varRes.boolVal == VARIANT_TRUE;
	}



	//Get Editor Element
	//CComPtr<IHTMLElement> pEditorElement;
	//if (GetElement(_T("editor"), &pEditorElement) != S_OK)
	//	return FALSE;




	return FALSE;
}


///
/// \brief Returns a map of [DispId, Method Name] for the passed-in IDispatch object
///	std::map<long, std::wstring> functions;
///GetIDispatchMethods(pScriptDisp, functions);
///
HRESULT GetIDispatchMethods(_In_ IDispatch* pDisp,
	_Out_ std::map<long, std::wstring>& methodsMap)
{
	HRESULT hr = S_OK;

	CComPtr<IDispatch> spDisp(pDisp);
	if (!spDisp)
		return E_INVALIDARG;

	CComPtr<ITypeInfo> spTypeInfo;
	hr = spDisp->GetTypeInfo(0, 0, &spTypeInfo);
	if (SUCCEEDED(hr) && spTypeInfo)
	{
		TYPEATTR* pTatt = nullptr;
		hr = spTypeInfo->GetTypeAttr(&pTatt);
		if (SUCCEEDED(hr) && pTatt)
		{
			FUNCDESC* fd = nullptr;
			for (int i = 0; i < pTatt->cFuncs; ++i)
			{
				hr = spTypeInfo->GetFuncDesc(i, &fd);
				if (SUCCEEDED(hr) && fd)
				{
					CComBSTR funcName;
					spTypeInfo->GetDocumentation(fd->memid, &funcName, nullptr, nullptr, nullptr);
					if (funcName.Length() > 0)
					{
						methodsMap[fd->memid] = funcName;
					}

					spTypeInfo->ReleaseFuncDesc(fd);
				}
			}

			spTypeInfo->ReleaseTypeAttr(pTatt);
		}
	}

	return hr;

}







void CCodeEditor::OnKeyDown(WPARAM wParmam, LPARAM lParam)
{

}

void CCodeEditor::OnKeyUp(WPARAM wParmam, LPARAM lParam)
{


}

BOOL CCodeEditor::GetCommandState(const CString& command)
{
	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2))) {
		VARIANT_BOOL vOut = 0;
		CComVariant _command = command;
		pIDoc2->queryCommandState(_command.bstrVal, &vOut);

		return vOut == VARIANT_TRUE;
	}

	return FALSE;
}

BOOL CCodeEditor::GetCommandEnabled(const CString& command)
{

	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2))) {
		VARIANT_BOOL vOut = 0;
		CComVariant _command = command;
		pIDoc2->queryCommandEnabled(_command.bstrVal, &vOut);

		return vOut == VARIANT_TRUE;
	}

	return FALSE;
}

CString CCodeEditor::GetContent()
{

	CComPtr<IHTMLDocument2> pIDoc2;
	if (SUCCEEDED(this->GetDHtmlDocument(&pIDoc2)))  //Uses CDHtmlDialog as 'this'
	{
		CStringArray arrArgs;
		arrArgs.Add(L"editor.getValue()");
		CComVariant varRes = 0;
		CComVariant _command = L"eval";

		if (CallClientScript(_command.bstrVal, &arrArgs, &varRes))
			return varRes.bstrVal;


	}

	return CString();
}

BOOL CCodeEditor::CanUndo()
{
	CComVariant result = Eval(L"editor.session.getUndoManager().canUndo()");
	return (result.boolVal == VARIANT_TRUE);
}

BOOL CCodeEditor::CanRedo()
{
	CComVariant result = Eval(L"editor.session.getUndoManager().canRedo()");
	return (result.boolVal == VARIANT_TRUE);
}



void CCodeEditor::Undo()
{
	Eval(L"editor.undo()");
}

void CCodeEditor::Redo()
{
	Eval(L"editor.redo()");
}


void CCodeEditor::OnCancel()
{
	//Stop closing this (is a dialog) when the user
	//press Esc button

	//CDHtmlDialog::OnCancel();


}


void CCodeEditor::OnOK()
{
	//Stop closing this	(is a dialog) when the user
	//press Enter button

	//CDHtmlDialog::OnOK();

}

void CCodeEditor::SetLanguage(const CString& language, const CString& langpath)
{
	m_language = language;
	CString str;
	str.Format(L"editor.session.setMode('%s')", langpath);
	Eval(str);
}

void CCodeEditor::SetLanguageByFileName(const CString& fileName)
{
	CBumbleEditView* pView = ((CBumbleEditView*)this->GetParent());
	if (!pView)
		return;

	CString _fileName(fileName);
	_fileName.Replace(L"\\", L"/");
	CString str;
	str.Format(SET_LANGUAGE_BY_FILE_NAME_TEXT
		, _fileName);


	CComVariant mode = Eval(str);

	CString strMode(mode.bstrVal);
	CString lastPart = strMode.Mid(strMode.ReverseFind('/') + 1);


	if (languageMap.size() == 0) {
		InitLanguageMap();
	}
	for (auto it : languageMap) {
		if (it.second.Compare(lastPart) == 0) {
			m_language = it.first;

			break;
		}
	}


	((CMainFrame*)AfxGetMainWnd())->m_wndOptionDockPane.SetLanguage(m_language);
	pView->ConfigEditor();
	//The xml files are named according to the values of the map
	pView->GetDocument()->LoadXmlConfigFile(lastPart);


}

CString CCodeEditor::GetCursorPos()
{
	//CComVariant var = Eval(L"JSON.stringify(editor.selection.getCursor())");

	CComVariant var = Eval(L"editor.selection.getCursor().row");
	int row = var.intVal;
	row++;

	var = Eval(L"editor.selection.getCursor().column");
	int col = var.intVal;
	col++;

	CString str;
	str.Format(L"Ln: %d  Col:%d", row, col);


	return CString(str);
}

void CCodeEditor::OnCodefold()
{
	Eval(L"if(editor.getSelectedText().length > 0){editor.session.toggleFold()}");
}

void CCodeEditor::OnUnfold()
{
	Eval(L"editor.session.toggleFold()");
}

void CCodeEditor::OnFoldAll()
{
	Eval(L"editor.session.foldAll()");
}

void CCodeEditor::OnUnfoldAll()
{
	Eval(L"editor.session.unfold()");
}

void CCodeEditor::OnEditToggleComments()
{
	Eval(L"editor.toggleCommentLines()");
}

void CCodeEditor::OnEditToggleBlockComments()
{
	Eval(L"editor.toggleBlockComment()");
}

void CCodeEditor::OnAdvancedMakeUpperCase()
{
	Eval(L"editor.toUpperCase()");
}

void CCodeEditor::OnAdvancedMakeLowerCase()
{
	Eval(L"editor.toLowerCase()");
}

void CCodeEditor::OnIndent()
{
	Eval(L"editor.blockIndent()");
}

void CCodeEditor::OnOutdent()
{
	Eval(L"editor.blockOutdent()");
}


void CCodeEditor::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here


}

STDMETHODIMP_(HRESULT __stdcall) CCodeEditor::ShowContextMenu(DWORD dwID, POINT* ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_CODE_EDITOR_CTX));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (pPopup)
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
		pPopupMenu->SetAnimationType(CMFCPopupMenu::SLIDE);
		pPopupMenu->SetForceShadow(true);
		pPopupMenu->Create(this, ppt->x, ppt->y, pPopup->Detach());
	}

	return S_OK;
}





void CCodeEditor::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu* pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);


	CDHtmlDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}



void CCodeEditor::RegisterHotKeys()
{

	//if(RegisterHotKey(m_hWnd, 1, MOD_NOREPEAT, VK_TAB)) {}//ALT + L


}


BOOL CCodeEditor::PreTranslateMessage(MSG* pMsg)
{

	if ((pMsg->message == WM_LBUTTONDOWN) || (pMsg->message == WM_RBUTTONDOWN))
	{
		((CMainFrame*)theApp.GetMainWnd())->CloseMenu();
		theApp.GetMainWnd()->PostMessageW(WM_KEYDOWN, VK_ESCAPE, 0);
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			//Destroy inline editor, if available
			Eval(L"if(inlineEnv.running) inlineEnv.destroy();inlineEnv.running = false;");
			//return TRUE;
		}
	}


	//ace_search_field


	//if ((pMsg->message == WM_KEYDOWN) && (GetKeyState(VK_CONTROL) & 0x8000)) {

	//	//Refresh debug
	//	if (pMsg->wParam == 'R') {

	//		GetParent()->SendMessage(WM_COMMAND, ID_DEBUG_REFRESH);

	//		return TRUE;
	//	}

	//	if (pMsg->wParam == VK_OEM_COMMA) {
	//		return TRUE;
	//	}

	//	//Search box is focused;
	//	if ((pMsg->wParam == 'F') || (pMsg->wParam == 'H')) {
	//		Eval(L"var editFields = document.querySelectorAll('.ace_search_field');\neditFields.item(0).focus();");
	//	}

	//}


	if (pMsg->message == WM_SYSKEYDOWN) {
		if ((GetKeyState(VK_CONTROL) & 0x8000)) {


		}
		else if ((GetKeyState(VK_SHIFT) & 0x8000)) {
			switch (pMsg->wParam)
			{
			case 'L':
				OnUnfold();
				break;

			case 'O':
				OnUnfoldAll();
				break;
			default:
				break;
			}
		}
		else {


			switch (pMsg->wParam)
			{
			case 'L':
				OnCodefold();
				break;

			case 'O':
				OnFoldAll();
				break;

			default:
				theApp.GetMainWnd()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				return FALSE;
			}

		}

		return TRUE;
	}





	return CDHtmlDialog::PreTranslateMessage(pMsg);
}


void CCodeEditor::InsertAtCursor(const CString& str)
{
	CString _str;
	_str.Format(L"editor.session.insert(editor.getCursorPosition(), '%s')", str);
	Eval(_str);
}
