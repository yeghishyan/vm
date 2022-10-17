#include <streambuf>

class Buffer : public std::streambuf {
public:
	Buffer(std::streambuf* source)
		: source_(source)
		, stream_(nullptr)
		, sol_(true)
		, line_(0)
	{}

	Buffer(std::istream& stream)
		: source_(stream.rdbuf())
		, stream_(&stream)
		, sol_(true)
		, line_(0) 
	{
		stream_->rdbuf(this);
	}
	
	~Buffer() {
		if (stream_ != nullptr) {
			stream_->rdbuf(source_);
		}
	}

	int line() const {
		return line_;
	}

protected:
	int underflow() {
		int ch = source_->sbumpc();
		if (ch != EOF) {
			buffer_ = ch;
	
			setg(&buffer_, &buffer_, &buffer_ + 1);
			if (sol_) {
				++line_;
			}
			sol_ = (buffer_ == '\n');
		}

		return ch;
	}

private:
	std::streambuf* source_;
	std::istream* stream_;
	char buffer_;
	int line_;
	bool sol_;
};