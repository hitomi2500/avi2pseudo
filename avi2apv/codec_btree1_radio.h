#ifndef CODEC_BTREE1_RADIO_H
#define CODEC_BTREE1_RADIO_H

#include "codec.h"

class Codec_Btree1_Radio : public Codec
{
public:
    Codec_Btree1_Radio();
    ~Codec_Btree1_Radio();
    void Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed);
    void Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image);
    void DrawPrevState(QVector<QVector<unsigned char>> Prev_Values, QImage * image);
};

#endif // CODEC_BTREE1_RADIO_H
