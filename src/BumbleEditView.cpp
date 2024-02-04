// BumbleEditView.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "BumbleEditView.h"
#include "BumbleEditDoc.h"
#include "MainFrm.h"
#include "OptionDock.h"
#include "MFCPopupMenuEx.h"
#include "BlankOperations.h"
#include "CGotoLine.h"
#include <pathcch.h>

#include "CInsertAtPropSheet.h"
// CBumbleEditView

IMPLEMENT_DYNCREATE(CBumbleEditView, CRichEditView)

CBumbleEditView::CBumbleEditView()
{
	m_wordWrap = TRUE;
}

CBumbleEditView::~CBumbleEditView()
{
}

BEGIN_MESSAGE_MAP(CBumbleEditView, CRichEditView)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_PASTE, &CBumbleEditView::OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CBumbleEditView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CBumbleEditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CBumbleEditView::OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CBumbleEditView::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CBumbleEditView::OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, &CBumbleEditView::OnUpdateEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CBumbleEditView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_FIND, &CBumbleEditView::OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CBumbleEditView::OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_REPEAT, &CBumbleEditView::OnFindNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, &CBumbleEditView::OnUpdateFindNext)
	ON_COMMAND(ID_EDIT_REPLACE, &CBumbleEditView::OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CBumbleEditView::OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_UNDO, &CBumbleEditView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CBumbleEditView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CBumbleEditView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CBumbleEditView::OnUpdateEditRedo)

	ON_COMMAND(ID_EDIT_SHOWALLCHARACTERS, &CBumbleEditView::OnEditShowAllCharacters)
	ON_COMMAND(ID_EDIT_SHOWINDENTGUID, &CBumbleEditView::OnEditShowIndentGuids)
	ON_COMMAND(ID_EDIT_WORDWRAP, &CBumbleEditView::OnEditWordwrap)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WORDWRAP, &CBumbleEditView::OnUpdateEditWordwrap)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWALLCHARACTERS, &CBumbleEditView::OnUpdateEditShowallcharacters)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWINDENTGUID, &CBumbleEditView::OnUpdateEditShowIndentGuids)
	ON_COMMAND(ID_CODEFOLD_UNFOLD, &CBumbleEditView::OnUnfold)
	ON_COMMAND(ID_CODEFOLD_FOLDALL, &CBumbleEditView::OnFoldAll)
	ON_COMMAND(ID_CODEFOLD_UNFOLDALL, &CBumbleEditView::OnUnfoldAll)
	ON_COMMAND(ID_EDIT_TOGGLECOMMENTS, &CBumbleEditView::OnEditToggleComments)
	ON_COMMAND(ID_CODEFOLD_SELECTION, &CBumbleEditView::OnCodefold)
	ON_COMMAND(ID_EDIT_TOGGLEBLOCKCOMMENTS, &CBumbleEditView::OnEditToggleBlockComments)
	ON_COMMAND(ID_ADVANCED_MAKEUPPERCASE, &CBumbleEditView::OnAdvancedMakeUpperCase)
	ON_COMMAND(ID_ADVANCED_MAKELOWERCASE, &CBumbleEditView::OnAdvancedMakeLowerCase)
	ON_COMMAND(ID_WITHSELECTION_INDENT, &CBumbleEditView::OnIndent)
	ON_COMMAND(ID_WITHSELECTION_OUTDENT, &CBumbleEditView::OnOutdent)

	ON_COMMAND(ID_FILE_PRINT, &CBumbleEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBumbleEditView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE_COPY_AS, &CBumbleEditView::OnFileSaveCopyAs)
	ON_COMMAND(ID_FILE_SAVEALL, &CBumbleEditView::OnFileSaveall)
	ON_COMMAND(ID_FILE_CLOSEALL, &CBumbleEditView::OnFileCloseAll)
	ON_UPDATE_COMMAND_UI(ID_COLOREDITBOX, &CBumbleEditView::OnUpdateColorEditbox)
	ON_COMMAND(ID_COLORPICKER, &CBumbleEditView::OnColorpicker)
	ON_CONTROL_RANGE(BN_CLICKED, ID_COPY_AS_HTML, ID_COPY_AS_CMYK, OnColorFormatSelected)
	ON_CONTROL_RANGE(BN_CLICKED, ID_INSERT_AS_HTML, ID_INSERT_AS_CMYK, OnInsertColor)
	ON_COMMAND(ID_EDIT_FINDPREVIOUS, &CBumbleEditView::OnFindPrevious)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDPREVIOUS, &CBumbleEditView::OnUpdateFindPrevious)
	ON_COMMAND(RECONFIG_ALL_EDITORS, &CBumbleEditView::OnReconfigAllEditors)
	ON_COMMAND(ID_SANITIZE_REMOVELEADINGSPACES, &CBumbleEditView::OnSanitizeRemoveLeadingSpaces)
	ON_COMMAND(ID_SANITIZE_REMOVETRAILINGSPACES, &CBumbleEditView::OnSanitizeRemoveTrailingSpaces)
	ON_COMMAND(ID_SANITIZE_REMOVELEADINGTRAILINGSPACES, &CBumbleEditView::OnSanitizeRemoveLeadingAndTrailingSpaces)
	ON_COMMAND(ID_SANITIZE_REMOVEONLYEXTRASPACES, &CBumbleEditView::OnSanitizeRemoveOnlyExtraSpaces)
	ON_COMMAND(ID_SANITIZE_TRANSFORMTOASINGLELINE, &CBumbleEditView::OnSanitizeTransformToASingleLine)

	ON_COMMAND(ID_CHARACTERINCLUSIONS_STRINGINBACK, &CBumbleEditView::OnStringInBack)
	ON_COMMAND(ID_STRING_IN_FRONT, &CBumbleEditView::OnStringInFront)
	ON_COMMAND(ID_EDIT_GOTOLINE, &CBumbleEditView::OnEditGotoLine)
	ON_COMMAND(ID_DEBUG_RUN, &CBumbleEditView::OnDebugRun)
	ON_COMMAND(ID_RUN_TESTONBROWSER, &CBumbleEditView::OnRunTestOnBrowser)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_REFRESH, &CBumbleEditView::OnUpdateDebugRefresh)
	ON_COMMAND(ID_SERVER_START, &CBumbleEditView::OnServerStart)
	ON_COMMAND(ID_SERVER_STOP, &CBumbleEditView::OnServerStop)
	ON_UPDATE_COMMAND_UI(ID_SERVER_START, &CBumbleEditView::OnUpdateServerStart)
	ON_UPDATE_COMMAND_UI(ID_SERVER_STOP, &CBumbleEditView::OnUpdateServerStop)
	ON_COMMAND(ID_OPEN_INLINE_EDITOR, &CBumbleEditView::OnOpenInlineEditor)
	ON_COMMAND(ID_CODEEDITORCONTEXTMENU_BEAUTIFY, &CBumbleEditView::OnCodeeditorcontextmenuBeautify)
