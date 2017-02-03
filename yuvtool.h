#pragma once

#include "ui_yuvtool.h"

#include <QtWidgets>
#include <QtConcurrent>
#include <QComboBox>

#define MAX_FORMAT_LENGTH 256
#define PREVIEW_SIZE 100

class YuvTool : public QWidget
{
    Q_OBJECT
public:
    YuvTool(QWidget *parent = 0);
    ~YuvTool();
    public Q_SLOTS:
    void open();
    void getYuvProperty();
    void refreshDisplay();
    void refreshImage();
public slots:
    void gotoPrevFrame();
    void gotoNextFrame();
    void gotoSpecifiedFrame();
    void setScaleFactor(int factor);

private:
    QString yuvFilePath;
    int picWidth;
    int picHeight;
    int dstWidth;
    int dstHeight;
    char yuvFormat[MAX_FORMAT_LENGTH];
    int curFrameIndex;
    int totalFrameNum;

    QPushButton *openButton;
    QComboBox *comboBoxFormat;
    QLabel *labelFormat;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QLabel *labelFrameIdx;
    QLabel *labelScale;
    QLineEdit *editWidth;
    QLineEdit *editHeight;
    QLineEdit *editFrameIdx;
    QSlider *scaleSlider;
    QHBoxLayout *inputLayout;
    QGroupBox *inputGroupBox;
    QPushButton *btnPrevFrame;
    QPushButton *btnNextFrame;
    QLabel *labelCurFrameIndex;

    QGroupBox *imageGroupBox;
    QVBoxLayout *imageLayout;
    QScrollArea *scrollArea;
    QLabel *imageLabel;

    QVBoxLayout *mainLayout;
};
