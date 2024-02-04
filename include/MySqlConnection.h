#pragma once
#include "SqlConnection.h"

// CMySqlConnection command target

class CMySqlConnection : public CSqlConnection
{
public:
	CMySqlConnection();
	virtual ~CMySqlConnection();

	// Inherited via CSqlConnection
	virtual void Start() override;
	virtual void Stop() override;
	CString RunMySqlD(const CString &csExecute);
	//int Test(void);


protected:
	HANDLE m_mySqlHandle;
};


