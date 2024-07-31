

// BumbleEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BumbleEdit.h"
#include "MainFrm.h"
#include "MultiDocTemplateEx.h"

#include "ChildFrm.h"
#include "BumbleEditDoc.h"
#include "BumbleEditView.h"
#include "SettingsFormView.h"
#include "SettingsDoc.h"
#include "FileDialogEx.h"
#include "DocManagerEx.h"
#include "CShellThread.h"
#include "ProjectView.h"
#include "CProjectDoc.h"
#include "CBumbleMessageBox.h"
#include "CUpdateCheck.h"
#include "TutorialBrowser.h"
#include <DirectoryWatcher.h>

#include <filesystem>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define APP_MUTEX_NAME L"BumbleUniqueMutex"

// CBumbleEditApp

BEGIN_MESSAGE_MAP(CBumbleEditApp, CWinAppEx)
ON_COMMAND(ID_APP_ABOUT, &CBumbleEditApp::OnAppAbout)
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
ON_COMMAND(ID_FILE_OPEN, &CBumbleEditApp::OnFileOpen)
ON_COMMAND(ID_FILE_NEW, &CBumbleEditApp::OnFileNew)
ON_COMMAND(ID_ACTION_STOP, &CBumbleEditApp::OnActionStop)
ON_THREAD_MESSAGE(ID_MSG_FROM_SHELL, &CBumbleEditApp::OnMessageFromShell)
ON_THREAD_MESSAGE(ID_MESSAGE_SHELL_EXITED, &CBumbleEditApp::OnShellExited)
ON_THREAD_MESSAGE(ID_FILEMONITOR_FILECHANGED, &CBumbleEditApp::OnFileMonitorFileChanged)
// From DLLs
ON_THREAD_MESSAGE(IN_BUMBLE_CONTENT_READY, &CBumbleEditApp::OnProcessedContentReady)
ON_THREAD_MESSAGE(IN_BUMBLE_CONTENT_APPEND, &CBumbleEditApp::OnAppendContent)
ON_COMMAND(ID_HELP_TUTORIALS, &CBumbleEditApp::OnHelpTutorials)
END_MESSAGE_MAP()

void CBumbleEditApp::OnShellExited(WPARAM wparam, LPARAM lparam)
{
	m_bRunning = FALSE;
	m_pTutorialBrowser = nullptr;
}

// CBumbleEditApp construction

CBumbleEditApp::CBumbleEditApp() noexcept
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("HexonianLabs.BumbleEdit.AppID.1.1.0"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_pDocTextEditorTemplate = NULL;
	m_pProjectTemplate = NULL;

	m_pServerManager = new CServerManager();
	m_bRunning = FALSE;

	m_pMyThread = nullptr;
	m_bServerRunning = FALSE;
	m_mainWindowClassName = L"BumbleWindowClass";

	// Font
	InitFont();
}

void CBumbleEditApp::OnFileMonitorFileChanged(WPARAM wparam, LPARAM lparam)
{
	m_dirWatcher.OnFileMonitorFileChanged(wparam, lparam);
}

void CBumbleEditApp::InitFont()
{
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	m_edtFont = ncm.lfMessageFont;
	CreateFontIndirect(&m_edtFont);
}

void CBumbleEditApp::OnFileNewProject()
{
	ASSERT_VALID(m_pProjectTemplate);

	POSITION pos = m_pProjectTemplate->GetFirstDocPosition();
	if (pos == NULL)
	{
		m_pProjectTemplate->OpenDocumentFile(NULL);
		return;
	}

	CDocument *pDoc = m_pProjectTemplate->GetNextDoc(pos);
	ASSERT_VALID(pDoc);

	pos = pDoc->GetFirstViewPosition();
	ASSERT(pos != NULL);

	CView *pView = pDoc->GetNextView(pos);
	ASSERT_VALID(pView);

	CFrameWnd *pFrame = pView->GetParentFrame();
	ASSERT_VALID(pFrame);

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	::SendMessage(pMainFrame->m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)pFrame->GetSafeHwnd(), 0);
}

