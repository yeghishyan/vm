#pragma once

constexpr auto ip_index_number = 0u;
constexpr auto rip_index_number = 1u;
constexpr auto sp_index_number = 2u;
constexpr auto sf_index_number = 3u;

constexpr auto reg_argtype = 1;
constexpr auto adr_argtype = 2;
constexpr auto immd_argtype = 3;
constexpr auto lbl_argtype = 4;
constexpr auto var_argtype = 5;
constexpr auto func_argtype = 6;

// OPCODE_BYTE CCODE_BYTE OPSIZE_BYTE ARG_TYPE1 QWORD_ARG1 ARG_TYPE2 QWORD_ARG2
// 1		   1		  1			  1			8		   1		 8
constexpr auto instruction_size = 21;
