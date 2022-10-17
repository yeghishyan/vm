#include "serializer.hpp"

std::ostream& operator << (std::ostream& out, const Variable& var) {
	out << "\"var\":\n{\n  ";

	out << "\"type\":  ";
	switch (var.type)
	{

	case ValueType::CHAR:
		out << "char\n";
		break;
	case ValueType::BYTE:
		out << "byte\n";
		break;
	case ValueType::WORD:
		out << "word\n";
		break;
	case ValueType::DWORD:
		out << "dword\n";
		break;
	case ValueType::QWORD:
		out << "qword\n";
		break;
	}

	out << "  \"name\":  " << var.name << "\n  ";

	if (var.array) {
		out << "\"array\":  ";
	}
	else {
		out << "\"value\":  ";
	}

	for (int i = 0; i < var.values.size(); ++i) {
		switch (var.type)
		{
		case ValueType::CHAR:
			out << std::get<0>(var.values[i].data);
			break;
		case ValueType::BYTE:
			out << std::get<1>(var.values[i].data);
			break;
		case ValueType::WORD:
			out << std::get<2>(var.values[i].data);
			break;
		case ValueType::DWORD:
			out << std::get<3>(var.values[i].data);
			break;
		case ValueType::QWORD:
			out << std::get<4>(var.values[i].data);
			break;
		}

		if (i != var.values.size() - 1) {
			out << ", ";
		}
	}
	out << "\n}";

	return out;
}

