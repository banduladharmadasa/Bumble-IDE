
// BumbleEdit.h : main header file for the BumbleEdit application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ServerManager.h"
#include <string>
#include "Config.h"
#include "CPluginManager.h"
#include "DirectoryWatcher.h"
#include "TutorialBrowser.h"

class CShellThread;
class CBumbleEditDoc;
class CShellUtility;
class CMainFrame;
class CBumbleEditView;
class CCodeEditor;



// CBumbleEditApp:
// See BumbleEdit.cpp for the implementation of this class
//

class CBumbleEditApp : public CWinAppEx
{
public:
	void OnShellExited(WPARAM wparam, LPARAM lparam);
	CBumbleEditApp() noexcept;

	void OnFileMonitorFileChanged(WPARAM wparam, LPARAM lparam);

	void InitFont();
	void OnFileNewProject();

	BOOL HaveAlreadyOpened();

	virtual BOOL InitInstance();
	void AddDirToWatch(const CString& path);
	virtual int ExitInstance();

// Implementation
	virtual void PreLoadState();
	/*virtual void LoadCustomState();
	virtual void SaveCustomState();*/

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	


public:
	const CString GetApplicationPath();
	const CString GetCurrentVersion();
	CMultiDocTemplate* m_pDocTextEditorTemplate;
	CMultiDocTemplate *m_pProjectTemplate;

	CServerManager *m_pServerManager;
	LOGFONT  m_edtFont;

	void RunAction(const CString &args);
	BOOL m_bRunning;
	
protected:
	HACCEL m_hAccelTable;
	CConfig m_config;

	CShellThread* m_pMyThread;
	CPluginManager m_pluginManager;
public:

	CPluginManager* GetPluginManger();
	
	/*CString GetEditorThemeName() const;
	void SetEditorThemeName(const CString &theme);*/
	
	virtual BOOL SaveAllModified();
	
	afx_msg void OnMessageFromShell(WPARAM wparam, LPARAM lparam);

	void LoadPlugins();


	void WriteSettings();
	void LoadSettings();
	afx_msg void OnFileOpen();

	void SetUpDebugEnviorenment(const CString &path);
	void StopServer();
	void AddMessage(const CString &message, AppMessageType type);
	void ClearMessages();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void AddNewTextDoc();
	void OpenFile(const CString& path);
	CConfig *GetConfig();
	const CString LoadScript(int name);
	void ThrowLastError(const CString& caption = CString());
	afx_msg void OnActionStop();

	void PostWinMessageToShellThread(UINT message, WPARAM wParam, LPARAM lParam);

	
	afx_msg void OnUpdateActionRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionStop(CCmdUI* pCmdUI);

	CMainFrame *GetMainWindow();
	CBumbleEditView* GetActiveView();
	CCodeEditor *GetActiveCodeEditor();
	void OnProcessedContentReady(WPARAM wparam, LPARAM lparam);
	void OnAppendContent(WPARAM wparam, LPARAM lparam);
	CCodeEditor* GetEditorByDocID(UINT ID);
	void OnAfterFileRenamed(const CString &oldName, const CString &newName, LRESULT* pResult);
	void IgnoreNextDirectoryWatch();

	BOOL m_bServerRunning;
protected:
	ULONG_PTR m_gdiplusToken;
	//std::vector<std::unique_ptr<CSingleDirectoryWatcher>> m_dirWatchList;
	//BOOL m_bIgnoreFileWatcher;
	CDirectoryWatcher m_dirWatcher;
	CString m_mainWindowClassName;

	
	CTutorialBrowser* m_pTutorialBrowser;
public:
	
	CString GetMainWindowClassName() const;

	BOOL SearchInAllFiles(const CString &options);
	BOOL ReplaceInAllFiles(const CString& options);
	
	
	afx_msg void OnHelpTutorials();
};

extern CBumbleEditApp theApp;
