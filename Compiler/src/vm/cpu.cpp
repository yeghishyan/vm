#include "cpu.hpp"
#include "..\common\vm_defs.hpp"

Cpu::Cpu(std::shared_ptr<Memory>& mem)
	: mem_(mem) {
	//initInstructions

	state_.ip = mem_->pop<uint16_t>();
}

void Cpu::run() {
	auto codeSize = mem_->codeSize();

	while (state_.ip <= codeSize) {
		auto opcode = fetch();
		auto command = decode(opcode);
		
		state_.ip += instruction_size;

		execute(command);
	}
}

Opcode Cpu::fetch() {
	return Opcode(static_cast<uint8_t>(mem_->readChar(state_.ip)));
}

Command* Cpu::decode(Opcode op) {
	auto index = state_.ip + 1;

	auto opcode = op;
	auto ccode = CCode(static_cast<uint8_t>(mem_->readChar(index)));
	index++;
	auto opsize = Opsize(static_cast<uint8_t>(mem_->readChar(index)));
	index++;
	
	ArgumentType argType[2];
	Qword argVal[2];

	for (size_t argIndex = 0; argIndex < 2; ++argIndex) {
		argType[argIndex] = ArgumentType(static_cast<uint8_t>(mem_->readChar(index)));
		index++;

		argVal[argIndex] = static_cast<Qword>(mem_->readQword(index));
		index += sizeof(Qword);

		switch (argType[argIndex]) {
		case ArgumentType::REG:
			break;
		case ArgumentType::ADR:
			break;
		case ArgumentType::IMMD:
			break;
		case ArgumentType::VAR:
			break;
		case ArgumentType::LBL:
			break;
		case ArgumentType::FUNC:
			break;
		}
	}

	//return command;
	return nullptr;
}

void Cpu::execute(Command* command) {
	//command->execute();
}

std::ostream& operator<<(std::ostream& out, const Cpu::State& state) {
	out << std::endl;
	out << "Cpu flags: " << std::endl;
	out << " cf = " << std::boolalpha << static_cast<bool>(state.flags.cf);
	out << " zF = " << std::boolalpha << static_cast<bool>(state.flags.zf);
	out << " sF = " << std::boolalpha << static_cast<bool>(state.flags.sf);
	out << " oF = " << std::boolalpha << static_cast<bool>(state.flags.of);

	out << std::endl;
	out << "Address registers: " << std::endl;
	out << "ip = " << std::hex << state.ip << "  ";
	out << "rip = " << std::hex << state.rip << "  ";
	out << "sp = " << std::hex << state.sp << "  ";
	out << "sf = " << std::hex << state.sf << "  ";
	out << "A4 = " << std::hex << state.adr[0] << "  ";
	out << "A5 = " << std::hex << state.adr[1] << "  ";
	out << "A6 = " << std::hex << state.adr[2] << "  ";
	out << "A7 = " << std::hex << state.adr[3] << "  ";
	out << std::endl;

	out << std::endl;
	out << "Registers: " << std::endl;

	for (std::size_t index = 0; index < state.reg.size() / 4; index += 4) {
		out << "R" << std::dec << index << ": ";
		out << std::hex << state.reg[index];
		out << "R" << std::dec << index + 1 << ": ";
		out << std::hex << state.reg[index + 1];
		out << "R" << std::dec << index + 2 << ": ";
		out << std::hex << state.reg[index + 2];
		out << "R" << std::dec << index + 3 << ": ";
		out << std::hex << state.reg[index + 3];
	}

	return out;
}