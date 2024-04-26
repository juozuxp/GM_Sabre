#pragma once
#include <string>
#include <vector>

class LowTrustString
{
public:
	bool GetStringLength(const char* string, size_t& length); // get length of a low trust string, length marks the distance to the invlid character if result false
	bool GetStringLength(const wchar_t* string, size_t& length); // get length of a low trust string, length marks the distance to the invlid character if result false

private:
	bool CheckCharacterTypes(size_t& length);

private:
	std::vector<uint16_t> m_CharacterTypes;
};