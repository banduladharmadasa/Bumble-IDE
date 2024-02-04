// OptionDock.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "OptionDock.h"
#include "resource.h"
#include "MainFrm.h"
#include "BumbleEditView.h"
#include <strsafe.h>
#include <fstream>

#include "CMFCPropertyGridMultiFileProperty.h"
#include "ComType.h"



using namespace Gdiplus;
// COptionDock

IMPLEMENT_DYNAMIC(COptionDock, CDockablePane)

#define PREPARE_CASTED_VALUE() CString str = (LPCTSTR)(_bstr_t)pProp->GetValue();\
							 BOOL isYes = str.Compare(L"Yes") == 0;\
							 int castedVal = isYes ? 1 : 0;

COptionDock::COptionDock()
{
	if (languageMap.size() == 0) {
		InitLanguageMap();
	}
}

COptionDock::~COptionDock()
{


}


BEGIN_MESSAGE_MAP(COptionDock, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()



// COptionDock message handlers




void COptionDock::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndPropertyGrid.GetSafeHwnd()) {
		m_wndPropertyGrid.MoveWindow(0, 0, cx, cy);
	}
}


int COptionDock::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndPropertyGrid.Create(WS_CHILD | WS_VISIBLE, CRect(), this, ID_PROPERTGRID)) {

		return	-1;
	}

	m_wndPropertyGrid.EnableDescriptionArea();
	//m_wndPropertyGrid.SetVSDotNetLook();

	font.CreatePointFont(100, L"Arial");
	m_wndPropertyGrid.SetFont(&font);
	//m_wndPropertyGrid.SetCustomColors(RGB(255, 255, 255), RGB(0, 0, 0), RGB(43,43, 43), RGB(255, 255, 255), RGB(50, 150, 250), RGB(83, 83, 83), RGB(0,0,0));

	m_wndPropertyGrid.MarkModifiedProperties(TRUE);
	m_wndPropertyGrid.SetAlphabeticMode(FALSE);


	AddComponents();

	return 0;
}

LRESULT COptionDock::OnPropertyChanged(WPARAM, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	CMainFrame* pFrmMain = (CMainFrame*)theApp.GetMainWnd();

	CView *pView = pFrmMain->GetActiveView();
	if (!pView)
		return HRESULT();

	if (!pView->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
		return LRESULT();

	CBumbleEditView* pEditorView = (CBumbleEditView*)pView;

	switch ((int)pProp->GetData())
	{
	case idThemes:
	{
		CString strTheme = (LPCTSTR)(_bstr_t)pProp->GetValue();
		Config()->SetTheme(strTheme);
	}
	break;

	case idLanguage:
	{
		CString language = (LPCTSTR)(_bstr_t)pProp->GetValue();
		CString langPath = L"ace/mode/" + languageMap[language];
		pEditorView->OnUpdateLanguage(language, langPath);

	}
	break;

	case idFontName:
	{
		CString name = (LPCTSTR)(_bstr_t)pProp->GetValue();
		Config()->SetFontName(name);
	}
	break;

	case idFontSize:
	{
		CString strSize = (LPCTSTR)(_bstr_t)pProp->GetValue();
		int size = min(30, max(5, _wtoi(strSize.GetBuffer())));
		Config()->SetFontSize(size);

	}

	break;

	case idRelativeLineNumbers:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetHasRelativeLineNumbers(castedVal);
	}
		break;

	case idShowLineNumbers:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetShowLineNumbers(castedVal);
	}

	break;

	case idShowGutter: {
		PREPARE_CASTED_VALUE()
		Config()->SetShowGutter(castedVal);
	}
	break;

	case idShowPrintMargin:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetShowPrintMargin(castedVal);
	}
	break;

	case idShowTokenInfo:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetShowTokenInfo(castedVal);
	}
	break;

	case idHighlightActiveLine:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetHighlightActiveLine(castedVal);
	}
	break;

	case idHighligtSelectedWord:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetHighligtSelectedWord(castedVal);
	}
	break;

	case idEnableBehaviours:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetEnableBehaviours(castedVal);
	}
	break;

	case idEnableAutoCompletion:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetEnableAutoCompletion(castedVal);
	}
	break;

	case idEnableSnippets:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetEnableSnippets(castedVal);
	}
	break;

	case idEnableEmmet:
	{
		PREPARE_CASTED_VALUE()
		Config()->SetEnableEmmet(castedVal);
	}
	break;

	case idCgiInterpreter:
	{
		CString strInterpreter = (LPCTSTR)(_bstr_t)pProp->GetValue();
		Config()->SetCgiBinPath(strInterpreter);
	}
	break;


	case idListeningPort:
	{
		CString strPort = (LPCTSTR)(_bstr_t)pProp->GetValue();
		int nPort = _wtoi(strPort.GetBuffer());
		Config()->SetListeningPort(nPort);
	}
	break;


	case idCgiPattern:
	{
		CString strPattern = (LPCTSTR)(_bstr_t)pProp->GetValue();
		Config()->SetCgiPattern(strPattern);

	}
	break;


	case idIndexFiles:
	{
		CString strIndexFiles = (LPCTSTR)(_bstr_t)pProp->GetValue();
		Config()->SetIndexFiles(strIndexFiles);
	}
	break;


	case idRootDirectory:
	{
		CString strRootDir = (LPCTSTR)(_bstr_t)pProp->GetValue();
		Config()->SetRootDirectory(strRootDir);
	}
	break;




	}

	((CMainFrame*)theApp.GetMainWnd())->ReconfigAllEditors();
	return LRESULT();
}

