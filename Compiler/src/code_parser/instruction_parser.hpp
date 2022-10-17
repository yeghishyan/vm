#pragma once
#include <variant>
#include <optional>
#include <unordered_map>

#include "../code_generator/instruction.hpp"
#include "../code_generator/instruction_defs.hpp"

using SymbolMap = std::unordered_map<std::string, std::size_t>;

class InstructionParser {
public:
	InstructionParser(SymbolMap& variableMap);

	~InstructionParser() = default;
	InstructionParser(const InstructionParser&) = default;
	InstructionParser& operator=(const InstructionParser&) = default;
	InstructionParser(InstructionParser&&) = default;
	InstructionParser& operator=(InstructionParser&&) = default;

	void parseInstructions(std::vector<std::pair<std::size_t, std::vector<std::string>>>& tokens);
	Instruction parseInstruction(std::vector<std::string>& tokenLine, std::size_t lineNumber);

	Argument parseArgument(std::string& token, size_t lineNumber);
	InstructionList data();

	void setFunctionMap(SymbolMap& functionMap);
	void setLabelMap(SymbolMap& labelMap);


private:
	struct InstructionInfo {
		using ArgCount = uint8_t;

		InstructionInfo(Opcode op, Requirement cc, Requirement ops)
			: opcode(op), ccode(cc), opsize(ops), count{ 0 } {}

		InstructionInfo(Opcode op, Requirement cc, Requirement ops, ArgumentType argt1)
			: opcode(op), ccode(cc), opsize(ops), argtype{ argt1 }, count{ 1 } {}

		InstructionInfo(Opcode op, Requirement cc, Requirement ops, ArgumentType argt1, ArgumentType argt2)
			: opcode(op), ccode(cc), opsize(ops), argtype{ argt1, argt2 }, count{ 2 } {}

		InstructionInfo(Opcode op, Requirement cc, Requirement ops, ArgumentType argt1, ArgumentType argt2, ArgumentType argt3)
			: opcode(op), ccode(cc), opsize(ops), argtype{ argt1, argt2, argt3 }, count{ 3 } {}

		Opcode opcode{};
		Requirement ccode{};
		Requirement opsize{};
		ArgCount count{};
		ArgumentType argtype[3]{};
	};

private:
	using InstructionMap = std::unordered_map<std::string, InstructionInfo>;
	using OpsizeMap = std::unordered_map<std::string, Opsize>;
	using CCodeMap = std::unordered_map<std::string, CCode>;

