// BIOS ���������� ��86

char reboot()                                  @ 0xC000;                           // C000 ������������
char getch()                                   @ 0xC803;                           // C003 ���� ������� � ���������� � ���������
char getTape(char syncFlag)                    @ 0xC806;                           // C806 ������ ����� � ����������� (FF - � ������� �����������, 08 - ��� ������ �����������)
void putch(char c)                             @ "spec/putch.c";                   // C809 ����� ������� �� �����
void putTape(char c)                           @ "spec/puttape.c";                 // C80C ������ ����� �� ����������
// C80F ���� ������ �������� � ����������
char kbhit()                                   @ 0xC812;                           // C812 ����� ��������� ���������� (FF - �� ���� ������� �� ������)
void puthex(char)                              @ 0xC815;                           // C815 ����� �� ����� 16-������� �����
void puts(const char*)                         @ 0xC818;                           // C818 ����� �� ����� ��������� ������
int  wherexy()                                 @ 0xC81E;                           // C81E ������ ��������� �������(� - ����� �����, L - ����� �������)

void loadTape(void* start)                     @ "spec/loadtape.c";                // C824 ������ ����� � ����������� (�� ��� ��������� ������������)
void saveTape(void* start, void* end, int crc) @ "spec/savetape.c";                // C827 ����� ����� �� ����������
int  crcTape(void* start, void* end)           @ "spec/crctape.c";                 // C82A ������� ����������� ����� �����
int  getMemTop()                               @ 0xC830;                           // F830 ������ ������� ������� ��������� ������
void setMemTop(int)                            @ 0xC833;                           // F833 ��������� ������� ������� ��������� ������

// �������� ����������� ���������� ��

char wherex()                                  @ "spec/wherex.c";                  // C81E ��������� ������� �� ��� X @ (char)wherexy()
char wherey()                                  @ "spec/wherey.c";                  // C81E ��������� ������� �� ��� Y @ (char)(wherexy()>>8)
void gotoxy(char,char)                         @ "spec/gotoxy.c";                  // C809 ����������� ������ � ���������� @ putch(0x1B, 'Y', y+0x20, x+0x20)
void clrscr()                                  @ "spec/clrscr.c";                  // C809 �������� ����� = putch(0x1F)
