#include "stack_section.hpp"

#include "parser_defs.hpp"

#include "../parser/parser.hpp"
#include "../common/error.hpp"

void StackSectionParser::parse(std::shared_ptr<Parser> parser) {
	try {
		parseStack(parser);
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
}


void StackSectionParser::parseStack(std::shared_ptr<Parser> parser) {
	//skipComments(parser);

	auto token = parser->getToken();
	
	if (token.value != STACK_SECTION_KEYWORD) {
		parser->putBack(std::move(token));
		return;
	}
	
	parseAssignmentSign(parser->getToken(), parser->line());

	token = parser->getToken();

	if (token.type != TokenType::NUMBER_LITERAL) {
		throw PARSER_ERROR(std::string{ "Stack size didn't specified" }, parser->line());
	}

	auto size = std::strtoll(token.value.c_str(), nullptr, 10);

	if (size < 0) {
		throw PARSER_ERROR(std::string{ "Stack size can't be negative" }, parser->line());
	};
	if (size < 256) {
		throw PARSER_ERROR(std::string{ "Stack size specified is too small, specify at least 256K" }, parser->line());
	}

	stack_.size = size;
}