void COptionDock::AddComponents()
{
	m_wndPropertyGrid.EnableHeaderCtrl(TRUE, _T("Option"), _T("Value"));
	m_wndPropertyGrid.SetVSDotNetLook(TRUE);

	AddTextEditorGroup();
	AddApplicationGroup();
	//AddCompileGroup();
	//AddScreenRecorderGroup();
}

void COptionDock::AddTextEditorGroup()
{
	CMFCPropertyGridProperty * pGroupTextEditor = new CMFCPropertyGridProperty(_T("Text Editor"));
	m_wndPropertyGrid.AddProperty(pGroupTextEditor);

	AddLanguageCombo(pGroupTextEditor);
	AddThemeCombo(pGroupTextEditor);
	AddFontNamesCombo(pGroupTextEditor);
	AddFontSizeCombo(pGroupTextEditor);
	AddYesNoCombo(pGroupTextEditor, _T("Show Line Numbers"), Config()->GetShowLineNumbers(), _T("Show/hide line numbers on the gutter"), idShowLineNumbers);
	AddYesNoCombo(pGroupTextEditor, _T("Relative Line Numbers"), Config()->GetHasRelativeLineNumbers(), _T("Show relative line numbers on the gutter"), idRelativeLineNumbers);
	/*AddYesNoCombo(pGroupTextEditor, _T("Show Indents"), Config()->GetShowIndents(), _T("Show/hide the indents guids"), idShowIndents);*/
	/*AddYesNoCombo(pGroupTextEditor, _T("Show Invisibles"), Config()->GetShowInvisibles(), _T("Show/hide the invisible symbols"), idShowInvisibles);*/
	AddYesNoCombo(pGroupTextEditor, _T("Show Gutter"), Config()->GetShowGutter(), _T("Show/hide the gutter"), idShowGutter);
	AddYesNoCombo(pGroupTextEditor, _T("Show Print Margin"), Config()->GetShowPrintMargin(), _T("Show/hide the print margin"), idShowPrintMargin);
	/*AddYesNoCombo(pGroupTextEditor, _T("Show Token Info"), Config()->GetShowTokenInfo(), _T("Show/hide the info when mouse hover on a token"), idShowTokenInfo);*/
	AddYesNoCombo(pGroupTextEditor, _T("Highlight Active Line"), Config()->GetHighlightActiveLine(), _T("Enable this feature to highlight the current line"), idHighlightActiveLine);
	AddYesNoCombo(pGroupTextEditor, _T("Highlight Selected Word"), Config()->GetHighligtSelectedWord(), _T("Enable this feature to highlight the current selection"), idHighligtSelectedWord);
	AddYesNoCombo(pGroupTextEditor, _T("Enable Behaviours"), Config()->GetEnableBehaviours(), _T("Allows Bumble automatically pairs special characters, like quotation marks, parenthesis, or brackets "), idEnableBehaviours);
	AddYesNoCombo(pGroupTextEditor, _T("Enable Auto Completion"), Config()->GetEnableAutoCompletion(), _T("Allows Bumble predicts the rest of a word you are typing"), idEnableAutoCompletion);
	//AddYesNoCombo(pGroupTextEditor, _T("Enable Snippets"), Config()->GetEnableSnippets(), _T("Allows Bumble to predicts a predefined code block according to your partially completed keyword"), idEnableSnippets);
	AddYesNoCombo(pGroupTextEditor, _T("Enable Emmet"), Config()->GetEnableEmmet(), _T("Allows Bumble to generate a block of code according to your keyword dynamically"), idEnableEmmet);

}



