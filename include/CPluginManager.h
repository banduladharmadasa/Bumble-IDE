#pragma once
#include <vector>
#include <map>
#include <memory>
#include <map>

class CCodeEditor;

#include "BumbleExtLib.h"
// CPluginManager command target

using namespace BumbleIDE;

struct ExtMenuInfo
{
	UINT nPluginPos;
	UINT nIDActual;
};

class CPluginManager : public CObject
{
public:
	CPluginManager();
	virtual ~CPluginManager();

	void Unload();

	void Enumerate();

	BOOL IterateMenu(CMenu* pMenu, UINT nPluginPos);

	void AddMenuItemsToHashTable(CMenu* pMenu, const CString& name, UINT nPluginPos);

	

	
	static void OnProcessedContentReady(BumbleDocument *pBumbleDoc);//Notified by the plugin
	static void OnAppendContent(BumbleDocument* pBumbleDoc);//Notified by the plugin
	void OnCommand(UINT nID);
	std::vector<std::unique_ptr<CBumbleExtLib>> m_lstPlugins;
	std::map<UINT/*Assigned ID*/, std::unique_ptr<ExtMenuInfo>> m_menuHashTable;
	std::vector<CMenu*> m_pMenuList;
private:
	
	std::vector<BumbleDocument*> m_pBumbleDocList;
};