END_MESSAGE_MAP()


// CBumbleEditView drawing

void CBumbleEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc, RGB(255, 0, 0));
}

void CBumbleEditView::Eval(const CString& str)
{
	m_codeEditor.Eval(str);
}

// CBumbleEditView diagnostics

#ifdef _DEBUG
void CBumbleEditView::AssertValid() const
{
	CRichEditView::AssertValid();
}



#ifndef _WIN32_WCE
void CBumbleEditView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

#endif

CBumbleEditDoc* CBumbleEditView::GetDocument() const // non-debug version is inline
{
	if (!m_pDocument) {
		return nullptr;
	}

	if (!m_pDocument->IsKindOf(RUNTIME_CLASS(CBumbleEditDoc))) {
		return nullptr;
	}

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBumbleEditDoc)));
	return (CBumbleEditDoc*)m_pDocument;
}

#endif //_DEBUG


// CBumbleEditView message handlers
void CBumbleEditView::OpenFile(const CString& file)
{
	m_codeEditor.OpenFile(file);
}

CString CBumbleEditView::GetFileName()
{
	return GetDocument()->GetPathName();
}



void CBumbleEditView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_codeEditor.Create(IDD_CODE_EDITOR, this);
	m_codeEditor.ShowWindow(SW_SHOW);


	if (languageMap.size() == 0) {
		InitLanguageMap();
	}


	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));

}


