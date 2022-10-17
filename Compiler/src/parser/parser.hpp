#pragma once
#include <string>
#include <vector>
#include <optional>
#include "token.hpp"

class Parser {
public:
	Parser() = default;
	~Parser() = default;
	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;
	Parser(const Parser&) = default;
	Parser& operator=(const Parser&) = default;

	void setStream(std::istream& stream);

	std::vector<Token> getLine();
	Token getToken();

	Token peekToken();
	void putBack(Token&&);
	
	void parseNext();

	size_t line() const;
	bool eof() const;

	//TokenList tokenList();

	bool isIdentifier(char ch);
	bool isNumber(char ch);
	void skipSpaces();
private:

	std::string parseToken();
	std::string parseId();
	std::string parseChar();
	std::string parseString();
	std::string parseNumber();
	std::string parseComment();

private:
	size_t line_{};
	std::istream* stream_;
	TokenList tokenList_; //tokenList_;
};
