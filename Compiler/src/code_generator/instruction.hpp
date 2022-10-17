#pragma once
#include <vector>
#include <variant>

#include "../common/defs.hpp"
#include "instruction_defs.hpp"

struct Argument {
	//std::variant<Byte, Word, Dword, Qword> data;
	Qword data;
	ArgumentType type{};
};

struct Instruction {
	Instruction() = default;
	~Instruction() = default;
	Instruction(Instruction&&) = default;
	Instruction(const Instruction&) = default;
	Instruction& operator=(Instruction&&) = default;
	Instruction& operator=(const Instruction&) = default;
	

	Opcode opcode{};
	CCode ccode{};
	Opsize opsize{};
	size_t argCount{};
	Argument args[2]{};
	//ArgumentType argtype[2]{};
};
using InstructionList = std::vector<Instruction>;