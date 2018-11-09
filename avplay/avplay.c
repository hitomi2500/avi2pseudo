#include <mem.h>
#include <screen_constructor.h>
#include <fs/fs.h>
//#include <apogey/bios.h>
#include "unpack_btree1.h"

uchar* VG75;
uchar* VT57;

void apogey_hires();
void apogey_lores();
void radio_lores();
void monitor_mode();
uchar* apogeyVideoMem;
uchar apogeyVideoBpl;
 void * ScreenStartPointer;

void main() {
  int i,j;
  char c;
  void * FifoReadPointer;
  void * FifoWritePointer;

  int iNumberOfFrames;
  int iFrameCounter;
  char Machine_Type;
  char Screen_Type;
  char Fifo_Write_Threshold_1;
  char Fifo_Write_Threshold_2;
  char Fifo_Write_Threshold_3;
  char Fifo_Write_Threshold_4;
  char Fifo_Read_Threshold_1;
  char Fifo_Read_Threshold_2;
  char Fifo_Read_Threshold_3;
  
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
	  LXI  H, 0EF00h
	  SHLD VG75
	  LXI  H, 0F000h
	  SHLD VT57
      MVI  A, 1		; ������ �����������
      LXI  B, 0DE17h; BiosEntry  ; ����� ����� SD BIOS
      LXI  D, 0DBF3h; SELF_NAME  ; ����������� ���
      LXI  H, 0DCF3h; CMD_LINE   ; ��������� ������	 
  }	  
	  fs_init();
asm {
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
      MVI  A, 1		; ������ �����������
      LXI  B, 07417h; BiosEntry  ; ����� ����� SD BIOS
      LXI  D, 071F3h; SELF_NAME  ; ����������� ���
      LXI  H, 072F3h; CMD_LINE   ; ��������� ������	  
  }	  
	  fs_init();
asm {
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
  }

  asm {

  }
    
  // c������� ������       
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
	CALL fs_entry ; HL-������, DE-����� / HL-������� ���������, A-��� ������
	LHLD main_FifoReadPointer
	INX H
	MOV A,M
	STA main_Screen_Type
	CPI 0h
	JNZ SetScreen128x60
SetScreen192x102:
	}
	apogey_hires();
	//ScreenStartPointer = (void*)0xC113;
	asm {
	JMP SetScreenDone
SetScreen128x60:
	LDA main_Machine_Type
	CPI 0 ;is apogey?
	JNZ SetScreen128x60_Radio
	}
	apogey_lores();
	//ScreenStartPointer = (void*)0xE1DA;	
	asm{
	JMP SetScreenDone
SetScreen128x60_Radio:
	}
	radio_lores();
	//ScreenStartPointer = (void*)0x76DA;	
	
	
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
	  LXI H, 03000h ; ������ �������� 12k
      MVI  A, 004h;read command
	  CALL fs_entry ; HL-������, DE-����� / HL-������� ���������, A-��� ������
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
	LXI H, 00400h ; ������ �������� 1024 ����
    MVI  A, 004h;read command
	CALL fs_entry ; HL-������, DE-����� / HL-������� ���������, A-��� ������
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
  }
	//restore video mode
	asm{
	LDA main_Machine_Type
	CPI 0 ;is apogey?
	JNZ Do_Exit_Radio
	call apogey_stdmode
	JMP 0F875h ;jump to monitor
Do_Exit_Radio:
	call rk_stdmode
	JMP 0F875h ;jump to monitor	
	}
	
	asm{
str_Unknown_Machine:	.db "UNKNOWN MACHINE",0
	}
	
}


void apogey_hires() {
  APOGEY_SCREEN_ECONOMY(0xC100,64, 51, 7, 0x33, 75, 1, 0, 1);
  ScreenStartPointer = (void*)0xC116;
}

void apogey_lores() {
  APOGEY_SCREEN_ECONOMY(0xE1D0, 37, 31, 3, 0x77, 75, 1, 0, 0);
  ScreenStartPointer = (void*)0xE1DF;
}	

void radio_lores() {
  APOGEY_SCREEN_ECONOMY(0x76D0, 37, 31, 3, 0x77, 75, 1, 0, 0);
  ScreenStartPointer = (void*)0x76DF;
}	

void apogey_stdmode() {
  APOGEY_SCREEN_STD(0xE1D0, 30, 25, 3, 0x99, 78, 0, 0, 0);
  ScreenStartPointer = (void*)0xE1DF;
}	

void rk_stdmode() {
  APOGEY_SCREEN_STD(0x76D0, 30, 25, 3, 0x99, 78, 0, 0, 0);
  ScreenStartPointer = (void*)0x76DF;
}	
	
	