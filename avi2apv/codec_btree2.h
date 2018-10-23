#ifndef CODEC_BTREE2_H
#define CODEC_BTREE2_H

#include "codec.h"

class Codec_Btree2 : public Codec
{
public:
    Codec_Btree2();
    ~Codec_Btree2();
    void Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed);
    void Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image);
};

#endif // CODEC_BTREE2_H
