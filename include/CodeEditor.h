#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif

// CCodeEditor dialog

class CCodeEditor : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CCodeEditor)

public:
	CCodeEditor(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCodeEditor();
	void SetEditorFocus();

// Overrides


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CODE_EDITOR, IDH = 311 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
public:
	virtual void OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl);

	void OpenFile(const CString &file);
	void Reload();
	void OnFindResultReady();
	void OnStartFindAll(LPCTSTR options);
	void OnStartReplaceAll(LPCTSTR options);
	BOOL CallClientScript(LPCTSTR pStrFuncName, CStringArray * pArrFuncArgs, CComVariant * pOutVarRes);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	void ChangeTheme();
	void SetContent(const CString str);
	void AppendContent(const CString str);
	//STDMETHOD(GetExternal)(IDispatch** ppDispatch);
	CString m_myText;
	void OnGoToLine(int line, int col);
	void InsertMultiLineText(const CString& str, BOOL front=TRUE);
	void InsertMultiLineNumber(int start, int inc, int pad, const CString& format);
	CString GetGoToLineInfo();
	void SetFileName(const CString& fileName);
protected:
	CString m_editingFile;
	CString m_language;
	
	virtual BOOL IsExternalDispatchSafe() { return TRUE; }

public:
	BOOL ExecuteGlobalCommand(const CString &command);
	BOOL ExecuteEditorCommand(const CString &command, const CStringArray &args);


	void OnKeyDown(WPARAM wParmam, LPARAM lParam);
	void OnKeyUp(WPARAM wParmam, LPARAM lParam);

	BOOL GetCommandState(const CString &command);
	BOOL GetCommandEnabled(const CString &command);
	CString GetContent();

	BOOL CanUndo();
	BOOL CanRedo();

	void Undo();
	void Redo();

	CComVariant Eval(const CString &exp);
	int GetZoomLevel();
	void SetZoomLevel(int level);
	int GetEditorZoomLevel();


	virtual void OnCancel();
	virtual void OnOK();
	void SetLanguage(const CString &language, const CString &langpath);
	void SetLanguageByFileName(const CString &fileName);
	CString GetLanguage() const;

	CString GetCursorPos();

	void OnCodefold();
	void OnUnfold();
	void OnFoldAll();
	void OnUnfoldAll();
	void OnEditToggleComments();
	void OnEditToggleBlockComments();
	void OnAdvancedMakeUpperCase();
	void OnAdvancedMakeLowerCase();
	void OnIndent();
	void OnOutdent();

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void RegisterHotKeys();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	

	void InsertAtCursor(const CString &str);

};
