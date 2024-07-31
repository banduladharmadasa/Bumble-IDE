// CShellThread.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CShellThread.h"
#include <pathcch.h>

#define BUFSIZE 4096

// CShellThread

IMPLEMENT_DYNCREATE(CShellThread, CWinThread)

CShellThread::CShellThread(const CString &args)
{
	ASSERT(!args.IsEmpty());
	m_strArgs = args;
}

CShellThread::~CShellThread()
{
}

BOOL CShellThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	Execute();

	AfxEndThread(0);
	return TRUE;
}

int CShellThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CShellThread, CWinThread)
END_MESSAGE_MAP()

#define PIPEBUFSZIE 64
// CShellThread message handlers

BOOL CShellThread::CreatePipes()
{
	SECURITY_ATTRIBUTES saAttr;
	// Set the bInheritHandle flag so pipe handles are inherited.

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT.
	if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, PIPEBUFSZIE))
	{
		AddMessage(TEXT("Unexpecter error[P-01]!\n"), AppMessageType_Error);
		return FALSE;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, PIPEBUFSZIE))
	{
		AddMessage(TEXT("Unexpecter error[P-01]!\n"), AppMessageType_Error);
		return FALSE;
	}

	// Create a pipe for the child process's STDIN.
	if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, PIPEBUFSZIE))
	{
		AddMessage(TEXT("Unexpecter error[P-01]!\n"), AppMessageType_Error);
		return FALSE;
	}

	// Ensure the write handle to the pipe for STDIN is not inherited.
	if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, PIPEBUFSZIE))
	{
		AddMessage(TEXT("Unexpecter error[P-01]!\n"), AppMessageType_Error);
		return FALSE;
	}

	// Create a pipe for the child process's STDERR.
	if (!CreatePipe(&hChildStd_ERR_Rd, &hChildStd_ERR_Wr, &saAttr, 0))
	{
		AddMessage(TEXT("Unexpecter error[P-01]!\n"), AppMessageType_Error);
		return FALSE;
	}
	// Ensure the read handle to the pipe for STDERR is not inherited.
	if (!SetHandleInformation(hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
	{
		AddMessage(TEXT("Unexpecter error[P-01]!\n"), AppMessageType_Error);
		return FALSE;
	}

	return TRUE;
}

int CShellThread::Execute()
{
#ifdef DEBUG
	AddMessage(L"Creatring process\n", AppMessageType_Info);
#endif // DEBUG

	if (!CreatePipes())
	{
		return 0;
	}

	STARTUPINFO info = {sizeof(info)};

	// Assign pipes
	info.hStdError = hChildStd_ERR_Wr;
	info.hStdOutput = hChildStd_OUT_Wr;
	info.hStdInput = hChildStd_IN_Rd;
	info.dwFlags = /* STARTF_USESHOWWINDOW |*/ STARTF_USESTDHANDLES;
	;

	CString currentDir;
	if (m_strArgs.Find(L"run.bat") > -1)
	{
		currentDir = m_strArgs;
		PathCchRemoveFileSpec(currentDir.GetBuffer(), currentDir.GetLength());
		currentDir.UnlockBuffer();

		if (!CreateProcess(NULL,
						   m_strArgs.GetBuffer(),
						   NULL,
						   NULL,
						   TRUE,
						   CREATE_NO_WINDOW,
						   NULL,
						   currentDir,
						   &info,
						   &processInfo))
		{
			AddMessage(L"Unable to create the runtime environment!\n", AppMessageType_Error);
			return 0;
		}
	}
	else
	{
		if (!CreateProcess(NULL,
						   m_strArgs.GetBuffer(),
						   NULL,
						   NULL,
						   TRUE,
						   CREATE_NO_WINDOW,
						   NULL,
						   NULL,
						   &info,
						   &processInfo))
		{
			AddMessage(L"Unable to create the runtime environment!\n", AppMessageType_Error);
			return 0;
		}
	}

	//--------------------------------------------------------------

	// EnumWindows(EnumWindowsProc, processInfo.dwProcessId);
	//--------------------------------------------------------------

	m_bRunning = TRUE;

	BOOL bDoingBackgroundProcessing = TRUE;
	MSG msg;
	CStringA buffer;
	while (m_bRunning)
	{

		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_CHAR))
			{
				AddMessage(CString(TCHAR(msg.wParam)), AppMessageType_Character);

				DWORD dwWritten = 0;
				if (msg.wParam == VK_RETURN)
				{
					buffer.Append("\r\n\0\0");
					// send characters in the buffer to std-in
					WriteFile(hChildStd_IN_Wr, buffer.GetBuffer(), buffer.GetLength(), &dwWritten, NULL);
					buffer.Empty();
				}
				else
				{
					// poll the characters
					buffer.AppendChar(msg.wParam);
				}
			}
			else if ((msg.message == WM_COMMAND) && (msg.wParam == ID_ACTION_STOP))
			{
				m_bRunning = FALSE;
				AddMessage(L"Stopping", AppMessageType_Info);
			}
		}

		ReadFromPipe(hChildStd_OUT_Rd, FALSE);
		ReadFromPipe(hChildStd_ERR_Rd, TRUE);
	}

	if (WAIT_OBJECT_0 != WaitForSingleObject(processInfo.hProcess, 1000))
	{
		AddMessage(L"The process does not respond and forcefully terminating..\n", AppMessageType_Error);
		::TerminateProcess(processInfo.hProcess, 2);
	}

	DWORD dwExitCode = 0;
	//-- get the process exit code
	GetExitCodeProcess(processInfo.hProcess, &dwExitCode);

	//-- the task has ended so close the handle

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	CString str;
	str.Format(L"\n\nExit with code %d\n", dwExitCode);
	AddMessage(str, AppMessageType_Info);

	CloseHandle(hChildStd_OUT_Wr);
	CloseHandle(hChildStd_IN_Rd);
	CloseHandle(hChildStd_IN_Wr);
	CloseHandle(hChildStd_OUT_Rd);

	CloseHandle(hChildStd_ERR_Wr);

	theApp.PostThreadMessage(ID_MESSAGE_SHELL_EXITED, dwExitCode, 0);
	return dwExitCode;
}