// The one and only CBumbleEditApp object

CBumbleEditApp theApp;

BOOL CBumbleEditApp::HaveAlreadyOpened()
{
	//=========================================================
	// Check for already running instances. If found close this instance.

	HANDLE h;
	if ((h = OpenMutex(MUTEX_ALL_ACCESS, TRUE, APP_MUTEX_NAME)))
	{

		ReleaseMutex(h);
		CWnd *pOtherWnd = NULL;
		for (int i = 0; i < 10; i++)
		{
			pOtherWnd = CWnd::FindWindow(m_mainWindowClassName, 0);
			if (pOtherWnd->GetSafeHwnd())
			{
				if (pOtherWnd->IsWindowVisible())
				{
					break;
				}
			}
			Sleep(100);
		}

		if (pOtherWnd->GetSafeHwnd())
		{
			pOtherWnd->ShowWindow(SW_SHOW);
			pOtherWnd->SetForegroundWindow();

			// Parse command line for standard shell commands, DDE, file open
			CCommandLineInfo cmdInfo;
			ParseCommandLine(cmdInfo);

			if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
			{
				LRESULT copyDataResult;
				CString dataToSend = cmdInfo.m_strFileName;
				COPYDATASTRUCT cpd;
				cpd.dwData = 0;
				int length = dataToSend.GetLength() + 1;
				cpd.cbData = sizeof(TCHAR) * length;
				cpd.lpData = (void *)dataToSend.GetBuffer(cpd.cbData);

				copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA, (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);

				dataToSend.ReleaseBuffer();
			}
		}

		return TRUE;
	}

	if (!CreateMutex(NULL, TRUE, APP_MUTEX_NAME))
	{
		ThrowLastError(L"Unable to create the mutex!");
	}

	return FALSE;
}

// CBumbleEditApp initialization

BOOL CBumbleEditApp::InitInstance()
{

	if (HaveAlreadyOpened())
	{

		return FALSE;
	}

	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		BumbleMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Hexonian Labs"));

	// 10 --> Number of recent files
	LoadStdProfileSettings(10); // Load standard INI file options (including MRU)

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
												 RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	m_pDocManager = new CDocManagerEx();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

	m_pDocTextEditorTemplate = new CMultiDocTemplateEx(IDR_TYPE_BUMBLEEDIT, RUNTIME_CLASS(CBumbleEditDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CBumbleEditView));
	if (!m_pDocTextEditorTemplate)
		return FALSE;

	// m_pDocTextEditorTemplate->SetContainerInfo(IDR_TYPE_BUMBLEEDIT_CNTR_IP);
	AddDocTemplate(m_pDocTextEditorTemplate);

	m_pProjectTemplate = new CMultiDocTemplateEx(IDR_TYPE_PROJECT, RUNTIME_CLASS(CProjectDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CProjectView));
	if (!m_pProjectTemplate)
		return FALSE;

	// m_pDocTextEditorTemplate->SetContainerInfo(IDR_TYPE_BUMBLEEDIT_CNTR_IP);
	AddDocTemplate(m_pProjectTemplate);

	// create main MDI Frame window
	CMainFrame *pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		if (!pMainFrame->LoadMDIState(GetRegSectionPath()))
		{
			m_pDocTextEditorTemplate->OpenDocumentFile(NULL);
		}
	}
	else
	{
		// Dispatch commands specified on the command line
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	m_pMainWnd->DragAcceptFiles();

	//// Dispatch commands specified on the command line.  Will return FALSE if
	//// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	// if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	//  The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	// https://stackoverflow.com/questions/1290752/why-is-cmfcmenubar-not-utilizing-the-accelerator-table?rq=1
	GetKeyboardManager()->ResetAll();

	CUpdateCheck updateCheck;

	if (updateCheck.HasNewVerson())
	{
		AfxMessageBox(L"Bumble has a new version!\n Please visit https://www.hexonianlabs.com/index.php/products/downloads to download it.");
	}

	return TRUE;
}

