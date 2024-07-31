#pragma once
#include "WebServer.h"

// CBuiltInServer command target
struct mg_connection;

class CBuiltInServer : public CWebServer
{
public:
	CBuiltInServer();
	virtual ~CBuiltInServer();

protected:
	CString m_documentRoot;
	static int ExampleHandler(mg_connection *conn, void *cbdata);
	BOOL Start(const CString &path);
	void Stop();
};
