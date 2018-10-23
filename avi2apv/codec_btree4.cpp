#include "codec_btree4.h"

Codec_Btree4::Codec_Btree4()
{

}

Codec_Btree4::~Codec_Btree4()
{

}

void Codec_Btree4::Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed)
{
    int i,j,k,z;
    uchar c,c2;
    int btree_first[16];
    int btree_second[128];
    int btree_third[1024];
    char btree_first_bin[2];
    char btree_second_bin[16];
    char btree_third_bin[128];

    //packing into binary tree
    int iblock,iline,ipix;
    int istart;
    //calculate first, 13 blocks, 4 lines per block
    for (iblock=0;iblock<16;iblock++) btree_first[iblock] = 0;
    for (iblock=0;iblock<13;iblock++)
    {
        for (iline=iblock*4;iline<(iblock+1)*4;iline++)
            for (ipix=0;ipix<64;ipix++)
                if (iline <51)
                    if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                        btree_first[iblock] = 1;
    }
    //calculate seconds, 51*2=102 blocks
    for (iblock=0;iblock<128;iblock++) btree_second[iblock] = 0;
    for (iblock=0;iblock<102;iblock++)
    {
        iline = iblock/2;
        istart = iblock % 2;
        for (ipix=istart*32;ipix<(istart+1)*32;ipix++)
            if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                btree_second[iblock] = 1;
    }
    //calculate thirds, 51*2*8 = 816 blocks
    for (iblock=0;iblock<1024;iblock++) btree_third[iblock] = 0;
    for (iblock=0;iblock<816;iblock++)
    {
        iline = iblock/16;
        istart = iblock%16;
        for (ipix=istart*4;ipix<(istart+1)*4;ipix++)
            if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                btree_third[iblock] = 1;
    }
    //calculate binary parts
    btree_first_bin[0] = 0;
    btree_first_bin[1] = 0;
    for (i=0;i<13;i++)
    {
        if (btree_first[i]) btree_first_bin[i/8] |= (1<<(i%8));
        btree_second_bin[i] = 0;
    }
    for (i=0;i<128;i++)
    {
        if (btree_second[i]) btree_second_bin[i/8] |= (1<<(i%8));
        btree_third_bin[i] = 0;
    }
    for (i=0;i<1024;i++)
    {
        if (btree_third[i]) btree_third_bin[i/8] |= (1<<(i%8));
    }
    //now pack stream
    packed->clear();
    packed->append(btree_first_bin[0]);
    packed->append(btree_first_bin[1]);
    for (i=0;i<13;i++)
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
                            for (z=0;z<4;z++)
                                packed->append(Curr_Values[(j%2)*32+k*4+z][i*4+j/2]);
                        }
                    }
                }
            }
        }
    }
}

void Codec_Btree4::Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image)
{
    //reconstructing btree4
    int i,j,k,z;
    int iReconstructIndex=1;
    int iReconstructL2=0;
    int iReconstructL3=0;
    for (i=0;i<13;i++)
    {
        if (packed->at(i/8)&(0x1<<(i%8)))
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
                            for (z=0;z<4;z++)
                            {
                                iReconstructIndex++;
                                int xi = (j%2)*32+k*4+z;
                                int xj = i*4+j/2;
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
