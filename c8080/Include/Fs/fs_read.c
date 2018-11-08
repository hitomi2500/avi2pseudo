#include <fs/fs.h>

uchar fs_read(void* buf, uint size) {
  asm {
    PUSH B
    ; hl = fs_read_2
    XCHG
    LHLD fs_read_1
    XCHG
    MVI  A, 4
    CALL fs_entry ; HL-������, DE-����� / HL-������� ���������, A-��� ������
    SHLD fs_low
    POP  B    
  }
}
