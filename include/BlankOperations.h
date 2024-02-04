#pragma once
#include <string>
#include <vector>
class CBlankOperations
{
public:
	CBlankOperations(const CString &str);
	~CBlankOperations();


	void RemoveLeadingSpaces();
	void RemoveTrailingSpaces();
	void RemoveLeadingAndTrailingSpaces();
	void RemoveOnlyExtraSpaces();
	void TransformToASingleLine();

	const CString GetResult();

private:
	std::vector<std::string> m_stringList;

};

