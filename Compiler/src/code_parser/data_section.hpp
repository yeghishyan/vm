#pragma once
#include <memory>
#include <unordered_map>

#include "../code_generator/variable.hpp"
#include "../code_generator/variable_defs.hpp"

using SymbolMap = std::unordered_map<std::string, std::size_t>;
using DataSection = std::unordered_map<std::string, Variable>;
class Parser;

class DataSectionParser {
public:
	DataSectionParser() = default;
	~DataSectionParser() = default;
	DataSectionParser(DataSectionParser&&) = default;
	DataSectionParser& operator=(DataSectionParser&&) = default;
	DataSectionParser(const DataSectionParser&) = delete;
	DataSectionParser& operator=(const DataSectionParser&) = delete;

	void parse(std::shared_ptr<Parser> parser);
	DataSection data();
	SymbolMap variableMap();

private:
	void parseData();
	bool isVariable();
	
	Variable parseVariable();

private:
	std::shared_ptr<Parser> parser_;

	DataSection data_;
	SymbolMap varMap_;
};

