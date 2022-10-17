#pragma once
#include <istream>

#include "..\common\vm_defs.hpp"
#include "..\code_generator\variable.hpp"
#include "..\code_generator\function.hpp"
#include "../code_generator/code_generator.hpp"

class Memory;

class Loader {
public:
	Loader(std::shared_ptr<Memory> ram);

	~Loader() = default;

	void setPrg(ProgramOutput& prgOut);

public:
	void load(std::istream& istream);

private:
	std::shared_ptr<Memory> ram_;

	//SymbolTable symbols_;
	ProgramOutput prgOut_;
};