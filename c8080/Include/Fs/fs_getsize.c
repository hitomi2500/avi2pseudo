#include <fs/fs.h>

uchar fs_getsize() { // fs_high:fs_low - ������
  return fs_seek(0, 0, 100);
}
