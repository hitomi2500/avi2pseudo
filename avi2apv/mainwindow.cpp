#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTFFmpegWrapper/QVideoDecoder.h>
#include <QTimer>
#include <QRgb>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_Btree_Reconstruct2_1->hide();
    ui->label_Btree_Reconstruct2_2->hide();
    ui->label_Btree_Reconstruct4_1->hide();
    ui->label_Btree_Reconstruct4_2->hide();
    ui->label_Btree_Reconstruct8_1->hide();
    ui->label_Btree_Reconstruct8_2->hide();
    ui->label_Btrees2_1->hide();
    ui->label_Btrees2_2->hide();
    ui->label_Btrees4_1->hide();
    ui->label_Btrees4_2->hide();
    ui->label_Btrees8_1->hide();
    ui->label_Btrees8_2->hide();
    decoder = new QVideoDecoder();
    out_file = new QFile();
    Prev_Values.resize(64);
    for (int i=0;i<Prev_Values.size();i++)
        Prev_Values[i].resize(51);
    Curr_Values.resize(64);
    for (int i=0;i<Curr_Values.size();i++)
        Curr_Values[i].resize(51);
    Prev_Values2.resize(64);
    for (int i=0;i<Prev_Values2.size();i++)
        Prev_Values2[i].resize(51);
    Curr_Values2.resize(64);
    for (int i=0;i<Curr_Values2.size();i++)
        Curr_Values2[i].resize(51);
    Reconstruct_Btree8_1.resize(64);
    for (int i=0;i<Reconstruct_Btree8_1.size();i++)
        Reconstruct_Btree8_1[i].resize(51);
    Reconstruct_Btree8_2.resize(64);
    for (int i=0;i<Reconstruct_Btree8_2.size();i++)
        Reconstruct_Btree8_2[i].resize(51);
    Reconstruct_Btree4_1.resize(64);
    for (int i=0;i<Reconstruct_Btree4_1.size();i++)
        Reconstruct_Btree4_1[i].resize(51);
    Reconstruct_Btree4_2.resize(64);
    for (int i=0;i<Reconstruct_Btree4_2.size();i++)
        Reconstruct_Btree4_2[i].resize(51);
    Reconstruct_Btree2_1.resize(64);
    for (int i=0;i<Reconstruct_Btree2_1.size();i++)
        Reconstruct_Btree2_1[i].resize(51);
    Reconstruct_Btree2_2.resize(64);
    for (int i=0;i<Reconstruct_Btree2_2.size();i++)
        Reconstruct_Btree2_2[i].resize(51);
    Reconstruct_Btree1_1.resize(64);
    for (int i=0;i<Reconstruct_Btree1_1.size();i++)
        Reconstruct_Btree1_1[i].resize(51);
    Reconstruct_Btree1_2.resize(64);
    for (int i=0;i<Reconstruct_Btree1_2.size();i++)
        Reconstruct_Btree1_2[i].resize(51);
    ui->radioButton_2->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Select_Input_clicked()
{
    ui->lineEdit_Input->setText( QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Video Files (*.avi)")) );
}

void MainWindow::on_pushButton_Select_Output_clicked()
{
    ui->lineEdit_Output->setText( QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Apogey Video Files (*.apv)")) );
}

void MainWindow::do_next_frame()
{
    int i,j;
    uchar c,c2;
    QByteArray pseudo_data;
    QByteArray pseudo_data_btree8_1;
    QByteArray pseudo_data_btree8_2;
    QByteArray pseudo_data_btree4_1;
    QByteArray pseudo_data_btree4_2;
    QByteArray pseudo_data_btree2_1;
    QByteArray pseudo_data_btree2_2;
    QByteArray pseudo_data_btree1_1;
    QByteArray pseudo_data_btree1_2;
    decoder->getFrame(my_img_raw);
    QRect CropRect;
    CropRect.setSize(QSize(192,102));
    CropRect.moveCenter(my_img_raw.rect().center());
    my_img = my_img_raw.copy(CropRect);
    my_img_uniform = my_img.convertToFormat(QImage::Format_Mono);
    my_img_uniform2 = my_img.convertToFormat(QImage::Format_Grayscale8);
    for (j=0;j<my_img_uniform2.height();j++)
        for (i=0;i<my_img_uniform2.width();i++)
        {
            if(qGray(my_img_uniform2.pixel(i,j)) > ui->spinBox_Level->value() )
                my_img_uniform2.setPixelColor(i,j,Qt::white);
            else
                my_img_uniform2.setPixelColor(i,j,Qt::black);
        }
    ui->label_InputFrame->setPixmap(QPixmap::fromImage(my_img));
    ui->label_OutputFrame->setPixmap(QPixmap::fromImage(my_img_uniform));
    ui->label_OutputFrameFilter->setPixmap(QPixmap::fromImage(my_img_uniform2));
    pseudo_data.clear();
    Delta = 0;Delta2 = 0;
    my_img_deltaview = my_img.copy();
    my_img_deltaview.fill(Qt::black);
    my_img_deltaview2 = my_img.copy();
    my_img_deltaview2.fill(Qt::black);
    for (i=0;i<Curr_Values.size();i++)
        for (j=0;j<Curr_Values[i].size();j++)
        Curr_Values[i][j]=0;
    for (i=0;i<Prev_Values.size();i++)
         for (j=0;j<Prev_Values[i].size();j++)
         {
             if (Prev_Values[i][j] & 0x01 ) my_img_deltaview.setPixelColor(i*3,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x02 ) my_img_deltaview.setPixelColor(i*3+1,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x04 ) my_img_deltaview.setPixelColor(i*3+2,j*2,Qt::white);
             if (Prev_Values[i][j] & 0x08 ) my_img_deltaview.setPixelColor(i*3,j*2+1,Qt::white);
             if (Prev_Values[i][j] & 0x10 ) my_img_deltaview.setPixelColor(i*3+1,j*2+1,Qt::white);
             if (Prev_Values[i][j] & 0x20 ) my_img_deltaview.setPixelColor(i*3+2,j*2+1,Qt::white);
             if (Prev_Values2[i][j] & 0x01 ) my_img_deltaview2.setPixelColor(i*3,j*2,Qt::white);
             if (Prev_Values2[i][j] & 0x02 ) my_img_deltaview2.setPixelColor(i*3+1,j*2,Qt::white);
             if (Prev_Values2[i][j] & 0x04 ) my_img_deltaview2.setPixelColor(i*3+2,j*2,Qt::white);
             if (Prev_Values2[i][j] & 0x08 ) my_img_deltaview2.setPixelColor(i*3,j*2+1,Qt::white);
             if (Prev_Values2[i][j] & 0x10 ) my_img_deltaview2.setPixelColor(i*3+1,j*2+1,Qt::white);
             if (Prev_Values2[i][j] & 0x20 ) my_img_deltaview2.setPixelColor(i*3+2,j*2+1,Qt::white);
         }
    for (j=0;j<my_img_uniform.height();j+=2)
    {
        for (i=0;i<my_img_uniform.width();i+=3)
        {
            int iTreshold = 128;
            c=0;c2=0;
            if (qGray(my_img_uniform.pixel(i,j)) > iTreshold) c |= 0x01;
            if (qGray(my_img_uniform.pixel(i+1,j)) > iTreshold) c |= 0x02;
            if (qGray(my_img_uniform.pixel(i+2,j)) > iTreshold) c |= 0x04;
            if (qGray(my_img_uniform.pixel(i,j+1)) > iTreshold) c |= 0x08;
            if (qGray(my_img_uniform.pixel(i+1,j+1)) > iTreshold) c |= 0x10;
            if (qGray(my_img_uniform.pixel(i+2,j+1)) > iTreshold) c |= 0x20;
            if (qGray(my_img_uniform2.pixel(i,j)) > iTreshold) c2 |= 0x01;
            if (qGray(my_img_uniform2.pixel(i+1,j)) > iTreshold) c2 |= 0x02;
            if (qGray(my_img_uniform2.pixel(i+2,j)) > iTreshold) c2 |= 0x04;
            if (qGray(my_img_uniform2.pixel(i,j+1)) > iTreshold) c2 |= 0x08;
            if (qGray(my_img_uniform2.pixel(i+1,j+1)) > iTreshold) c2 |= 0x10;
            if (qGray(my_img_uniform2.pixel(i+2,j+1)) > iTreshold) c2 |= 0x20;
            pseudo_data.append(c);
            if (Prev_Values[i/3][j/2] != c)
            {
                if (Prev_Values[i/3][j/2] & 0x01 ) my_img_deltaview.setPixelColor(i,j,Qt::green);
                else my_img_deltaview.setPixelColor(i,j,Qt::darkGreen);
                if (Prev_Values[i/3][j/2] & 0x02 ) my_img_deltaview.setPixelColor(i+1,j,Qt::green);
                else my_img_deltaview.setPixelColor(i+1,j,Qt::darkGreen);
                if (Prev_Values[i/3][j/2] & 0x04 ) my_img_deltaview.setPixelColor(i+2,j,Qt::green);
                else my_img_deltaview.setPixelColor(i+2,j,Qt::darkGreen);
                if (Prev_Values[i/3][j/2] & 0x08 ) my_img_deltaview.setPixelColor(i,j+1,Qt::green);
                else my_img_deltaview.setPixelColor(i,j+1,Qt::darkGreen);
                if (Prev_Values[i/3][j/2] & 0x10 ) my_img_deltaview.setPixelColor(i+1,j+1,Qt::green);
                else my_img_deltaview.setPixelColor(i+1,j+1,Qt::darkGreen);
                if (Prev_Values[i/3][j/2] & 0x20 ) my_img_deltaview.setPixelColor(i+2,j+1,Qt::green);
                else my_img_deltaview.setPixelColor(i+2,j+1,Qt::darkGreen);
                Delta++;
            }
            if (Prev_Values2[i/3][j/2] != c2)
            {
                if (Prev_Values2[i/3][j/2] & 0x01 ) my_img_deltaview2.setPixelColor(i,j,Qt::green);
                else my_img_deltaview.setPixelColor(i,j,Qt::darkGreen);
                if (Prev_Values2[i/3][j/2] & 0x02 ) my_img_deltaview2.setPixelColor(i+1,j,Qt::green);
                else my_img_deltaview.setPixelColor(i+1,j,Qt::darkGreen);
                if (Prev_Values2[i/3][j/2] & 0x04 ) my_img_deltaview2.setPixelColor(i+2,j,Qt::green);
                else my_img_deltaview.setPixelColor(i+2,j,Qt::darkGreen);
                if (Prev_Values2[i/3][j/2] & 0x08 ) my_img_deltaview2.setPixelColor(i,j+1,Qt::green);
                else my_img_deltaview.setPixelColor(i,j+1,Qt::darkGreen);
                if (Prev_Values2[i/3][j/2] & 0x10 ) my_img_deltaview2.setPixelColor(i+1,j+1,Qt::green);
                else my_img_deltaview.setPixelColor(i+1,j+1,Qt::darkGreen);
                if (Prev_Values2[i/3][j/2] & 0x20 ) my_img_deltaview2.setPixelColor(i+2,j+1,Qt::green);
                else my_img_deltaview.setPixelColor(i+2,j+1,Qt::darkGreen);
                Delta2++;
            }
            Curr_Values[i/3][j/2] = c;
            Curr_Values2[i/3][j/2] = c2;
        }
    }

    //draw deltas
    ui->label_Delta_View->setPixmap(QPixmap::fromImage(my_img_deltaview));
    ui->label_Delta_View2->setPixmap(QPixmap::fromImage(my_img_deltaview2));

    /*_codec_btree8.Pack(Curr_Values,Prev_Values,&pseudo_data_btree8_1);
    _codec_btree8.Pack(Curr_Values2,Prev_Values2,&pseudo_data_btree8_2);

    _codec_btree4.Pack(Curr_Values,Prev_Values,&pseudo_data_btree4_1);
    _codec_btree4.Pack(Curr_Values2,Prev_Values2,&pseudo_data_btree4_2);

    _codec_btree2.Pack(Curr_Values,Prev_Values,&pseudo_data_btree2_1);
    _codec_btree2.Pack(Curr_Values2,Prev_Values2,&pseudo_data_btree2_2);*/

    _codec_btree1.Pack(Curr_Values,Prev_Values,&pseudo_data_btree1_1);
    _codec_btree1.Pack(Curr_Values2,Prev_Values2,&pseudo_data_btree1_2);

    /*my_img_tree_reconstruct8_1 = my_img.copy();
    my_img_tree_reconstruct8_2 = my_img.copy();
    my_img_tree_reconstruct4_1 = my_img.copy();
    my_img_tree_reconstruct4_2 = my_img.copy();
    my_img_tree_reconstruct2_1 = my_img.copy();
    my_img_tree_reconstruct2_2 = my_img.copy();*/
    my_img_tree_reconstruct1_1 = my_img.copy();
    my_img_tree_reconstruct1_2 = my_img.copy();
    /*_codec_btree8.DrawPrevState(Prev_Values,&my_img_tree_reconstruct8_1);
    _codec_btree8.DrawPrevState(Prev_Values2,&my_img_tree_reconstruct8_2);
    _codec_btree8.DrawPrevState(Prev_Values,&my_img_tree_reconstruct4_1);
    _codec_btree8.DrawPrevState(Prev_Values2,&my_img_tree_reconstruct4_2);
    _codec_btree8.DrawPrevState(Prev_Values,&my_img_tree_reconstruct2_1);
    _codec_btree8.DrawPrevState(Prev_Values2,&my_img_tree_reconstruct2_2);*/
    _codec_btree8.DrawPrevState(Prev_Values,&my_img_tree_reconstruct1_1);
    _codec_btree8.DrawPrevState(Prev_Values2,&my_img_tree_reconstruct1_2);

    for (i=0;i<Reconstruct_Btree8_1.size();i++)
        for (j-0;j<Reconstruct_Btree8_1[i].size();j++)
        {
           /* Reconstruct_Btree8_1[i][j] = Prev_Values[i][j];
            Reconstruct_Btree8_2[i][j] = Prev_Values2[i][j];
            Reconstruct_Btree4_1[i][j] = Prev_Values[i][j];
            Reconstruct_Btree4_2[i][j] = Prev_Values2[i][j];
            Reconstruct_Btree2_1[i][j] = Prev_Values[i][j];
            Reconstruct_Btree2_2[i][j] = Prev_Values2[i][j];*/
            Reconstruct_Btree1_1[i][j] = Prev_Values[i][j];
            Reconstruct_Btree1_2[i][j] = Prev_Values2[i][j];
        }

    /*_codec_btree8.Unpack(&pseudo_data_btree8_1,&Reconstruct_Btree8_1,&my_img_tree_reconstruct8_1);
    _codec_btree8.Unpack(&pseudo_data_btree8_2,&Reconstruct_Btree8_2,&my_img_tree_reconstruct8_2);

    _codec_btree4.Unpack(&pseudo_data_btree4_1,&Reconstruct_Btree4_1,&my_img_tree_reconstruct4_1);
    _codec_btree4.Unpack(&pseudo_data_btree4_2,&Reconstruct_Btree4_2,&my_img_tree_reconstruct4_2);

    _codec_btree2.Unpack(&pseudo_data_btree2_1,&Reconstruct_Btree2_1,&my_img_tree_reconstruct2_1);
    _codec_btree2.Unpack(&pseudo_data_btree2_2,&Reconstruct_Btree2_2,&my_img_tree_reconstruct2_2);*/

    _codec_btree1.Unpack(&pseudo_data_btree1_1,&Reconstruct_Btree1_1,&my_img_tree_reconstruct1_1);
    _codec_btree1.Unpack(&pseudo_data_btree1_2,&Reconstruct_Btree1_2,&my_img_tree_reconstruct1_2);

    for (i=0;i<Curr_Values.size();i++)
         for (j=0;j<Curr_Values[i].size();j++)
         {
            /* if (Reconstruct_Btree8_1[i][j]!=Curr_Values[i][j])
                    iErrors++;
             if (Reconstruct_Btree8_2[i][j]!=Curr_Values2[i][j])
                    iErrors++;
             if (Reconstruct_Btree4_1[i][j]!=Curr_Values[i][j])
                    iErrors++;
             if (Reconstruct_Btree4_2[i][j]!=Curr_Values2[i][j])
                    iErrors++;
             if (Reconstruct_Btree2_1[i][j]!=Curr_Values[i][j])
                    iErrors++;
             if (Reconstruct_Btree2_2[i][j]!=Curr_Values2[i][j])
                    iErrors++;*/
             if (Reconstruct_Btree1_1[i][j]!=Curr_Values[i][j])
                    iErrors++;
             if (Reconstruct_Btree1_2[i][j]!=Curr_Values2[i][j])
                    iErrors++;
         }
    ui->label_Errors->setText(QString("Errors:%1").arg(iErrors));

    /*ui->label_Btree_Reconstruct8_1->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct8_1));
    ui->label_Btree_Reconstruct8_2->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct8_2));
    ui->label_Btree_Reconstruct4_1->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct4_1));
    ui->label_Btree_Reconstruct4_2->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct4_2));
    ui->label_Btree_Reconstruct2_1->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct2_1));
    ui->label_Btree_Reconstruct2_2->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct2_2));*/
    ui->label_Btree_Reconstruct1_1->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct1_1));
    ui->label_Btree_Reconstruct1_2->setPixmap(QPixmap::fromImage(my_img_tree_reconstruct1_2));

    //copy currents to prevs
    //Prev_Values = Curr_Values;
    for (i=0;i<Curr_Values.size();i++)
    {
        for (j=0;j<Curr_Values[i].size();j++)
        {
            Prev_Values[i][j] = Curr_Values[i][j];
            Prev_Values2[i][j] = Curr_Values2[i][j];
        }
    }

    if (Delta > 0)
    {
        Deltas.append(Delta);
        if(Delta_Max<Delta) Delta_Max = Delta;
        if(Delta_Min>Delta) Delta_Min = Delta;
        long int e=0;
        for (i=0;i<Deltas.size();i++) e+= Deltas.at(i);
        e/= Deltas.size();
        ui->label_Delta->setText(QString("Delta:%1/%2/%3").arg(Delta_Min).arg(e).arg(Delta_Max));

       /* Btrees8_1.append(pseudo_data_btree8_1.size());
        if(Btrees8_1_Max<pseudo_data_btree8_1.size()) Btrees8_1_Max = pseudo_data_btree8_1.size();
        if(Btrees8_1_Min>pseudo_data_btree8_1.size()) Btrees8_1_Min = pseudo_data_btree8_1.size();
        e=0;
        for (i=0;i<Btrees8_1.size();i++) e+= Btrees8_1.at(i);
        e/= Btrees8_1.size();
        ui->label_Btrees8_1->setText(QString("Btree8:%1/%2/%3").arg(Btrees8_1_Min).arg(e).arg(Btrees8_1_Max));

        Btrees4_1.append(pseudo_data_btree4_1.size());
        if(Btrees4_1_Max<pseudo_data_btree4_1.size()) Btrees4_1_Max = pseudo_data_btree4_1.size();
        if(Btrees4_1_Min>pseudo_data_btree4_1.size()) Btrees4_1_Min = pseudo_data_btree4_1.size();
        e=0;
        for (i=0;i<Btrees4_1.size();i++) e+= Btrees4_1.at(i);
        e/= Btrees4_1.size();
        ui->label_Btrees4_1->setText(QString("Btree4:%1/%2/%3").arg(Btrees4_1_Min).arg(e).arg(Btrees4_1_Max));*/

        Btrees2_1.append(pseudo_data_btree2_1.size());
        if(Btrees2_1_Max<pseudo_data_btree2_1.size()) Btrees2_1_Max = pseudo_data_btree2_1.size();
        if(Btrees2_1_Min>pseudo_data_btree2_1.size()) Btrees2_1_Min = pseudo_data_btree2_1.size();
        e=0;
        for (i=0;i<Btrees2_1.size();i++) e+= Btrees2_1.at(i);
        e/= Btrees2_1.size();
        ui->label_Btrees2_1->setText(QString("Btree2:%1/%2/%3").arg(Btrees2_1_Min).arg(e).arg(Btrees2_1_Max));

        Btrees1_1.append(pseudo_data_btree1_1.size());
        if(Btrees1_1_Max<pseudo_data_btree1_1.size()) Btrees1_1_Max = pseudo_data_btree1_1.size();
        if(Btrees1_1_Min>pseudo_data_btree1_1.size()) Btrees1_1_Min = pseudo_data_btree1_1.size();
        e=0;
        for (i=0;i<Btrees1_1.size();i++) e+= Btrees1_1.at(i);
        e/= Btrees1_1.size();
        ui->label_Btrees1_1->setText(QString("Btree1:%1/%2/%3").arg(Btrees1_1_Min).arg(e).arg(Btrees1_1_Max));

    }
    if (Delta2 > 0)
    {
        Deltas2.append(Delta2);
        if(Delta2_Max<Delta2) Delta2_Max = Delta2;
        if(Delta2_Min>Delta2) Delta2_Min = Delta2;
        long int e=0;
        for (i=0;i<Deltas2.size();i++) e+= Deltas2.at(i);
        e/= Deltas2.size();
        ui->label_Delta2->setText(QString("Delta:%1/%2/%3").arg(Delta2_Min).arg(e).arg(Delta2_Max));

      /*  Btrees8_2.append(pseudo_data_btree8_2.size());
        if(Btrees8_2_Max<pseudo_data_btree8_2.size()) Btrees8_2_Max = pseudo_data_btree8_2.size();
        if(Btrees8_2_Min>pseudo_data_btree8_2.size()) Btrees8_2_Min = pseudo_data_btree8_2.size();
        e=0;
        for (i=0;i<Btrees8_2.size();i++) e+= Btrees8_2.at(i);
        e/= Btrees8_2.size();
        ui->label_Btrees8_2->setText(QString("Btree8:%1/%2/%3").arg(Btrees8_2_Min).arg(e).arg(Btrees8_2_Max));

        Btrees4_2.append(pseudo_data_btree4_2.size());
        if(Btrees4_2_Max<pseudo_data_btree4_2.size()) Btrees4_2_Max = pseudo_data_btree4_2.size();
        if(Btrees4_2_Min>pseudo_data_btree4_2.size()) Btrees4_2_Min = pseudo_data_btree4_2.size();
        e=0;
        for (i=0;i<Btrees4_2.size();i++) e+= Btrees4_2.at(i);
        e/= Btrees4_2.size();
        ui->label_Btrees4_2->setText(QString("Btree4:%1/%2/%3").arg(Btrees4_2_Min).arg(e).arg(Btrees4_2_Max));*/

        Btrees2_2.append(pseudo_data_btree2_2.size());
        if(Btrees2_2_Max<pseudo_data_btree2_2.size()) Btrees2_2_Max = pseudo_data_btree2_2.size();
        if(Btrees2_2_Min>pseudo_data_btree2_2.size()) Btrees2_2_Min = pseudo_data_btree2_2.size();
        e=0;
        for (i=0;i<Btrees2_2.size();i++) e+= Btrees2_2.at(i);
        e/= Btrees2_2.size();
        ui->label_Btrees2_2->setText(QString("Btree2:%1/%2/%3").arg(Btrees2_2_Min).arg(e).arg(Btrees2_2_Max));

        Btrees1_2.append(pseudo_data_btree1_2.size());
        if(Btrees1_2_Max<pseudo_data_btree1_2.size()) Btrees1_2_Max = pseudo_data_btree1_2.size();
        if(Btrees1_2_Min>pseudo_data_btree1_2.size()) Btrees1_2_Min = pseudo_data_btree1_2.size();
        e=0;
        for (i=0;i<Btrees1_2.size();i++) e+= Btrees1_2.at(i);
        e/= Btrees1_2.size();
        ui->label_Btrees1_2->setText(QString("Btree1:%1/%2/%3").arg(Btrees1_2_Min).arg(e).arg(Btrees1_2_Max));

    }
    QByteArray data_to_save;
    if (ui->radioButton->isChecked()) data_to_save = pseudo_data_btree1_1;
    else data_to_save = pseudo_data_btree1_2;

    int tsize = data_to_save.size();
    data_to_save.prepend((char)(tsize/0x100));
    data_to_save.prepend((char)(tsize%0x100));
    data_to_save.prepend((char)0);
    data_to_save.prepend((char)0);

    out_file->write(data_to_save);

    iCurrentFrame++;
    if (true == decoder->seekNextFrame())
    {
        if ( bPaused == false )
            QTimer::singleShot(2, this, SLOT(do_next_frame()));
    }
    else
    {
        out_file->close();
        decoder->close();
    }
}

