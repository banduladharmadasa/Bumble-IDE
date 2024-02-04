// DatabaseManager.cpp : implementation file
//

#include "stdafx.h"
#include "AceEditor.h"
#include "DatabaseManager.h"
//#include "MySqlConnection.h"

// CDatabaseManager

CDatabaseManager::CDatabaseManager()
{
	m_pSqlConn = nullptr;
}

CDatabaseManager::~CDatabaseManager()
{
	delete m_pSqlConn;
}

void CDatabaseManager::Start()
{
	/*if (!m_pSqlConn) {
		m_pSqlConn = new CMySqlConnection();
	}


	m_pSqlConn->Start();*/
}

void CDatabaseManager::Stop()
{
}


// CDatabaseManager member functions
