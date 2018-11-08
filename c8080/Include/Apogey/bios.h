// ���� ������

#define KEY_F1    0
#define KEY_F2    1
#define KEY_F3    2
#define KEY_F4    3
#define KEY_LEFT  8
#define KEY_TAB   9
#define KEY_ENTER 13
#define KEY_ESC   27
#define KEY_RIGHT 0x18
#define KEY_UP    0x19
#define KEY_DOWN  0x1A
#define KEY_SPACE 0x20
#define KEY_BKSPC 0x7F

// BIOS ���������� ��86

char reboot()                                  @ 0xF800;                           // F800 ������������
char getch()                                   @ 0xF803;                           // F803 ���� ������� � ���������� � ���������
char getTape(char syncFlag)                    @ 0xF806;                           // F806 ������ ����� � ����������� (FF - � ������� �����������, 08 - ��� ������ �����������)
void putch(char c)                             @ "apogey/putch.c";                 // F809 ����� ������� �� �����
void putTape(char c)                           @ "apogey/puttape.c";               // F80C ������ ����� �� ����������
char kbhit()                                   @ "apogey/kbhit.c";                 // F812 ����� ��������� ���������� (FF - �� ���� ������� �� ������)
void puthex(char)                              @ 0xF815;                           // F815 ����� �� ����� 16-������� �����
void puts(const char*)                         @ 0xF818;                           // F818 ����� �� ����� ��������� ������
char bioskey()                                 @ 0xF81B;                           // F81B ����� ��������� ���������� 2 (FF - �� ���� ������� �� ������, FE - ������ ������� ���/���, ����� A - ��� �������)
int  wherexy()                                 @ 0xF81E;                           // F81E ������ ��������� �������(� - ����� �����, L - ����� �������)
char getCharFromCursor()                       @ "apogey/getcharfromcursor.c";     // F821 ������ ������� ��� ��������
void loadTape(void* start)                     @ "apogey/loadtape.c";              // F824 ������ ����� � ����������� (�� ��� ��������� ������������)
void saveTape(void* start, void* end, int crc) @ "apogey/savetape.c";              // F827 ����� ����� �� ����������
int  crcTape(void* start, void* end)           @ "apogey/crctape.c";               // F82A ������� ����������� ����� �����
void initVideo()                               @ "apogey/initvideo.c";             // F82D ������������� ����������������
int  getMemTop()                               @ 0xF830;                           // F830 ������ ������� ������� ��������� ������
void setMemTop(int)                            @ 0xF833;                           // F833 ��������� ������� ������� ��������� ������

// ���������� BIOS ���������� ������ ��01

void putTapeMSX(char c)                        @ "apogey/puttapemsx.c";            // F003 ������ ����� � ������� MSX �� ����������
int  getTapeMSX()                              @ "apogey/gettapemsx.c";            // F006 ������ ����� � ������� MSX � ����������� (���� ���� ������, �� H@1)
void saveTapeMSX(void* start, void* end)       @ "apogey/savetapemsx.c";           // F009 ������ ����� � ������� MSX �� ����������
char loadTapeMSX(void* start, void* end)       @ "apogey/loadtapemsx.c";           // F00� ������ ����� � ������� MSX � ����������� (���� ���� �������, �� ��������� �� ����)
void putTapeLongMarkerMSX()                    @ "apogey/puttapelongmarkermsx.c";  // F00F ������ �������� ������� MSX
void putTapeShortMarkerMSX()                   @ "apogey/puttapeshortmarkermsx.c"; // F012 ������ ��������� ������� MSX
void getTapeLongMarkerMSX()                    @ "apogey/gettapelongmarkermsx.c";  // F015 ������ �������� ������� MSX

// �������� ����������� ���������� ��

char wherex()                                  @ "apogey/wherex.c";             // F81E ��������� ������� �� ��� X @ (char)wherexy()
char wherey()                                  @ "apogey/wherey.c";             // F81E ��������� ������� �� ��� Y @ (char)(wherexy()>>8)
void gotoxy(char,char)                         @ "apogey/gotoxy.c";             // F809 ����������� ������ � ���������� @ putch(0x1B, 'Y', y+0x20, x+0x20)
void clrscr()                                  @ "apogey/clrscr.c";             // F809 �������� ����� = putch(0x1F)
