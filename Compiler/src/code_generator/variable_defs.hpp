#pragma once
#include <optional>

enum class ValueType {
	INV = -1,
	CHAR = 0,
	BYTE = 1, 
	WORD = 2, 
	DWORD = 4,
	QWORD = 8,
};

struct VariableTypeMap {
	const char* name;
	ValueType type;
};

constexpr VariableTypeMap variableTypeMap[] = {
	{ "CHAR",		ValueType::CHAR },
	{ "BYTE",		ValueType::BYTE },
	{ "WORD",		ValueType::WORD },
	{ "DWORD",		ValueType::DWORD },
	{ "QWORD",		ValueType::QWORD },
};

static std::optional<ValueType> getVariableType(const std::string& str) {
	for (auto item : variableTypeMap) {
		if (item.name == str) {
			return item.type;
		}
	}

	return std::nullopt;
}