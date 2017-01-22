#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const int imageSize = 400;

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h);

YuvTool::YuvTool(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    mainLayout = new QVBoxLayout();

    inputGroupBox = new QGroupBox(tr("Input Area"));
    inputLayout = new QHBoxLayout();
    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));
    labelWidth = new QLabel(tr("Width:"));
    labelHeight = new QLabel(tr("Height:"));
    editWidth = new QLineEdit("480");
    editHeight = new QLineEdit("360");
    inputLayout->addWidget(openButton);
    inputLayout->addWidget(labelWidth);
    inputLayout->addWidget(editWidth);
    inputLayout->addWidget(labelHeight);
    inputLayout->addWidget(editHeight);
    inputLayout->addStretch();
    inputGroupBox->setLayout(inputLayout);
    mainLayout->addWidget(inputGroupBox);

    previewLabel = new QLabel;
    imageLabel = new QLabel;
    previewLabel->setFixedSize(100, 100);
    imageLabel->setFixedSize(400, 400);
    previewGroupBox = new QGroupBox(tr("Preview"));
    imageGroupBox = new QGroupBox(tr("Image"));
    previewLayout = new QGridLayout();
    imageLayout = new QGridLayout();
    previewLayout->addWidget(previewLabel);
    imageLayout->addWidget(imageLabel);
    previewGroupBox->setLayout(previewLayout);
    imageGroupBox->setLayout(imageLayout);
    mainLayout->addWidget(previewGroupBox);
    mainLayout->addWidget(imageGroupBox);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

YuvTool::~YuvTool()
{
}

void YuvTool::open()
{
    // Show a file open dialog at QStandardPaths::PicturesLocation.
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "*.jpg *.png *.yuv");

    if (files.count() == 0)
        return;

    picWidth = editWidth->text().toInt();
    picHeight = editHeight->text().toInt();

    // Do a simple layout.
    qDeleteAll(labels);
    labels.clear();

    /*QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(imageSize, imageSize);
    imagesLayout->addWidget(imageLabel, 0, 0);
    labels.append(imageLabel);*/

    const uchar *data = NULL;
    int bytesPerLine = picWidth * 3;
    QImage::Format format = QImage::Format_RGB888;

    data = yuv2rgb(files[0].toStdString().c_str(), picWidth, picHeight);
    QImage image(data, picWidth, picHeight, bytesPerLine, format);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    openButton->setEnabled(true);
}

