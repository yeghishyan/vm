#pragma once
//
//#include <unordered_map>
//#include "..\code_generator\instruction_defs.hpp"
//
//struct CommandInfo {
//	Opcode opcode;
//	CCode ccode;
//	Opsize opsize;
//	ArgumentType argtype[2];
//};
//
//using CommandMap = std::unordered_map<Opcode, CommandInfo>;
//
//const CommandMap commandMap = {
//	{ Opcode::NOP	, Requirement::NON, Requirement::NON },
//	{ Opcode::BREAK	, Requirement::NON, Requirement::NON },
//	{ Opcode::ENDF	, Requirement::NON, Requirement::NON },
//	{ Opcode::RET	, Requirement::NON, Requirement::NON },
//
//	{ Opcode::JUMPA	, Requirement::NON, Requirement::NON, ArgumentType::ADR },
//	{ Opcode::JUMPR	, Requirement::OPT, Requirement::NON, ArgumentType::REG, ArgumentType::IMMD },
//
//	{ Opcode::CALL	, Requirement::NON, Requirement::NON, ArgumentType::ADR },
//	{ Opcode::GFLR	, Requirement::NON, Requirement::NON, ArgumentType::REG },
//	{ Opcode::SFLR	, Requirement::NON, Requirement::NON, ArgumentType::REG },
//
//	{ Opcode::LOAD 	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR },
//	{ Opcode::STORE	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR },
//	{ Opcode::LDREL	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR },
//
//	{ Opcode::STREL	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR },
//
//
//	{ Opcode::PUSHSF, Requirement::NON, Requirement::NON, ArgumentType::OPT_REG_IMMD },
//	{ Opcode::POPSF , Requirement::NON, Requirement::NON, ArgumentType::OPT_REG_IMMD },
//	{ Opcode::PUSH	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::OPT_IMMD },
//	{ Opcode::POP	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::OPT_IMMD },
//	{ Opcode::PUSHA	, Requirement::NON, Requirement::NON, ArgumentType::ADR },
//	{ Opcode::POPA	, Requirement::NON, Requirement::NON, ArgumentType::ADR },
//	{ Opcode::POPR	, Requirement::NON, Requirement::NON, ArgumentType::REG, ArgumentType::IMMD },
//
//	{ Opcode::ASSIGNA, Requirement::NON, Requirement::NON, ArgumentType::ADR, ArgumentType::ADR },
//	{ Opcode::ASSIGNR, Requirement::NON, Requirement::NON, ArgumentType::REG, ArgumentType::IMMD },
//
//	{ Opcode::SET	, Requirement::MND, Requirement::OPT, ArgumentType::REG },
//
//	{ Opcode::MOVA	, Requirement::OPT, Requirement::NON, ArgumentType::ADR, ArgumentType::ADR_REG },
//	{ Opcode::MOVR	, Requirement::OPT, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//
//	{ Opcode::SWAPA	, Requirement::OPT, Requirement::NON, ArgumentType::ADR, ArgumentType::ADR_REG },
//	{ Opcode::SWAPR	, Requirement::OPT, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//
//
//	{ Opcode::INC	, Requirement::NON, Requirement::NON, ArgumentType::ADR, ArgumentType::REG_IMMD },
//	{ Opcode::DEC	, Requirement::NON, Requirement::NON, ArgumentType::ADR, ArgumentType::REG_IMMD },
//
//	{ Opcode::TEST	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR_REG },
//	{ Opcode::CMP	, Requirement::NON, Requirement::OPT, ArgumentType::ADR_REG, ArgumentType::ADR_REG },
//
//	{ Opcode::IN	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::OUT	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//
//	{ Opcode::AND	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::OR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::XOR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::NAND	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::NOR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::NOT	, Requirement::NON, Requirement::OPT, ArgumentType::REG },
//
//	{ Opcode::SHL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::SHR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::ROL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::ROR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::SAL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::SAR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::RCL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//	{ Opcode::RCR	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG_IMMD },
//
//	{ Opcode::ADD	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::ADC	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::SUB	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::SBB	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::MUL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::IMUL	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::DIV	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::IDIV	, Requirement::NON, Requirement::OPT, ArgumentType::REG, ArgumentType::REG },
//	{ Opcode::NEG	, Requirement::NON, Requirement::OPT, ArgumentType::REG },
//	{ Opcode::CAST	, Requirement::NON, Requirement::MND, ArgumentType::IMMD, ArgumentType::REG },
//};