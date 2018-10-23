#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTFFmpegWrapper/QVideoDecoder.h>
#include <codec_btree8.h>
#include <codec_btree4.h>
#include <codec_btree2.h>
#include <codec_btree1.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Select_Input_clicked();

    void on_pushButton_Select_Output_clicked();

    void on_pushButton_Convert_clicked();

    void do_next_frame();

    void on_pushButton_Pause_clicked();

private:
    Ui::MainWindow *ui;
    QVideoDecoder *decoder;
    QFile *out_file;
    QImage my_img;
    QImage my_img_raw;
    QImage my_img_uniform;
    QImage my_img_uniform2;
    QImage my_img_tree_reconstruct8_1;
    QImage my_img_tree_reconstruct8_2;
    QImage my_img_tree_reconstruct4_1;
    QImage my_img_tree_reconstruct4_2;
    QImage my_img_tree_reconstruct2_1;
    QImage my_img_tree_reconstruct2_2;
    QImage my_img_tree_reconstruct1_1;
    QImage my_img_tree_reconstruct1_2;
    QImage my_img_deltaview;
    QImage my_img_deltaview2;
    int iCurrentFrame;
    QVector<QVector<unsigned char>> Prev_Values;
    QVector<QVector<unsigned char>> Prev_Values2;
    QVector<QVector<unsigned char>> Curr_Values;
    QVector<QVector<unsigned char>> Curr_Values2;
    QVector<QVector<unsigned char>> Reconstruct_Btree8_1;
    QVector<QVector<unsigned char>> Reconstruct_Btree8_2;
    QVector<QVector<unsigned char>> Reconstruct_Btree4_1;
    QVector<QVector<unsigned char>> Reconstruct_Btree4_2;
    QVector<QVector<unsigned char>> Reconstruct_Btree2_1;
    QVector<QVector<unsigned char>> Reconstruct_Btree2_2;
    QVector<QVector<unsigned char>> Reconstruct_Btree1_1;
    QVector<QVector<unsigned char>> Reconstruct_Btree1_2;
    int Delta;
    QList<int> Deltas;
    int Delta_Min;
    int Delta_Max;
    QList<int> Btrees8_1;
    int Btrees8_1_Min;
    int Btrees8_1_Max;
    QList<int> Btrees4_1;
    int Btrees4_1_Min;
    int Btrees4_1_Max;
    QList<int> Btrees2_1;
    int Btrees2_1_Min;
    int Btrees2_1_Max;
    QList<int> Btrees1_1;
    int Btrees1_1_Min;
    int Btrees1_1_Max;
    int Delta2;
    QList<int> Deltas2;
    int Delta2_Min;
    int Delta2_Max;
    QList<int> Btrees8_2;
    int Btrees8_2_Min;
    int Btrees8_2_Max;
    QList<int> Btrees4_2;
    int Btrees4_2_Min;
    int Btrees4_2_Max;
    QList<int> Btrees2_2;
    int Btrees2_2_Min;
    int Btrees2_2_Max;
    QList<int> Btrees1_2;
    int Btrees1_2_Min;
    int Btrees1_2_Max;
    bool bPaused;
    int iErrors;
    Codec_Btree8 _codec_btree8;
    Codec_Btree4 _codec_btree4;
    Codec_Btree2 _codec_btree2;
    Codec_Btree1 _codec_btree1;
};

#endif // MAINWINDOW_H