void CBumbleEditApp::AddDirToWatch(const CString &path)
{
	m_dirWatcher.AddDirToWatch(path);
}

int CBumbleEditApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	m_pluginManager.Unload();
	// TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	if (m_pMyThread)
	{
		m_pMyThread->m_bRunning = FALSE;
		Sleep(1000);
		DWORD result = WaitForSingleObject(m_pMyThread->m_hThread, INFINITE);

		m_pMyThread = NULL;
	}

	delete m_pServerManager;

	return CWinAppEx::ExitInstance();
}

// CBumbleEditApp message handlers

// CAboutDlg dialog used for App About

class CAboutDlg : public CBumbleDialog
{
public:
	CAboutDlg() noexcept;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_ABOUTBOX
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CBumbleButton m_btnOK;
	CMFCLinkCtrl m_link;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() noexcept : CBumbleDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CBumbleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_SYSLINK1, m_link);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBumbleDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CBumbleEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

const CString CBumbleEditApp::GetApplicationPath()
{
	CString appPath;
	AfxGetModuleFileName(m_hInstance, appPath);

	if (!PathRemoveFileSpec(appPath.GetBuffer()))
	{

		return L"";
	}

	return appPath;
}

// Current Version
const CString CBumbleEditApp::GetCurrentVersion()
{
	return CString(L"1.1.0");
}

BOOL CAboutDlg::OnInitDialog()
{
	CBumbleDialog::OnInitDialog();

	m_link.SetURL(L"https://www.hexonianlabs.com");
	m_link.SetTooltip(L"Hexonian Labs");
	m_link.SizeToContent();

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

// CBumbleEditApp customization load/save methods

void CBumbleEditApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	// GetContextMenuManager()->AddMenu(strName, IDR_CONTEXTMENU);//IDR_POPUP_EDIT);
}

void CBumbleEditApp::RunAction(const CString &args)
{
	// m_pShellUtility->SetArgs(args);
	if (m_pMyThread)
	{
		if (WaitForSingleObject(m_pMyThread, INFINITE))
		{
			m_pMyThread = NULL;
		}
	}

	m_pMyThread = new CShellThread(args);
	m_pMyThread->CreateThread(CREATE_SUSPENDED);
	m_pMyThread->m_bAutoDelete = TRUE; // Let me delete it.
	m_pMyThread->ResumeThread();
	m_bRunning = TRUE;
}

void CBumbleEditApp::LoadPlugins()
{
	m_pluginManager.Enumerate();
}

CPluginManager *CBumbleEditApp::GetPluginManger()
{
	return &m_pluginManager;
}

BOOL CBumbleEditApp::SaveAllModified()
{
	// TODO: Add your specialized code here and/or call the base class

	// return CWinAppEx::SaveAllModified();
	if (!CWinAppEx::SaveAllModified())
	{
		return FALSE;
	}

	/*CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, m_pMainWnd);
	if (pMainFrame != NULL)
	{
		pMainFrame->SaveMDIState(GetRegSectionPath());
	}*/

	return TRUE;
}

/// <summary>
/// Receive messages from the shell
/// </summary>
/// <param name="wparam">contains the message type</param>
/// <param name="lparam">a pointer to a CStirng and contains the message</param>
void CBumbleEditApp::OnMessageFromShell(WPARAM wparam, LPARAM lparam)
{
	CString *pMessage = (CString *)lparam;
	AddMessage(*pMessage, (AppMessageType)wparam);
	delete pMessage;
	pMessage = NULL;
}

void CBumbleEditApp::WriteSettings()
{
}

void CBumbleEditApp::LoadSettings()
{
}

