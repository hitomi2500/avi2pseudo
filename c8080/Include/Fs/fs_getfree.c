#include <fs/fs.h>

uchar fs_getfree() { // fs_high:fs_low - ������ � ��
  return fs_seek(0, 0, 102);
}