std::ostream& operator<<(std::ostream& out, const Opcode& opcode) {
	switch (opcode)
	{
	case Opcode::NOP:
		out << "nop";
		break;
	case Opcode::BREAK:
		out << "break";
		break;
	case Opcode::ENDF:
		out << "endf";
		break;
	case Opcode::RET:
		out << "ret";
		break;
	case Opcode::JUMPA:
		out << "jumpa";
		break;
	case Opcode::JUMPR:
		out << "jumpr";
		break;
	case Opcode::CALL:
		out << "call";
		break;
	case Opcode::GFLR:
		out << "gflr";
		break;
	case Opcode::SFLR:
		out << "sflr";
		break;
	case Opcode::LOAD:
		out << "load";
		break;
	case Opcode::STORE:
		out << "store";
		break;
	case Opcode::LDREL:
		out << "ldrel";
		break;
	case Opcode::STREL:
		out << "strel";
		break;
	case Opcode::PUSHSF:
		out << "pushsf";
		break;
	case Opcode::POPSF:
		out << "popsf";
		break;
	case Opcode::PUSHA:
		out << "pusha";
		break;
	case Opcode::POPA:
		out << "popa";
		break;
	case Opcode::PUSH:
		out << "push";
		break;
	case Opcode::POP:
		out << "pop";
		break;
	case Opcode::POPR:
		out << "popr";
		break;
	case Opcode::ASSIGNR:
		out << "assingr";
		break;
	case Opcode::ASSIGNA:
		out << "assigna";
		break;
	case Opcode::MOVA:
		out << "mova";
		break;
	case Opcode::MOVR:
		out << "movr";
		break;
	case Opcode::SWAPA:
		out << "swapa";
		break;
	case Opcode::SWAPR:
		out << "swapr";
		break;
	case Opcode::SET:
		out << "set";
		break;
	case Opcode::INC:
		out << "inc";
		break;
	case Opcode::DEC:
		out << "dec";
		break;
	case Opcode::TEST:
		out << "test";
		break;
	case Opcode::CMP:
		out << "cmp";
		break;
	case Opcode::IN:
		out << "in";
		break;
	case Opcode::OUT:
		out << "out";
		break;
	case Opcode::AND:
		out << "and";
		break;
	case Opcode::OR:
		out << "or";
		break;
	case Opcode::XOR:
		out << "xor";
		break;
	case Opcode::NAND:
		out << "nand";
		break;
	case Opcode::NOR:
		out << "nor";
		break;
	case Opcode::NOT:
		out << "nor";
		break;
	case Opcode::SHL:
		out << "shl";
		break;
	case Opcode::SHR:
		out << "shr";
		break;
	case Opcode::ROL:
		out << "rol";
		break;
	case Opcode::ROR:
		out << "ror";
		break;
	case Opcode::SAL:
		out << "sal";
		break;
	case Opcode::SAR:
		out << "sar";
		break;
	case Opcode::RCL:
		out << "rcl";
		break;
	case Opcode::RCR:
		out << "rcr";
		break;
	case Opcode::ADD:
		out << "add";
		break;
	case Opcode::ADC:
		out << "adc";
		break;
	case Opcode::SUB:
		out << "sub";
		break;
	case Opcode::SBB:
		out << "sbb";
		break;
	case Opcode::MUL:
		out << "mul";
		break;
	case Opcode::IMUL:
		out << "imul";
		break;
	case Opcode::DIV:
		out << "div";
		break;
	case Opcode::IDIV:
		out << "idiv";
		break;
	case Opcode::NEG:
		out << "neg";
		break;
	case Opcode::CAST:
		out << "cast";
		break;
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, const CCode& ccode) {
	switch (ccode)
	{
	case CCode::EQUAL:
		out << "equal";
		break;
	case CCode::N_EQUAL:
		out << "not_equal";
		break;
	case CCode::ZERO:
		out << "zero";
		break;
	case CCode::N_ZERO:
		out << "not_zero";
		break;
	case CCode::ABOVE:
		out << "above";
		break;
	case CCode::N_ABOVE:
		out << "not_above";
		break;
	case CCode::BELOW:
		out << "below";
		break;
	case CCode::N_BELOW:
		out << "not_below";
		break;
	case CCode::GREATE:
		out << "greate";
		break;
	case CCode::N_GREATE:
		out << "not_greate";
		break;
	case CCode::LESS:
		out << "less";
		break;
	case CCode::N_LESS:
		out << "not_less";
		break;
	case CCode::OVER:
		out << "over";
		break;
	case CCode::N_OVER:
		out << "not_over";
		break;
	case CCode::SIGN:
		out << "sign";
		break;
	case CCode::N_SIGN:
		out << "not_sign";
		break;
	case CCode::LESS_EQUAL:
		out << "less_equal";
		break;
	case CCode::N_LESS_EQUAL:
		out << "not_less_equal";
		break;
	case CCode::ABOVE_EQUAL:
		out << "above_equal";
		break;
	case CCode::N_ABOVE_EQUAL:
		out << "not_above_equal";
		break;
	case CCode::BELOW_EQUAL:
		out << "below_equal";
		break;
	case CCode::N_BELOW_EQUAL:
		out << "not_below_equal";
		break;
	case CCode::GREATE_EQUAL:
		out << "greate_equal";
		break;
	case CCode::N_GREATE_EQUAL:
		out << "not_greate_equal";
		break;
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, const Opsize& opsize) {
	switch (opsize)
	{
	case Opsize::BYTE:
		out << "byte";
		break;

	case Opsize::WORD:
		out << "word";
		break;

	case Opsize::DWORD:
		out << "dword";
		break;

	case Opsize::QWORD:
		out << "qword";
		break;
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, const Argument& arg) {
	size_t value{};

	switch (arg.type) {
	case ArgumentType::ADR:
		value = std::get<Byte>(arg.data);
		out << "A" + std::to_string(value);
		break;

	case ArgumentType::REG:
		value = std::get<Byte>(arg.data);
		out << "R" + std::to_string(value);
		break;

	case ArgumentType::IMMD:
		value = std::get<Qword>(arg.data);
		out << std::to_string(value);
		break;

	case ArgumentType::VAR:
		value = std::get<Dword>(arg.data);
		out << "var[" + std::to_string(value) + "]";
		break;

	case ArgumentType::LBL:
		value = std::get<Dword>(arg.data);
		out << "lbl[" + std::to_string(value) + "]";
		break;
	case ArgumentType::FUNC:
		value = std::get<Dword>(arg.data);
		out << "func[" + std::to_string(value) + "]";
		break;
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, const Function& func) {
	out << "\"func\":\n{\n";
	out << "  \"name\":  " << func.name << "\n";
	out << "  \"line\":  " << func.line << "\n";

	out << "  \"label\":\n  {\n";
	for (auto& label : func.labels) {
		out << "    \"name\":  " << label.name << "\n";
		out << "    \"line\":  " << label.line << "\n";
		out << "\n";
	}
	out << "  }\n";

	out << "  \"instruction\":\n  {\n";
	for (auto& instruction : func.instrList) {
		out << "    \"opcode\":  " << instruction.opcode << "\n";
		if (instruction.ccode != CCode::INV)
			out << "    \"ccode\":   " << instruction.ccode << "\n";
		if (instruction.opsize != Opsize::INV)
			out << "    \"opsize\":  " << instruction.opsize << "\n";
		if (instruction.args[0].type != ArgumentType::INV)
			out << "    \"arg0\":    " << instruction.args[0] << "\n";
		if (instruction.args[1].type != ArgumentType::INV)
			out << "    \"arg1\":    " << instruction.args[1] << "\n";
		if (instruction.args[2].type != ArgumentType::INV)
			out << "    \"arg2\":    " << instruction.args[2] << "\n";
		out << "\n";
	}
	out << "  }\n";
	out << "}\n";
	return out;
}