void CBumbleEditView::OnSize(UINT nType, int cx, int cy)
{
	CRichEditView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_codeEditor.GetSafeHwnd())
		m_codeEditor.MoveWindow(0, 0, cx, cy);
}




void CBumbleEditView::OnEditPaste()
{
	m_codeEditor.ExecuteGlobalCommand(L"Paste");
}


void CBumbleEditView::OnEditCut()
{
	m_codeEditor.ExecuteGlobalCommand(L"Cut");
}


void CBumbleEditView::OnEditCopy()
{
	m_codeEditor.ExecuteGlobalCommand(L"Copy");
}


void CBumbleEditView::OnEditSelectAll()
{
	m_codeEditor.ExecuteGlobalCommand(L"selectAll");
}


void CBumbleEditView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_codeEditor.GetCommandEnabled(L"Cut"));
}


void CBumbleEditView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_codeEditor.GetCommandEnabled(L"Copy"));
}


void CBumbleEditView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_codeEditor.GetCommandEnabled(L"SelectAll"));
}


void CBumbleEditView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_codeEditor.GetCommandEnabled(L"Paste"));
}


void CBumbleEditView::OnEditFind()
{
	CStringArray args;
	args.Add(L"find");
	m_codeEditor.ExecuteEditorCommand(L"executeEditorCommand", args);	
}

void CBumbleEditView::OnFindPrevious()
{
	// TODO: Add your command handler code here
}

void CBumbleEditView::OnUpdateFindPrevious(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CBumbleEditView::OnUpdateEditFind(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CBumbleEditView::OnFindNext()
{
	CStringArray args;
	args.Add(L"editor.findNext()");
	m_codeEditor.ExecuteEditorCommand(L"eval", args);
}


void CBumbleEditView::OnUpdateFindNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CBumbleEditView::OnEditReplace()
{
	CStringArray args;
	args.Add(L"replace");
	m_codeEditor.ExecuteEditorCommand(L"executeEditorCommand", args);
}


void CBumbleEditView::OnUpdateEditReplace(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

const CString CBumbleEditView::GetPathName()
{
	CString str = this->GetDocument()->GetPathName();
	if (str.IsEmpty()) {
		str = this->GetDocument()->GetTitle();
	}

	return str;
}

CString CBumbleEditView::GetContent()
{
	return m_codeEditor.GetContent();
}



void CBumbleEditView::OnEditUndo()
{
	m_codeEditor.Undo();
}


void CBumbleEditView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	BOOL flag = m_codeEditor.CanUndo();
	GetDocument()->SetModifiedFlag(flag);
	pCmdUI->Enable(flag);
}


void CBumbleEditView::OnEditRedo()
{
	m_codeEditor.Redo();
}


void CBumbleEditView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_codeEditor.CanRedo());
}

void CBumbleEditView::SetZoomLevel(int zoomLevel)
{
	m_codeEditor.SetZoomLevel(zoomLevel);
}

int CBumbleEditView::GetEditorZoomLevel()
{
	return m_codeEditor.GetEditorZoomLevel();
}


void CBumbleEditView::OnEditShowAllCharacters()
{
	int val = theApp.GetConfig()->GetShowInvisibles();
	theApp.GetConfig()->SetShowInvisibles(val == 1 ? 0 : 1);

	OnShowInvisiblesChanged(val == 0 ? L"true" : L"false");
}


