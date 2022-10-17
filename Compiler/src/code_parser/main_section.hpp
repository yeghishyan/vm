#pragma once
#include <memory>
#include <string>
#include <unordered_map>

using SymbolMap = std::unordered_map<std::string, std::size_t>;

struct MainSection {
	std::string name{};
	std::size_t index{};
};

class Parser;

class MainSectionParser {
public:
	MainSectionParser(SymbolMap& functions);

	~MainSectionParser() = default;
	MainSectionParser(MainSectionParser&&) = default;
	MainSectionParser& operator=(MainSectionParser&&) = default;
	MainSectionParser(const MainSectionParser&) = delete;
	MainSectionParser& operator=(const MainSectionParser&) = delete;

	void parse(std::shared_ptr<Parser> parser);
	MainSection data();

private:
	void parseMain(std::shared_ptr<Parser> parser);

private:
	MainSection main_{};
	SymbolMap functionMap_;
};

inline MainSection MainSectionParser::data() {
	return main_;
}