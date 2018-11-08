  .include "stdlib8080.inc"
main:
	  LXI  H, 0FF57h
	  MOV A,M
	  CPI 090h
	  JNZ Machine_Test_Not_Apogey
	  INX  H
	  MOV A,M
	  CPI 061h
	  JNZ Machine_Test_Not_Apogey
	  INX  H
	  MOV A,M
	  CPI 070h
	  JNZ Machine_Test_Not_Apogey
	  INX  H
	  MOV A,M
	  CPI 06Fh
	  JNZ Machine_Test_Not_Apogey
	  MVI A,00
	  STA main_Machine_Type
	  ;apogey-specific init
	  LXI  H, 0EF00h
	  SHLD VG75
	  LXI  H, 0F000h
	  SHLD VT57
      MVI  A, 1		; ‚ерсиЯ контроллера
      LXI  B, 0DE17h; BiosEntry  ; ’очка входа SD BIOS
      LXI  D, 0DBF3h; SELF_NAME  ; ‘обственное имЯ
      LXI  H, 0DCF3h; CMD_LINE   ; ЉоманднаЯ строка	 
  
  ; 66 fs_init();
  call fs_init
  ; 67 asm {
	  ;FIFO from 4000 to 7FFF - 16 KB total, ~8 full frames / ~80 packed frames
	  LXI H, 04000h
	  SHLD main_FifoReadPointer
	  SHLD main_FifoWritePointer
	  MVI A, 07Ch
	  STA main_Fifo_Write_Threshold_1
	  MVI A, 045h
	  STA main_Fifo_Write_Threshold_2
	  MVI A, 080h
	  STA main_Fifo_Write_Threshold_3
	  MVI A, 040h
	  STA main_Fifo_Write_Threshold_4
	  MVI A, 030h
	  STA main_Fifo_Read_Threshold_1
	  MVI A, 010h
	  STA main_Fifo_Read_Threshold_2
	  MVI A, 040h
	  STA main_Fifo_Read_Threshold_3
  	  ;apogey-specific init done
	  JMP Machine_Test_Done
Machine_Test_Not_Apogey:
	  LXI  H, 0FF5Bh
	  MOV A,M
	  CPI 072h
	  JNZ Machine_Test_Not_Radio
	  INX  H
	  MOV A,M
	  CPI 061h
	  JNZ Machine_Test_Not_Radio
	  INX  H
	  MOV A,M
	  CPI 064h
	  JNZ Machine_Test_Not_Radio
	  INX  H
	  MOV A,M
	  CPI 069h
	  JNZ Machine_Test_Not_Radio
	  MVI A,01
	  STA main_Machine_Type
	  ;radio-specific init
	  LXI  H, 0C000h
	  SHLD VG75
	  LXI  H, 0E000h
	  SHLD VT57
      MVI  A, 1		; ‚ерсиЯ контроллера
      LXI  B, 07417h; BiosEntry  ; ’очка входа SD BIOS
      LXI  D, 071F3h; SELF_NAME  ; ‘обственное имЯ
      LXI  H, 072F3h; CMD_LINE   ; ЉоманднаЯ строка	  
  
  ; 117 fs_init();
  call fs_init
  ; 118 asm {
	  ;FIFO from 2000 to 5FFF - 16 KB total, ~8 full frames / ~50 packed frames
	  LXI H, 02000h
	  SHLD main_FifoReadPointer
	  SHLD main_FifoWritePointer
	  MVI A, 05Ch
	  STA main_Fifo_Write_Threshold_1
	  MVI A, 025h
	  STA main_Fifo_Write_Threshold_2
	  MVI A, 060h
	  STA main_Fifo_Write_Threshold_3
	  MVI A, 020h
	  STA main_Fifo_Write_Threshold_4
	  MVI A, 030h
	  STA main_Fifo_Read_Threshold_1
	  MVI A, 010h
	  STA main_Fifo_Read_Threshold_2
	  MVI A, 040h
	  STA main_Fifo_Read_Threshold_3
  	  ;radio-specific init done
	  JMP Machine_Test_Done
Machine_Test_Not_Radio:
	  LXI H, str_Unknown_Machine
	  CALL 0F818h ;using standard monitor function on unknown machines
	  JMP 0F875h ;jump to monitor
Machine_Test_Done:
  
  ; 146 asm {

  
  ; 151 VG75[1] = 0x80;Сложение с константой 1
  lhld VG75
  inx h
  mvi m, 128
  ; 152 VG75[0] = 0xFF;Сложение с константой 0
  lhld VG75
  mvi m, 255
  ; 153 VG75[0] = 0xFF;Сложение с константой 0
  mvi m, 255
  ; 155 fs_open("VIDEO/APPLE.APV");
  lxi h, string0
  call fs_open
  ; 158 asm{
	LHLD main_FifoReadPointer
	XCHG
	LXI H, 00100h ; header 256 bytes
    MVI  A, 004h;read command
	CALL fs_entry ; HL-размер, DE-адрес / HL-сколько загрузили, A-код ошибки
	LHLD main_FifoReadPointer
	MOV A,H
	STA main_Screen_Type
	CPI 0h
	JNZ SetScreen128x60
SetScreen192x102:
	
  ; 171 apogey_hires();
  call apogey_hires
  ; 173 asm {
	JMP SetScreenDone
SetScreen128x60:
	LDA main_Machine_Type
	CPI 0 ;is apogey?
	JNZ SetScreen128x60_Radio
	
  ; 180 apogey_lores();
  call apogey_lores
  ; 182 asm{
	JMP SetScreenDone
SetScreen128x60_Radio:
	
  ; 186 radio_lores();
  call radio_lores
  ; 190 asm
SetScreenDone:
	LHLD main_FifoReadPointer
	LXI D,4
	DAD D
	MOV E,M
	INX H
	MOV D,M
	XCHG
	SHLD main_iNumberOfFrames
  
  ; 204 asm {
	  LHLD main_FifoWritePointer
	  XCHG
	  LXI H, 03000h ; размер передачи 12k
      MVI  A, 004h;read command
	  CALL fs_entry ; HL-размер, DE-адрес / HL-сколько загрузили, A-код ошибки
	  LHLD main_FifoWritePointer
	  LXI D, 03000h
	  DAD D
	  SHLD main_FifoWritePointer
  
  ; 216 iFrameCounter = iNumberOfFrames;
  lhld main_iNumberOfFrames
  shld main_iFrameCounter
  ; 218 asm{
Main_Loop_Start:
	LHLD main_iFrameCounter
	XRA A ; A=0
	CMP H
	JNZ Fifo_Write_Start
	CMP L
	JNZ Fifo_Write_Start
	JMP Do_Exit
Fifo_Write_Start:
	; first check if we have enough free space in fifo, granularity is 1024 bytes
	; fifo is almost full when either (write!=7C00 and read-write>0 and read-write-8 < 0), or ( write=7C00 and (read> 7C00 or read < 4400) )
	LHLD main_FifoWritePointer
	LDA main_Fifo_Write_Threshold_1
	CMP H
	JZ Fifo_Write_Start2
	;not end-buffer case, check read-write>0
	LHLD main_FifoReadPointer
	MOV A,H
	LHLD main_FifoWritePointer
	CMP H
	JM Fifo_Write_Do ;read-write < 0, not skipping
	;check read-write-8 < 0
	SUB H
	SUI 08h
	JP Fifo_Write_Do ;read-write-8 > 0, not skipping
	JMP Fifo_Read_Start ;skipping after all
Fifo_Write_Start2:
	;end-buffer case, checking if read > 7C00 (not wrapped) 
	LDA main_Fifo_Write_Threshold_1
	MOV B,A
	LHLD main_FifoReadPointer
	MOV A,H
	CMP B
	JP Fifo_Read_Start ;read is > 7C00, skipping write
	;now check if read < 4400 (wrapped)
	LDA main_Fifo_Write_Threshold_2
	MOV B,A	
	MOV A,H
	CMP B
	JM Fifo_Read_Start ;read is < 4400, skipping write
	JMP Fifo_Write_Do ;writing after all
Fifo_Write_Do:
	LHLD main_FifoWritePointer
	XCHG
	LXI H, 00400h ; размер передачи 1024 байт
    MVI  A, 004h;read command
	CALL fs_entry ; HL-размер, DE-адрес / HL-сколько загрузили, A-код ошибки
	LHLD main_FifoWritePointer
	MVI A, 004h
	ADD H
	MOV H, A
	LDA main_Fifo_Write_Threshold_3
	CMP H
	JNZ Fifo_Write_Do2 ;if fifo write pointer is not at 0x8000, move on
	LDA main_Fifo_Write_Threshold_4
	MOV H,A ; else wrap back to 0x4000 and move on
Fifo_Write_Do2:	
	SHLD main_FifoWritePointer

	
Fifo_Read_Start:
	;first thing to do is to check that we have at least full frame worth of fifo data
	;if we dont, wait for fifo to fill up
	;there are 2 cases of empty fifo: (write > read) and (write - read < 16) (normal case),
	;								or (write < read) and (read - write > 48) (wrap case)
	LHLD main_FifoWritePointer
	MOV A,H
	LHLD main_FifoReadPointer
	SUB H
	JP Fifo_Read_Normal
	;wrap case, calculating (read - write) instead
	LDA main_Fifo_Read_Threshold_1
	MOV B,A
	LDA main_Fifo_Read_Threshold_2
	MOV C,A
	LHLD main_FifoReadPointer
	MOV A,H
	LHLD main_FifoWritePointer
	SUB H
	;ok, weve got (read - write),it should be positive. now check if its bigger than 48
	SUB B
	JP Main_Loop_Start ;it IS bigger, meaning FIFO is almost empty, skipping read
	JMP Fifo_Read_Do
Fifo_Read_Normal:
	;normal case, diff (write - read) is already in A, checking if its bigger than 16
	SUB C
	JM Main_Loop_Start ;it is NOT bigger, meaning FIFO is almost empty, skipping read	
Fifo_Read_Do:
	;decrease frame counter
	LHLD main_iFrameCounter
	DCX H
	SHLD main_iFrameCounter
	;okay, FIFO is not empty, but current frame might be wrapping around FIFO end
	;we check this by adding frame size to read pointer
	LHLD main_FifoReadPointer
	INX H
	INX H
	MOV E,M
	INX H
	MOV D,M
	INX H
	DAD D
	LDA main_Fifo_Write_Threshold_3
	MOV B,A
	MOV A,H
	SUB B
	JM Fifo_Read_Do2 ;if frame is NOT wrapped, unpack it as is
	;frame IS wrapped, copy part that doesnt fit from 4000 to 8000
	;HL still stores overwrapped value, so use that
	PUSH H
	POP B
	MOV A,B
	ANI 00Fh
	MOV B,A ;now we have a size in BC
	XRA A ; A=0
	MOV L,A
	MOV E,A
	LDA main_Fifo_Write_Threshold_4
	MOV H,A
	LDA main_Fifo_Write_Threshold_3
	MOV D,A
Fifo_Read_Copy_Loop:
	MOV A,M
	STAX D
	INX H
	INX D
	DCX B
	XRA A ; A=0
	CMP B
	JNZ Fifo_Read_Copy_Loop
	CMP C
	JNZ Fifo_Read_Copy_Loop
	;copy done, now processing frame as-is
	;we should init DE and HL before calling unpack
	LHLD ScreenStartPointer
	XCHG
	LHLD main_FifoReadPointer
	CALL unpack_btree1
	;now move read pointer
	LHLD main_FifoReadPointer
	INX H
	INX H
	MOV E,M
	INX H
	MOV D,M
	INX H
	DAD D
	LDA main_Fifo_Read_Threshold_3
	MOV B,A
	MOV A,H
	SUB B
	MOV H,A
	SHLD main_FifoReadPointer
	JMP Main_Loop_Start ;go back to mail loop start	
	
Fifo_Read_Do2:	
	;non-wrapped unpack
	;we should init DE before calling unpack
	LHLD ScreenStartPointer
	XCHG
	LHLD main_FifoReadPointer
	CALL unpack_btree1
	;now move read pointer
	LHLD main_FifoReadPointer
	INX H
	INX H
	MOV E,M
	INX H
	MOV D,M
	INX H
	DAD D
	SHLD main_FifoReadPointer
	JMP Main_Loop_Start ;go back to mail loop start

Do_Exit:
  
  ; 396 asm{
	LDA main_Machine_Type
	CPI 0 ;is apogey?
	JNZ Do_Exit_Radio
	call apogey_stdmode
	JMP 0F875h ;jump to monitor
Do_Exit_Radio:
	call rk_stdmode
	JMP 0F875h ;jump to monitor	
	
  ; 407 asm{
str_Unknown_Machine:	.db "UNKNOWN MACHINE",0
	
  ret
  ; --- apogey_hires -----------------------------------------------------------------
apogey_hires:
  push b
  ; 3 memset((uchar*)MEM_ADDR, 0, (HEIGHT)*(BPL)+(TOP_INVISIBLE)*2+2); 
  lxi h, 49408
  shld memset_1
  xra a
  sta memset_2
  lxi h, 3841
  call memset
  ; 4 for(v=(uchar*)(MEM_ADDR)-1, i=TOP_INVISIBLE; i; --i) 
  lxi b, 49407
  mvi a, 7
  sta apogey_hires_i
l0:
  ; convertToConfition
  lda apogey_hires_i
  ora a
  jz l1
  ; 5 v+=2, *v = 0xF1; Сложение BC с константой 2
  inx b
  inx b
  mvi a, 241
  stax b
l2:
  lxi h, apogey_hires_i
  dcr m
  jmp l0
l1:
  ; 6 if(FILL_EOL) 7 for(i = HEIGHT; i; --i) 
  mvi a, 51
  sta apogey_hires_i
l4:
  ; convertToConfition
  lda apogey_hires_i
  ora a
  jz l5
  ; 8 v += (BPL), *v = 0xF1; Сложение с BC
  lxi h, 75
  dad b
  mov b, h
  mov c, l
  mvi a, 241
  stax b
l6:
  lxi h, apogey_hires_i
  dcr m
  jmp l4
l5:
  ; 9 ((uchar*)MEM_ADDR)[(HEIGHT)*(BPL)+(TOP_INVISIBLE)*2+1] = 0xFF; 
  lxi h, 53248
  mvi m, 255
  ; 10 apogeyVideoMem = (uchar*)(MEM_ADDR) + (TOP_INVISIBLE)*2 + 9; 
  lxi h, 49431
  shld apogeyVideoMem
  ; 11 apogeyVideoBpl = (BPL); 
  mvi a, 75
  sta apogeyVideoBpl
  ; 1 VG75[1] = 0; Сложение с константой 1
  lhld VG75
  inx h
  mvi m, 0
  ; 2 VG75[0] = 78-1; Сложение с константой 0
  lhld VG75
  mvi m, 77
  ; 3 VG75[0] = (((FONT&0xF) >= 9) ? 0 : 0x40) | (FULL_HEIGHT-1); Сложение с константой 0
  mvi m, 127
  ; 4 VG75[0] = FONT; Сложение с константой 0
  mvi m, 51
  ; 5 VG75[0] = ((HIDDEN_ATTRIB) ? 0 : 0x40) | ((FONT&0xF)==9 ? 0x80 : 0) | 0x13; Сложение с константой 0
  mvi m, 83
  ; 6 VG75[1] = 0x23; Сложение с константой 1
  inx h
  mvi m, 35
  ; 7 while((VG75[1] & 0x20) == 0); 
l13:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l14
  jmp l13
l14:
  ; 8 while((VG75[1] & 0x20) == 0); 
l15:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l16
  jmp l15
l16:
  ; 9 VT57[8] = 0x80; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 128
  ; 10 VT57[4] = (uchar)(MEM_ADDR); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 0
  ; 11 VT57[4] = (uchar)((MEM_ADDR)>>8); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 193
  ; 12 VT57[5] = (uchar)((MEM_SIZE)-1); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 0
  ; 13 VT57[5] = 0x40 | (uchar)(((MEM_SIZE)-1)>>8); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 79
  ; 14 VT57[8] = 0xA4; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 164
  ; 15 if(CHAR_GEN) asm { ei } else asm { di } 15 asm { ei } else asm { di } 
 ei 
  ; 15 asm { di } 416 ScreenStartPointer = (void*)0xC113;
  lxi h, 49427
  shld ScreenStartPointer
  pop b
  ret
  ; --- apogey_lores -----------------------------------------------------------------
apogey_lores:
  push b
  ; 3 memset((uchar*)MEM_ADDR, 0, (HEIGHT)*(BPL)+(TOP_INVISIBLE)*2+2); 
  lxi h, 57808
  shld memset_1
  xra a
  sta memset_2
  lxi h, 2333
  call memset
  ; 4 for(v=(uchar*)(MEM_ADDR)-1, i=TOP_INVISIBLE; i; --i) 
  lxi b, 57807
  mvi a, 3
  sta apogey_lores_i
l19:
  ; convertToConfition
  lda apogey_lores_i
  ora a
  jz l20
  ; 5 v+=2, *v = 0xF1; Сложение BC с константой 2
  inx b
  inx b
  mvi a, 241
  stax b
l21:
  lxi h, apogey_lores_i
  dcr m
  jmp l19
l20:
  ; 6 if(FILL_EOL) 7 for(i = HEIGHT; i; --i) 
  mvi a, 31
  sta apogey_lores_i
l23:
  ; convertToConfition
  lda apogey_lores_i
  ora a
  jz l24
  ; 8 v += (BPL), *v = 0xF1; Сложение с BC
  lxi h, 75
  dad b
  mov b, h
  mov c, l
  mvi a, 241
  stax b
l25:
  lxi h, apogey_lores_i
  dcr m
  jmp l23
l24:
  ; 9 ((uchar*)MEM_ADDR)[(HEIGHT)*(BPL)+(TOP_INVISIBLE)*2+1] = 0xFF; 
  lxi h, 60140
  mvi m, 255
  ; 10 apogeyVideoMem = (uchar*)(MEM_ADDR) + (TOP_INVISIBLE)*2 + 9; 
  lxi h, 57823
  shld apogeyVideoMem
  ; 11 apogeyVideoBpl = (BPL); 
  mvi a, 75
  sta apogeyVideoBpl
  ; 1 VG75[1] = 0; Сложение с константой 1
  lhld VG75
  inx h
  mvi m, 0
  ; 2 VG75[0] = 78-1; Сложение с константой 0
  lhld VG75
  mvi m, 77
  ; 3 VG75[0] = (((FONT&0xF) >= 9) ? 0 : 0x40) | (FULL_HEIGHT-1); Сложение с константой 0
  mvi m, 100
  ; 4 VG75[0] = FONT; Сложение с константой 0
  mvi m, 119
  ; 5 VG75[0] = ((HIDDEN_ATTRIB) ? 0 : 0x40) | ((FONT&0xF)==9 ? 0x80 : 0) | 0x13; Сложение с константой 0
  mvi m, 83
  ; 6 VG75[1] = 0x23; Сложение с константой 1
  inx h
  mvi m, 35
  ; 7 while((VG75[1] & 0x20) == 0); 
l32:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l33
  jmp l32
l33:
  ; 8 while((VG75[1] & 0x20) == 0); 
l34:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l35
  jmp l34
l35:
  ; 9 VT57[8] = 0x80; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 128
  ; 10 VT57[4] = (uchar)(MEM_ADDR); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 208
  ; 11 VT57[4] = (uchar)((MEM_ADDR)>>8); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 225
  ; 12 VT57[5] = (uchar)((MEM_SIZE)-1); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 28
  ; 13 VT57[5] = 0x40 | (uchar)(((MEM_SIZE)-1)>>8); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 73
  ; 14 VT57[8] = 0xA4; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 164
  ; 15 if(CHAR_GEN) asm { ei } else asm { di } 15 asm { ei } else asm { di } 15 asm { di } 
 di 
  ; 421 ScreenStartPointer = (void*)0xE1DA;
  lxi h, 57818
  shld ScreenStartPointer
  pop b
  ret
  ; --- radio_lores -----------------------------------------------------------------
radio_lores:
  push b
  ; 3 memset((uchar*)MEM_ADDR, 0, (HEIGHT)*(BPL)+(TOP_INVISIBLE)*2+2); 
  lxi h, 30416
  shld memset_1
  xra a
  sta memset_2
  lxi h, 2333
  call memset
  ; 4 for(v=(uchar*)(MEM_ADDR)-1, i=TOP_INVISIBLE; i; --i) 
  lxi b, 30415
  mvi a, 3
  sta radio_lores_i
l38:
  ; convertToConfition
  lda radio_lores_i
  ora a
  jz l39
  ; 5 v+=2, *v = 0xF1; Сложение BC с константой 2
  inx b
  inx b
  mvi a, 241
  stax b
l40:
  lxi h, radio_lores_i
  dcr m
  jmp l38
l39:
  ; 6 if(FILL_EOL) 7 for(i = HEIGHT; i; --i) 
  mvi a, 31
  sta radio_lores_i
l42:
  ; convertToConfition
  lda radio_lores_i
  ora a
  jz l43
  ; 8 v += (BPL), *v = 0xF1; Сложение с BC
  lxi h, 75
  dad b
  mov b, h
  mov c, l
  mvi a, 241
  stax b
l44:
  lxi h, radio_lores_i
  dcr m
  jmp l42
l43:
  ; 9 ((uchar*)MEM_ADDR)[(HEIGHT)*(BPL)+(TOP_INVISIBLE)*2+1] = 0xFF; 
  lxi h, 32748
  mvi m, 255
  ; 10 apogeyVideoMem = (uchar*)(MEM_ADDR) + (TOP_INVISIBLE)*2 + 9; 
  lxi h, 30431
  shld apogeyVideoMem
  ; 11 apogeyVideoBpl = (BPL); 
  mvi a, 75
  sta apogeyVideoBpl
  ; 1 VG75[1] = 0; Сложение с константой 1
  lhld VG75
  inx h
  mvi m, 0
  ; 2 VG75[0] = 78-1; Сложение с константой 0
  lhld VG75
  mvi m, 77
  ; 3 VG75[0] = (((FONT&0xF) >= 9) ? 0 : 0x40) | (FULL_HEIGHT-1); Сложение с константой 0
  mvi m, 100
  ; 4 VG75[0] = FONT; Сложение с константой 0
  mvi m, 119
  ; 5 VG75[0] = ((HIDDEN_ATTRIB) ? 0 : 0x40) | ((FONT&0xF)==9 ? 0x80 : 0) | 0x13; Сложение с константой 0
  mvi m, 83
  ; 6 VG75[1] = 0x23; Сложение с константой 1
  inx h
  mvi m, 35
  ; 7 while((VG75[1] & 0x20) == 0); 
l51:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l52
  jmp l51
l52:
  ; 8 while((VG75[1] & 0x20) == 0); 
l53:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l54
  jmp l53
l54:
  ; 9 VT57[8] = 0x80; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 128
  ; 10 VT57[4] = (uchar)(MEM_ADDR); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 208
  ; 11 VT57[4] = (uchar)((MEM_ADDR)>>8); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 118
  ; 12 VT57[5] = (uchar)((MEM_SIZE)-1); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 28
  ; 13 VT57[5] = 0x40 | (uchar)(((MEM_SIZE)-1)>>8); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 73
  ; 14 VT57[8] = 0xA4; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 164
  ; 15 if(CHAR_GEN) asm { ei } else asm { di } 15 asm { ei } else asm { di } 15 asm { di } 
 di 
  ; 426 ScreenStartPointer = (void*)0x76DA;
  lxi h, 30426
  shld ScreenStartPointer
  pop b
  ret
  ; --- apogey_stdmode -----------------------------------------------------------------
apogey_stdmode:
  push b
  ; 3 memset((uchar*)(MEM_ADDR), 0, (FULL_HEIGHT)*(BPL)); 
  lxi h, 57808
  shld memset_1
  xra a
  sta memset_2
  lxi h, 2340
  call memset
  ; 4 if(FILL_EOL) { 5 v = (uchar*)(MEM_ADDR)-1; 9 apogeyVideoMem = (uchar*)(MEM_ADDR) + (TOP_INVISIBLE)*(BPL) + ((HIDDEN_ATTRIB) ? 9 : 8); 
  lxi h, 58050
  shld apogeyVideoMem
  ; 10 apogeyVideoBpl = (BPL); 
  mvi a, 78
  sta apogeyVideoBpl
  ; 1 VG75[1] = 0; Сложение с константой 1
  lhld VG75
  inx h
  mvi m, 0
  ; 2 VG75[0] = 78-1; Сложение с константой 0
  lhld VG75
  mvi m, 77
  ; 3 VG75[0] = (((FONT&0xF) >= 9) ? 0 : 0x40) | (FULL_HEIGHT-1); Сложение с константой 0
  mvi m, 29
  ; 4 VG75[0] = FONT; Сложение с константой 0
  mvi m, 153
  ; 5 VG75[0] = ((HIDDEN_ATTRIB) ? 0 : 0x40) | ((FONT&0xF)==9 ? 0x80 : 0) | 0x13; Сложение с константой 0
  mvi m, 211
  ; 6 VG75[1] = 0x23; Сложение с константой 1
  inx h
  mvi m, 35
  ; 7 while((VG75[1] & 0x20) == 0); 
l69:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l70
  jmp l69
l70:
  ; 8 while((VG75[1] & 0x20) == 0); 
l71:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l72
  jmp l71
l72:
  ; 9 VT57[8] = 0x80; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 128
  ; 10 VT57[4] = (uchar)(MEM_ADDR); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 208
  ; 11 VT57[4] = (uchar)((MEM_ADDR)>>8); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 225
  ; 12 VT57[5] = (uchar)((MEM_SIZE)-1); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 35
  ; 13 VT57[5] = 0x40 | (uchar)(((MEM_SIZE)-1)>>8); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 73
  ; 14 VT57[8] = 0xA4; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 164
  ; 15 if(CHAR_GEN) asm { ei } else asm { di } 15 asm { ei } else asm { di } 15 asm { di } 
 di 
  ; 431 ScreenStartPointer = (void*)0xE1DA;
  lxi h, 57818
  shld ScreenStartPointer
  pop b
  ret
  ; --- rk_stdmode -----------------------------------------------------------------
rk_stdmode:
  push b
  ; 3 memset((uchar*)(MEM_ADDR), 0, (FULL_HEIGHT)*(BPL)); 
  lxi h, 30416
  shld memset_1
  xra a
  sta memset_2
  lxi h, 2340
  call memset
  ; 4 if(FILL_EOL) { 5 v = (uchar*)(MEM_ADDR)-1; 9 apogeyVideoMem = (uchar*)(MEM_ADDR) + (TOP_INVISIBLE)*(BPL) + ((HIDDEN_ATTRIB) ? 9 : 8); 
  lxi h, 30658
  shld apogeyVideoMem
  ; 10 apogeyVideoBpl = (BPL); 
  mvi a, 78
  sta apogeyVideoBpl
  ; 1 VG75[1] = 0; Сложение с константой 1
  lhld VG75
  inx h
  mvi m, 0
  ; 2 VG75[0] = 78-1; Сложение с константой 0
  lhld VG75
  mvi m, 77
  ; 3 VG75[0] = (((FONT&0xF) >= 9) ? 0 : 0x40) | (FULL_HEIGHT-1); Сложение с константой 0
  mvi m, 29
  ; 4 VG75[0] = FONT; Сложение с константой 0
  mvi m, 153
  ; 5 VG75[0] = ((HIDDEN_ATTRIB) ? 0 : 0x40) | ((FONT&0xF)==9 ? 0x80 : 0) | 0x13; Сложение с константой 0
  mvi m, 211
  ; 6 VG75[1] = 0x23; Сложение с константой 1
  inx h
  mvi m, 35
  ; 7 while((VG75[1] & 0x20) == 0); 
l87:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l88
  jmp l87
l88:
  ; 8 while((VG75[1] & 0x20) == 0); 
l89:
  ; Сложение с константой 1
  lhld VG75
  inx h
  mov a, m
  ani 32
  jnz l90
  jmp l89
l90:
  ; 9 VT57[8] = 0x80; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 128
  ; 10 VT57[4] = (uchar)(MEM_ADDR); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 208
  ; 11 VT57[4] = (uchar)((MEM_ADDR)>>8); Сложение с константой 4
  lhld VT57
  inx h
  inx h
  inx h
  inx h
  mvi m, 118
  ; 12 VT57[5] = (uchar)((MEM_SIZE)-1); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 35
  ; 13 VT57[5] = 0x40 | (uchar)(((MEM_SIZE)-1)>>8); Сложение
  lhld VT57
  lxi d, 5
  dad d
  mvi m, 73
  ; 14 VT57[8] = 0xA4; Сложение
  lhld VT57
  lxi d, 8
  dad d
  mvi m, 164
  ; 15 if(CHAR_GEN) asm { ei } else asm { di } 15 asm { ei } else asm { di } 15 asm { di } 
 di 
  ; 436 ScreenStartPointer = (void*)0x76DA;
  lxi h, 30426
  shld ScreenStartPointer
  pop b
  ret
  ; --- unpack_btree1 -----------------------------------------------------------------
unpack_btree1:
	INX H
	INX H
	INX H
	INX H
	;LXI D, 0C113h ;ScreenStart
	;LXI D, 0E1DAh ;ScreenStart
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
  
  ret
  ; --- fs_init -----------------------------------------------------------------
fs_init:
    SHLD fs_cmdLine
    XCHG
    SHLD fs_selfName
    MOV H, B
    MOV L, C
    SHLD fs_entry_n+1
  
  ret
  ; --- fs_entry -----------------------------------------------------------------
fs_entry:
fs_entry_n:
    JMP 0000h
  
  ret
  ; --- fs_open -----------------------------------------------------------------
fs_open:
  shld fs_open_1
  ; 5 return fs_open0(name, O_OPEN);
  shld fs_open0_1
  xra a
  jmp fs_open0
  ret
  ; --- memset -----------------------------------------------------------------
memset:
  shld memset_3
  ; 2 asm {
    push b
    lda memset_2
    xchg
    lhld memset_1
    xchg
    lxi b, -1    
memset_l1:
    dad b
    jnc memset_l2
    stax d
    inx d
    jmp memset_l1
memset_l2:
    pop b
  
  ret
  ; --- fs_open0 -----------------------------------------------------------------
fs_open0:
  sta fs_open0_2
  ; 5 asm {      
      
    PUSH B
    ; a = fs_open0_2
    MOV  D, A 
    LHLD fs_open0_1
    MVI  A, 2
    CALL fs_entry
    POP  B
  
  ret
VG75:
 .dw $+2
 .ds 1
VT57:
 .dw $+2
 .ds 1
apogeyVideoMem:
 .dw $+2
 .ds 1
apogeyVideoBpl:
 .ds 1
ScreenStartPointer:
 .dw $+2
main_i:
 .ds 2
main_j:
 .ds 2
main_c:
 .ds 1
main_FifoReadPointer:
 .ds 2
main_FifoWritePointer:
 .ds 2
main_iNumberOfFrames:
 .ds 2
main_iFrameCounter:
 .ds 2
main_Machine_Type:
 .ds 1
main_Screen_Type:
 .ds 1
main_Fifo_Write_Threshold_1:
 .ds 1
main_Fifo_Write_Threshold_2:
 .ds 1
main_Fifo_Write_Threshold_3:
 .ds 1
main_Fifo_Write_Threshold_4:
 .ds 1
main_Fifo_Read_Threshold_1:
 .ds 1
main_Fifo_Read_Threshold_2:
 .ds 1
main_Fifo_Read_Threshold_3:
 .ds 1
apogey_hires_i:
 .ds 1
apogey_lores_i:
 .ds 1
radio_lores_i:
 .ds 1
apogey_stdmode_i:
 .ds 1
rk_stdmode_i:
 .ds 1
fs_cmdLine:
 .dw $+2
 .ds 1
fs_selfName:
 .dw $+2
 .ds 1
fs_low:
 .ds 2
fs_high:
 .ds 2
fs_addr:
 .ds 1
fs_open_1:
 .ds 2
memset_1:
 .ds 2
memset_2:
 .ds 1
memset_3:
 .ds 2
fs_open0_1:
 .ds 2
fs_open0_2:
 .ds 1
string0:
 .db 86,73,68,69,79,47,65,80,80,76,69,46,65,80,86,0
  .end