	const OpsizeMap opsizeMap_= {
		{ "B", Opsize::BYTE },
		{ "W",		Opsize::WORD },
		{ "DW",		Opsize::DWORD },
		{ "QW",		Opsize::QWORD },
	};
	const CCodeMap ccodeMap_ = {
		{ "E",			CCode::EQUAL			},
		{ "Z, ",		CCode::ZERO				},
		{ "NE",			CCode::N_EQUAL			},
		{ "NZ",			CCode::N_ZERO			},
		{ "A",			CCode::ABOVE			},
		{ "NB",			CCode::N_BELOW			},
		{ "AE",			CCode::ABOVE_EQUAL		},
		{ "NB",			CCode::N_BELOW			},
		{ "B",			CCode::BELOW			},
		{ "NAE",		CCode::N_ABOVE_EQUAL	},
		{ "BE",			CCode::BELOW_EQUAL		},
		{ "NA",			CCode::N_ABOVE			},
		{ "G",			CCode::GREATE			},
		{ "NLE",		CCode::N_LESS_EQUAL		},
		{ "GE",			CCode::GREATE_EQUAL		},
		{ "NL",			CCode::N_LESS			},
		{ "L, ",		CCode::LESS				},
		{ "NGE",		CCode::N_GREATE_EQUAL	},
		{ "LE",			CCode::LESS_EQUAL		},
		{ "NG",			CCode::N_GREATE			},
		{ "O",			CCode::OVER				},
		{ "NO",			CCode::N_OVER			},
		{ "S",			CCode::SIGN				},
		{ "NS",			CCode::N_SIGN			},
	};
	const InstructionMap instructionMap_ = {
		{	"NOP",			{ Opcode::NOP	, Requirement::NON, Requirement::NON }},
		{	"BREAK",		{ Opcode::BREAK	, Requirement::NON, Requirement::NON }},
		{	"EndF",			{ Opcode::ENDF	, Requirement::NON, Requirement::NON }},
		{	"RET",			{ Opcode::RET	, Requirement::NON, Requirement::NON }},

		{	"JUMPA",		{ Opcode::JUMPA	, Requirement::NON, Requirement::NON, ArgumentType::ADR }},
		{	"JUMPR",		{ Opcode::JUMPR	, Requirement::OPT, Requirement::NON, ArgumentType::REG, ArgumentType::IMMD }},
		////
		{	"CALL",			{ Opcode::CALL	, Requirement::NON, Requirement::NON, ArgumentType::ADR }},
		{	"GFLR",			{ Opcode::GFLR	, Requirement::NON, Requirement::NON, ArgumentType::REG }},
		{	"SFLR",			{ Opcode::SFLR	, Requirement::NON, Requirement::NON, ArgumentType::REG }},

		{	"LOAD",			{ Opcode::LOAD 	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR }},
		{	"STORE",		{ Opcode::STORE	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR }},
		{	"LDREL",		{ Opcode::LDREL	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR }},
		//
		{	"STREL",		{ Opcode::STREL	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR }},
		//

		{	"PUSHSF",		{ Opcode::PUSHSF, Requirement::NON, Requirement::NON, ArgumentType::OPT_REG_IMMD}},
		{	"POPSF",		{ Opcode::POPSF , Requirement::NON, Requirement::NON, ArgumentType::OPT_REG_IMMD}},
		{	"PUSH",			{ Opcode::PUSH	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::OPT_IMMD}},
		{	"POP",			{ Opcode::POP	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::OPT_IMMD}},
		{	"PUSHA",		{ Opcode::PUSHA	, Requirement::NON, Requirement::NON, ArgumentType::ADR}},
		{	"POPA",			{ Opcode::POPA	, Requirement::NON, Requirement::NON, ArgumentType::ADR}},
		{	"POPR",			{ Opcode::POPR	, Requirement::NON, Requirement::NON, ArgumentType::REG, ArgumentType::IMMD}},

		{	"ASSIGNA",		{ Opcode::ASSIGNA, Requirement::NON, Requirement::NON, ArgumentType::ADR, ArgumentType::ADR}},
		{	"ASSIGNR",		{ Opcode::ASSIGNR, Requirement::NON, Requirement::NON, ArgumentType::REG, ArgumentType::IMMD}},

		{	"SET",			{ Opcode::SET	, Requirement::MND, Requirement::OPT, ArgumentType::REG}},

		{	"MOVA",			{ Opcode::MOVA	, Requirement::OPT, Requirement::NON, ArgumentType::ADR, ArgumentType::ADR_REG}},
		{	"MOVR",			{ Opcode::MOVR	, Requirement::OPT, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		//
		{	"SWAPA",		{ Opcode::SWAPA	, Requirement::OPT, Requirement::NON, ArgumentType::ADR, ArgumentType::ADR_REG}},
		{	"SWAPR",		{ Opcode::SWAPR	, Requirement::OPT, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		//

		{	"INC",			{ Opcode::INC	, Requirement::NON, Requirement::NON, ArgumentType::ADR, ArgumentType::REG_IMMD}},
		{	"DEC",			{ Opcode::DEC	, Requirement::NON, Requirement::NON, ArgumentType::ADR, ArgumentType::REG_IMMD}},

		{	"TEST",			{ Opcode::TEST	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR_REG}},
		{	"CMP",			{ Opcode::CMP	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR_REG}},

		{	"IN",			{ Opcode::IN	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"OUT",			{ Opcode::OUT	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},

		{	"AND",			{ Opcode::AND	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"OR",			{ Opcode::OR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"XOR",			{ Opcode::XOR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"NAND",			{ Opcode::NAND	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"NOR",			{ Opcode::NOR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"NOT",			{ Opcode::NOT	, Requirement::NON, Requirement::OPT, ArgumentType::REG}},

		{	"SHL",			{ Opcode::SHL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"SHR",			{ Opcode::SHR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"ROL",			{ Opcode::ROL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"ROR",			{ Opcode::ROR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"SAL",			{ Opcode::SAL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"SAR",			{ Opcode::SAR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"RCL",			{ Opcode::RCL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},
		{	"RCR",			{ Opcode::RCR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD}},

		{	"ADD",			{ Opcode::ADD	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"ADC",			{ Opcode::ADC	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"SUB",			{ Opcode::SUB	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"SBB",			{ Opcode::SBB	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"MUL",			{ Opcode::MUL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"IMUL",			{ Opcode::IMUL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"DIV",			{ Opcode::DIV	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"IDIV",			{ Opcode::IDIV	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG}},
		{	"NEG",			{ Opcode::NEG	, Requirement::NON, Requirement::OPT, ArgumentType::REG}},
		{	"CAST",			{ Opcode::CAST	, Requirement::NON, Requirement::MND, ArgumentType::IMMD, ArgumentType::REG}},
	};

private:
	SymbolMap variables_;
	SymbolMap functions_;
	SymbolMap labels_;

	InstructionList instructions_;
};
//using OpsizeMap = InstructionParser::OpsizeMap;
//using CCodeMap = InstructionParser::CCodeMap;
//using InstructionMap = InstructionParser::InstructionMap;
//using InstructionInfo = InstructionParser::InstructionInfo;
