// ���� ������

#define KEY_F1    0
#define KEY_F2    1
#define KEY_F3    2
#define KEY_F4    3
#define KEY_F5    4
#define KEY_LEFT  0x08
#define KEY_TAB   0x09
#define KEY_RIGHT 0x18
#define KEY_UP    0x19
#define KEY_DOWN  0x1A
#define KEY_SPACE 0x20
#define KEY_ENTER 0x0D
#define KEY_BKSPC 0x7F

// BIOS ���������� ��86

char reboot()                                  @ 0xF800;                          // F800 ������������
char getch()                                   @ 0xF803;                          // F803 ���� ������� � ���������� � ���������
char getTape(char syncFlag)                    @ 0xF806;                          // F806 ������ ����� � ����������� (FF - � ������� �����������, 08 - ��� ������ �����������)
void putch(char c)                             @ "orion/putch.c";                 // F809 ����� ������� �� �����
void putTape(char c)                           @ "orion/puttape.c";               // F80C ������ ����� �� ����������
char kbhit()                                   @ "orion/kbhit.c";                 // F812 ����� ��������� ���������� (FF - �� ���� ������� �� ������)
void puthex(char)                              @ 0xF815;                          // F815 ����� �� ����� 16-������� �����
void puts(const char*)                         @ 0xF818;                          // F818 ����� �� ����� ��������� ������
char bioskey()                                 @ 0xF81B;                          // F81B ����� ��������� ���������� 2 (FF - �� ���� ������� �� ������, FE - ������ ������� ���/���, ����� A - ��� �������)
int  wherexy()                                 @ 0xF81E;                          // F81E ������ ��������� �������(� - ����� �����, L - ����� �������)
// F821 �� �����������
void loadTape(void* start)                     @ "orion/loadtape.c";              // F824 ������ ����� � ����������� (�� ��� ��������� ������������)
void saveTape(void* start, void* end, int crc) @ "orion/savetape.c";              // F827 ����� ����� �� ����������
int  crcTape(void* start, void* end)           @ "orion/crctape.c";               // F82A ������� ����������� ����� �����
void initVideo()                               @ "orion/initvideo.c";             // F82D ������������� ���������������
int  getMemTop()                               @ 0xF830;                          // F830 ������ ������� ������� ��������� ������
void setMemTop(int)                            @ 0xF833;                          // F833 ��������� ������� ������� ��������� ������

char getExtMem(int, char)                      @ "orion/getextmem.c";             // F836 ������ ����� �� ����������� ������
void setExtMem(int, char, char)                @ "orion/setextmem.c";             // F839 ������ ����� � ����������� ������
void gotoxy(char, char)                        @ "orion/gotoxy.c";                // F83C ��������� ��������� �������

// �������� ����������� ���������� ��

char wherex()                                  @ "orion/wherex.c";             // F81E ��������� ������� �� ��� X @ (char)wherexy()
char wherey()                                  @ "orion/wherey.c";             // F81E ��������� ������� �� ��� Y @ (char)(wherexy()>>8)
void clrscr()                                  @ "orion/clrscr.c";             // F809 �������� ����� = putch(0x1F)

// �����

#define SCREEN_MODE (*(char*)0xF800)