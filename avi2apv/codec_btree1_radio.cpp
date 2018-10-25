#include "codec_btree1_radio.h"

Codec_Btree1_Radio::Codec_Btree1_Radio()
{

}

Codec_Btree1_Radio::~Codec_Btree1_Radio()
{

}

void Codec_Btree1_Radio::Pack(QVector<QVector<unsigned char>> Curr_Values, QVector<QVector<unsigned char>> Prev_Values, QByteArray * packed)
{
    int i,j,k,z;
    uchar c,c2;
    int btree_first[8];
    int btree_second[64];
    int btree_third[512];
    int btree_fourth[4096];
    char btree_first_bin;
    char btree_second_bin[8];
    char btree_third_bin[64];
    char btree_fourth_bin[512];

    //packing into binary tree
    int iblock,iline,ipix;
    int istart;
    //calculate first, 8 blocks, 8 lines per block
    for (iblock=0;iblock<8;iblock++) btree_first[iblock] = 0;
    for (iblock=0;iblock<7;iblock++)
    {
        for (iline=iblock*8;iline<(iblock+1)*8;iline++)
            for (ipix=0;ipix<64;ipix++)
                if (iline<60)
                {
                    if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                        btree_first[iblock] = 1;
                }
    }
    //calculate seconds, 60 blocks = lines
    for (iblock=0;iblock<64;iblock++) btree_second[iblock] = 0;
    for (iline=0;iline<60;iline++)
    {
        for (ipix=0;ipix<64;ipix++)
            if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                btree_second[iline] = 1;
    }
    //calculate thirds, 60*8 = 480 blocks
    for (iblock=0;iblock<512;iblock++) btree_third[iblock] = 0;
    for (iblock=0;iblock<480;iblock++)
    {
        btree_third[iblock] = 0;
        iline = iblock/8;
        istart = iblock%8;
        for (ipix=istart*8;ipix<(istart+1)*8;ipix++)
            if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
                btree_third[iblock] = 1;
    }
    //calculate fourth, 60*8*8 = 3840 blocks
    for (iblock=0;iblock<4096;iblock++) btree_fourth[iblock] = 0;
    for (iblock=0;iblock<3840;iblock++)
    {
        btree_fourth[iblock] = 0;
        iline = iblock/64;
        ipix = iblock%64;
        if (Curr_Values[ipix][iline]!=Prev_Values[ipix][iline])
            btree_fourth[iblock] = 1;
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
        btree_fourth_bin[i]=0;
    }
    for (i=0;i<4096;i++)
    {
        if (btree_fourth[i]) btree_fourth_bin[i/8] |= (1<<(i%8));
    }
    //now pack stream
    packed->clear();
    packed->append(btree_first_bin);
    for (i=0;i<8;i++)
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
                            packed->append(btree_fourth_bin[i*64+j*8+k]);
                            for (z=0;z<8;z++)
                            {
                                if (btree_fourth[i*512+j*64+k*8+z])
                                    packed->append(Curr_Values[k*8+z][i*8+j]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Codec_Btree1_Radio::Unpack(QByteArray * packed, QVector<QVector<unsigned char>>* Unpack_Values, QImage* unpack_image)
{
    //reconstructing btree
    int i,j,k,z;
    int iReconstructIndex=0;
    int iReconstructL2=0;
    int iReconstructL3=0;
    int iReconstructL4=0;
    for (i=0;i<8;i++)
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
                            iReconstructIndex++;
                            iReconstructL4=packed->at(iReconstructIndex);
                            for (z=0;z<8;z++)
                            {
                                if (iReconstructL4&(0x1<<z))
                                {
                                    iReconstructIndex++;
                                    int xi = k*8+z;
                                    int xj = i*8+j;
                                    if (xj < Unpack_Values[0][0].size())
                                    {
                                        Unpack_Values[0][xi][xj] = packed->at(iReconstructIndex);
                                        if (Unpack_Values[0][xi][xj] & 0x01 ) unpack_image->setPixelColor(xi*2,xj*2,Qt::red);
                                        else unpack_image->setPixelColor(xi*2,xj*2,Qt::darkRed);
                                        if (Unpack_Values[0][xi][xj] & 0x02 ) unpack_image->setPixelColor(xi*2+1,xj*2,Qt::red);
                                        else unpack_image->setPixelColor(xi*2+2,xj*2,Qt::darkRed);
                                        if (Unpack_Values[0][xi][xj] & 0x04 ) unpack_image->setPixelColor(xi*2,xj*2+1,Qt::red);
                                        else unpack_image->setPixelColor(xi*2,xj*2+1,Qt::darkRed);
                                        if (Unpack_Values[0][xi][xj] & 0x10 ) unpack_image->setPixelColor(xi*2+1,xj*2+1,Qt::red);
                                        else unpack_image->setPixelColor(xi*2+1,xj*2+1,Qt::darkRed);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Codec_Btree1_Radio::DrawPrevState(QVector<QVector<unsigned char>> Prev_Values, QImage * image)
{
    int i,j;
    image->fill(Qt::black);
    for (i=0;i<Prev_Values.size();i++)
         for (j=0;j<Prev_Values[i].size();j++)
         {
             if (Prev_Values[i][j] & 0x01 ) image->setPixelColor(i*2,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x02 ) image->setPixelColor(i*2+1,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x04 ) image->setPixelColor(i*2,j*2+1,Qt::white);
             if (Prev_Values[i][j] & 0x10 ) image->setPixelColor(i*2+1,j*2+1,Qt::white);
         }

}
