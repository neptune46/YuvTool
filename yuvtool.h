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

    QGroupBox *previewGroupBox;
    QVBoxLayout *previewLayout;
    QLabel *previewLabel;

    QGroupBox *imageGroupBox;
    QVBoxLayout *imageLayout;
    QLabel *imageLabel;

    QVBoxLayout *mainLayout;
};
