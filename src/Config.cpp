#include "stdafx.h"
#include "Config.h"
#include "BumbleEdit.h"

CConfig::CConfig()
{
	
	
}


CConfig::~CConfig()
{
}

void CConfig::SetTheme(const CString & theme)
{
	theApp.WriteProfileStringW(L"Profile", L"Theme", theme);
}

CString CConfig::GetTheme()
{
	return  theApp.GetProfileStringW(L"Profile", L"Theme", L"Tomorrow");
}

void CConfig::SetFontName(const CString & name)
{
	theApp.WriteProfileStringW(L"Profile", L"Font Name", name);
}

CString CConfig::GetFontName()
{
	return theApp.GetProfileStringW(L"Profile", L"Font Name", L"Arial");
}

void CConfig::SetFontSize(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Font Size", val);
}

int CConfig::GetFontSize()
{
	return theApp.GetProfileIntW(L"Profile", L"Font Size", 14);
}

void CConfig::SetHasRelativeLineNumbers(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Has Relative Line Numbers", val);
}

int CConfig::GetHasRelativeLineNumbers()
{
	return theApp.GetProfileIntW(L"Profile", L"Has Relative Line Numbers", 0);
}

void CConfig::SetShowLineNumbers(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Show Line Numbers", val);
}

int CConfig::GetShowLineNumbers()
{
	return theApp.GetProfileIntW(L"Profile", L"Show Line Numbers", 1);
}

void CConfig::SetShowIndents(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Show Indents", val);
}

int CConfig::GetShowIndents()
{
	return theApp.GetProfileIntW(L"Profile", L"Show Indents", 1);
}

void CConfig::SetShowInvisibles(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Show Invisibles", val);
}

int CConfig::GetShowInvisibles()
{
	return theApp.GetProfileIntW(L"Profile", L"Show Invisibles", 0);
}

void CConfig::SetShowGutter(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Show Gutter", val);
}

int CConfig::GetShowGutter()
{
	return theApp.GetProfileIntW(L"Profile", L"Show Gutter", 1);
}

void CConfig::SetShowPrintMargin(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Show Print Marging", val);
}

int CConfig::GetShowPrintMargin()
{
	return theApp.GetProfileIntW(L"Profile", L"Show Print Marging", 1);
}

void CConfig::SetShowTokenInfo(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Show Token Info", val);
}

int CConfig::GetShowTokenInfo()
{
	return theApp.GetProfileIntW(L"Profile", L"Show Token Info", 0);
}

void CConfig::SetHighlightActiveLine(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Highlight Active Line", val);
}

int CConfig::GetHighlightActiveLine()
{
	return theApp.GetProfileIntW(L"Profile", L"Highlight Active Line", 1);
}

void CConfig::SetHighligtSelectedWord(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Highligt Selected Word", val);
}

int CConfig::GetHighligtSelectedWord()
{
	return theApp.GetProfileIntW(L"Profile", L"Highligt Selected Word", 1);
}

void CConfig::SetEnableBehaviours(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Enable Behaviours", val);
}

int CConfig::GetEnableBehaviours()
{
	return theApp.GetProfileIntW(L"Profile", L"Enable Behaviours", 1);
}

void CConfig::SetEnableAutoCompletion(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Enable Auto Completion", val);
}

void CConfig::SetEnableSnippets(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Enable Snippets", val);
}

void CConfig::SetEnableEmmet(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Enable Emmet", val);
}


int CConfig::GetEnableAutoCompletion()
{
	return theApp.GetProfileIntW(L"Profile", L"Enable Auto Completion", 1);
}

int CConfig::GetEnableSnippets()
{
	return theApp.GetProfileIntW(L"Profile", L"Enable Snippets", 1);
}

int CConfig::GetEnableEmmet()
{
	return theApp.GetProfileIntW(L"Profile", L"Enable Emmet", 1);
}

void CConfig::SetEnableWordWrap(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Enable Word Wrap", val);
}


int CConfig::GetEnableWordWrap()
{
	return theApp.GetProfileIntW(L"Profile", L"Enable Word Wrap", 0);
}

void CConfig::SetCgiBinPath(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"CGI Bin Path", val);
}

CString CConfig::GetCgiBinPath()
{
	return theApp.GetProfileString(L"Profile", L"CGI Bin Path", L"");
}

void CConfig::SetListeningPort(int val)
{
	theApp.WriteProfileInt(L"Profile", L"Listening Port", val);
}

int CConfig::GetListeningPort()
{
	return theApp.GetProfileInt(L"Profile", L"Listening Port", 8080);
}

void CConfig::SetCgiPattern(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"CGI Pattern", val);
}

CString CConfig::GetCGIPattern()
{
	return theApp.GetProfileStringW(L"Profile", L"CGI Pattern", L"**.cgi$|**.pl$|**.php$");
}

void CConfig::SetIndexFiles(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"Index Files", val);
}

CString CConfig::GetIndexFiles()
{
	return theApp.GetProfileStringW(L"Profile", L"Index Files", L"index.xhtml,index.html,index.htm,index.cgi,index.shtml,index.php");
}

void CConfig::SetRootDirectory(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"Root Directory", val);
}

CString CConfig::GetRootDirectory()
{
	return theApp.GetProfileStringW(L"Profile", L"Root Directory", L"");
}

void CConfig::SetScreenRecorderSaveLocation(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"Screen Recorder Save Location", val);
}

CString CConfig::GetScreenRecorderSaveLocation()
{
	return theApp.GetProfileStringW(L"Profile", L"Screen Recorder Save Location", L"");
}

void CConfig::SetScreenRecorderFileFormat(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"Screen Recorder File Format", val);
}

CString CConfig::GetScreenRecorderFileFormat()
{
	return theApp.GetProfileStringW(L"Profile", L"Screen Recorder File Format", L"wmv");
}

void CConfig::SetScreenRecorderCodec(const CString& val)
{
	theApp.WriteProfileStringW(L"Profile", L"Screen Recorder Codec", val);
}

CString CConfig::GetScreenRecorderCodec()
{
	return theApp.GetProfileStringW(L"Profile", L"Screen Recorder Codec", L"");
}




