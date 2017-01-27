#pragma once

#include "ui_yuvtool.h"

#include <QtWidgets>
#include <QtConcurrent>
#include <QComboBox>

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
    QLabel *labelFormat;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QComboBox *comboBoxFormat;
    QLineEdit *editWidth;
    QLineEdit *editHeight;
    QHBoxLayout *inputLayout;
    QGroupBox *inputGroupBox;

    QGroupBox *previewGroupBox;
    QVBoxLayout *previewLayout;
    QLabel *previewLabel;

    QGroupBox *imageGroupBox;
    QVBoxLayout *imageLayout;
    QScrollArea *scrollArea;
    QLabel *imageLabel;

    QVBoxLayout *mainLayout;
};
