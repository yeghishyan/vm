#include "main_section.hpp"

#include "parser_defs.hpp"

#include "../parser/parser.hpp"
#include "../common/error.hpp"

MainSectionParser::MainSectionParser(SymbolMap& functions)
	: functionMap_(functions) {}

void MainSectionParser::parse(std::shared_ptr<Parser> parser) {
	try {
		parseMain(parser);
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
}

void MainSectionParser::parseMain(std::shared_ptr<Parser> parser) {
	//skipComments(parser);

	auto token = parser->getToken();

	if (token.value != MAIN_SECTION_KEYWORD) {
		parser->putBack(std::move(token));
		return;
	}

	parseAssignmentSign(parser->getToken(), parser->line());

	token = parser->getToken();
	auto funcName = token.value;

	auto it = functionMap_.find(funcName);

	if (it != functionMap_.end()) {
		main_.name = it->first;
		main_.index = it->second;
	}
	else {
		throw PARSER_ERROR(std::string{"function not found" }, parser->line());
	}
}