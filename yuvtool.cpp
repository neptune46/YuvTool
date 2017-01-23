#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h);

YuvTool::YuvTool(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Image loading and scaling example"));
    QSize resolution = QGuiApplication::primaryScreen()->availableSize();
    resize(QGuiApplication::primaryScreen()->availableSize() * 8 / 10);

    mainLayout = new QVBoxLayout();

    // Input area
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
    inputGroupBox->setLayout(inputLayout);
    mainLayout->addWidget(inputGroupBox);

    // preview
    previewGroupBox = new QGroupBox(tr("Preview"));
    previewLayout = new QVBoxLayout();
    previewLabel = new QLabel;
    previewLabel->setFixedSize(100, 100);
    previewLayout->addWidget(previewLabel);
    previewGroupBox->setLayout(previewLayout);
    mainLayout->addWidget(previewGroupBox);

    // image
    imageGroupBox = new QGroupBox(tr("Image"));
    imageLayout = new QVBoxLayout();
    imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLayout->addWidget(imageLabel);
    imageLayout->addStretch();
    imageGroupBox->setLayout(imageLayout);
    mainLayout->addWidget(imageGroupBox);

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

    const uchar *data = NULL;
    int bytesPerLine = picWidth * 3;
    QImage::Format format = QImage::Format_RGB888;

    data = yuv2rgb(files[0].toStdString().c_str(), picWidth, picHeight);
    QImage image(data, picWidth, picHeight, bytesPerLine, format);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    openButton->setEnabled(true);
}

