#pragma once
#include <exception>
#include <iostream>
#include <string>

class Parser;

class Error : public std::exception {
	friend class CodeParser;
public:
	Error(std::string&& desc = "");

	virtual ~Error() = default;

	Error(const Error&) = default;
	Error& operator=(const Error&) = default;

	std::string getMsg() const noexcept;

protected:
	std::string desc_;

private:
	static void setFile(const std::string& file);
	static std::string file_;
};


#define FILE_OPEN_ERROR(file) Error(file + " Cannot open source file: No such file or directory.");
/*
['main.cpp'] Cannot open source file: No such file or directory.
*/

#define UNRECOGNIZED_FILE(file) Error(file + " Urecognized source file type.");
/*
compile error: ['main'] Unrecognized source file type.
*/

#define SYNTAX_ERROR(desc, token, line) Error("sytax error: " + desc + " <" + token + "> in line: " + std::to_string(line));
/*
['main.cpp'] sytax error: %token in line: %line
*/