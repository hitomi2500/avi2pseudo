#include "codec_btree8.h"

Codec_Btree8::Codec_Btree8()
{

}

Codec_Btree8::~Codec_Btree8()
{

}

void Codec_Btree8::Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed)
{
    int i,j,k,z;
    uchar c,c2;
    int btree_first[8];
    int btree_second[64];
    int btree_third[512];
    char btree_first_bin;
    char btree_second_bin[8];
    char btree_third_bin[64];

    //packing into binary tree
    int iblock,iline,ipix;
    int istart;
    //calculate first, 8 blocks, 8 lines per block
    for (iblock=0;iblock<8;iblock++) btree_first[iblock] = 0;
    for (iblock=0;iblock<7;iblock++)
    {
        for (iline=iblock*8;iline<(iblock+1)*8;iline++)
            for (ipix=0;ipix<64;ipix++)
                if (iline<51)
                {
                    if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                        btree_first[iblock] = 1;
                }
    }
    //calculate seconds, 51 blocks = lines
    for (iblock=0;iblock<64;iblock++) btree_second[iblock] = 0;
    for (iline=0;iline<51;iline++)
    {
        for (ipix=0;ipix<64;ipix++)
            if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                btree_second[iline] = 1;
    }
    //calculate thirds, 51*8 = 408 blocks
    for (iblock=0;iblock<512;iblock++) btree_third[iblock] = 0;
    for (iblock=0;iblock<408;iblock++)
    {
        btree_third[iblock] = 0;
        iline = iblock/8;
        istart = iblock%8;
        for (ipix=istart*8;ipix<(istart+1)*8;ipix++)
            if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                btree_third[iblock] = 1;
    }
    //calculate binary parts
    btree_first_bin = 0;
    for (i=0;i<8;i++)
    {
        if (btree_first[i]) btree_first_bin |= (1<<i);
        btree_second_bin[i] = 0;
    }
    for (i=0;i<64;i++)
    {
        if (btree_second[i]) btree_second_bin[i/8] |= (1<<(i%8));
        btree_third_bin[i] = 0;
    }
    for (i=0;i<512;i++)
    {
        if (btree_third[i]) btree_third_bin[i/8] |= (1<<(i%8));
    }
    //now pack stream
    packed->clear();
    packed->append(btree_first_bin);
    for (i=0;i<7;i++)
    {
        if (btree_first[i])
        {
            packed->append(btree_second_bin[i]);
            for (j=0;j<8;j++)
            {
                if (btree_second[i*8+j])
                {
                    packed->append(btree_third_bin[i*8+j]);
                    for (k=0;k<8;k++)
                    {
                        if (btree_third[i*64+j*8+k])
                        {
                            for (z=0;z<8;z++)
                                packed->append(Curr_Values[k*8+z][i*8+j]);
                        }
                    }
                }
            }
        }
    }
}

void Codec_Btree8::Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image)
{
    //reconstructing btree
    int i,j,k,z;
    int iReconstructIndex=0;
    int iReconstructL2=0;
    int iReconstructL3=0;
    for (i=0;i<7;i++)
    {
        if (packed->at(0)&(0x1<<i))
        {
            iReconstructIndex++;
            iReconstructL2=packed->at(iReconstructIndex);
            for (j=0;j<8;j++)
            {
                if (iReconstructL2&(0x1<<j))
                {
                    iReconstructIndex++;
                    iReconstructL3=packed->at(iReconstructIndex);
                    for (k=0;k<8;k++)
                    {
                        if (iReconstructL3&(0x1<<k))
                        {
                            for (z=0;z<8;z++)
                            {
                                iReconstructIndex++;
                                int xi = k*8+z;
                                int xj = i*8+j;
                                if (xj < Unpack_Values[0][0].size())
                                {
                                    Unpack_Values[0][xi][xj] = packed->at(iReconstructIndex);
                                    if (Unpack_Values[0][xi][xj] & 0x01 ) unpack_image->setPixelColor(xi*3,xj*2,Qt::red);
                                    else unpack_image->setPixelColor(xi*3,xj*2,Qt::darkRed);
                                    if (Unpack_Values[0][xi][xj] & 0x02 ) unpack_image->setPixelColor(xi*3+1,xj*2,Qt::red);
                                    else unpack_image->setPixelColor(xi*3+1,xj*2,Qt::darkRed);
                                    if (Unpack_Values[0][xi][xj] & 0x04 ) unpack_image->setPixelColor(xi*3+2,xj*2,Qt::red);
                                    else unpack_image->setPixelColor(xi*3+2,xj*2,Qt::darkRed);
                                    if (Unpack_Values[0][xi][xj] & 0x08 ) unpack_image->setPixelColor(xi*3,xj*2+1,Qt::red);
                                    else unpack_image->setPixelColor(xi*3,xj*2+1,Qt::darkRed);
                                    if (Unpack_Values[0][xi][xj] & 0x10 ) unpack_image->setPixelColor(xi*3+1,xj*2+1,Qt::red);
                                    else unpack_image->setPixelColor(xi*3+1,xj*2+1,Qt::darkRed);
                                    if (Unpack_Values[0][xi][xj] & 0x20 ) unpack_image->setPixelColor(xi*3+2,xj*2+1,Qt::red);
                                    else unpack_image->setPixelColor(xi*3+2,xj*2+1,Qt::darkRed);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