void CBumbleEditApp::OnFileOpen()
{

	CFileDialogEx dlgOpenFile(TRUE, L"*.*", NULL, NULL, szFilter, AfxGetMainWnd());
	dlgOpenFile.GetOFN().Flags |= OFN_ALLOWMULTISELECT;

	if (IDOK == dlgOpenFile.DoModal())
	{
		IFileOpenDialog *pIFileOpenDialog = dlgOpenFile.GetIFileOpenDialog();
		ASSERT(NULL != pIFileOpenDialog);

		CComPtr<IShellItemArray> pIShellItemArray;
		HRESULT hr = pIFileOpenDialog->GetResults(&pIShellItemArray);
		if (SUCCEEDED(hr))
		{
			DWORD dwItemCount = 0;
			hr = pIShellItemArray->GetCount(&dwItemCount);
			ASSERT(SUCCEEDED(hr));
			for (DWORD dwItem = 0; dwItem < dwItemCount; dwItem++)
			{
				CComPtr<IShellItem> pIShellItem;
				pIShellItemArray->GetItemAt(dwItem, &pIShellItem);

				LPWSTR pszFilePath = NULL;
				hr = pIShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				if (SUCCEEDED(hr))
				{
					CString strFilePath(pszFilePath);
					CWinApp::OpenDocumentFile(strFilePath);
					::CoTaskMemFree(pszFilePath);
				}
			}
		}
		pIFileOpenDialog->Release(); // release IFileOpenDialog
	}

	// CWinApp::OnFileOpen();
}

void CBumbleEditApp::SetUpDebugEnviorenment(const CString &path)
{
	m_pServerManager->Start(path);
}

void CBumbleEditApp::StopServer()
{
	m_pServerManager->Stop();
}

void CBumbleEditApp::AddMessage(const CString &message, AppMessageType type)
{
	CMainFrame *pMainFrame = ((CMainFrame *)m_pMainWnd);
	if (pMainFrame->GetSafeHwnd())
	{
		pMainFrame->AddMessage(message, type);
	}
}

void CBumbleEditApp::ClearMessages()
{
	CMainFrame *pMainFrame = ((CMainFrame *)m_pMainWnd);
	if (pMainFrame->GetSafeHwnd())
	{
		((CMainFrame *)m_pMainWnd)->ClearMessages();
	}
}

BOOL CBumbleEditApp::PreTranslateMessage(MSG *pMsg)
{
	if (m_hAccelTable)
	{
		if (::TranslateAccelerator(*m_pMainWnd, m_hAccelTable, pMsg))
		{
			return (TRUE);
		}
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

void CBumbleEditApp::AddNewTextDoc()
{
	((CDocManagerEx *)m_pDocManager)->AddNewTextDoc();
}

void CBumbleEditApp::OpenFile(const CString &path)
{
	if (!PathIsDirectory(path))
	{
		theApp.OpenDocumentFile(path);
	}
}

CConfig *CBumbleEditApp::GetConfig()
{
	return &m_config;
}

const CString CBumbleEditApp::LoadScript(int name)
{
	DWORD size = 0;
	const char *data = NULL;

	HMODULE handle = ::GetModuleHandle(NULL);
	HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name),
							  MAKEINTRESOURCE(IDR_JAVASCRIPT));
	HGLOBAL rcData = ::LoadResource(handle, rc);
	size = ::SizeofResource(handle, rc);
	data = static_cast<const char *>(::LockResource(rcData));

	char *buffer = new char[size + 1];
	::memcpy(buffer, data, size);
	buffer[size] = 0; // append NULL terminator

	CString szResult(buffer);
	delete[] buffer;

	return szResult;
}

// Returns the last Win32 error, in string format. Returns an empty string if there is no error.
void CBumbleEditApp::ThrowLastError(const CString &caption)
{
	// Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return; // No error message has been recorded

	LPWSTR messageBuffer = nullptr;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

	CString message(messageBuffer, size);

	// Free the buffer.
	LocalFree(messageBuffer);

	if (GetMainWnd())
	{
		MessageBox(GetMainWnd()->m_hWnd, message, caption, MB_OKCANCEL | MB_ICONERROR);
	}
	else
	{
		MessageBox(NULL, message, caption, MB_OKCANCEL | MB_ICONERROR);
	}
}