void CBumbleEditView::OnEditShowIndentGuids()
{
	int val = theApp.GetConfig()->GetShowIndents();
	theApp.GetConfig()->SetShowIndents(val == 1 ? 0 : 1);

	OnShowIndentGuidsChanged(val == 0 ? L"true" : L"false");
}


void CBumbleEditView::OnEditWordwrap()
{
	/*int val = theApp.GetConfig()->GetEnableWordWrap();
	theApp.GetConfig()->SetEnableWordWrap(val == 1 ? 0 : 1);

	OnEnableWordWrapChanged(val == 0 ? L"true" : L"false");*/
	m_wordWrap = !m_wordWrap;
	OnEnableWordWrapChanged(m_wordWrap ? L"true" : L"false");
}


void CBumbleEditView::OnUpdateEditWordwrap(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck(theApp.GetConfig()->GetEnableWordWrap());
	pCmdUI->SetCheck(m_wordWrap);

}


void CBumbleEditView::OnUpdateEditShowallcharacters(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(theApp.GetConfig()->GetShowInvisibles());
}


void CBumbleEditView::OnUpdateEditShowIndentGuids(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(theApp.GetConfig()->GetShowIndents());
}

void CBumbleEditView::OnUpdateTheme()
{
	m_codeEditor.ChangeTheme();
}

void CBumbleEditView::OnUpdateLanguage(const CString& language, const CString& langPath)
{
	m_codeEditor.SetLanguage(language, langPath);


	//CBumbleEditDoc* pdoc = (CBumbleEditDoc*)GetDocument();
	//if (pdoc) {
	//	pdoc->AddExtraMenu(language);
	//}

}

