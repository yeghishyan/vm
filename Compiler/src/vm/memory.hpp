#pragma once
#include <vector>
#include <cstdint>

#include "../code_generator/code_generator.hpp"

class Memory {
public:
	Memory() = default;
	~Memory() = default;

	template<typename uintxx_t>
	void push(const uintxx_t& value) {
		std::vector<uint8_t> byteArray;

		byteArray.resize(sizeof(value));

		memcpy(&byteArray[0], &value, sizeof(value));

		for (auto& byte : byteArray) {
			data_.insert(data_.end(),byte);
		}
	}

	template<typename uintxx_t>
	uintxx_t pop(/*const uint16_t& index*/) {
		uintxx_t value;

		memcpy(&value, &data_[data_.size() - sizeof(uintxx_t)], sizeof(uintxx_t));

		return value;
	}

	void resize(std::size_t size) {
		data_.resize(size);
	}

	uint16_t codeSize() {
		return code_.size();
	}

	void writeCode(std::vector<uint8_t>& block) {
		code_ = block;
	}

	void writeData(std::vector<uint8_t>& block) {
		data_ = block;
	}

	void setSymbolTable(SymbolTable& symbolTable) {
		symbols_ = symbolTable;
	}

	uint8_t& readChar(const uint8_t ip) {
		return code_[ip];
	}

	uint16_t& readWord(const uint8_t ip) {
		uint16_t value;
		memcpy(&value, &code_[ip], sizeof(uint16_t));
		return value;
	}

	uint32_t& readDword(const uint8_t ip) {
		uint32_t value;
		memcpy(&value, &code_[ip], sizeof(uint32_t));
		return value;
	}

	uint64_t& readQword(const uint8_t ip) {
		uint64_t value;
		memcpy(&value, &code_[ip], sizeof(uint64_t));
		return value;
	}

private:
	SymbolTable symbols_;
	std::vector<uint8_t> code_;
	std::vector<uint8_t> data_;
};

