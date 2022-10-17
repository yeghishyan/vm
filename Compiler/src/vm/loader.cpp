#include "loader.hpp"

#include <string>
#include <iterator>
#include <unordered_map>

#include "../code_generator/code_defs.hpp"
#include "memory.hpp"

#include <iostream>

//using SymbolTable = std::unordered_map<std::string, SymbolInfo>;

Loader::Loader(std::shared_ptr<Memory> ram)
	: ram_(ram) {}

void Loader::setPrg(ProgramOutput& prgOut) {
	prgOut_ = prgOut;
}

void Loader::load(std::istream& istream) {
	ProgramInfo pInfo;
	istream.read((char*)&pInfo, sizeof(ProgramInfo));
	
	uint16_t stackSize = 0;
	istream.seekg(pInfo.stackSizePos);
	istream.read((char*)&stackSize, pInfo.stackSizeLen);
	
	ram_->resize(stackSize);

	uint16_t entryPoint;
	istream.seekg(pInfo.entryPointPos);
	istream.read((char*)&entryPoint, pInfo.entryPointSize);

	//std::vector<uint8_t> code;
	//code.resize(pInfo.codeSectionSize);
	istream.seekg(pInfo.codeSectionPos);
	//istream.read((char*)&code, pInfo.codeSectionSize);
	ram_->writeCode(prgOut_.code);
	
	//std::vector<uint8_t> data;
	//data.resize(pInfo.dataSectionSize);
	istream.seekg(pInfo.dataSectionPos);
	//istream.read((char*)&data, pInfo.dataSectionSize);
	ram_->writeData(prgOut_.data);

	ram_->push(entryPoint);

	istream.seekg(pInfo.symbolTablePos);

	while(!istream.eof()) {
		std::string  name{};

		while (istream.peek() != '\0') {
			name += istream.get();
		}
		istream.get();

		Symbol sInfo;
		istream.read((char*)&sInfo, sizeof(Symbol));

		//symbols.emplace(name, sInfo);
	}

	ram_->setSymbolTable(prgOut_.symbolTable);
}
