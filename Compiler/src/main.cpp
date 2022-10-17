#include <iostream>
#include <fstream>

#include "vm.hpp"
#include "common/error.hpp"

int main(int argc, char* argv[]) try {
	VirtualMachine* cmp = new VirtualMachine;
	
	cmp->compile(argc, argv);
	cmp->run("test.asm.bin");

	delete cmp;
}
catch (Error & e) {
	std::cerr << e.getMsg() << std::endl;
}
catch (std::exception & e) {
	std::cerr << "Unhandled Error: " << e.what()
		<< ", application will now exit." << std::endl;
	system("pause");
}
catch (...) {
	std::cerr << "Unknown Error. "<<std::endl;
}