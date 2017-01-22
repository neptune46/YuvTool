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

private:
    int picWidth;
    int picHeight;
    QPushButton *openButton;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QLineEdit *editWidth;
    QLineEdit *editHeight;
    QHBoxLayout *inputLayout;
    QGroupBox *inputGroupBox;

    QLabel *imageLabel;
    QLabel *previewLabel;
    QGroupBox *previewGroupBox;
    QGroupBox *imageGroupBox;
    QGroupBox *displayGroupBox;
    QGridLayout *displayLayout;

    QVBoxLayout *mainLayout;
    QList<QLabel *> labels;
};
