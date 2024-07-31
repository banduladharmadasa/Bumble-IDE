#pragma once

// CSqlConnection command target

class CSqlConnection : public CObject
{
public:
	CSqlConnection();
	virtual ~CSqlConnection();
	virtual void Start() = 0;
	virtual void Stop() = 0;
};