void MainWindow::on_pushButton_Convert_clicked()
{
    int i,j;
    out_file->setFileName(ui->lineEdit_Output->text());
    out_file->open(QIODevice::Truncate|QIODevice::ReadWrite);
    decoder->openFile(ui->lineEdit_Input->text());
    decoder->seekFrame(50);
    iCurrentFrame = 0;
    Delta_Max=0;
    Delta_Min=100500;
    Delta2_Max=0;
    Delta2_Min=100500;
    Deltas.clear();
    Deltas2.clear();
    Btrees8_1_Max=0;
    Btrees8_1_Min=100500;
    Btrees8_2_Max=0;
    Btrees8_2_Min=100500;
    Btrees4_1_Max=0;
    Btrees4_1_Min=100500;
    Btrees4_2_Max=0;
    Btrees4_2_Min=100500;
    Btrees2_1_Max=0;
    Btrees2_1_Min=100500;
    Btrees2_2_Max=0;
    Btrees2_2_Min=100500;
    Btrees1_1_Max=0;
    Btrees1_1_Min=100500;
    Btrees1_2_Max=0;
    Btrees1_2_Min=100500;
    Btrees8_1.clear();
    Btrees8_2.clear();
    Btrees4_1.clear();
    Btrees4_2.clear();
    Btrees2_1.clear();
    Btrees2_2.clear();
    Btrees1_1.clear();
    Btrees1_2.clear();
    for (i=0;i<Prev_Values.size();i++)
        for (j=0;j<Prev_Values[i].size();j++)
        {
            Prev_Values[i][j] = 0;
            Prev_Values2[i][j] = 0;
            Curr_Values[i][j] = 0;
            Curr_Values2[i][j] = 0;
            Reconstruct_Btree8_1[i][j]=0;
            Reconstruct_Btree8_2[i][j]=0;
            Reconstruct_Btree4_1[i][j]=0;
            Reconstruct_Btree4_2[i][j]=0;
            Reconstruct_Btree2_1[i][j]=0;
            Reconstruct_Btree2_2[i][j]=0;
            Reconstruct_Btree1_1[i][j]=0;
            Reconstruct_Btree1_2[i][j]=0;
        }
    bPaused = false;
    iErrors=0;
    QTimer::singleShot(200, this, SLOT(do_next_frame()));
}

void MainWindow::on_pushButton_Pause_clicked()
{
    if (false==bPaused)
        bPaused = true;
    else
    {
        bPaused = false;
        QTimer::singleShot(2, this, SLOT(do_next_frame()));
    }
}