void CBumbleEditApp::OnActionStop()
{
	AddMessage(L"Running process is terminating..\n", AppMessageType_Info);
	if (m_pMyThread)
	{
		m_pMyThread->m_bRunning = FALSE;
		m_pMyThread = nullptr;
		m_bRunning = FALSE;
		return;
	}

	WaitForSingleObject(m_pMyThread, 1000);

	m_pMyThread = nullptr;
	m_bRunning = FALSE;
}

void CBumbleEditApp::PostWinMessageToShellThread(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMyThread)
	{
		if (m_pMyThread->m_bRunning)
		{
			m_pMyThread->PostThreadMessageW(message, wParam, lParam);
		}
	}
}

void CBumbleEditApp::OnUpdateActionRun(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(!m_bRunning);
}

void CBumbleEditApp::OnUpdateActionStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bRunning);
}

CMainFrame *CBumbleEditApp::GetMainWindow()
{
	return (CMainFrame *)GetMainWnd();
}

CBumbleEditView *CBumbleEditApp::GetActiveView()
{
	if (GetMainWindow())
	{

		CView *pView = GetMainWindow()->GetActiveView();
		if (!pView->GetSafeHwnd())
		{

			return nullptr;
		}

		if (pView->IsKindOf(RUNTIME_CLASS(CBumbleEditView)))
		{
			return GetMainWindow()->GetActiveView();
		}
		return nullptr;
	}

	else
	{
		return nullptr;
	}
}

CCodeEditor *CBumbleEditApp::GetActiveCodeEditor()
{
	if (GetActiveView())
		return GetActiveView()->GetCodeEditor();
	else
		return nullptr;
}

/// <summary>
/// Call from DLLs to notify that the processed content is avialable
/// in shared memory
/// </summary>
/// <param name="wparam"></param>
/// <param name="lparam"></param>
void CBumbleEditApp::OnProcessedContentReady(WPARAM wparam, LPARAM lparam)
{
	m_pluginManager.OnProcessedContentReady((BumbleDocument *)wparam);
}

void CBumbleEditApp::OnAppendContent(WPARAM wparam, LPARAM lparam)
{
	m_pluginManager.OnAppendContent((BumbleDocument *)wparam);
}

CCodeEditor *CBumbleEditApp::GetEditorByDocID(UINT ID)
{
	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate)
	{
		CDocTemplate *pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);

			CBumbleEditDoc *_pDoc = (CBumbleEditDoc *)pDoc;
			UINT nID = _pDoc->GetUniqueDocID();
			if (nID == ID)
			{
				POSITION posView = pDoc->GetFirstViewPosition();
				return ((CBumbleEditView *)pDoc->GetNextView(posView))->GetCodeEditor();
			}
		}
	}

	return nullptr;
}

void CBumbleEditApp::OnAfterFileRenamed(const CString &oldName, const CString &newName, LRESULT *pResult)
{

	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate)
	{
		CDocTemplate *pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);

			CBumbleEditDoc *_pDoc = (CBumbleEditDoc *)pDoc;

			CString pathName = pDoc->GetPathName();

			const size_t newsizew = (oldName.GetLength() + 1) * 2;
			char *nstringw = new char[newsizew];
			size_t convertedCharsw = 0;
			wcstombs_s(&convertedCharsw, nstringw, newsizew, oldName, _TRUNCATE);

			const size_t newsizew2 = (pathName.GetLength() + 1) * 2;
			char *nstringw2 = new char[newsizew2];
			wcstombs_s(&convertedCharsw, nstringw2, newsizew2, pathName, _TRUNCATE);

			std::filesystem::path p1 = nstringw;
			std::filesystem::path p2 = nstringw2;

			std::error_code ec;
			bool bResult = std::filesystem::equivalent(p1, p2, ec);

			if (bResult)
			{
				int code = _wrename(oldName, newName);
				if (code != 0)
				{
					theApp.ThrowLastError(L"Unable to rename the file");
				}
				else
				{
					*pResult = 1;
					pDoc->SetPathName(newName, TRUE);
				}

				break;
			}
		}
	}

	if (*pResult == 0)
	{
		if (PathFileExists(oldName))
		{
			int code = _wrename(oldName, newName);
			if (code != 0)
			{
				theApp.ThrowLastError(L"Unable to rename the file");
			}
			else
			{
				*pResult = 1;
			}
		}
	}
}

