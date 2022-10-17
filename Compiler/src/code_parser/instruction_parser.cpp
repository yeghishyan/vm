#include "instruction_parser.hpp"

#include "..\common\error.hpp"
#include "..\common\vm_defs.hpp"

InstructionParser::InstructionParser(SymbolMap& variableMap)
	: variables_(variableMap) {}

InstructionList InstructionParser::data() {
	return instructions_;
}

void InstructionParser::setFunctionMap(SymbolMap& functionMap) {
	functions_ = functionMap;
}

void InstructionParser::setLabelMap(SymbolMap& labelMap) {
	labels_ = labelMap;
}

void InstructionParser::parseInstructions(std::vector<std::pair<std::size_t, std::vector<std::string>>>& tokens) {
	for (auto& line : tokens) {
		try {
			auto instr = parseInstruction(line.second, line.first);
			instructions_.push_back(instr);
		}
		catch (Error & e) {
			std::cerr << e.getMsg() << std::endl;
		}
	}
}

Instruction InstructionParser::parseInstruction(std::vector<std::string>& tokenLine, std::size_t lineNumber) {
	auto currIndex = 0;

	std::string opName = tokenLine.at(currIndex++);

	if (opName.empty()) {
		throw Error(std::string{ "missing opcode in line: " + std::to_string(lineNumber) });
	}

	auto instrInfo = instructionMap_.find(opName.c_str());
	if (instrInfo == instructionMap_.end()) {
		throw Error(std::string{ "invalid opcode " + opName + " in line: " + std::to_string(lineNumber) });
	}

	Instruction instr;
	instr.opcode = instrInfo->second.opcode;

	if (instrInfo->second.ccode != Requirement::NON && tokenLine.size() > currIndex) {
		auto ccodeStr = tokenLine.at(currIndex++);
		auto ccode = ccodeMap_.find(ccodeStr.c_str());

		if (ccode == ccodeMap_.end()) {
			currIndex--;
			if (instrInfo->second.ccode == Requirement::MND) {
				throw Error(std::string{ opName + " instruction must have ccode: line: " + std::to_string(lineNumber) });
			}
		}
		else {
			instr.ccode = ccode->second;
		}
	}

	if (instrInfo->second.opsize != Requirement::NON && tokenLine.size() > currIndex) {
		auto opsizeStr = tokenLine.at(currIndex++);
		auto c = opsizeStr.c_str();
		auto opsize = opsizeMap_.find(opsizeStr.c_str());

		if (opsize == opsizeMap_.end()) {
			currIndex--;
			if (instrInfo->second.ccode == Requirement::MND) {
				throw Error(std::string{ opName + " instruction must have opsize: line: " + std::to_string(lineNumber) });
			}
		}
		else {
			instr.opsize = opsize->second;
		}
	}

	size_t argIndex = 0;

	if (instrInfo->second.opcode == Opcode::CAST) {
		if (tokenLine.size() < currIndex) {
			throw Error(std::string{ "missing target opsize: line: " + std::to_string(lineNumber) });
		}

		auto opsizeStr = tokenLine.at(currIndex++);
		auto opsize = opsizeMap_.find(opsizeStr.c_str());

		if (opsize == opsizeMap_.end()) {
			currIndex--;
			throw Error(std::string{ "missing target opsize: line: " + std::to_string(lineNumber) });
		}

		instr.args[0].type = ArgumentType::IMMD;
		instr.args[0].data = (uint64_t)opsize->second;//.emplace<3>((uint64_t)opsize->second);
		argIndex++;
	}

	auto argCount = instrInfo->second.count;
	instr.argCount = argCount;

	for (argIndex; argIndex < argCount; ++argIndex, ++currIndex) {
		if (tokenLine.size() > currIndex) {
			auto arg = parseArgument(tokenLine.at(currIndex), lineNumber);

			switch (arg.type) {
			case ArgumentType::VAR:
			case ArgumentType::LBL:
			case ArgumentType::FUNC:
			case ArgumentType::ADR:
				if (instrInfo->second.argtype[argIndex] == ArgumentType::ADR ||
					instrInfo->second.argtype[argIndex] == ArgumentType::ADR_REG)
				{
					break;
				}
				throw Error(std::string{ tokenLine.at(currIndex) + " invalid argument type: line: " + std::to_string(lineNumber) });
			case ArgumentType::REG:
				if (instrInfo->second.argtype[argIndex] == ArgumentType::REG ||
					instrInfo->second.argtype[argIndex] == ArgumentType::ADR_REG ||
					instrInfo->second.argtype[argIndex] == ArgumentType::REG_IMMD ||
					instrInfo->second.argtype[argIndex] == ArgumentType::OPT_REG_IMMD)
				{
					break;
				}
				throw Error(std::string{ tokenLine.at(currIndex) + " invalid argument type: line: " + std::to_string(lineNumber) });

			case ArgumentType::IMMD:
				if (instrInfo->second.argtype[argIndex] == ArgumentType::IMMD ||
					instrInfo->second.argtype[argIndex] == ArgumentType::OPT_IMMD ||
					instrInfo->second.argtype[argIndex] == ArgumentType::REG_IMMD ||
					instrInfo->second.argtype[argIndex] == ArgumentType::OPT_REG_IMMD)
				{
					break;
				}
				throw Error(std::string{ tokenLine.at(currIndex) + " invalid argument type: line: " + std::to_string(lineNumber) }); //TODO
			}

			instr.args[argIndex] = arg;
		}
		/*
				case ArgumentType::OPT_IMMD:
				case ArgumentType::OPT_REG_IMMD:
		*/
	}

	return instr;
}

