// CPluginManager.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "CPluginManager.h"
#include "BumbleEditView.h"
#include "BumbleEditDoc.h"

#define HLEDTPLUGIN_API __declspec(dllimport)
#define BUMBLE_USER_BEGIN 0x7C00
#include "BumbleExtInterface.h"


#include <string>
#include <algorithm>

using namespace BumbleIDE;

typedef std::unique_ptr<CBumbleExtLib>(__cdecl* ObjProc)(CPluginManager*);
typedef BOOL(__cdecl* IsBumbleExtension)(void);

WCHAR MSG1[] = L"Unable to load %d plugin(s)\n\0";
WCHAR MSG2[] = L"Loaded %d plugin(s) successfuly\n\0";

// CPluginManager

CPluginManager::CPluginManager()
{
}

CPluginManager::~CPluginManager()
{

}


void CPluginManager::Unload()
{
	m_lstPlugins.clear();
	m_menuHashTable.clear();
}

void CPluginManager::Enumerate()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;


#ifdef DEBUG
	hFind = FindFirstFile(L"..\\x64\\Debug\\plugins\\*.dll", &FindFileData);
#else
	hFind = FindFirstFile(L".\\plugins\\*.dll", &FindFileData);
#endif // DEBUG


	int pluginCount = 0;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		/*theApp.ThrowLastError(L"Plugin Manager");*/
		//BumbleMessageBox(L"Unable to find any plugin in the \"plugins\" directory.");
		return;
	}


	do {
		pluginCount++;
		HINSTANCE hInst = AfxLoadLibrary((L".\\plugins\\" + CString(FindFileData.cFileName)));


		if (!hInst) {
			theApp.AddMessage(L"Couldn't load plugin " + CString(FindFileData.cFileName) + L"\n", AppMessageType_Error);
			continue;
		}

		
		//----------------------------------
		ObjProc objFunc = (ObjProc)GetProcAddress(hInst, "?GetInstance@@YA?AV?$unique_ptr@VCBumbleExtLib@BumbleIDE@@U?$default_delete@VCBumbleExtLib@BumbleIDE@@@std@@@std@@XZ");
		BOOL IsBumbleExtension = (BOOL)GetProcAddress(hInst, "?IsBumbleExtension@@YAHXZ");


		if (!(objFunc && IsBumbleExtension)) {
			theApp.AddMessage(L"Missing exported functions from " + CString(FindFileData.cFileName), AppMessageType_Error);
			continue;
		}


		
		m_lstPlugins.push_back(objFunc(this));
		CMenu* menu = m_lstPlugins.back()->GetMenu();

		if (menu) {

			AddMenuItemsToHashTable(menu, m_lstPlugins.back()->GetName() , m_lstPlugins.size() - 1);
		}




	} while (FindNextFile(hFind, &FindFileData));

	if (m_lstPlugins.size() < pluginCount) {
		CString _str;
		_str.Format(MSG1, pluginCount, m_lstPlugins.size());
		theApp.AddMessage(_str, AppMessageType_Error);
	}
	else {
		CString _str;
		_str.Format(MSG2, pluginCount);
		theApp.AddMessage(_str, AppMessageType_Info);
	}
	FindClose(hFind);

}

static int start = BUMBLE_USER_BEGIN;
BOOL  CPluginManager::IterateMenu(CMenu* pMenu, UINT nPluginPos)
{
	
	if (pMenu == NULL)
	{
		return FALSE;
	}

	int iCount = pMenu->GetMenuItemCount();
	for (int i = 0; i < iCount; i++)
	{
		UINT uiID = pMenu->GetMenuItemID(i);

		if (uiID == -1) // Pop-up menu
		{
			CMenu* pPopupMenu = pMenu->GetSubMenu(i);
			ENSURE(pPopupMenu != NULL);

			IterateMenu(pPopupMenu, nPluginPos);
		}
		else {
			ExtMenuInfo *info = new ExtMenuInfo;
			info->nIDActual = uiID;
			info->nPluginPos = nPluginPos;
			
			CString str;
			pMenu->GetMenuString(uiID, str, MF_STRING | MF_BYCOMMAND);
			pMenu->ModifyMenu(uiID, MF_BYCOMMAND | MF_STRING, start, str);
			
			m_menuHashTable[start] = std::unique_ptr<ExtMenuInfo>(info);
			start++;
		}
		
	}

	return FALSE;
}

/// <summary>
/// Add menu in each plugin to the hash table with a unique command id
/// to avoid conflicts
/// </summary>
/// <param name="pMenu"></param>
/// <param name="guid"></param>
void CPluginManager::AddMenuItemsToHashTable(CMenu* pMenu, const CString &name, UINT nPluginPos)
{
	int iCount = pMenu->GetMenuItemCount();
	
	IterateMenu(pMenu, nPluginPos);
	m_pMenuList.push_back(pMenu);

	
	
	
}




/// <summary>
/// The callback function to receive processed
/// content from the dll
/// </summary>
void CPluginManager::OnProcessedContentReady(BumbleDocument *pBumbleDoc) {

	CCodeEditor *pEditor = theApp.GetEditorByDocID(pBumbleDoc->nID);
	if (pEditor) {
		pEditor->SetContent(CString(pBumbleDoc->content->GetBuffer()));
	}

	pBumbleDoc->content->Empty();
	/*delete pBumbleDoc;
	pBumbleDoc = NULL;*/

	
}

///
////Notified by the plugin
///
void CPluginManager::OnAppendContent(BumbleDocument* pBumbleDoc)
{
	CCodeEditor* pEditor = theApp.GetEditorByDocID(pBumbleDoc->nID);
	if (pEditor) {
		pEditor->AppendContent(CString(pBumbleDoc->content->GetBuffer()));
	}
}

void CPluginManager::OnCommand(UINT nID)
{
	BumbleDocument* pDoc;
	UINT nDocID = theApp.GetActiveView()->GetDocument()->GetUniqueDocID();
	UINT actualID = m_menuHashTable[nID]->nIDActual;

	
	CString* str = NULL;
	auto it = std::find_if(m_pBumbleDocList.begin(), m_pBumbleDocList.end(), [&nDocID](const BumbleDocument* obj) {return obj->nID == nDocID; });

		if(it != m_pBumbleDocList.end())
		{
			pDoc = *it;
		}
		else {
			pDoc = new BumbleDocument();
		}

	m_pBumbleDocList.push_back(pDoc);

	str = new CString(theApp.GetActiveView()->GetCodeEditor()->GetContent());
	pDoc->content = str;
	pDoc->nID = nDocID;

	m_lstPlugins[m_menuHashTable[nID]->nPluginPos]->OnIDECommand(actualID, pDoc);

	
}




// CPluginManager member functions
