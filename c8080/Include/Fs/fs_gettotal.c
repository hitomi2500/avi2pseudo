#include <fs/fs.h>

uchar fs_gettotal() { // fs_high:fs_low - ������ � ��
  return fs_seek(0, 0, 101);
}
