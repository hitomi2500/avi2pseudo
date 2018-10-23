#ifndef CODEC_H
#define CODEC_H

#include <QImage>
#include <QByteArray>

class Codec
{
public:
    Codec();
    ~Codec();
    virtual void Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed);
    virtual void Unpack(QByteArray * packed, QVector<QVector<unsigned char>> Unpack_Values, QImage* unpack_image);
    void DrawPrevState(QVector<QVector<unsigned char>> Prev_Values, QImage * image);
};

#endif // CODEC_H
