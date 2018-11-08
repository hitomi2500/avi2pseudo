// BIOS ���������� ����� ��01

char getch()                                                   @ 0xF803;                // F803 ���� ������� � ���������� � ���������� ����������� �������
char getch2()                                                  @ 0xF806;                // F806 ���� ������� ��� ��������� ����������� �������
char kbhit()                                                   @ 0xF812;                // F812 ������ ���������� (FF=������, 0=�� ������)
void putch(char c)                                             @ "lvov/putch.c";        // F809 ����� ������� �� �����
void putchPrinter(char c)                                      @ "lvov/putchprinter.c"; // F80C ����� ������� �� �������
void putchAll(char c)                                          @ "lvov/putchall.c";     // F80F ����� ������� �� ����� �/��� �������
void puts(const char*)                                         @ 0xE4A4;                // E4A4 ����� �� ����� ���������� ���������
void putHexByte(uchar)                                         @ 0xFFD6;                // FFD6 ����� �� ����� ����������� �������� A � ����������������� ����
void putHexWord(uint)                                          @ 0xFFD1;                // FFD1 ����� �� ����� ����������� ����������� ���� HL � ����������������� ����
void putCrc(void* start, void* end)                            @ "lvov/putcrc.c";       // F815 ����� �� ����� ����������� ����� �����
void locate(uchar x, uchar y, uchar cursor)                    @ "lvov/locate.c";       // F82D ���������������� ������� (cursor=0 �������� ������, cursor=FF ������ ������)
void clrscr()                                                  @ 0xF836;                // F836 ������� ������
void pset(uchar x, uchar y, uchar c)                           @ "lvov/pset.c";         // F821 ����� �� ����� �����
void preset(uchar x, uchar y)                                  @ "lvov/preset.c";       // F020 �������� �����
void line(uchar x0, uchar y0, uchar x1, uchar y1, uchar c)     @ "lvov/line.c";         // F824 ����� �� ����� �����
void rect(uchar x0, uchar y0, uchar x1, uchar y1, uchar c)     @ "lvov/rect.c";         // F827 ����� �� ����� ��������������
void fillRect(uchar x0, uchar y0, uchar x1, uchar y1, uchar c) @ "lvov/fillrect.c";     // F82A ����� ������������ ��������������
void paint(uchar x, uchar y, uchar c, uchar borderColor)       @ "lvov/paint.c";        // F830 �������� ��������� ������
void color(uchar palette, uchar bgcolor)                       @ "lvov/color.c";        // F833 ��������� �������� ������� (palette=0-6, bgcolor=0-7)
void copy()                                                    @ 0xE627;                // E627 ����������� ������ �� ��������
void beep()                                                    @ 0xF81B;                // F81B �������� �������� ������ ( BEEP ).
void sound(uchar note, uchar delay)                            @ "lvov/sound.c";        // F81E ����������� �������� ������ ( SOUND ).
uint vaddr(uchar x, uchar y)                                   @ "lvov/vaddr.c";        // F818 ���������� ��������� ������ �� ����������� ( VADDR ).

// ������� ������ � ������ � ����������� �� �������

// E2BE �������� ���� "0"
// E2C5 �������� ���� "1"
// E2D5 �������� ������� �������.
// E42B WR_PILOT �������� �����-�������
// E437 WR_BYTE �������� �����
// DD86 WR_WORD �������� �����
// E3E4 WR_HEAD �������� ��������� �����
// DD31 BSAVE �������� �������� �����
// E4D0 ����� �����-������� ( RD_PILOT )
// E4BE ����� ����� ( RD_BYTE ).
// DDCA ����� ����� ( RD_WORD ).
// E443 ����� ��������� ����� ( RD_HEAD ).
// DDBC ����� ����� ( RD_BLOCK ).
// DD94 ����� �������� ����� ( BLOAD ).

// ����������

#define KEYB_CODE     (*(uchar*)0xBE10) // ��� ������� ������� 
#define KEYB_STAT     (*(uchar*)0xBE14) // ��������� ����������
#define KEYB_MODE     (*(uchar*)0xBE1D) // ����� ����������
#define KEYB_BEEP     (*(uchar*)0xBE1E) // ���� ����������

#define TEXT_ADDR     (*(uchar*)0xBE30) // ����� ������ ������� (������ ������)
#define TEXT_COL      (*(uchar*)0xBE32) // X ������� ������ ������ (������ ������)
#define TEXT_ROW      (*(uchar*)0xBE33) // Y ������� ������ ������ (������ ������)
#define TEXT_COLOR    (*(uchar*)0xBE36) // ���� ������
#define BORDER_COLOR  (*(uchar*)0xBE38) // ���� ���������� �����
#define SCROLL_LOCK   (*(uchar*)0xBE39) // C�������� ���./����.
#define CURSOR_HIDDEN (*(uchar*)0xBE3C) // ������ ����� (������ ������)
#define PALETTE       (*(uchar*)0xBEC0) // ��������� ������� (������ ������)
#define GROUND_COLOR  (*(uchar*)0xBEC1) // ���� ���� (������ ������)

#define PRN_SHIFT     (*(uchar*)0xBE40) // �������. �������� ���� ���������.
#define PRN_XOR       (*(uchar*)0xBE41) // �������. ��������� �����.
#define DISP_OUT      (*(uchar*)0xBE1B) // 0 ���� putchAll ������ �������� �� �����, ����� FFh
#define PRN_OUT       (*(uchar*)0xBEF3) // 80h ���� putchAll ������ �������� �� �������, ����� 0

// ����������  ������ � ������ � ����������� �� �������

// BE80/BE81 WR0PERIOD ����.������� ����."0"  
// BE82/BE83 WR1PERIOD ����.������� ����."1"  
// BE84      PILOT_DUR ������������ ������    
// BE85                - ��������� ������ ��  
// BE86                L (�����. ���������.)  
// BE87      FILE_TYPE �����������.���� ����� 
// BE88/BE89 BASFL_ORG ������ BASIC-�����     
// BE8A/BE8B BASFL_END ����� BASIC-�����      
// BE8C-BE91 SAVE_NAME ��� ����� ������/������
// BE92-BE97 LOAD_NAME ��������� ��� �����    
// BEA3      AUTOSTART ������� ����������     
// BEA4/BEA5 CODFL_ORG ������ CODE-�����      
// BEA6/BEA7 CODFL_END ����� CODE-�����       
// BEA9/BEAA CODFL_RUN ����� CODE-�����       
// BEAB/BEAC OFFSET    �������� CODE-�����    
// BEF1/BEF2 LOAD_ERR  ���.�����. �� ����.�/