#include <apogey/video.h>
                             
void print(uchar x, uchar y, char* text) {
  print2(charAddr(x, y), text);
}
