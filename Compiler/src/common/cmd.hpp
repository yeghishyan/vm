#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

//-----------------------------------------------------------------------------------
//	Option:					|	Argument:					|	Description:		|
//--------------------------|-------------------------------|-----------------------|
//	[-h | --help  ]			|								|	show help menu		|
//	[-d | --debug ]			|								|	debug enable flag	|
//	[-o | --output]			|	std::string					|	output file name	|
//	<file0> ... <filen>		|	std::vector<std::string>	|	input file(s)		|
//-----------------------------------------------------------------------------------


class CmdParser {
public:
	CmdParser();
	~CmdParser() = default;
	//CommandLineParser(const CommandLineParser&) = default;
	//CommandLineParser& operator=(CommandLineParser&&) = default;
	//CommandLineParser(const CommandLineParser&) = default;
	//CommandLineParser& operator=(CommandLineParser&&) = default;

	bool parse(int argc, char** argv);

	inline bool debugFlag() const;
	inline const std::string outputFile() const;
	inline std::vector<std::string> inputFiles() const;
	//inline po::options_description usage() const;

private:

	void setup();

private:
	po::options_description desc_{ "Allowed options" };
	po::variables_map varMap_{};
	
	bool debugFlag_{ false };
	std::string outputFile_{};
	std::vector<std::string> inputFiles_{};
};

inline bool CmdParser::debugFlag() const {
	return debugFlag_;
}

inline const std::string CmdParser::outputFile() const {
	return outputFile_;
}

inline std::vector<std::string> CmdParser::inputFiles() const {
	return inputFiles_;
}
