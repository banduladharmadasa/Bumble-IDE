// WebServer.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "WebServer.h"





// CWebServer

CWebServer::CWebServer()
{
}

CWebServer::~CWebServer()
{
}


#pragma comment(lib, "mpr.lib")
std::wstring CWebServer::ConvertToUNC(std::wstring sPath)
{
	WCHAR temp;
	UNIVERSAL_NAME_INFO * puni = NULL;
	DWORD bufsize = 0;
	std::wstring sRet = sPath;
	//Call WNetGetUniversalName using UNIVERSAL_NAME_INFO_LEVEL option
	if (WNetGetUniversalName(sPath.c_str(),
		UNIVERSAL_NAME_INFO_LEVEL,
		(LPVOID)&temp,
		&bufsize) == ERROR_MORE_DATA)
	{
		// now we have the size required to hold the UNC path
		WCHAR * buf = new WCHAR[bufsize + 1];
		puni = (UNIVERSAL_NAME_INFO *)buf;
		if (WNetGetUniversalName(sPath.c_str(),
			UNIVERSAL_NAME_INFO_LEVEL,
			(LPVOID)puni,
			&bufsize) == NO_ERROR)
		{
			sRet = std::wstring(puni->lpUniversalName);
		}
		delete[] buf;
	}

	return sRet;;
}
