#pragma once

// CLanguageSpecificConfigManager command target

class CLanguageSpecificConfigManager : public CObject
{
public:
	CLanguageSpecificConfigManager();
	virtual ~CLanguageSpecificConfigManager();
	BOOL Read(const CString& fileName);
	void Write(const CString& languageMode);
};


