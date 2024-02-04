#pragma once
class CWebServer;
class CDatabaseManager;

// CServerManager command target

class CServerManager : public CObject
{
public:
	CServerManager();
	virtual ~CServerManager();
	BOOL Start(const CString &path);
	void Stop();

protected:
	CWebServer *m_pCurrentServer;
	/*CDatabaseManager *m_pDBManager;*/

public:
	void SetCurrentServer(CWebServer *pServer);
};


