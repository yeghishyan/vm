	
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