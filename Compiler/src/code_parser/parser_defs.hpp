#pragma once
#include <memory>
#include "..\common\error.hpp"
#include "..\parser\parser.hpp"

constexpr const char ASSIGNMENT_SIGN = '=';

constexpr const char* DATA_SECTION_KEYWORD = ".DATA";
constexpr const char* STACK_SECTION_KEYWORD = ".STACK";
constexpr const char* CODE_SECTION_KEYWORD = ".CODE";
constexpr const char* MAIN_SECTION_KEYWORD = ".MAIN";

constexpr const int DEFAULT_STACK_SIZE = 512;


#define PARSER_ERROR(desc, line) Error("sytax error: " + desc + " in line: " + std::to_string(line))

static void parseAssignmentSign(const Token& token, size_t line) {
	try {
		if (token.value.at(0) != ASSIGNMENT_SIGN) {
			throw PARSER_ERROR(std::string{ "Missing equal sign" }, line);
		}
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
}

/*
static void skipComments(std::shared_ptr<Parser> parser) {
	auto token = 
	getToken();

	while (token.value.back() == '#') {
		token = parser->getToken();
	}

	parser->putBack(std::move(token));
}
*/