#include <mem.h>
#include <apogey/screen_constrcutor.h>
#include <fs/fs.h>
//#include <apogey/bios.h>
#include "unpack_btree1.h"

void main() {
  int i,j;
  char c;
  void * FifoReadPointer;
  void * FifoWritePointer;
  void * ScreenStartPointer;
  int iNumberOfFrames;
  int iFrameCounter;
  char Machine_Type;
  char Screen_Type;
  char Fifo_Write_Threshold_1;
  char Fifo_Write_Threshold_2;
  
  //checking machine type
  asm{
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
      MVI  A, 1		; Версия контроллера
      LXI  B, 0DE17h; BiosEntry  ; Точка входа SD BIOS
      LXI  D, 0DBF3h; SELF_NAME  ; Собственное имя
      LXI  H, 0DCF3h; CMD_LINE   ; Командная строка	 
  }	  
	  fs_init();
asm {
	  ;FIFO from 4000 to BFFF - 32 KB total, ~8 full frames / ~80 packed frames
	  LXI H, 04000h
	  SHLD main_FifoReadPointer
	  SHLD main_FifoWritePointer
	  MVI A, 07Ch
	  STA main_Fifo_Write_Threshold_1
	  MVI A, 045h
	  STA main_Fifo_Write_Threshold_2
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
      MVI  A, 1		; Версия контроллера
      LXI  B, 07417h; BiosEntry  ; Точка входа SD BIOS
      LXI  D, 071F3h; SELF_NAME  ; Собственное имя
      LXI  H, 072F3h; CMD_LINE   ; Командная строка	  
  }	  
	  fs_init();
asm {
	  ;FIFO from 2000 to 5FFF - 16 KB total, ~8 full frames / ~50 packed frames
	  LXI H, 02000h
	  SHLD main_FifoReadPointer
	  SHLD main_FifoWritePointer
  	  ;radio-specific init done
	  JMP Machine_Test_Done
Machine_Test_Not_Radio:
	  LXI H, str_Unknown_Machine
	  CALL 0F818h ;using standard monitor function on unknown machines
	  JMP 0F875h ;jump to monitor
Machine_Test_Done:
  }

  asm {

  }
    
  // cкрываем курсор       
  VG75[1] = 0x80;
  VG75[0] = 0xFF;
  VG75[0] = 0xFF;

  fs_open("VIDEO/APPLE.APV");
  
  //read header
  asm{
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
	LXI H, 0C113h
	SHLD main_ScreenStartPointer
  }
	apogeyScreen3A();
  asm {
	JMP SetScreenDone
SetScreen128x60:
	LXI H, 0E1DAh
	SHLD main_ScreenStartPointer
  }
	apogeyScreen2A();
  asm
  {
SetScreenDone:
	LHLD main_FifoReadPointer
	LXI D,4
	DAD D
	MOV E,M
	INX H
	MOV D,M
	XCHG
	SHLD main_iNumberOfFrames
  }	  
  
//PRE-READ
  asm {
	  LHLD main_FifoWritePointer
	  XCHG
	  LXI H, 03000h ; размер передачи 12k
      MVI  A, 004h;read command
	  CALL fs_entry ; HL-размер, DE-адрес / HL-сколько загрузили, A-код ошибки
	  LHLD main_FifoWritePointer
	  LXI D, 03000h
	  DAD D
	  SHLD main_FifoWritePointer
  }

  iFrameCounter = iNumberOfFrames;
  
  asm{
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
	LHLD main_FifoReadPointer
	MOV A,H
	CPI 07Ch
	JP Fifo_Read_Start ;read is > 7C00, skipping write
	;now check if read < 4400 (wrapped) 
	CPI 045h
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
	MVI A,080h
	CMP H
	JNZ Fifo_Write_Do2 ;if fifo write pointer is not at 0x8000, move on
	MVI H,040h ; else wrap back to 0x4000 and move on
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
	LHLD main_FifoReadPointer
	MOV A,H
	LHLD main_FifoWritePointer
	SUB H
	;ok, weve got (read - write),it should be positive. now check if its bigger than 48
	SUI 030h
	JP Main_Loop_Start ;it IS bigger, meaning FIFO is almost empty, skipping read
	JMP Fifo_Read_Do
Fifo_Read_Normal:
	;normal case, diff (write - read) is already in A, checking if its bigger than 16
	SUI 010h
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
	MOV A,H
	SUI 080h
	JM Fifo_Read_Do2 ;if frame is NOT wrapped, unpack it as is
	;frame IS wrapped, copy part that doesnt fit from 4000 to 8000
	;HL still stores overwrapped value, so use that
	PUSH H
	POP B
	MOV A,B
	ANI 00Fh
	MOV B,A ;now we have a size in BC
	LXI H, 04000h
	LXI D, 08000h
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
	LHLD main_ScreenStartPointer
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
	MOV A,H
	SUI 040h
	MOV H,A
	SHLD main_FifoReadPointer
	JMP Main_Loop_Start ;go back to mail loop start	
	
Fifo_Read_Do2:	
	;non-wrapped unpack
	;we should init DE before calling unpack
	LHLD main_ScreenStartPointer
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
  }
	//restore video mode
	apogeyScreen0();
	asm {
		JMP 0F875h ;jump to monitor
	}
	
	asm{
str_Unknown_Machine:	.db "UNKNOWN MACHINE",0
	}
}