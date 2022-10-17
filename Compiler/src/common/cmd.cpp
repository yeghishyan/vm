#include "cmd.hpp"

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include <boost/algorithm/string.hpp>

#include "error.hpp"

CmdParser::CmdParser() {
	setup();
}

void CmdParser::setup() {
	desc_.add_options()
		("help", "Display help menu")
		("debug", po::bool_switch(&debugFlag_)->default_value(false), "Debug enable flag")
		("output", po::value<std::string>(&outputFile_)->default_value("a"), "Output file name")
		("compile", po::value<std::vector<std::string>>(&inputFiles_)->required(), "Input file(s)");
}

bool CmdParser::parse(int argc, char** argv) {
	try {
		po::store(po::parse_command_line(argc, argv, desc_), varMap_);
		po::notify(varMap_);

		if (varMap_.count("help")) {
			std::cout << desc_ << std::endl;
		}

		for (auto file : inputFiles_) {
			if (!boost::ends_with(file, ".asm")) {
				throw UNRECOGNIZED_FILE(file);
			}

			if (!fs::exists(file)) {
				throw FILE_OPEN_ERROR(file);
			}
		}

		return true;
	}
	catch (po::required_option & e) {
		std::cerr << "required option error: " << e.what() << std::endl;
	}
	catch (po::error & e) {
		std::cerr << "program option error: " << e.what() << std::endl;
	}

	return false;
}
