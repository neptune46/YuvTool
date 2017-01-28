#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h, char* src_fmt);

YuvTool::YuvTool(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("YuvTool"));
    QSize resolution = QGuiApplication::primaryScreen()->availableSize();
    resize(QGuiApplication::primaryScreen()->availableSize() * 6 / 10);

    mainLayout = new QVBoxLayout();

    // Input area
    inputGroupBox = new QGroupBox(tr("Input Area"));
    inputLayout = new QHBoxLayout();
    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));
    labelFormat = new QLabel(tr("Format"));
    labelWidth = new QLabel(tr("Width:"));
    labelHeight = new QLabel(tr("Height:"));
    comboBoxFormat = new QComboBox;
    comboBoxFormat->addItem(tr("YUV420"));
    comboBoxFormat->addItem(tr("NV12"));
    editWidth = new QLineEdit("480");
    editHeight = new QLineEdit("360");
    inputLayout->addWidget(openButton);
    inputLayout->addWidget(labelFormat);
    inputLayout->addWidget(comboBoxFormat);
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
    scrollArea = new QScrollArea;
    imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignCenter);
    scrollArea->setVisible(true);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageLabel);
    imageLayout->addWidget(scrollArea);
    imageGroupBox->setLayout(imageLayout);
    mainLayout->addWidget(imageGroupBox);

    setLayout(mainLayout);

    connect(comboBoxFormat, SIGNAL(currentIndexChanged(int)), SLOT(refreshImage()));
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

    yuvFilePath = files[0];

    refreshImage();

    scrollArea->setVisible(true);

    openButton->setEnabled(true);
}

void YuvTool::refreshImage()
{
    picWidth = editWidth->text().toInt();
    picHeight = editHeight->text().toInt();
    yuvFormat = comboBoxFormat->itemText(comboBoxFormat->currentIndex());
    QByteArray ba = yuvFormat.toLatin1();
    char *yuvFmt = ba.data();

    const uchar *data = NULL;
    int bytesPerLine = picWidth * 3;
    QImage::Format format = QImage::Format_RGB888;

    data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFmt);
    QImage image(data, picWidth, picHeight, bytesPerLine, format);

    imageLabel->setPixmap(QPixmap::fromImage(image));
}
