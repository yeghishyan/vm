#pragma once
#include <string>

enum class TokenType {
	//KEYWORD,
	//COMMENT,
	PUNCT,
	IDENTIFIER, 
	CHAR_LITERAL,
	NUMBER_LITERAL,
	STRING_LITERAL,
};

using TokenValue = std::string;

struct Token {
	Token(TokenType t,const TokenValue& v)
		: type(t)
		, value(v)
	{}

	Token() = default;
	~Token() = default;
	Token(Token&&) = default;
	Token(const Token&) = default;
	Token& operator=(Token&&) = default;
	Token& operator=(const Token&) = default;

	TokenType type{};
	TokenValue value{};
};
using TokenList = std::vector<Token>;
