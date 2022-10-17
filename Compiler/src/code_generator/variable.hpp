#pragma once
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include "../parser/token.hpp"
#include "../common/defs.hpp"
#include "variable_defs.hpp"

#include <ostream>

struct Value {
	//Value(const std::string& str, TokenType type) {
	//	init(str, type);
	//}

	Value() = default;
	~Value() = default;
	Value(Value&&) = default;
	Value(const Value&) = default;
	Value& operator=(Value&&) = default;
	Value& operator=(const Value&) = default;

	void init(const std::string& str, ValueType type) {

		switch (type)
		{
		case ValueType::CHAR: {
			data.emplace<0>(str.at(0));
			break;
		}
		case ValueType::BYTE: {
			Byte value = static_cast<Byte>(std::stoull(str));
			data.emplace<1>(std::move(value));
			break;
		}
		case ValueType::WORD: {
			Word value = static_cast<Word>(std::stoull(str));
			data.emplace<2>(std::move(value));
			break;
		}
		case ValueType::DWORD: {
			Dword value = static_cast<Dword>(std::stoull(str));
			data.emplace<3>(std::move(value));
			break;
		}
		case ValueType::QWORD: {
			Qword value = std::stoull(str);
			data.emplace<4>(std::move(value));
			break;
		}
		}
	}

	std::variant<
		Char,			//0
		Byte,			//1
		Word,			//2
		Dword,			//3
		Qword			//4
	> data;
};
using ValueList = std::vector<Value>;

struct Variable {
	Variable() = default;
	~Variable() = default;
	Variable(Variable&&) = default;
	Variable(const Variable&) = default;
	Variable& operator=(Variable&&) = default;
	Variable& operator=(const Variable&) = default;
	
	void addValue(const Token& token) {
		switch (token.type)
		{
			case TokenType::CHAR_LITERAL:
			case TokenType::NUMBER_LITERAL: {
				Value value;
				value.init(token.value, type);
				values.push_back(value);
				break;
			}
			
			//case TokenType::IDENTIFIER: 
			case TokenType::STRING_LITERAL: {
				for (auto ch : token.value) {
					Value value;
					value.init(std::to_string(ch), type /*ValueType::CHAR*/);
					values.push_back(value);
				}
				break;
			}
		}
	}
	
	std::vector<Byte> getValues() {
		std::vector<Byte> data;
		std::size_t index = 0;

		switch (type)
		{
		case ValueType::CHAR: {
			for (auto& value : values) {
				data.resize(index + sizeof(Char));
				std::memcpy(&data[index], &value, sizeof(Char));
				index += sizeof(Char);
			}
			break;
		}
		case ValueType::BYTE: {
			for (auto& value : values) {
				data.resize(index + sizeof(Byte));
				std::memcpy(&data[index], &value, sizeof(Byte));
				index += sizeof(Byte);
			}
			break;
		}
		case ValueType::WORD: {
			for (auto& value : values) {
				data.resize(index + sizeof(Word));
				std::memcpy(&data[index], &value, sizeof(Word));
				index += sizeof(Word);
			}
			break;
		}
		case ValueType::DWORD: {
			for (auto& value : values) {
				data.resize(index + sizeof(Dword));
				std::memcpy(&data[index], &value, sizeof(Dword));
				index += sizeof(Dword);
			}
			break;
		}
		case ValueType::QWORD: {
			for (auto& value : values) {
				data.resize(index + sizeof(Qword));
				std::memcpy(&data[index], & value, sizeof(Qword));
				index += sizeof(Qword);
			}
			break;
		}
		}

		return data;
	}

	void setSize(std::size_t size) {
		while (values.size() != size) {
			addValue({ TokenType::NUMBER_LITERAL, "0" });
		}
	}

	/*std::size_t size() {
		int size = int(type);
		return (size ? size : 1) * values.size();
	}*/

	ValueType type{};
	ValueList values{};
	bool array{ false };

	std::string name{};
};
using VariableList = std::vector<Variable>;