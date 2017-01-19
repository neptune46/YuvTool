#pragma once

#include "ui_yuvtool.h"

#include <QtWidgets>
#include <QtConcurrent>

class YuvTool : public QWidget
{
    Q_OBJECT
public:
    YuvTool(QWidget *parent = 0);
    ~YuvTool();
    public Q_SLOTS:
    void open();
    void showImage(int num);
    void finished();
private:
    int picWidth;
    int picHeight;
    QPushButton *openButton;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QLineEdit *editWidth;
    QLineEdit *editHeight;
    QVBoxLayout *mainLayout;
    QList<QLabel *> labels;
    QGridLayout *imagesLayout;
    QFutureWatcher<QImage> *imageScaling;
};
