#ifndef CODEC_BTREE8_H
#define CODEC_BTREE8_H

#include "codec.h"

class Codec_Btree8 : public Codec
{
public:
    Codec_Btree8();
    ~Codec_Btree8();
    void Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed);
    void Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image);
};

#endif // CODEC_BTREE8_H
