// MySqlConnection.cpp : implementation file
//

#include "stdafx.h"
#include "AceEditor.h"
#include "MySqlConnection.h"

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <winsvc.h>

const TCHAR* MYSQLDPath = L"C:\\wamp\\bin\\mysql\\mysql5.7.24\\bin\\mysqld.exe";
// CMySqlConnection

CMySqlConnection::CMySqlConnection()
{
	m_mySqlHandle = NULL;
}

CMySqlConnection::~CMySqlConnection()
{
	//CloseHandle(m_mySqlHandle);
}

void CMySqlConnection::Start()
{
	//Test();
	CString ExePath = L"C:\\wamp\\bin\\mysql\\mysql5.7.24\\bin\\mysqld.exe";

	
	CString sResult = L"";

	CString strCommandLine = ExePath/* + " " + arg1 + " " + arg2*/;

	theApp.AddMessage(L"Starting MySQL...");

	sResult = RunMySqlD(strCommandLine);

	
	theApp.AddMessage(sResult);
}

void CMySqlConnection::Stop()
{
}

CString CMySqlConnection::RunMySqlD(const CString &csExecute)
{
	SECURITY_ATTRIBUTES secattr;
	ZeroMemory(&secattr, sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;

	//Create pipes to write and read data
	CreatePipe(&rPipe, &wPipe, &secattr, 0);
	//
	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));

	PROCESS_INFORMATION pInfo;
	ZeroMemory(&pInfo, sizeof(pInfo));

	sInfo.cb = sizeof(sInfo);
	sInfo.dwFlags = STARTF_USESTDHANDLES;
	sInfo.hStdInput = NULL;
	sInfo.hStdOutput = wPipe;
	sInfo.hStdError = wPipe;

	CString command(csExecute);

	//Create the process here.
	if (!CreateProcess(0, command.GetBuffer(), 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo)) {

		CString strError(theApp.GetLastErrorAsString().c_str());
		strError.Insert(0, L"mysqld.exe error: ");
		return strError;
	}
	CloseHandle(wPipe);

	//now read the output pipe here.

	char buf[100];
	DWORD reDword;
	CString m_csOutput, csTemp;
	BOOL res;
	do
	{
		res = ::ReadFile(rPipe, buf, 100, &reDword, 0);
		csTemp = buf;
		m_csOutput += csTemp.Left(reDword);
	} while (res);


	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);

	if (m_csOutput.IsEmpty())
	{
		m_csOutput = L"MySQL server started.";
	}
	return  m_csOutput;
}




//int CMySqlConnection::Test(void)
//{
//	if (!m_mySqlHandle) {
//		SHELLEXECUTEINFO info;
//		memset(&info, 0, sizeof(SHELLEXECUTEINFO));
//
//		info.cbSize = sizeof(SHELLEXECUTEINFO);
//		info.fMask = SEE_MASK_NOCLOSEPROCESS;
//		info.hwnd = AfxGetMainWnd()->m_hWnd;
//		info.lpVerb = L"open";
//		info.lpFile = L"mysqld.exe";
//		info.lpParameters = NULL;
//		info.lpDirectory = MYSQLDPath;
//		info.nShow = SW_SHOWNORMAL;
//		
//		try {
//			ShellExecuteEx(&info);
//			m_mySqlHandle = info.hProcess;
//		}
//		catch (...) {
//			int result = (int)info.hInstApp;
//		}
//
//		/*switch ((int)hInstance)
//		{
//		case ERROR_FILE_NOT_FOUND:
//
//			break;
//
//		default:
//			break;
//		}*/
//	}
//	
//
//	return EXIT_SUCCESS;
//}