void CBumbleEditApp::IgnoreNextDirectoryWatch()
{
	// Ignore self modifications
	m_dirWatcher.BoolIgnoreNextWatch();
}

CString CBumbleEditApp::GetMainWindowClassName() const
{
	return m_mainWindowClassName;
}

BOOL CBumbleEditApp::SearchInAllFiles(const CString &options)
{

	CDocTemplate *pTempl;
	CDocument *pDoc;
	CView *pView;

	POSITION posT = theApp.GetFirstDocTemplatePosition();

	while (posT)
	{
		pTempl = theApp.GetNextDocTemplate(posT);

		POSITION posD = pTempl->GetFirstDocPosition();
		while (posD)
		{

			pDoc = pTempl->GetNextDoc(posD);

			POSITION posV = pDoc->GetFirstViewPosition();
			while (posV)
			{
				pView = pDoc->GetNextView(posV);
				((CBumbleEditView *)pView)->SearchWord(options);
			}
		}
	}

	return TRUE;
}

BOOL CBumbleEditApp::ReplaceInAllFiles(const CString &options)
{
	CDocTemplate *pTempl;
	CDocument *pDoc;
	CView *pView;

	POSITION posT = GetFirstDocTemplatePosition();

	CWnd *pWnd = (CMainFrame *)GetMainWnd()->GetDlgItem(IDD_OUTPUT_WND);
	if (pWnd)
	{
		CFindResultWnd *pWndFindRes = ((COutputWnd *)pWnd)->GetFindResultWnd();

		if (pWndFindRes)
		{
			pWndFindRes->ClearResult();

			((COutputWnd *)pWnd)->SetActiveTab(1);
		}
	}

	while (posT)
	{
		pTempl = theApp.GetNextDocTemplate(posT);

		POSITION posD = pTempl->GetFirstDocPosition();
		while (posD)
		{

			pDoc = pTempl->GetNextDoc(posD);

			POSITION posV = pDoc->GetFirstViewPosition();
			while (posV)
			{
				pView = pDoc->GetNextView(posV);
				((CBumbleEditView *)pView)->ReplaceAllInFile(options);
			}
		}
	}

	return TRUE;
}

void CBumbleEditApp::OnHelpTutorials()
{
	if (!m_pTutorialBrowser)
	{
		m_pTutorialBrowser = new CTutorialBrowser();
		m_pTutorialBrowser->Create(IDD_TUTORIALBROWSER, 0);
	}

	WINDOWPLACEMENT place;
	memset(&place, 0, sizeof(WINDOWPLACEMENT));
	place.length = sizeof(WINDOWPLACEMENT);
	m_pTutorialBrowser->GetWindowPlacement(&place);

	switch (place.showCmd)
	{
	case SW_SHOWMAXIMIZED:
		m_pTutorialBrowser->ShowWindow(SW_SHOWMAXIMIZED);
		break;
	case SW_SHOWMINIMIZED:
		m_pTutorialBrowser->ShowWindow(SW_RESTORE);
		break;
	default:
		m_pTutorialBrowser->ShowWindow(SW_NORMAL);
		break;
	}

	m_pTutorialBrowser->ShowWindow(SW_SHOW);
	m_pTutorialBrowser->BringWindowToTop();
}
