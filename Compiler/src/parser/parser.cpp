#include "parser.hpp"
//#include "buffer.hpp"

#include <fstream>
#include <optional>

#include"..\common\error.hpp"

void Parser::setStream(std::istream& stream) {
	stream_ = &stream;	
	line_ = 1;
	skipSpaces();
}

bool Parser::eof() const {
	return stream_->eof();
}

size_t Parser::line() const {
	return line_;
}

std::vector<Token> Parser::getLine() {
	std::vector<Token> tokens;
	skipSpaces();

	auto startLine = line_;
	auto token = getToken();

	while ((startLine - line_) == 0) {
		tokens.push_back(std::move(token));
		token = getToken();
	}

	putBack(std::move(token));
	return tokens;
}

Token Parser::getToken() {
	Token token;

	while (tokenList_.empty() && !eof()) {
		parseNext();
	}

	token = std::move(tokenList_.back());
	tokenList_.pop_back();
	
	return std::move(token);
}

Token Parser::peekToken() {
	auto token = Token();
	if (tokenList_.empty() && !eof()){
		parseNext();
	}

	token = tokenList_.back();
	return token;
}

void Parser::putBack(Token&& token) {
	tokenList_.push_back(std::move(token));
}

//TokenList Parser::tokenList() {
//	return tokenList_;
//}

void Parser::parseNext() {
	skipSpaces();

	char ch = stream_->peek();
	Token currToken;

	try {
		if (isIdentifier(ch)) {
			currToken.value = parseId();
			currToken.type = TokenType::IDENTIFIER;
		}
		else if (isNumber(ch)) {
			currToken.value = parseNumber();
			currToken.type = TokenType::NUMBER_LITERAL;
		}
		else if (std::ispunct(ch)) {
			if (ch == '\'') {
				currToken.value = parseChar();
				currToken.type = TokenType::CHAR_LITERAL;
			}
			else if (ch == '\"') {
				currToken.value = parseString();
				currToken.type = TokenType::STRING_LITERAL;
			}
			else if (ch == '#') {
				/*currToken.value = */
				parseComment();
				return;
				//currToken.type = TokenType::COMMENT;
			}
			else {
				currToken.value += static_cast<char>(ch);
				currToken.type = TokenType::PUNCT;

				stream_->get();
			}
		}

		tokenList_.push_back(std::move(currToken));
	}
	catch (Error & e) {
		std::cerr << e.getMsg() << std::endl;
	}
}

void Parser::skipSpaces() {
	while (std::isspace(stream_->peek())) {
		auto ch = stream_->get();
		if (ch == '\n' || ch == '\r')
			line_++;
	}
}
bool Parser::isIdentifier(char ch) {
	return ch == '.' || ch == '_' || std::isalpha(ch);
}

bool Parser::isNumber(char ch) {
	return  ch == '+' || ch == '-' || std::isdigit(ch);
}

std::string Parser::parseComment() {
	std::string res = {};

	char ch = stream_->get();

	while (ch != '\n' && ch != '\r' && !eof()) {
		res += ch;
		ch = stream_->get();
	}

	stream_->putback(ch);
	return std::move(res);
}

std::string Parser::parseToken() {
	static bool newLine = false;

	if (newLine) {
		line_++;
		newLine = false;
	}
	
	char ch = stream_->get();
	std::string str = {};

	while (!eof() && !std::isspace(ch)) {
		if (!str.empty() &&	std::ispunct(ch) && ch != '_') {
			stream_->putback(ch);
			break;
		}

		str += ch;
		ch = stream_->get();
	}

	if (ch == '\n' || ch == '\r') {
		if (str.front() != ch)
			newLine = true;
	}

	return str;
}

std::string Parser::parseNumber() {
	auto str = parseToken();
	std::string res{};
	
	size_t num = {};

	if (std::ispunct(str.front())) {
		if (str.front() == '-') {
			res += "-";
		}
		else if (str.front() != '+') {
			throw SYNTAX_ERROR(std::string{ "Invalid number sign" }, str.front(), line());
		}
		str.erase(str.begin());
	}

	size_t end_pos;
	if (str.front() == '0' && str.size() > 1) {
		//auto temp = str.substr(2, str.size() - 2);

		if (str.at(1) == 'x') {
			if (str.substr(2, str.size()).find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos) {
				throw SYNTAX_ERROR(std::string{ "Invalid hexadecimal number" }, str, line());
			}
			num = std::stoull(str, &end_pos, 16);
		}
		else if (str.at(1) == 'b') {
			if (str.substr(2, str.size()).find_first_not_of("01") != std::string::npos) {
				throw SYNTAX_ERROR(std::string{ "Invalid binary number" }, str, line());
			}
			num = std::stoull(str, &end_pos, 2);
		}
		else {
			if (str.substr(1, str.size()).find_first_not_of("01234567") != std::string::npos) {
				throw SYNTAX_ERROR(std::string{ "Invalid octal number" }, str, line());
			}
			num = std::stoull(str, &end_pos, 8);
		}
	}
	else {
		if (str.find_first_not_of("0123456789") != std::string::npos) {
			throw SYNTAX_ERROR(std::string{ "Invalid decimal number" }, str, line());
		}
		num = std::stoull(str, &end_pos);
	}

	if (end_pos != str.size()) {
		throw SYNTAX_ERROR(std::string{ "Invalid nuber" }, res, line());
	}
	
	res += std::to_string(num);

	return std::move(res);
}

std::string Parser::parseId() {
	auto str = parseToken();

	//if (!std::isalpha(str.front()) && str.front() != '_' && str.front() != '.') {
	//	throw SYNTAX_ERROR(std::string{ "Invalid identifier name" }, str, line());
	//}
	//for (auto token : tokens_) {
	//	if (token.second == TokenType::IDENTIFIER) {
	//		if (std::get<std::string>(token.first) == str) {
	//			msg = "Redifinition";
	//			throw SYNTAX_ERROR(msg, str, getLine());
	//		}
	//	}
	//}

	return std::move(str);
}

std::string Parser::parseString() {
	std::string str = {};

	do {
		if (stream_->peek() == '\n') {
			if (str.back() != '\\') {
				throw SYNTAX_ERROR(std::string{ "missing backslashe before newline" }, "\\", line());
			}
			else {
				line_++;
			}
		}
		str += stream_->get();
		
	} while (stream_->peek() != '\"' && !eof());
	
	str += stream_->get();

	auto open = str.find_first_of('\"') + 1;
	auto close = str.find_last_of('\"');
	
	if (close == std::string::npos || close == open - 1) {
		throw SYNTAX_ERROR(std::string{ "missing closing quote" }, "\"", line());
	}

	return std::move(str.substr(open, close - open));
}

std::string Parser::parseChar() {
	std::string str = {};
	uint8_t count = 0;

	stream_->get();
	char ch = stream_->get();

	while (ch != '\'' && ch != '\n' && !eof()) {
		count++;
		str.push_back(ch);
		ch = stream_->get();
	}

	if (ch == '\n' || eof()) {
		stream_->putback(ch);
		throw SYNTAX_ERROR(std::string{ "missing closing quote" }, "\'", line());
	}

	if (count == 0 || str.empty()) {
		throw SYNTAX_ERROR(std::string{ "quoted string should contain at least one character" }, "" , line());
	}

	if (count > 4) {
		throw SYNTAX_ERROR(std::string{ "too many character in character constant" }, str, line());
	}

	return std::move(str);
}
