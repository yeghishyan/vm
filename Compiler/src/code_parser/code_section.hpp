#pragma once
#include <memory>
#include <unordered_map>

#include "../code_generator/function.hpp"
#include "../code_generator/instruction_defs.hpp"
#include "instruction_parser.hpp"

class Parser;
using CodeSection = FunctionList; //std::unordered_map<std::string, Function>;

class CodeSectionParser {
public:
	CodeSectionParser(SymbolMap& variableMap);

	~CodeSectionParser() = default;
	CodeSectionParser(CodeSectionParser&&) = default;
	CodeSectionParser& operator=(CodeSectionParser&&) = default;
	CodeSectionParser(const CodeSectionParser&) = delete;
	CodeSectionParser& operator=(const CodeSectionParser&) = delete;

	void parse(std::shared_ptr<Parser> parser);
	CodeSection data();
	SymbolMap functionMap();

private:
	void parseCode();
	void parseFunction();
	std::optional<Label> parseLabel(TokenList& tokenList, std::size_t line);
	
private:
	std::shared_ptr<Parser> parser_;
	CodeSection data_;
	SymbolMap variableMap_;
	SymbolMap functionMap_;
};

