
void unpack_btree1() 
{
  //unpack test frame
  asm {
	INX H
	INX H
	INX H
	INX H
	LXI D, 0C113h ;ScreenStart
	MOV A,M ;load 1st byte into A
	INX H ;move to next byte
	MOV B,A ;save A
	ANI 001h
	JZ unpack_level1_1
	CALL unpack_level2
	JMP unpack_level1_1_end
unpack_level1_1:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_1_end:
	MOV A,B ;restore A
	ANI 002h
	JZ unpack_level1_2
	CALL unpack_level2
	JMP unpack_level1_2_end
unpack_level1_2:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_2_end:
	MOV A,B ;restore A
	ANI 004h
	JZ unpack_level1_3
	CALL unpack_level2
	JMP unpack_level1_3_end
unpack_level1_3:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_3_end:
	MOV A,B ;restore A
	ANI 008h
	JZ unpack_level1_4
	CALL unpack_level2
	JMP unpack_level1_4_end
unpack_level1_4:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_4_end:
	MOV A,B ;restore A
	ANI 010h
	JZ unpack_level1_5
	CALL unpack_level2
	JMP unpack_level1_5_end
unpack_level1_5:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_5_end:
	MOV A,B ;restore A
	ANI 020h
	JZ unpack_level1_6
	CALL unpack_level2
	JMP unpack_level1_6_end
unpack_level1_6:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_6_end:
	MOV A,B ;restore A
	ANI 040h
	JZ unpack_level1_7
	CALL unpack_level2
	JMP unpack_level1_7_end
unpack_level1_7:
	PUSH H
	LXI H,0258h
	DAD D
	XCHG
	POP H
unpack_level1_7_end:
	MOV A,B ;restore A
	ANI 080h
	JZ unpack_level1_8
	CALL unpack_level2
unpack_level1_8:
	JMP unpack_end
	
unpack_level2:
	PUSH B
	MOV A,M ;load byte into A
	INX H ;move to next byte
	MOV B,A ;save A
	ANI 001h
	JZ unpack_level2_1
	CALL unpack_level3
	JMP unpack_level2_1_end
unpack_level2_1:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_1_end:
	MOV A,B ;restore A
	ANI 002h
	JZ unpack_level2_2
	CALL unpack_level3
	JMP unpack_level2_2_end
unpack_level2_2:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_2_end:
	MOV A,B ;restore A
	ANI 004h
	JZ unpack_level2_3
	CALL unpack_level3
	JMP unpack_level2_3_end
unpack_level2_3:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_3_end:
	MOV A,B ;restore A
	ANI 008h
	JZ unpack_level2_4
	CALL unpack_level3
	JMP unpack_level2_4_end
unpack_level2_4:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_4_end:
	MOV A,B ;restore A
	ANI 010h
	JZ unpack_level2_5
	CALL unpack_level3
	JMP unpack_level2_5_end
unpack_level2_5:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_5_end:
	MOV A,B ;restore A
	ANI 020h
	JZ unpack_level2_6
	CALL unpack_level3
	JMP unpack_level2_6_end
unpack_level2_6:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_6_end:
	MOV A,B ;restore A
	ANI 040h
	JZ unpack_level2_7
	CALL unpack_level3
	JMP unpack_level2_7_end
unpack_level2_7:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_7_end:
	MOV A,B ;restore A
	ANI 080h
	JZ unpack_level2_8
	CALL unpack_level3
	JMP unpack_level2_8_end
unpack_level2_8:
	PUSH H
	LXI H,04Bh
	DAD D
	XCHG
	POP H
unpack_level2_8_end:
	POP B
	RET

unpack_level3:
	PUSH B
	MOV A,M ;load byte into A
	INX H ;move to next byte
	MOV B,A ;save A
	ANI 001h
	JZ unpack_level3_1
	CALL unpack_level4
	JMP unpack_level3_1_end
unpack_level3_1:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_1_end:
	MOV A,B ;restore A
	ANI 002h
	JZ unpack_level3_2
	CALL unpack_level4
	JMP unpack_level3_2_end
unpack_level3_2:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_2_end:
	MOV A,B ;restore A
	ANI 004h
	JZ unpack_level3_3
	CALL unpack_level4
	JMP unpack_level3_3_end
unpack_level3_3:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_3_end:
	MOV A,B ;restore A
	ANI 008h
	JZ unpack_level3_4
	CALL unpack_level4
	JMP unpack_level3_4_end
unpack_level3_4:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_4_end:
	MOV A,B ;restore A
	ANI 010h
	JZ unpack_level3_5
	CALL unpack_level4
	JMP unpack_level3_5_end
unpack_level3_5:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_5_end:
	MOV A,B ;restore A
	ANI 020h
	JZ unpack_level3_6
	CALL unpack_level4
	JMP unpack_level3_6_end
unpack_level3_6:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_6_end:
	MOV A,B ;restore A
	ANI 040h
	JZ unpack_level3_7
	CALL unpack_level4
	JMP unpack_level3_7_end
unpack_level3_7:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_7_end:
	MOV A,B ;restore A
	ANI 080h
	JZ unpack_level3_8
	CALL unpack_level4
	JMP unpack_level3_8_end
unpack_level3_8:
	PUSH H
	LXI H,008h
	DAD D
	XCHG
	POP H
unpack_level3_8_end:
	;end of line: add 11 bytes
	PUSH H
	LXI H,00Bh
	DAD D
	XCHG
	POP H
	;going back
	POP B
	RET

unpack_level4:
	PUSH B
	MOV A,M ;load byte into A
	INX H ;move to next byte
	MOV B,A ;save A
	ANI 001h
	JZ unpack_level4_1
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_1:
	INX D
	MOV A,B ;restore A
	ANI 002h
	JZ unpack_level4_2
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_2:
	INX D
	MOV A,B ;restore A
	ANI 004h
	JZ unpack_level4_3
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_3:
	INX D
	MOV A,B ;restore A
	ANI 008h
	JZ unpack_level4_4
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_4:
	INX D
	MOV A,B ;restore A
	ANI 010h
	JZ unpack_level4_5
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_5:
	INX D
	MOV A,B ;restore A
	ANI 020h
	JZ unpack_level4_6
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_6:
	INX D
	MOV A,B ;restore A
	ANI 040h
	JZ unpack_level4_7
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_7:
	INX D
	MOV A,B ;restore A
	ANI 080h
	JZ unpack_level4_8
	MOV A,M ;load byte into A
	INX H ;move to next byte
	STAX D
unpack_level4_8:
	INX D
	POP B
	RET
	
unpack_end:
	NOP
  }
}