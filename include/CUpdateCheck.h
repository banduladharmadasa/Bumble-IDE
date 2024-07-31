#pragma once

// CUpdateCheck command target

class CUpdateCheck : public CObject
{
public:
	CUpdateCheck();
	virtual ~CUpdateCheck();

	BOOL HasNewVerson();
};
