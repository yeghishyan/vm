#include "data_section.hpp"

#include "../parser/parser.hpp"
#include "../common/error.hpp"
#include "../common/defs.hpp"
#include "parser_defs.hpp"

#include "../code_generator/variable_defs.hpp"
#include "../code_generator/variable.hpp"


void DataSectionParser::parse(std::shared_ptr<Parser> parser) {
	try {
		parser_ = parser;
		parseData();
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
}

DataSection DataSectionParser::data() {
	return data_;
}

SymbolMap DataSectionParser::variableMap() {
	return varMap_;
}

void DataSectionParser::parseData() {
	auto token = parser_->getToken();

	if (token.value != DATA_SECTION_KEYWORD) {
		parser_->putBack(std::move(token));
		return;
	}

	std::size_t index = 0;
	while (isVariable()) {
		try {
			auto variable = parseVariable();
	
			varMap_.emplace(variable.name, index);
			index++;
	
			data_.emplace(variable.name, std::move(variable));
		}
		catch (Error & e) {
			std::cerr << e.getMsg() << std::endl;
		}
	}
}

Variable DataSectionParser::parseVariable() {
	Variable variable = {};
	
	auto token = parser_->getToken();

	auto var_type = getVariableType(token.value);
	if (var_type.has_value()) {
		variable.type = *var_type;
	}
	else {
		throw Error(std::string{ "invalid variable type <" + token.value + ">: in line : " + std::to_string(parser_->line()) });
	}

	variable.name = parser_->getToken().value;
	size_t size = -1;

	if (variable.name.empty()) {
		throw Error(std::string{ "missing variale name: in line: " + std::to_string(parser_->line()) });
	}

	token = parser_->getToken();

	if (token.value == "[") {
		variable.array = true;
		token = parser_->getToken();

		if (token.type == TokenType::NUMBER_LITERAL) {
			size = std::stoull(token.value);
			if (size == 0) {
				throw Error(std::string{ "the size of an array must be greater than zero: in line: " + std::to_string(parser_->line()) });
			}

			token = parser_->getToken();
		}

		if (token.value != "]") {
			PARSER_ERROR(std::string{ "missing closing bracket" }, parser_->line());
		}
		token = parser_->getToken();

	}	
	
	if (token.value == ";") {
		if (variable.array && size == -1) {
			throw Error(std::string{ "invalid use of array with unspecified bounds: in line : " + std::to_string(parser_->line()) });
		}
		return variable;
	}

	parseAssignmentSign(token, parser_->line());

	do {
		token = parser_->getToken();
		variable.addValue(token);
		token = parser_->getToken();
	} while (token.value == "," && variable.array);

	if (size != -1) {
		variable.setSize(size);
	}

	parser_->putBack(std::move(token));

	return variable;
}

bool DataSectionParser::isVariable(){
	auto token = parser_->getToken();
	bool res = getVariableType(token.value).has_value();
	parser_->putBack(std::move(token));
	
	return res;
}

//
//ValueType DataSectionParser::parseVariableType() {
//	auto token = parser_->getToken();
//
//	for (auto type : VARIABLE_TYPES) {
//		if (type == *token.getString()) {
//			break;
//		}
//	}
//}
//
//void DataSectionParser::parseVariableName() {
//}
//
//void DataSectionParser::parseVariableArguments() {
//}