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
    void refreshPreview();
    void refreshImage();

private:
    QString yuvFilePath;
    int picWidth;
    int picHeight;
    char yuvFormat[MAX_FORMAT_LENGTH];

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
    QVBoxLayout *previewOutLayout;
    QScrollArea *previewScrollArea;
    QWidget *previewWidget;
    QHBoxLayout *previewLayout;
    QList<QLabel *> previewLabels;

    QGroupBox *imageGroupBox;
    QVBoxLayout *imageLayout;
    QScrollArea *scrollArea;
    QLabel *imageLabel;

    QVBoxLayout *mainLayout;
};
