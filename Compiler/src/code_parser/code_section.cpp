#include "code_section.hpp"

#include <unordered_map>
#include <optional>

#include "../parser/parser.hpp"
#include "../common/error.hpp"
#include "parser_defs.hpp"

CodeSectionParser::CodeSectionParser(SymbolMap& variableMap)
	: variableMap_(variableMap) {}

void CodeSectionParser::parse(std::shared_ptr<Parser> parser) {
	try {
		parser_ = parser;
		parseCode();
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
}

CodeSection CodeSectionParser::data() {
	return data_;
}

SymbolMap CodeSectionParser::functionMap() {
	return functionMap_;
}

void CodeSectionParser::parseCode() {
	auto token = parser_->getToken();

	if (token.value != CODE_SECTION_KEYWORD) {
		parser_->putBack(std::move(token));
		return;
	}

	std::size_t size = 0;
	token = parser_->getToken();

	while (token.value == "Func" && !parser_->eof()) {
		try {
			parseFunction();
			token = parser_->getToken();
		}
		catch (Error & e) {
			std::cerr << e.getMsg() << std::endl;
		}
	}
	if (!parser_->eof()) {
		parser_->putBack(std::move(token));
	}
}

void CodeSectionParser::parseFunction() {
	static std::size_t funcIndex = 0;

	auto name = parser_->getToken().value;
	auto token = parser_->getToken();

	if (name.empty()) {
		throw Error(std::string{ "missing function name: in line: " + std::to_string(parser_->line()) });
	}

	if (token.value == ";") {
		Function func;
		func.name = name;

		data_.push_back(func);
		functionMap_.emplace(func.name, funcIndex);

		funcIndex++;
		return;
	}
	
	if (token.value != ":") {
		throw Error(std::string{ "expected ':' after function name: in line: " + std::to_string(parser_->line()) });
	}

	Function* funcPtr = nullptr;

	auto cit = functionMap_.find(name);
	if (cit == functionMap_.cend()) {
		Function func;
		func.name = name;
		func.line = parser_->line();

		data_.push_back(func);
		functionMap_.emplace(func.name, funcIndex);

		//func = data_.at[funcIndex];
		funcPtr = &data_.at(functionMap_.find(func.name)->second);

		funcIndex++;
	}
	else {
		funcPtr = &data_.at(cit->second);
		funcPtr->line = parser_->line();

		if (!funcPtr->instrList.empty()) {
			throw Error(std::string{ "redefinition of function '" + name + "' in line: " + std::to_string(parser_->line()) });
		}
	}
	
	InstructionParser instructionParser(variableMap_);
	
	SymbolMap labelMap;
	std::size_t lblIndex = 0;
	std::vector<std::pair<std::size_t, std::vector<std::string>>> tokens;
	std::size_t line = 0;

	while (!parser_->eof()) {
		parser_->skipSpaces();
		//auto line = parser_->line();

		std::vector<std::string> tokenLine;
		auto tokenList = parser_->getLine();

		if (tokenList.at(0).value == "EndF") {
			tokens.push_back({ line, {"EndF"} });
			break;
		}

		auto label = parseLabel(tokenList, line);

		for (auto& token : tokenList) {
			if (token.value == ",")
				continue;
			tokenLine.push_back(token.value);
		}

		if (!tokenLine.empty()) {
			tokens.push_back({ line, tokenLine });
			line++;
		}
		if (label.has_value()) {
			funcPtr->labels.push_back(*label);
			labelMap.emplace(label->name, lblIndex);
			lblIndex++;
		}
	}

	instructionParser.setFunctionMap(functionMap_);
	instructionParser.setLabelMap(labelMap);

	instructionParser.parseInstructions(tokens);
	funcPtr->instrList = instructionParser.data();
}

std::optional<Label> CodeSectionParser::parseLabel(TokenList& tokenList, std::size_t line) {
	if (tokenList.size() >= 2) {
		Label lbl;
		auto token = tokenList.at(0);
		auto colon = tokenList.at(1);

		if (colon.value == ":") {
			lbl.name = token.value;
			lbl.line = line;
			tokenList.erase(tokenList.begin(), tokenList.begin() + 2);
			
			return lbl;
		}
	}
	return std::nullopt;
}
