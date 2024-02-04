#include "stdafx.h"
#include "BlankOperations.h"
#include <regex>
#include <vector>
#include <sstream>
#include <functional>

using namespace std;

std::vector<std::string> split(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (getline(ss, item, delim)) {
		elems.push_back(std::move(item));
	}
	return elems;
}

CBlankOperations::CBlankOperations(const CString &str)
{
	string input = CT2A(str);
	m_stringList = split(input, '\n');
}


CBlankOperations::~CBlankOperations()
{
}

void CBlankOperations::RemoveLeadingSpaces()
{
	for (int i = 0; i < m_stringList.size(); i++) {
		m_stringList[i] = regex_replace(m_stringList[i], regex("^ +"), "");
	}
}

void CBlankOperations::RemoveTrailingSpaces()
{
	for (int i = 0; i < m_stringList.size(); i++) {
		m_stringList[i]  = std::regex_replace(m_stringList[i], std::regex(" +$"), "");
	}
}

void CBlankOperations::RemoveLeadingAndTrailingSpaces()
{
	RemoveTrailingSpaces();
	RemoveLeadingSpaces();
}

void CBlankOperations::RemoveOnlyExtraSpaces()
{
	for (int i = 0; i < m_stringList.size(); i++) {
		m_stringList[i] = regex_replace(m_stringList[i], std::regex(" +"), " ");
	}
}

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

void CBlankOperations::TransformToASingleLine()
{
	string op;
	const char* t = " \t\n\r\f\v";
	for (int i = 0; i < m_stringList.size(); i++) {
		op.append(ltrim(rtrim(m_stringList[i], t), t));
	}

	m_stringList.clear();
	m_stringList.push_back(op);
}

const CString CBlankOperations::GetResult()
{
	string output;
	for (auto str : m_stringList) {
		
		output.append(str);
		output.push_back('\n');

	}
	
	return CString(output.substr(0, output.size() - 1).c_str());
}
