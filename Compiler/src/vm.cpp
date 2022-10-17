#include "vm.hpp"

#include <iostream>

#include "common\cmd.hpp"
#include "common\error.hpp"

#include "code_parser\code_parser.hpp"
#include "code_generator/code_generator.hpp"

#include "vm/loader.hpp"
#include "vm/memory.hpp"
#include "vm/cpu.hpp"

ProgramOutput global_out;

VirtualMachine::VirtualMachine()
	:cmdParser_(std::make_unique<CmdParser>())
{}

void VirtualMachine::compile(int argc, char** argv) {
	if (cmdParser_->parse(argc, argv)) {
		fileList_ = cmdParser_->inputFiles();
		outputFile_ = cmdParser_->outputFile();
		cmpFlags_.debug = cmdParser_->debugFlag();

		for (auto& file : fileList_) {
			parse(file);
		}
	}
}

void VirtualMachine::run(const std::string& file) {
	std::ifstream inFile;

	inFile.open(file, std::ios::binary | std::ios::in);

	if (!inFile.is_open()) {
		throw FILE_OPEN_ERROR(file);
	}

	auto ram = std::make_shared<Memory>();
	
	Loader loader(ram);
	loader.setPrg(global_out);
	loader.load(inFile);

	Cpu cpu(ram);

	cpu.run();
}

void VirtualMachine::parse(const std::string& file) {
	CodeParser codeParser;

	try {
		codeParser.parse(file);
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
	
	auto trUnit = codeParser.trUnit();
	
	CodeGenerator codeGen;
	try {
		codeGen.build(trUnit);
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}

	auto programOut = codeGen.out();

	write(programOut);
}

void VirtualMachine::write(ProgramOutput& out) {
	std::ofstream outFile;

	global_out = out;
	
	outFile.open({ out.name + ".bin" }, std::ios::binary | std::ios::out);

	if (!outFile.is_open()) {
		throw FILE_OPEN_ERROR(out.name + ".bin");
	}

	ProgramInfo pInfo;

	pInfo.stackSizePos = sizeof(ProgramInfo);
	pInfo.stackSizeLen = sizeof(uint16_t);
	
	pInfo.entryPointPos = pInfo.stackSizePos + pInfo.stackSizeLen;
	pInfo.entryPointSize = sizeof(uint16_t);

	pInfo.codeSectionPos = pInfo.entryPointPos + pInfo.entryPointSize;
	pInfo.codeSectionSize = out.code.size();

	pInfo.dataSectionPos = pInfo.codeSectionPos + pInfo.codeSectionSize;
	pInfo.dataSectionSize = out.data.size();

	pInfo.symbolTablePos = pInfo.dataSectionPos + pInfo.dataSectionSize;
	
	outFile.write((const char*)(&pInfo), sizeof(ProgramInfo));
	outFile.write((const char*)(&out.stackSize), sizeof(uint16_t));
	outFile.write((const char*)(&out.entryPoint), sizeof(uint16_t));
	outFile.write((const char*)(out.code.data()), out.code.size());
	outFile.write((const char*)(&out.data), out.data.size());

	for (auto& symbol : out.symbolTable) {
		const char* name = symbol.first.c_str();

		Symbol sInfo;

		sInfo.symbolType = SymbolType(static_cast<uint8_t>(symbol.second.symbolType));
		sInfo.valueType = ValueType(static_cast<uint8_t>(symbol.second.valueType));
		sInfo.index = symbol.second.index;
		sInfo.size = symbol.second.size;

		outFile.write(name, sizeof(name) + 1);
		outFile.write((const char*)&sInfo, sizeof(Symbol));

		//binSymbolTable.resize(
		//	symbolIndex + sizeof(name)
		//	+ sizeof(symbolType) + sizeof(valueType)
		//	+ sizeof(index) + sizeof(size)
		//);
		//memcpy(&binSymbolTable[symbolIndex], &name, sizeof(name));
		//symbolIndex += sizeof(name);
		//memcpy(&binSymbolTable[symbolIndex], &symbolType, sizeof(symbolType));
		//symbolIndex += sizeof(symbolType);
		//memcpy(&binSymbolTable[symbolIndex], &valueType, sizeof(valueType));
		//symbolIndex += sizeof(valueType);
		//memcpy(&binSymbolTable[symbolIndex], &index, sizeof(index));
		//symbolIndex += sizeof(index);
		//memcpy(&binSymbolTable[symbolIndex], &size, sizeof(size));
		//symbolIndex += sizeof(size);
	}

	outFile.flush();
	outFile.close();

	//outFile.write((const char*)(&binSymbolTable), binSymbolTable.size());
}
