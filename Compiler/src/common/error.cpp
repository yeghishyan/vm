#include "error.hpp"
#include <sstream>

std::string Error::file_ = {};

Error::Error(std::string&& desc)
	: desc_(std::move(desc))
{}

std::string Error::getMsg() const noexcept {
	std::ostringstream msg;

	if (!file_.empty())
	{
		msg << "['" << file_ << "'] ";
	}
	
	msg << desc_;

	return std::move(msg.str());
}

void Error::setFile(const std::string& file) {
	file_ = file;

	auto temp = file_.find_last_of("\\/");
	if (temp != std::string::npos) {
		file_ = file_.substr(temp + 1, file_.size());
	}

}