void CBumbleEditView::OnFontNameChanged(const CString& str)
{
	CString exp;
	exp.Format(L"editor.setOptions({ fontFamily: '%s'}); ", str);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnFontSizeChanged(size_t size)
{
	CString exp;
	exp.Format(L"editor.setOptions({ fontSize: '%dpx'}); ", size);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnEnableRelativeLineNumbers(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ relativeLineNumbers: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnShowLineNumbersChanged(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ showLineNumbers: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnShowIndentGuidsChanged(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ displayIndentGuides: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnShowInvisiblesChanged(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ showInvisibles: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnEnableWordWrapChanged(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ wrap: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnShowGutterChanged(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ showGutter: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnShowPrintMarginChanged(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ printMargin: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnShowTokenInfoChanged(const CString& val)
{

}

void CBumbleEditView::OnEnableHighlightActiveLine(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ highlightActiveLine: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnEnableHighlightSelectedWord(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ highlightSelectedWord: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnEnableBehaviours(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ behavioursEnabled: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnEnableAutoCompletionSelectedWord(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ enableLiveAutocompletion: %s}); ", val);
	m_codeEditor.Eval(exp);
}

void CBumbleEditView::OnEnableSnippets(const CString val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ enableSnippets: %s}); ", val);
	m_codeEditor.Eval(exp);	
}

void CBumbleEditView::OnEnableEmmet(const CString& val)
{
	CString exp;
	exp.Format(L"editor.setOptions({ enableEmmet: %s}); ", val);
	m_codeEditor.Eval(exp);
}


void CBumbleEditView::ConfigEditor()
{
	OnUpdateTheme();
	OnFontSizeChanged(Config()->GetFontSize());
	OnEnableRelativeLineNumbers(Config()->GetHasRelativeLineNumbers() == 1 ? L"true" : L"false");
	OnShowLineNumbersChanged(Config()->GetShowLineNumbers() == 1 ? L"true" : L"false");
	OnShowIndentGuidsChanged(Config()->GetShowIndents() == 1 ? L"true" : L"false");
	OnShowInvisiblesChanged(Config()->GetShowInvisibles() == 1 ? L"true" : L"false");
	OnShowGutterChanged(Config()->GetShowGutter() == 1 ? L"true" : L"false");
	OnShowPrintMarginChanged(Config()->GetShowPrintMargin() == 1 ? L"true" : L"false");
	OnShowTokenInfoChanged(Config()->GetShowTokenInfo() == 1 ? L"true" : L"false");
	OnEnableHighlightActiveLine(Config()->GetHighlightActiveLine() == 1 ? L"true" : L"false");
	OnEnableHighlightSelectedWord(Config()->GetHighligtSelectedWord() == 1 ? L"true" : L"false");
	OnEnableBehaviours(Config()->GetEnableBehaviours() == 1 ? L"true" : L"false");
	OnEnableAutoCompletionSelectedWord(Config()->GetEnableAutoCompletion() == 1 ? L"true" : L"false");
	//OnEnableSnippets(Config()->GetEnableSnippets() == 1 ? L"true": L"false");
	OnEnableEmmet(Config()->GetEnableEmmet() == 1 ? L"true": L"false");
}

CString CBumbleEditView::GetCursorPos()
{
	return m_codeEditor.GetCursorPos();
}

void CBumbleEditView::OnUnfold()
{
	m_codeEditor.OnUnfold();
}


void CBumbleEditView::OnFoldAll()
{
	m_codeEditor.OnFoldAll();
}


void CBumbleEditView::OnUnfoldAll()
{
	m_codeEditor.OnUnfoldAll();
}




void CBumbleEditView::OnEditToggleComments()
{
	m_codeEditor.OnEditToggleComments();
}


void CBumbleEditView::OnCodefold()
{
	m_codeEditor.OnCodefold();
}


void CBumbleEditView::OnEditToggleBlockComments()
{
	m_codeEditor.OnEditToggleBlockComments();
}


void CBumbleEditView::OnAdvancedMakeUpperCase()
{
	m_codeEditor.OnAdvancedMakeUpperCase();
}


void CBumbleEditView::OnAdvancedMakeLowerCase()
{
	m_codeEditor.OnAdvancedMakeLowerCase();
}


void CBumbleEditView::OnIndent()
{
	m_codeEditor.OnIndent();
}


void CBumbleEditView::OnOutdent()
{
	m_codeEditor.OnOutdent();
}



void CBumbleEditView::OnFilePrint()
{
	// TODO: Add your command handler code here

	CRichEditView::OnFilePrint();
}


void CBumbleEditView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CRichEditView::OnPrint(pDC, pInfo);
	m_codeEditor.SetWindowTextW(L"");
}


BOOL CBumbleEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CString content = m_codeEditor.GetContent();
	this->SetWindowText(content);

	return DoPreparePrinting(pInfo);
}


void CBumbleEditView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}




void CBumbleEditView::OnFileSaveCopyAs()
{
	CString content = m_codeEditor.GetContent();

	if (GetDocument()) {
		((CBumbleEditDoc*)GetDocument())->OnFileSaveCopyAs();
		m_codeEditor.SetWindowTextW(L"");
	}

}


void CBumbleEditView::OnFileSaveall()
{
	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate)
	{
		CDocTemplate* pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc)
		{
			CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
			if (pDoc) {
				pDoc->SaveModified();
			}
		}
	}
}


void CBumbleEditView::OnFileCloseAll()
{
	((CMainFrame*)AfxGetMainWnd())->CloseAllFiles();

	theApp.AddNewTextDoc();
}


void CBumbleEditView::OnUpdateColorEditbox(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);

}

void CopyToClipboard(HWND owner, const CString& w)
{
	if (OpenClipboard(owner))
	{
		HGLOBAL hgClipBuffer = nullptr;
		std::size_t sizeInWords = w.GetLength() + 1;
		std::size_t sizeInBytes = sizeInWords * sizeof(wchar_t);
		hgClipBuffer = GlobalAlloc(GHND | GMEM_SHARE, sizeInBytes);
		if (!hgClipBuffer)
		{
			CloseClipboard();
			return;
		}
		wchar_t* wgClipBoardBuffer = static_cast<wchar_t*>(GlobalLock(hgClipBuffer));
		wcscpy_s(wgClipBoardBuffer, sizeInWords, w.GetString());
		GlobalUnlock(hgClipBuffer);
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hgClipBuffer);
		CloseClipboard();
	}
}


void CBumbleEditView::OnColorFormatSelected(UINT nID)
{
	COLORREF color = CMFCColorMenuButton::GetColorByCmdID(ID_COLORPICKER);
	/*m_codeEditor.InsertAtCursor(GetHTMLColorString(color));*/

	//CopyToClipboard(m_hWnd, GetHTMLColorString(color));


	switch (nID)
	{
	case ID_COPY_AS_HTML:
		CopyToClipboard(m_hWnd, GetHTMLColorString(color));
		break;

	case ID_COPY_AS_RGB:
		CopyToClipboard(m_hWnd, GetRGBColorString(color));
		break;

	case ID_COPY_AS_HSL:
		CopyToClipboard(m_hWnd, GetHSLColorString(color));
		break;

	case ID_COPY_AS_HSV:
		CopyToClipboard(m_hWnd, GetHSVColorString(color));
		break;

	case ID_COPY_AS_CMYK:
		CopyToClipboard(m_hWnd, GetCMYKColorString(color));
		break;

	default:
		break;
	}



}

void CBumbleEditView::OnInsertColor(UINT nID)
{
	COLORREF color = CMFCColorMenuButton::GetColorByCmdID(ID_COLORPICKER);
	CString str= L"";
	switch (nID)
	{
	case ID_INSERT_AS_HTML:
		str = GetHTMLColorString(color);
		break;

	case ID_INSERT_AS_RGB:
		str = GetRGBColorString(color);
		break;

	case ID_INSERT_AS_HSL:
		str = GetHSLColorString(color);
		break;

	case ID_INSERT_AS_HSV:
		str = GetHSVColorString(color);
		break;

	case ID_INSERT_AS_CMYK:
		str = GetCMYKColorString(color);
		break;

	default:
		break;
	}

	if (!str.IsEmpty()) {
		m_codeEditor.InsertAtCursor(str);
	}
}

void CBumbleEditView::OnColorpicker()
{
	COLORREF color = CMFCColorMenuButton::GetColorByCmdID(ID_COLORPICKER);	
	((CMainFrame*)theApp.GetMainWnd())->OnColorPick(color);



	POINT p;
	if (!::GetCursorPos(&p))
	{
		p.x = 100;
		p.y = 100;
	}


	//Notify user that the color is in the clipboard
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MESSAGE));

	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);

	if (pPopup)
	{
		CMFCPopupMenuEx* pPopupMenu = new CMFCPopupMenuEx;
		pPopupMenu->EnableMenuLogo(30);
		pPopupMenu->SetAnimationType(CMFCPopupMenu::SLIDE);
		pPopupMenu->SetForceShadow(true);
		pPopupMenu->Create(this, p.x, p.y, pPopup->Detach());
	}

}