Argument InstructionParser::parseArgument(std::string& token, size_t lineNumber) {
	Argument arg;
	SymbolMap::iterator it;

	if (token.front() == 'R' || token.front() == 'A') {
		auto num = token.substr(1, token.size());
		if (num.find_first_not_of("1234567890") == std::string::npos) {
			if (token.front() == 'R') {
				arg.type = ArgumentType::REG;
			}
			else {
				arg.type = ArgumentType::ADR;
			}
			arg.data = std::stoull(num); //.emplace<0>((static_cast<Byte>(std::stoul(num))));

			return arg;
		}
	}
	if (token == "RIP") {
		arg.type = ArgumentType::ADR;
		arg.data = rip_index_number;//arg.data.emplace<0>(rip_index_number);

		return arg;
	}
	if (token == "SP") {
		arg.type = ArgumentType::ADR;
		arg.data = sp_index_number; //arg.data.emplace<0>(sp_index_number);

		return arg;
	}
	if (token == "SF") {
		arg.type = ArgumentType::ADR;
		arg.data = arg.data = sf_index_number;//arg.data.emplace<0>(sf_index_number);

		return arg;
	}
	if (token.front() == '-' || token.front() == '+' || std::isdigit(token.front())) {
		arg.type = ArgumentType::IMMD;
		arg.data = std::stoull(token); //arg.data.emplace<3>(std::stoull(token));

		return arg;
	}
	if ((it = labels_.find(token.c_str())) != labels_.end()) {
		arg.type = ArgumentType::LBL;
		arg.data = it->second; //arg.data.emplace<2>(static_cast<Dword>(it->second));

		return arg;
	}
	if ((it = functions_.find(token.c_str())) != functions_.end()) {
		arg.type = ArgumentType::FUNC;
		arg.data = it->second;	//arg.data.emplace<2>(static_cast<Dword>(it->second));
		
		return arg;
	}
	if ((it = variables_.find(token.c_str())) != variables_.end()) {
		arg.type = ArgumentType::VAR;
		arg.data = it->second;	//arg.data.emplace<2>(static_cast<Dword>(it->second));
		
		return arg;
	}

	throw Error(std::string{ "invalid argument \"" + token + "\" : line: " + std::to_string(lineNumber) });
}