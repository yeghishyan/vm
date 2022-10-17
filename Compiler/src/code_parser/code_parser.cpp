#include "code_parser.hpp"

#include <iostream>

#include "..\parser\parser.hpp"
#include "..\common\error.hpp"
#include "..\code_generator\code_defs.hpp"
#include "..\code_generator\serializer.hpp"


CodeParser::CodeParser()
	: parser_(std::make_shared<Parser>())
{}

void CodeParser::setFile(const std::string& path) {
	Error::setFile(path);

	//currFile_ = new std::ifstream();
	currFile_ = std::make_shared<std::ifstream>();

	currFile_->open(path);

	if (!currFile_->is_open()) {
		throw FILE_OPEN_ERROR(path);
	}

	parser_->setStream(*currFile_);
}

void CodeParser::parse(const std::string& file) {
	setFile(file);

	std::string fileName = file;

	auto slashPos = file.find_last_of("/\\");
	if (slashPos != std::string::npos) {
		fileName.erase(0, slashPos + 1);
	}

	trUnit_.name = fileName;

	StackSectionParser stackSectionParser;
	stackSectionParser.parse(parser_);
	auto stack = stackSectionParser.data();
	trUnit_.stackSize = stack.size;

	DataSectionParser dataSectionParser;
	dataSectionParser.parse(parser_);
	auto variableMap = dataSectionParser.variableMap();
	trUnit_.data = dataSectionParser.data();
	trUnit_.varMap = variableMap;

	CodeSectionParser codeSectionParser(variableMap);
	codeSectionParser.parse(parser_);
	auto functionMap = codeSectionParser.functionMap();
	trUnit_.code = codeSectionParser.data();

	MainSectionParser mainSectionParser(functionMap);
	mainSectionParser.parse(parser_);
	auto main = mainSectionParser.data();
	trUnit_.entryPoint = main.index;
	
	/*
	std::cout << std::endl;

	if (stack.size) {
		std::cout << "Stack Size: " << trUnit_.stackSize << std::endl;
	}
	std::cout << std::endl;

	if (!trUnit_.data.empty()) {
		for (auto& var : trUnit_.data) {
			std::cout << var.second << std::endl;
		}
	}
	std::cout << std::endl;

	if (!trUnit_.code.empty()) {
		for (auto& func : trUnit_.code) {
			std::cout << func << std::endl;
		}
	}

	if (!main.name.empty()) {
		std::cout << "Main:    " << main.name << std::endl;
		std::cout << "Address: " << main.index << std::endl;
	}*/

	/*
	auto cLine = parser_->line();
	auto pLine = cLine - 1;

	while (!parser_->eof()) {
		auto start_cLine = parser_->line();
		auto token = parser_->getToken();
		cLine = parser_->line();

		if (cLine - pLine) {
			std::cout << std::endl;
			pLine = cLine;
			std::cout << start_cLine <<"->"<<cLine << ": ";
		}

		switch (token.type) {
		case TokenType::IDENTIFIER:
			std::cout << "identifier: " << token.value;
			break;
		case TokenType::NUMBER_LITERAL:
			std::cout << "number: " << token.value;
			break;
		case TokenType::STRING_LITERAL:
			std::cout << "string: " << token.value;
			break;
		//case TokenType::COMMENT:
		//	std::cout << "comment: " << token.value;
		//	break;
		case TokenType::CHAR_LITERAL:
			std::cout << "char: " << token.value;
			break;
		case TokenType::PUNCT:
			std::cout << "punct: " << token.value;
			break;
		}

		std::cout << "\n";
	}
	*/
}

TranslationUnit CodeParser::trUnit() {
	return trUnit_;
}

