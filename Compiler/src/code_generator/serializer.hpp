#pragma once
#include <ostream>

#include "code_defs.hpp"
#include "..\common\defs.hpp"

#include "..\code_parser\instruction_parser.hpp"

#include "variable.hpp"
#include "variable_defs.hpp"

#include "function.hpp"
#include "instruction_defs.hpp"

std::ostream& operator << (std::ostream& out, const Variable& var);

std::ostream& operator<<(std::ostream& out, const Opcode& opcode);

std::ostream& operator<<(std::ostream& out, const CCode& ccode);

std::ostream& operator<<(std::ostream& out, const Opsize& opsize);

std::ostream& operator<<(std::ostream& out, const Argument& arg);

std::ostream& operator<<(std::ostream& out, const Function& func);