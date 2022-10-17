# Virtual Machine

## Example

```assembly
.STACK = 1024
.DATA
	CHAR c0 = 'q'
	CHAR c1[] = "_werty"
	BYTE b2[10] = 0x0, 0x010, 0x001, 0x100, 0x101, 0x110
	WORD w3;
	DWORD d4 = 0xEEEEEEE
	QWORD q5 = -10
	QWORD q6 = 0xfffffff
	#DWORD arr[] = q6, b2, d4, q5, q5,

.CODE 
	Func f1;
	Func Main:
			PUSHSF
			ASSIGNA A1 c0
			ASSIGNA A2 c1

			LOAD B R0 A1
			STORE B R0 A2

			CAST DW QW R0

			ASSIGNA A2 Main
			CALL f1:
			POPSF
	label:
			RET
	EndF
	
	Func f1:				
			PUSHSF
	label:	MOVR DW R32, R0
			CMP R16, R32
			JUMPA skip
			MOVR R32, R0
	skip:
			POPSF
			RET
	EndF

.MAIN = Main
```

## Project structure

```
├── src
│   ├── code_generator
│   │   ├── code_defs.hpp
│   │   ├── code_generator.cpp
│   │   ├── code_generator.hpp
│   │   ├── function.hpp
│   │   ├── instruction.hpp
│   │   ├── instruction_defs.hpp
│   │   ├── serializer.cpp
│   │   ├── serializer.hpp
│   │   ├── variable.hpp
│   │   └── variable_defs.hpp
│   ├── code_parser
│   │   ├── code_parser.cpp
│   │   ├── code_parser.hpp
│   │   ├── code_section.cpp
│   │   ├── code_section.hpp
│   │   ├── data_section.cpp
│   │   ├── data_section.hpp
│   │   ├── instruction_parser.cpp
│   │   ├── instruction_parser.hpp
│   │   ├── main.section.cpp
│   │   ├── main_section.hpp
│   │   ├── parser_defs.hpp
│   │   ├── stack_section.cpp
│   │   └── stack_section.hpp
│   ├── common
│   │   ├── cmd.cpp
│   │   ├── cmd.hpp
│   │   ├── defs.hpp
│   │   ├── error.cpp
│   │   ├── error.hpp
│   │   └── vm_defs.hpp
│   ├── main.cpp
│   ├── parser
│   │   ├── buffer.hpp
│   │   ├── parser.cpp
│   │   ├── parser.hpp
│   │   └── token.hpp
│   ├── test.asm
│   ├── vm
│   │   ├── command.cpp
│   │   ├── command.hpp
│   │   ├── command_defs.hpp
│   │   ├── cpu.cpp
│   │   ├── cpu.hpp
│   │   ├── loader.cpp
│   │   ├── loader.hpp
│   │   ├── memory.cpp
│   │   └── memory.hpp
│   ├── vm.cpp
│   └── vm.hpp
└── test.asm.bin 
```

## Dependencies
* [Boost](https://github.com/boostorg/boost)
