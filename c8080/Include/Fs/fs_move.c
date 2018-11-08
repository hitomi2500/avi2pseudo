#include <fs/fs.h>

uchar fs_move(const char* from, const char* to) {
  asm {
    PUSH B
    XCHG
    LHLD fs_move_1
    MVI  A, 6
    CALL fs_entry ; HL-��, DE-� / A-��� ������
    POP  B
  }
}
