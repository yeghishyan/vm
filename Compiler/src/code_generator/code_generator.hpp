#pragma once
#include "code_defs.hpp"

using SymbolMap = std::unordered_map<std::string, std::size_t>;
using IndexMap = std::unordered_map<std::size_t, std::size_t>;

enum class SymbolType {
	VAR, FUNC
};

struct Symbol {
	Symbol() = default;
	~Symbol() = default;

	SymbolType symbolType{};
	ValueType valueType{};
	
	std::size_t size{};
	std::size_t index{};
	//std::size_t address() {};
};
using SymbolTable = std::vector<std::pair<std::string, Symbol>>;

struct ProgramOutput {	
	std::string name{};

	std::size_t stackSize{};
	std::size_t entryPoint{};

	std::vector<Byte> code{};
	std::vector<Byte> data{};

	SymbolTable symbolTable{};
};

class CodeGenerator {
public:
	CodeGenerator() = default;

	~CodeGenerator() = default;

	void build(TranslationUnit& trUnit);

	ProgramOutput out();

private:
	ProgramOutput out_;
	IndexMap varIndexMap_;
	IndexMap funcIndexMap_;
	IndexMap lblIndexMap_;
};


