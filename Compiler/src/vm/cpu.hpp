#pragma once
#include <memory>

#include "memory.hpp"
#include "command.hpp"
#include "..\code_generator\instruction_defs.hpp"

class Cpu {
public:
	Cpu(std::shared_ptr<Memory>& mem);

	struct State {
		State() {
			adr.resize(4);
			reg.resize(64);
		}

		struct CpuFlags {
			uint8_t of : 1;
			uint8_t cf : 1;
			uint8_t sf : 1;
			uint8_t zf : 1;
		};

		CpuFlags flags{};

		uint16_t rip{};
		uint16_t ip{};
		uint16_t sp{};
		uint16_t sf{};

		std::vector<uint8_t> reg{};
		std::vector<uint32_t> adr{};
	};

	void run();

private:
	Opcode fetch();
	Command* decode(Opcode);
	void execute(Command*);

private:
	State state_;

	Command* currCommand_;
	std::shared_ptr<Memory> mem_;
};

std::ostream& operator<<(std::ostream& out, const Cpu::State& state);