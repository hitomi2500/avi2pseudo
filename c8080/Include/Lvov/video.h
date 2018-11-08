// ����� ����� � ����������� 0,0
#define LVOV_VIDEO_MEM ((uchar*)0x42C7)

// ����� ������ � ������
#define LVOV_VIDEO_BPL 64

// ���������� ������ �����
#define COORDS(X,Y)    (LVOV_VIDEO_MEM+(X)+(Y)*LVOV_VIDEO_BPL)

// ��������� ����������� � �������� ������������ 0-0x7FFF
#define VIDEO_ON  { asm { mvi a, 00010b } asm { out 0C3h } }

// ���������� ����������� �� ��������� ������������ 
#define VIDEO_OFF { asm { mvi a, 00011b } asm { out 0C3h } }