void COptionDock::AddCompileGroup()
{
	CMFCPropertyGridProperty* pGroupBuild = new CMFCPropertyGridProperty(_T("Build and Run"));
	m_wndPropertyGrid.AddProperty(pGroupBuild);

	CMFCPropertyGridFileProperty* compiler= new CMFCPropertyGridFileProperty(L"Compiler", TRUE, Config()->GetCgiBinPath(), L"exe", 6UL, 0,
		L"", idCompiler);
	pGroupBuild->AddSubItem(compiler);


	CMFCPropertyGridMultiFileProperty* sourceFileList = new CMFCPropertyGridMultiFileProperty(L"Compiler", TRUE, Config()->GetCgiBinPath(), L"exe", 6UL | OFN_ALLOWMULTISELECT, 0,
		L"", idsourceFileList);
	pGroupBuild->AddSubItem(sourceFileList);



	CMFCPropertyGridFileProperty* linker = new CMFCPropertyGridFileProperty(L"Linker", TRUE, Config()->GetCgiBinPath(), L"exe", 6UL | OFN_ALLOWMULTISELECT, 0,
		L"", idLinker);
	pGroupBuild->AddSubItem(linker);


	



	CString strPort;
	strPort.Format(L"%d", Config()->GetListeningPort());
	pGroupBuild->AddSubItem(new CMFCPropertyGridProperty(_T("Listening Port"), strPort, _T("Enter the listening port number"), idListeningPort, _T("dddd"), _T("____")));

	pGroupBuild->AddSubItem(new CMFCPropertyGridProperty(_T("CGI Pattern"), Config()->GetCGIPattern(), _T("Use the pattern to specify the files to be treated as CGI files"), idCgiPattern));

	pGroupBuild->AddSubItem(new CMFCPropertyGridProperty(_T("Index Files"), Config()->GetIndexFiles(), _T("Specify the files to be treated as the default files"), idIndexFiles));

	CMFCPropertyGridFileProperty* pFolderProp = new CMFCPropertyGridFileProperty(_T("Root Directory"), Config()->GetRootDirectory(), idRootDirectory, _T("Select a directory to become the root directory to serve the documents"));
	pGroupBuild->AddSubItem(pFolderProp);
}

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

