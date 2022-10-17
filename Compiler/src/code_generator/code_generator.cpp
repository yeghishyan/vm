#include "code_generator.hpp"

//#include <cassert>
#include "..\common\vm_defs.hpp"
#include "variable.hpp"

void CodeGenerator::build(TranslationUnit& trUnit) {
	out_.name = trUnit.name;
	out_.stackSize = trUnit.stackSize;
	//trUnit.entryPoint;

	std::size_t dataIndex = 0;
	std::size_t codeIndex = 0;

	for (auto& variable : trUnit.data) {
		auto& var = variable.second;

		if (var.values.empty()) {
			continue;
		}

		Symbol symbol;
		symbol.symbolType = SymbolType::VAR;
		symbol.valueType = var.type;
		symbol.index = dataIndex;
		auto data = var.getValues();

		out_.data.resize(dataIndex + data.size());
		std::memcpy(&out_.data[dataIndex], data.data(), data.size());

		symbol.size = data.size();
		out_.symbolTable.push_back({ var.name, symbol });

		auto varMapIndex = trUnit.varMap[var.name];
		varIndexMap_[varMapIndex] = symbol.index;

		dataIndex += data.size();
	}

	for (size_t funcIndex = 0; funcIndex < trUnit.code.size(); ++funcIndex) {
		auto& func = trUnit.code.at(funcIndex);

		if (func.instrList.empty()) {
			continue;
		}

		Symbol symbol;
		symbol.symbolType = SymbolType::FUNC;
		symbol.valueType = ValueType::INV;
		symbol.index = codeIndex;
		symbol.size = (func.instrList.size() * instruction_size);
	
		if (funcIndex == trUnit.entryPoint) {
			out_.entryPoint = symbol.index;
		}
	
		out_.symbolTable.push_back({ func.name, symbol });

		out_.code.resize(codeIndex + symbol.size);
		codeIndex += symbol.size;
		
		funcIndexMap_[funcIndex] = symbol.index;

		for (size_t lblIndex = 0; lblIndex < func.labels.size(); ++lblIndex) {
			auto& lbl = func.labels[lblIndex];

			Symbol symbol;
			symbol.symbolType = SymbolType::VAR;
			symbol.valueType = ValueType::INV;
			symbol.index = dataIndex;
			symbol.size = sizeof(lbl.line);

			out_.data.resize(dataIndex + sizeof(lbl.line));
			memcpy(&out_.data[dataIndex], &lbl.line, sizeof(lbl.line));

			dataIndex += sizeof(lbl.line);
			out_.symbolTable.push_back({ lbl.name, symbol });

			lblIndexMap_[lblIndex] = symbol.index;
		}
	}

	codeIndex = 0;

	for (auto& func : trUnit.code) {
		for (auto& instr : func.instrList) {
			//out_.code.resize(codeIndex + instruction_size);

			//*code++ = (uint8_t)instr.opcode;
			uint8_t opcode = static_cast<uint8_t>(instr.opcode);
			memcpy(&out_.code[codeIndex], &opcode, sizeof(uint8_t));
			codeIndex++;

			if (instr.ccode != CCode::INV) {
				uint8_t ccode = static_cast<uint8_t>(instr.ccode);
				memcpy(&out_.code[codeIndex], &ccode, sizeof(uint8_t));
			}
			codeIndex++;

			if (instr.opsize != Opsize::INV) {
				uint8_t opsize = static_cast<uint8_t>(instr.opsize);
				memcpy(&out_.code[codeIndex], &opsize, sizeof(uint8_t));
			}
			codeIndex++;

			size_t argIndex = 0;
			if (instr.opcode == Opcode::CAST) {
				//if (instr.args[0].type == ArgumentType::IMMD)
				auto value = instr.args[0].data;//std::get<uint8_t>(instr.args[0].data);

				//*code++ = static_cast<uint8_t>(instr.args[0].type);
				uint8_t opsize = static_cast<uint8_t>(instr.args[0].type);
				memcpy(&out_.code[codeIndex], &opsize, sizeof(uint8_t));

				codeIndex++;

				memcpy(&out_.code[codeIndex], &value, sizeof(Qword));
				codeIndex += sizeof(Qword);

				argIndex = 1;
			}

			for (argIndex; argIndex < 2; ++argIndex) {
				auto& currArg = instr.args[argIndex];

				if (currArg.type != ArgumentType::INV && argIndex < instr.argCount) {
					//*code++ = (uint8_t)currArg.type;
					uint8_t argtype = static_cast<uint8_t>(currArg.type);
					memcpy(&out_.code[codeIndex], &argtype, sizeof(uint8_t));

					codeIndex++;

					switch (currArg.type) {
					case ArgumentType::ADR: {
						Qword index = currArg.data;//std::get<uint64_t>(currArg.data);
						memcpy(&out_.code[codeIndex], &index, sizeof(Qword));
						break;
					}
					case ArgumentType::REG: {
						Qword index = currArg.data;//std::get<uint64_t>(currArg.data);
						memcpy(&out_.code[codeIndex], &index, sizeof(Qword));
						break;
					}
					case ArgumentType::IMMD: {
						memcpy(&out_.code[codeIndex], &(currArg.data), sizeof(uint64_t));
						break;
					}
					case ArgumentType::VAR: {
						auto index = (currArg.data);//std::get<uint64_t>
						auto address = static_cast<uint64_t>(varIndexMap_.find(index)->second);
						memcpy(&out_.code[codeIndex], &address, sizeof(Qword));
						break;
					}
					case ArgumentType::LBL: {
						auto index = (currArg.data);//std::get<uint64_t>
						auto address = static_cast<uint64_t>(lblIndexMap_.find(index)->second);
						memcpy(&out_.code[codeIndex], &address, sizeof(Qword));
						break;
					}
					case ArgumentType::FUNC: {
						auto index = (currArg.data);//std::get<uint64_t>
						auto address = static_cast<uint64_t>(funcIndexMap_.find(index)->second);
						memcpy(&out_.code[codeIndex], &address, sizeof(Qword));
						break;
					}
					}
				}
				else {
					codeIndex += sizeof(Byte);
				}
				codeIndex += sizeof(Qword);
			}
		}
	}
}

ProgramOutput CodeGenerator::out() {
	return out_;
}
