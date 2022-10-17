#pragma once
#include <memory>

struct StackSection {
	std::size_t size{ 0u };
};

class Parser;

class StackSectionParser {
public:
	StackSectionParser() = default;
	~StackSectionParser() = default;
	StackSectionParser(StackSectionParser&&) = default;
	StackSectionParser& operator=(StackSectionParser&&) = default;
	StackSectionParser(const StackSectionParser&) = delete;
	StackSectionParser& operator=(const StackSectionParser&) = delete;
	
	void parse(std::shared_ptr<Parser> parser);
	StackSection data();

private:
	void parseStack(std::shared_ptr<Parser> parser);

private:
	StackSection stack_{};
};

inline StackSection StackSectionParser::data() {
	return stack_;
}