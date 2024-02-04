// ServerManager.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ServerManager.h"
#include "MainFrm.h"
#include "BuiltInServer.h"
//#include "DatabaseManager.h"

// CServerManager

CServerManager::CServerManager()
{
	m_pCurrentServer = nullptr;
	/*m_pDBManager = nullptr;*/
}

CServerManager::~CServerManager()
{
	/*delete m_pDBManager;*/
}

BOOL CServerManager::Start(const CString &path)
{
	if(!m_pCurrentServer)
		m_pCurrentServer = new CBuiltInServer();


	return m_pCurrentServer->Start(path);


}

void CServerManager::Stop()
{
	m_pCurrentServer->Stop();
}

void CServerManager::SetCurrentServer(CWebServer *pServer)
{
}


// CServerManager member functions
