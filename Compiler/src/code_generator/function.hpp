#pragma once
#include <string>
#include <vector>

#include "../parser/token.hpp"
#include "../common/defs.hpp"

#include "instruction.hpp"

#include <ostream>

struct Label {
	Label() = default;
	~Label() = default;
	Label(Label&&) = default;
	Label(const Label&) = default;
	Label& operator=(Label&&) = default;
	Label& operator=(const Label&) = default;

	std::size_t line{0u};
	std::string name{};
};
using LabelList = std::vector<Label>;

struct Function {
	Function() = default;
	~Function() = default;
	Function(Function&&) = default;
	Function(const Function&) = default;
	Function& operator=(Function&&) = default;
	Function& operator=(const Function&) = default;

	std::string name{};
	std::size_t line{};
	LabelList labels{};
	InstructionList instrList;
};
using FunctionList = std::vector<Function>;