void COptionDock::AddCodecCombo(CMFCPropertyGridProperty* pGroupScnRec)
{
	IWMProfileManager* pProfileMgr = NULL;
	
	HRESULT hr = WMCreateProfileManager(&pProfileMgr);
	if (FAILED(hr))
	{
		printf("Failed to Create a Profile Manager. ");

	}
	
	IWMCodecInfo3* pCodecInfo = NULL;

	
	hr = pProfileMgr->QueryInterface(IID_IWMCodecInfo3, (void**)&pCodecInfo);
	
	SAFE_RELEASE(pProfileMgr);

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Codec Name"), Config()->GetScreenRecorderCodec(), _T("Specifies the font name"), idScreenRecorderCodec);
	pGroupScnRec->AddSubItem(pProp);
	
	GetCodecNames(pCodecInfo, WMMEDIATYPE_Video, pProp);
	//GetCodecNames(pCodecInfo, WMMEDIATYPE_Audio);
	
	SAFE_RELEASE(pCodecInfo);
}

void COptionDock::AddScreenRecorderGroup()
{
	CMFCPropertyGridProperty* pGroupScnRec = new CMFCPropertyGridProperty(_T("Screen Recorder"));
	m_wndPropertyGrid.AddProperty(pGroupScnRec);

	AddCodecCombo(pGroupScnRec);


}

void COptionDock::AddFontNamesCombo(CMFCPropertyGridProperty * pGroup)
{
	/*CMFCPropertyGridFontProperty* pFntProp = new CMFCPropertyGridFontProperty(_T("Font"), theApp.m_edtFont, REGULAR_FONTTYPE, _T("Specifies the name style"), idFontName);

	pGroup->AddSubItem(pFntProp);*/

	CMFCPropertyGridProperty *pProp = new CMFCPropertyGridProperty(_T("Font Name"), Config()->GetFontName(), _T("Specifies the font name"), idFontName);

	FontFamily   fontFamily(L"Arial");
	Gdiplus::Font         font(&fontFamily, 8, FontStyleRegular, UnitPoint);


	INT          count = 0;
	INT          found = 0;
	WCHAR        familyName[LF_FACESIZE];  // enough space for one family name

	FontFamily*  pFontFamily = NULL;

	InstalledFontCollection installedFontCollection;

	// How many name families are installed?
	count = installedFontCollection.GetFamilyCount();

	// Allocate a buffer to hold the array of FontFamily
	// objects returned by GetFamilies.
	pFontFamily = new FontFamily[count];

	// Get the array of FontFamily objects.
	installedFontCollection.GetFamilies(count, pFontFamily, &found);


	for (INT j = 0; j < count; ++j)
	{
		pFontFamily[j].GetFamilyName(familyName);
		pProp->AddOption(familyName);
	}


	delete[] pFontFamily;

	pProp->AllowEdit(FALSE);

	pGroup->AddSubItem(pProp);
}

void COptionDock::AddFontSizeCombo(CMFCPropertyGridProperty * pGroup)
{
	CString strCurrent;
	strCurrent.Format(L"%d", Config()->GetFontSize());
	CMFCPropertyGridProperty *pProp = new CMFCPropertyGridProperty(_T("Font Size"), strCurrent, _T("Specifies the font size"), idFontSize);



	for (int i = 5; i < 31; i++) {
		CString str;
		str.Format(L"%d", i);
		pProp->AddOption(str);
	}

	pProp->AllowEdit(FALSE);

	pGroup->AddSubItem(pProp);
}




void COptionDock::AddYesNoCombo(CMFCPropertyGridProperty * pGroup, const CString &name, size_t val, const CString &prompt, int id)
{
	CMFCPropertyGridProperty *pProp = new CMFCPropertyGridProperty(name, val == 1 ? L"Yes" : L"No", prompt, id);

	pProp->AddOption(L"Yes");
	pProp->AddOption(L"No");

	pProp->AllowEdit(FALSE);

	pGroup->AddSubItem(pProp);
}

