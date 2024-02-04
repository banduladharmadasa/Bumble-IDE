#pragma once
#include <string>
// CWebServer command target

class CWebServer : public CObject
{
public:
	CWebServer();
	virtual ~CWebServer();

	std::wstring ConvertToUNC(std::wstring sPath);

	virtual BOOL Start(const CString &path) = 0;
	virtual void Stop() = 0;
};