//BOOL CBumbleEditView::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	/*if ((pMsg->message == WM_KEYDOWN) || (pMsg->message == WM_KEYUP) ||(pMsg->message == WM_SYSKEYDOWN) || (pMsg->message ==  WM_SYSKEYUP)) {
//
//		return FALSE;
//	}*/
//
//	
//	
//
//	return CRichEditView::PreTranslateMessage(pMsg);
//}

void CBumbleEditView::OnReconfigAllEditors()
{
	ConfigEditor();
}


void CBumbleEditView::OnSanitizeRemoveLeadingSpaces()
{
	CBlankOperations op(m_codeEditor.GetContent());
	op.RemoveLeadingSpaces();
	m_codeEditor.SetContent(op.GetResult());
}


void CBumbleEditView::OnSanitizeRemoveTrailingSpaces()
{
	CBlankOperations op(m_codeEditor.GetContent());
	op.RemoveTrailingSpaces();
	m_codeEditor.SetContent(op.GetResult());
}


void CBumbleEditView::OnSanitizeRemoveLeadingAndTrailingSpaces()
{
	CBlankOperations op(m_codeEditor.GetContent());
	op.RemoveLeadingAndTrailingSpaces();
	m_codeEditor.SetContent(op.GetResult());
}


void CBumbleEditView::OnSanitizeRemoveOnlyExtraSpaces()
{
	CBlankOperations op(m_codeEditor.GetContent());
	op.RemoveOnlyExtraSpaces();
	m_codeEditor.SetContent(op.GetResult());
}


