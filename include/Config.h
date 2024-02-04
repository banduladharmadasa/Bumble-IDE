#pragma once
class CConfig
{
public:
	CConfig();
	~CConfig();


	
	void SetTheme(const CString &theme);
	CString GetTheme();
	void SetFontName(const CString &name);
	CString GetFontName();
	void SetFontSize(int val);
	int GetFontSize();
	void SetHasRelativeLineNumbers(int val);
	int GetHasRelativeLineNumbers();
	void SetShowLineNumbers(int val);
	int GetShowLineNumbers();
	void SetShowIndents(int val);
	int GetShowIndents();
	void SetShowInvisibles(int val);
	int GetShowInvisibles();
	void SetShowGutter(int val);
	int GetShowGutter();
	void SetShowPrintMargin(int val);
	int GetShowPrintMargin();
	void SetShowTokenInfo(int val);
	int GetShowTokenInfo();
	void SetHighlightActiveLine(int val);
	int GetHighlightActiveLine();
	void SetHighligtSelectedWord(int val);
	int GetHighligtSelectedWord();
	void SetEnableBehaviours(int val);
	int GetEnableBehaviours();
	void SetEnableAutoCompletion(int val);
	void SetEnableSnippets(int val);
	void SetEnableEmmet(int val);
	int GetEnableAutoCompletion();
	int GetEnableSnippets();
	int GetEnableEmmet();
	void SetEnableWordWrap(int val);
	int GetEnableWordWrap();

	//Server
	void SetCgiBinPath(const CString &val);
	CString GetCgiBinPath();
	void SetListeningPort(int val);
	int GetListeningPort();
	void SetCgiPattern(const CString& val);
	CString GetCGIPattern();
	void SetIndexFiles(const CString& val);
	CString GetIndexFiles();
	void SetRootDirectory(const CString& val);
	CString GetRootDirectory();

	//Screen Capture
	void SetScreenRecorderSaveLocation(const CString &val);
	CString GetScreenRecorderSaveLocation();
	void SetScreenRecorderFileFormat(const CString& val);
	CString GetScreenRecorderFileFormat();
	void SetScreenRecorderCodec(const CString& val);
	CString GetScreenRecorderCodec();


	
	
};

