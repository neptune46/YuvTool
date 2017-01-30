#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h, char* src_fmt, int dst_w, int dst_h, int idx_frame);

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
    previewLayout = new QHBoxLayout();
    QLabel *label = new QLabel;
    label->setFixedSize(PREVIEW_SIZE, PREVIEW_SIZE);
    previewLayout->addWidget(label);
    previewLabels.append(label);
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

    connect(openButton, SIGNAL(clicked()), SLOT(open()));
    connect(comboBoxFormat, SIGNAL(currentIndexChanged(int)), SLOT(refreshDisplay()));
    connect(editWidth, SIGNAL(returnPressed()), SLOT(refreshDisplay()));
    connect(editHeight, SIGNAL(returnPressed()), SLOT(refreshDisplay()));
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

    refreshDisplay();

    scrollArea->setVisible(true);

    openButton->setEnabled(true);
}

void YuvTool::getYuvProperty()
{
    picWidth = editWidth->text().toInt();
    picHeight = editHeight->text().toInt();
    QString cbFormat = comboBoxFormat->itemText(comboBoxFormat->currentIndex());
    QByteArray ba = cbFormat.toLatin1();
    int length = strlen(ba.data());
    memset(yuvFormat, 0, MAX_FORMAT_LENGTH);
    memcpy_s(yuvFormat, length, ba.data(), length);
}

void YuvTool::refreshDisplay()
{
    getYuvProperty();
    refreshImage();
    refreshPreview();
}

void YuvTool::refreshImage()
{
    const uchar *data = NULL;
    int bytesPerLine = picWidth * 3;
    QImage::Format format = QImage::Format_RGB888;

    data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFormat, picWidth, picHeight, 0);
    QImage image(data, picWidth, picHeight, bytesPerLine, format);

    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void YuvTool::refreshPreview()
{
    const uchar *data = NULL;
    QImage::Format format = QImage::Format_RGB888;
    int indexFrame = 0;

    int previewWidth, previewHeight;
    if (picWidth > picHeight)
    {
        previewWidth = PREVIEW_SIZE;
        previewHeight = (picHeight * previewWidth) / picWidth;
    } 
    else
    {
        previewHeight = PREVIEW_SIZE;
        previewWidth = (picWidth * previewHeight) / picHeight;
    }
    int bytesPerLine = previewWidth * 3;

    data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFormat, previewWidth, previewHeight, indexFrame++);
    QImage image(data, previewWidth, previewHeight, bytesPerLine, format);
    previewLabels[0]->setPixmap(QPixmap::fromImage(image));

    while ((data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFormat, previewWidth, previewHeight, indexFrame)) != NULL)
    {
        QImage image2(data, previewWidth, previewHeight, bytesPerLine, format);
        QLabel *label = new QLabel;
        label->setFixedSize(PREVIEW_SIZE, PREVIEW_SIZE);
        previewLayout->addWidget(label);
        previewLabels.append(label);
        previewLabels[indexFrame]->setPixmap(QPixmap::fromImage(image2));
        indexFrame++;
    }
}

