#pragma once
#include <map>

// COptionDock
#define Config theApp.GetConfig
class COptionDock : public CDockablePane
{
	DECLARE_DYNAMIC(COptionDock)

public:
	COptionDock();
	virtual ~COptionDock();

	CMFCPropertyGridCtrl m_wndPropertyGrid;
	CFont font;

protected:
	DECLARE_MESSAGE_MAP()

	enum AppProperty
	{
		idLanguage,
		idThemes,
		idFontName,
		idFontSize,
		idShowLineNumbers,
		idRelativeLineNumbers,
		idShowIndents,
		idShowInvisibles,
		idShowGutter,
		idShowPrintMargin,
		idShowTokenInfo,
		idHighlightActiveLine,
		idHighligtSelectedWord,
		idEnableBehaviours,
		idEnableAutoCompletion,
		idEnableSnippets,
		idEnableEmmet,
		idCgiInterpreter,
		idListeningPort,
		idCgiPattern,
		idIndexFiles,
		idRootDirectory,
		idCompiler,
		idLinker,
		idsourceFileList,
		idScreenRecorderSaveLocation,
		idScreenRecorderFileFormat,
		idScreenRecorderCodec,
	};



protected:
	CMFCPropertyGridProperty* m_pCmbPropLanguage;
	CMFCPropertyGridProperty* m_pCmbPropTheme;


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);

	void AddComponents();
	void AddTextEditorGroup();
	void AddCompileGroup();
	void AddCodecCombo(CMFCPropertyGridProperty* pGroupScnRec);
	void AddScreenRecorderGroup();
	void AddFontNamesCombo(CMFCPropertyGridProperty * pGroup);
	void AddFontSizeCombo(CMFCPropertyGridProperty * pGroup);

	void AddYesNoCombo(CMFCPropertyGridProperty * pGroup, const CString & name, size_t val, const CString & prompt, int id);
	void AddApplicationGroup();

	void AddThemeCombo(CMFCPropertyGridProperty * pGroup);
	void AddLanguageCombo(CMFCPropertyGridProperty * pGroup);


	void SetLanguage(const CString &language);
	void SetTheme(const CString &theme);
};



