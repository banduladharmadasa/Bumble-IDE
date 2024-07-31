// CUpdateCheck.cpp : implementation file
//

#include "stdafx.h"
#include "CUpdateCheck.h"
#include <AfxInet.h>
#include "BumbleEdit.h"

// CUpdateCheck

CUpdateCheck::CUpdateCheck()
{
}

CUpdateCheck::~CUpdateCheck()
{
}

CString getInetError(DWORD err)
{
	HANDLE h = ::GetModuleHandle(_T("WinINet.dll"));
	if (h != NULL)
	{ /* got module */
		LPTSTR p;
		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								FORMAT_MESSAGE_FROM_HMODULE,
							(LPCVOID)h,
							err,
							0, // default language
							(LPTSTR)&p,
							0,
							NULL) != 0)
		{ /* format succeeded */
			CString s(p);
			s.Replace(_T("\r\n"), _T(""));
			return s;
		} /* format succeeded */
	} /* got module */

	CString s;
	s.Format(_T("%d"), err);
	return s;
} // getInetError

BOOL CUpdateCheck::HasNewVerson()
{
	CInternetSession session;

	CHttpFile *pHttpFile = NULL;
	try
	{
		pHttpFile = (CHttpFile *)session.OpenURL(_T("http://hexonianlabs.com/downloads/newversion.php"));
	}
	catch (CInternetException *ex)
	{
		// CString s = getInetError(ex->m_dwError);

		return FALSE;
	}
	if (pHttpFile != NULL)
	{
		CByteArray data;
		data.SetSize(1024);
		int nBytesRead = pHttpFile->Read(data.GetData(), data.GetSize());

		CString version(LPSTR(data.GetData()));

		// BumbleMessageBox(str);
		return (theApp.GetCurrentVersion().Compare(version) != 0);
	}
}

// CUpdateCheck member functions
