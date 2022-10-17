#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "function.hpp"
#include "variable.hpp"

using SymbolMap = std::unordered_map<std::string, std::size_t>;
using DataSection = std::unordered_map<std::string, Variable>;
using CodeSection = FunctionList; //std::unordered_map<std::string, Function>;

struct TranslationUnit {
	TranslationUnit() = default;
	~TranslationUnit() = default;

	std::string name{};
	
	DataSection data{};
	CodeSection code{};
	
	SymbolMap varMap;

	std::size_t entryPoint{};
	std::size_t stackSize{};
};

struct ProgramInfo {
	ProgramInfo() = default;
	~ProgramInfo() = default;

	uint16_t stackSizePos{};
	uint16_t stackSizeLen{};

	uint16_t entryPointPos{};
	uint16_t entryPointSize{};

	uint16_t codeSectionPos{};
	uint16_t codeSectionSize{};

	uint16_t dataSectionPos{};
	uint16_t dataSectionSize{};

	uint16_t symbolTablePos{};
	//uint16_t symbolTableSize{};
};

//struct SymbolInfo {
//	uint8_t symbolType{};
//	uint8_t valueType{};
//	uint16_t index{};
//	uint16_t size{};
//};