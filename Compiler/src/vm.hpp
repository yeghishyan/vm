#pragma once
#include <vector>
#include <string>
#include <memory>

#include "common\cmd.hpp"
#include "code_generator\code_generator.hpp"

class VirtualMachine {
	struct CompilerFlags {
		bool debug{ false };
	};

public:
	VirtualMachine();
	~VirtualMachine() = default;
	//Compiler(const Compiler&) = delete;
	//Compiler& operator=(const Compiler&) = delete;
	//Compiler(Compiler&&) = delete;
	//Compiler& operator=(Compiler&&) = delete;

	void compile(int argc, char** argv);
	void run(const std::string& file);

private:
	void parse(const std::string& file);
	void write(ProgramOutput& progOut);
	
private:
	std::unique_ptr<CmdParser> cmdParser_;

	std::vector<std::string> fileList_;
	std::string outputFile_;
	CompilerFlags cmpFlags_;
};