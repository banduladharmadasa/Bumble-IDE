#pragma once
#include "CodeEditor.h"
class CBumbleEditDoc;
// CBumbleEditView view

struct ExecutableEnv
{
	CString executable;
	CString preOptions;
	CString postOptions;
};

class CBumbleEditView : public CRichEditView
{
	DECLARE_DYNCREATE(CBumbleEditView)

public:
	CBumbleEditView(); // protected constructor used by dynamic creation
	virtual ~CBumbleEditView();
	void OpenFile(const CString &file);
	CString GetFileName();

public:
	virtual void OnDraw(CDC *pDC); // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext &dc) const;

#endif
#endif

	void Eval(const CString &str);

protected:
	DECLARE_MESSAGE_MAP()

	CCodeEditor m_codeEditor;
	BOOL m_wordWrap;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);
	afx_msg void OnFindNext();
	afx_msg void OnUpdateFindNext(CCmdUI *pCmdUI);
	afx_msg void OnFindPrevious();
	afx_msg void OnUpdateFindPrevious(CCmdUI *pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI *pCmdUI);
	const CString GetPathName();

	CString GetContent();

	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);

	void SetZoomLevel(int strZoom);
	int GetEditorZoomLevel();
	afx_msg void OnEditShowAllCharacters();
	afx_msg void OnEditShowIndentGuids();
	afx_msg void OnEditWordwrap();
	afx_msg void OnUpdateEditWordwrap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditShowallcharacters(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditShowIndentGuids(CCmdUI *pCmdUI);

	/// <summary>
	///
	/// </summary>
	void OnUpdateTheme();
	void OnUpdateLanguage(const CString &language, const CString &langPath);
	void OnFontNameChanged(const CString &str);
	void OnFontSizeChanged(size_t size);
	void OnEnableRelativeLineNumbers(const CString &val);
	void OnShowLineNumbersChanged(const CString &val);
	void OnShowIndentGuidsChanged(const CString &val);
	void OnShowInvisiblesChanged(const CString &val);
	void OnEnableWordWrapChanged(const CString &val);
	void OnShowGutterChanged(const CString &val);
	void OnShowPrintMarginChanged(const CString &val);
	void OnShowTokenInfoChanged(const CString &val);
	void OnEnableHighlightActiveLine(const CString &val);
	void OnEnableHighlightSelectedWord(const CString &val);
	void OnEnableBehaviours(const CString &val);
	void OnEnableAutoCompletionSelectedWord(const CString &val);
	void OnEnableSnippets(const CString val);
	void OnEnableEmmet(const CString &val);

	void ConfigEditor();
	//===============================================================================

	CString GetCursorPos();

	afx_msg void OnUnfold();
	afx_msg void OnFoldAll();
	afx_msg void OnUnfoldAll();

	afx_msg void OnEditToggleComments();
	afx_msg void OnCodefold();
	afx_msg void OnEditToggleBlockComments();
	afx_msg void OnAdvancedMakeUpperCase();
	afx_msg void OnAdvancedMakeLowerCase();
	afx_msg void OnIndent();
	afx_msg void OnOutdent();

	afx_msg void OnFilePrint();
	virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo *pInfo);
	afx_msg void OnFilePrintPreview();

	afx_msg void OnFileSaveCopyAs();
	afx_msg void OnFileSaveall();
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateColorEditbox(CCmdUI *pCmdUI);
	afx_msg void OnColorFormatSelected(UINT nID);
	afx_msg void OnInsertColor(UINT nID);
	afx_msg void OnColorpicker();
	// virtual BOOL PreTranslateMessage(MSG* pMsg);

	CBumbleEditDoc *GetDocument() const;

	afx_msg void OnReconfigAllEditors();

	afx_msg void OnSanitizeRemoveLeadingSpaces();
	afx_msg void OnSanitizeRemoveTrailingSpaces();
	afx_msg void OnSanitizeRemoveLeadingAndTrailingSpaces();
	afx_msg void OnSanitizeRemoveOnlyExtraSpaces();
	afx_msg void OnSanitizeTransformToASingleLine();
	afx_msg void OnStringInBack();
	afx_msg void OnStringInFront();
	afx_msg void OnEditGotoLine();

	CCodeEditor *GetCodeEditor();
	afx_msg void OnDebugRun();

	ExecutableEnv m_executableEnv;
	afx_msg void OnRunTestOnBrowser();
	afx_msg void OnUpdateDebugRefresh(CCmdUI *pCmdUI);
	afx_msg void OnServerStart();
	afx_msg void OnServerStop();
	afx_msg void OnUpdateServerStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServerStop(CCmdUI *pCmdUI);
	afx_msg void OnOpenInlineEditor();
	afx_msg void OnCodeeditorcontextmenuBeautify();

	BOOL SearchWord(const CString &options);
	BOOL ReplaceAllInFile(const CString &options);
	virtual void OnActivateView(BOOL bActivate, CView *pActivateView, CView *pDeactiveView);
};

#ifndef _DEBUG // debug version in TextAppView.cpp
inline CBumbleEditDoc *CBumbleEditView::GetDocument() const
{
	return reinterpret_cast<CBumbleEditDoc *>(m_pDocument);
}
#endif