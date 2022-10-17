#pragma once
#include <memory>
#include <string>
#include <fstream>

#include "../common/defs.hpp"
#include "../code_generator/code_defs.hpp"

#include "stack_section.hpp"
#include "data_section.hpp"
#include "code_section.hpp"
#include "main_section.hpp"

class Parser;

class CodeParser {
public:
	CodeParser();
	~CodeParser() = default;
	//CodeParser(const CodeParser&) = delete;
	//CodeParser& operator=(const CodeParser&) = delete;
	//CodeParser(CodeParser&&) = delete;
	//CodeParser& operator=(CodeParser&&) = delete;

	void parse(const std::string& file);
	TranslationUnit trUnit();

private:
	void setFile(const std::string& path);

private:
	std::shared_ptr<std::ifstream> currFile_;
	std::shared_ptr<Parser> parser_;

	TranslationUnit trUnit_;
};