void COptionDock::AddApplicationGroup()
{
	CMFCPropertyGridProperty * pGroupServer = new CMFCPropertyGridProperty(_T("Server"));
	m_wndPropertyGrid.AddProperty(pGroupServer);

	CMFCPropertyGridFileProperty* cgiBinFile = new CMFCPropertyGridFileProperty(L"CGI Interpreter", TRUE, Config()->GetCgiBinPath(), L"exe", 6UL, 0,
		L"Select the Common Gateway Interpreter(Ex: For PHP this is the php-cgi.exe)", idCgiInterpreter);
	pGroupServer->AddSubItem(cgiBinFile);
	
	CString strPort;
	strPort.Format(L"%d", Config()->GetListeningPort());
	pGroupServer->AddSubItem(new CMFCPropertyGridProperty(_T("Listening Port"),strPort, _T("Enter the listening port number"), idListeningPort, _T("dddd"), _T("____")));

	pGroupServer->AddSubItem(new CMFCPropertyGridProperty(_T("CGI Pattern"), Config()->GetCGIPattern(), _T("Use the pattern to specify the files to be treated as CGI files"), idCgiPattern));

	pGroupServer->AddSubItem(new CMFCPropertyGridProperty(_T("Index Files"), Config()->GetIndexFiles(), _T("Specify the files to be treated as the default files"), idIndexFiles));

	CMFCPropertyGridFileProperty* pFolderProp = new CMFCPropertyGridFileProperty(_T("Root Directory"), Config()->GetRootDirectory(), idRootDirectory, _T("Select a directory to become the root directory to serve the documents"));
	pGroupServer->AddSubItem(pFolderProp);

}

void COptionDock::AddThemeCombo(CMFCPropertyGridProperty * pGroup)
{
	m_pCmbPropTheme = new CMFCPropertyGridProperty(_T("Theme"), Config()->GetTheme(), _T("Select one of the themes"), idThemes);

	for (int i = 0; i < TEXT_EDITOR_THEME_SIZE; i++)
		m_pCmbPropTheme->AddOption(lpszTextEditorThemes[i]);


	m_pCmbPropTheme->AllowEdit(FALSE);
	pGroup->AddSubItem(m_pCmbPropTheme);
}

//Enable following definition to generate a new xml config file
//#define GEN_XML_CONFIG_FILE
void COptionDock::AddLanguageCombo(CMFCPropertyGridProperty * pGroup)
{
	m_pCmbPropLanguage = new CMFCPropertyGridProperty(_T("Language"), L"JavaScript", _T("Select one of the languages"), idLanguage);
	
#ifdef GEN_XML_CONFIG_FILE
	std::ofstream myfile;
	myfile.open("runtime_config.xml");

	myfile << "<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n" <<
		"<bumble>\n";
#endif // GEN_XML_CONFIG_FILE



	

	for (auto it : languageMap) {
		m_pCmbPropLanguage->AddOption(it.first);

#ifdef GEN_XML_CONFIG_FILE
		CW2A _langMode(it.second);
		CW2A _langModeDesc(it.first);
		
		//Use this commented codes to generate xml config files

		myfile << "<!--" << _langModeDesc << "-->\n" <<
			"<config mode = \"" << _langMode << "\" >\n" <<
			
			"<executable></executable>\n" <<
			"<preoptions></preoptions>\n" <<
			"<postoptions></postoptions>\n" <<
			"<theme></theme>\n</config>\n" <<
			"\n\n";
		
#endif // GEN_XML_CONFIG_FILE
		
	}

#ifdef GEN_XML_CONFIG_FILE
	myfile << "</bumble>\n";
	myfile.close();
#endif // GEN_XML_CONFIG_FILE
	
	

	m_pCmbPropLanguage->AllowEdit(FALSE);
	pGroup->AddSubItem(m_pCmbPropLanguage);

}

void COptionDock::SetLanguage(const CString & language)
{
	COleVariant var = language;
	m_pCmbPropLanguage->SetValue(var);

}

void COptionDock::SetTheme(const CString & theme)
{
	COleVariant var = theme;
	m_pCmbPropTheme->SetValue(var);
}



