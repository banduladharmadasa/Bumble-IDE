#pragma once
class CSqlConnection;
// CDatabaseManager command target

class CDatabaseManager : public CObject
{
public:
	CDatabaseManager();
	virtual ~CDatabaseManager();
	void Start();
	void Stop();


protected:
	CSqlConnection *m_pSqlConn;
};


