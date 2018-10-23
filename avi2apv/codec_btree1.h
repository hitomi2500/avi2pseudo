#ifndef CODEC_BTREE1_H
#define CODEC_BTREE1_H

#include "codec.h"

class Codec_Btree1 : public Codec
{
public:
    Codec_Btree1();
    ~Codec_Btree1();
    void Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed);
    void Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image);
};

#endif // CODEC_BTREE1_H
