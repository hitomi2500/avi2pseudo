#include <fs/fs.h>

uchar fs_seek(uint low, uint high, uchar mode) {
  asm {
    PUSH B
    ; a = fs_seek_3
    MOV  B, A
    LHLD fs_seek_2
    XCHG
    LHLD fs_seek_1
    MVI  A, 3
    CALL fs_entry ; B-�����, DE:HL-��� ����� / A-��� ������, DE:HL-�������
    SHLD fs_low
    XCHG
    SHLD fs_high
    POP  B
  }
}