void CBumbleEditView::OnSanitizeTransformToASingleLine()
{
	CBlankOperations op(m_codeEditor.GetContent());
	op.TransformToASingleLine();
	m_codeEditor.SetContent(op.GetResult());
}




void CBumbleEditView::OnStringInBack()
{
	CDHtmlDialog* pDlg = new CDHtmlDialog(IDD_TEXTINSERTDIALOG, IDR_TEXT_INSERT, this);

	pDlg->DoModal();

	delete pDlg;

}


void CBumbleEditView::OnStringInFront()
{
	CInsertAtPropSheet* pPropSheet = new CInsertAtPropSheet();// , IDB_ICONS_SMALL, 16);
	pPropSheet->m_psh.dwFlags |= PSH_NOAPPLYNOW;

	pPropSheet->EnablePageHeader(0);
	pPropSheet->DoModal();

	delete pPropSheet;
}


void CBumbleEditView::OnEditGotoLine()
{
	CGotoLine dlg;
	dlg.DoModal();
}

CCodeEditor* CBumbleEditView::GetCodeEditor()
{
	return &m_codeEditor;
}

BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CBumbleEditView::OnDebugRun()
{
	if (m_codeEditor.GetLanguage().Compare(L"HTML") == 0) {

		//OnRunTestOnBrowser();
		return;
	}

	if (!GetDocument()->DoFileSave()) {
		return;
	}

	CString fileName = GetDocument()->GetPathName();

	//try first for bat file
	CString batfile(fileName);
	PathCchRemoveFileSpec(batfile.GetBuffer(), batfile.GetLength());
	batfile.UnlockBuffer();

	CString fullBatchPath;
	fullBatchPath.Format(L"%s\\%s", batfile, L"run.bat");
	//batfile.Append(L"/run.bat");

	if (FileExists(fullBatchPath)) {
		theApp.ClearMessages();
		theApp.AddMessage(L"Executing via the batch file..\n", AppMessageType::AppMessageType_Info);
		theApp.RunAction(fullBatchPath);
		
		return;
	}




	GetDocument()->LoadXmlConfigFile(m_codeEditor.GetLanguage());
	CString strExecutable = m_executableEnv.executable;
	CString strPreOptions = m_executableEnv.preOptions;
	CString strPostOptions = m_executableEnv.postOptions;

	

	CString fullExeString = strExecutable + L" " + strPreOptions + L" \"" + fileName + L"\" " + strPostOptions;

	



	theApp.AddMessage(fullExeString, AppMessageType::AppMessageType_Info);


	theApp.RunAction(fullExeString);

	theApp.ClearMessages();
}




