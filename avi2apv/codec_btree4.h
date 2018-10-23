#ifndef CODEC_BTREE4_H
#define CODEC_BTREE4_H

#include "codec.h"

class Codec_Btree4 : public Codec
{
public:
    Codec_Btree4();
    ~Codec_Btree4();
    void Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed);
    void Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image);
};

#endif // CODEC_BTREE4_H
