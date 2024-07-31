// CLanguageSpecificConfigManager.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "BumbleEditView.h"
#include "CXmlConfigManager.h"

#include "tinyxml2.h"

// CLanguageSpecificConfigManager

CLanguageSpecificConfigManager::CLanguageSpecificConfigManager()
{
}

CLanguageSpecificConfigManager::~CLanguageSpecificConfigManager()
{
}

BOOL CLanguageSpecificConfigManager::Read(const CString &languageMode)
{
	ExecutableEnv env = {};

	CString appPath = theApp.GetApplicationPath();
	if (appPath.IsEmpty())
	{
		BumbleMessageBox(L"Unable to read the configuration file, runtime_config.xml!");
		return FALSE;
	}

	// strFileName.UnlockBuffer();

	CString fileName;
	fileName.Format(L"%s\\xmlconfig\\runtime_config.xml", appPath);

	tinyxml2::XMLDocument xml_doc;
	CW2A _fileName(fileName);

	tinyxml2::XMLError eResult = xml_doc.LoadFile(_fileName);

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		BumbleMessageBox(L"Could not load runtime_config.xml file.");
		return FALSE;
	}

	tinyxml2::XMLNode *root = xml_doc.FirstChildElement("bumble");
	if (root == nullptr)
		return FALSE;

	tinyxml2::XMLElement *element = root->FirstChildElement("config");
	if (element == nullptr)
		return FALSE;
	const char *_languageMode = element->GetText();

	while (element)
	{
		const char *mode = element->Attribute("mode");
		if (CString(mode).Compare(languageMode) == 0)
		{
			// executable name
			tinyxml2::XMLElement *elm = element->FirstChildElement("executable");
			if (elm == nullptr)
				return FALSE;
			env.executable = CString(elm->GetText());

			// pre options
			elm = element->FirstChildElement("preoptions");
			if (elm == nullptr)
				return FALSE;
			env.preOptions = CString(elm->GetText());

			// post options
			elm = element->FirstChildElement("postoptions");
			if (elm == nullptr)
				return FALSE;
			env.postOptions = CString(elm->GetText());

			theApp.GetActiveView()->m_executableEnv = env;
			break;
		}

		element = element->NextSiblingElement("config");
	}

	return TRUE;
}

void CLanguageSpecificConfigManager::Write(const CString &str)
{
}