void CBumbleEditView::OnRunTestOnBrowser()
{
	CString pathName = GetDocument()->GetPathName();

	if (!GetDocument()->DoFileSave()) {
		return;
	}

	pathName = GetDocument()->GetPathName();

	if (!theApp.m_bServerRunning) {

		if (IDCANCEL == BumbleMessageBox(L"The server is not running.\n The action called requires the server to be run.  Do you want to run the server?")) {
			return;
		}

		CString tempStr = pathName;
		PathCchRemoveFileSpec(tempStr.GetBuffer(), tempStr.GetLength());


		if (!theApp.m_pServerManager->Start(tempStr)) {
			return;
		}
	}


	m_codeEditor.Eval(L" if(typeof logWindow !== 'object'){\nvar logWindow = window.open();}\n");


	CString fileName = pathName.Mid(pathName.ReverseFind('\\') + 1);



	int port = theApp.GetConfig()->GetListeningPort();
	CString strPort;

	strPort.Format(L"%d", port);

	if (strPort.GetLength() != 4) {
		BumbleMessageBox(L"The server listening port is incorrect!");
		return;
	}





	fileName.Insert(0, L"logWindow.location.href='http://127.0.0.1:" + strPort + L"//");
	fileName.Append(L"'");

	m_codeEditor.Eval(fileName);
}


void CBumbleEditView::OnUpdateDebugRefresh(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_codeEditor.GetLanguage().Compare(L"HTML") == 0) || (m_codeEditor.GetLanguage().Compare(L"PHP") == 0));
}


void CBumbleEditView::OnServerStart()
{
	CString pathName = GetDocument()->GetPathName();

	if (!GetDocument()->DoFileSave()) {
		return;
	}	

	pathName = GetDocument()->GetPathName();

	if (S_OK == PathCchRemoveFileSpec(pathName.GetBuffer(), pathName.GetLength())) {

		pathName.ReleaseBuffer();

		if (!theApp.m_pServerManager->Start(pathName)) {
			return;
		}
	}
	else {
		theApp.ThrowLastError(L"");
	}

}


void CBumbleEditView::OnServerStop()
{
	theApp.StopServer();
}


void CBumbleEditView::OnUpdateServerStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!theApp.m_bServerRunning);
}


void CBumbleEditView::OnUpdateServerStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.m_bServerRunning);
}


void CBumbleEditView::OnOpenInlineEditor()
{
	CString script;
	script = L"if(inlineEnv.running === false) editor.execCommand('openInlineEditor')";
	m_codeEditor.Eval(script);
}


void CBumbleEditView::OnCodeeditorcontextmenuBeautify()
{
	CString lang = m_codeEditor.GetLanguage();
	CString script;

	if ((lang.Compare(L"HTML") == 0) || (lang.Compare(L"PHP") == 0)) {
		script = L" var val = editor.getValue();var array = val.split(/\\n/);array[0] = array[0].trim();val = array.join('\\n');val = html_beautify(val);editor.setValue(val);";
	}
	else if (lang.Compare(L"JavaScript") == 0) {
		script = L" var val = editor.getValue();var array = val.split(/\\n/);array[0] = array[0].trim();val = array.join('\\n');val = js_beautify(val);editor.setValue(val);";
	}

	else if (lang.Compare(L"CSS") == 0) {
		script = L" var val = editor.getValue();var array = val.split(/\\n/);array[0] = array[0].trim();val = array.join('\\n');val = css_beautify(val);editor.setValue(val);";
	}

	if (!script.IsEmpty()) {
		m_codeEditor.Eval(script);
	}
}

BOOL CBumbleEditView::SearchWord(const CString& options)
{
	
	CString script;
	script.Format(L"hyper.listAllSearch(%s);", options);
	
	Eval(script);

	return TRUE;
}

BOOL CBumbleEditView::ReplaceAllInFile(const CString& options)
{
	CString script;
	script.Format(L"var options = %s;editor.searchBox.replaceInput.value = options.replacement;\nhyper.replaceAllInFile(options); options.needle = options.replacement;hyper.listAllSearch(options)", options);

	Eval(script);

	

	return TRUE;
}


void CBumbleEditView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	GetCodeEditor()->Eval(L"editor.focus();");
	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
