#include "codec.h"

Codec::Codec()
{

}

Codec::~Codec()
{

}

void Codec::Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed)
{

}

void Codec::Unpack(QByteArray * packed, QVector<QVector<unsigned char> > Unpack_Values, QImage * unpack_image)
{

}

void Codec::DrawPrevState(QVector<QVector<unsigned char>> Prev_Values, QImage * image)
{
    int i,j;
    image->fill(Qt::black);
    for (i=0;i<Prev_Values.size();i++)
         for (j=0;j<Prev_Values[i].size();j++)
         {
             if (Prev_Values[i][j] & 0x01 ) image->setPixelColor(i*3,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x02 ) image->setPixelColor(i*3+1,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x04 ) image->setPixelColor(i*3+2,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x08 ) image->setPixelColor(i*3,j*2+1,Qt::white);
             if (Prev_Values[i][j] & 0x10 ) image->setPixelColor(i*3+1,j*2+1,Qt::white);
             if (Prev_Values[i][j] & 0x20 ) image->setPixelColor(i*3+2,j*2+1,Qt::white);
         }

}