void CShellThread::Stop()
{
	m_bRunning = FALSE;
}

void CShellThread::AddMessage(const CString &str, AppMessageType type)
{
	CString *pStr = new CString(str);
	if (!theApp.PostThreadMessageW(ID_MSG_FROM_SHELL, type, (LPARAM)pStr))
	{
		delete pStr;
	}
}

/// <summary>
/// Read output from the child process's pipe
/// </summary>
void CShellThread::ReadFromPipe(HANDLE hndl, BOOL isErr)
{
	DWORD dwBuffSize = 0;
	DWORD dwBytesRead = 0;
	DWORD dwBytesAvail = 0;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	CString buffer = L"\r\n> ";
	for (;;)
	{
		DWORD dwExitCode = WaitForSingleObject(processInfo.hProcess, 0);
		if ((dwExitCode == WAIT_FAILED) || (dwExitCode == WAIT_OBJECT_0) || (dwExitCode == WAIT_ABANDONED))
		{
			m_bRunning = FALSE;
		}

		// hChildStd_OUT_Rd
		if (PeekNamedPipe(hndl, chBuf, dwBuffSize, &dwBytesRead, &dwBytesAvail, NULL))
		{

			if (dwBytesAvail > 0)
			{
				bSuccess = ReadFile(hndl, chBuf, BUFSIZE, &dwBuffSize, NULL);

				CString strMessage(chBuf, dwBuffSize);
				buffer.Append(strMessage);

				if (!bSuccess || dwBuffSize == 0)
					break;
			}
			else
			{
				break;
			}

			if (!m_bRunning)
			{
				break;
			}
		}
		else
		{

			DWORD dwError = GetLastError();

			break;
		}
	}

	if (buffer.GetLength() > 4)
		AddMessage(buffer, isErr ? AppMessageType_Error : AppMessageTpe_ShellOutput);
}
