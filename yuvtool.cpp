#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h, char* src_fmt, int dst_w, int dst_h, int frame_idx, int* total_frame);

YuvTool::YuvTool(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("YuvTool"));
    QSize resolution = QGuiApplication::primaryScreen()->availableSize();
    resize(QGuiApplication::primaryScreen()->availableSize() * 6 / 10);

    curFrameIndex = 1;
    totalFrameNum = 1;

    mainLayout = new QVBoxLayout();

    // Input area
    inputGroupBox = new QGroupBox(tr("Input"));
    inputLayout = new QHBoxLayout();
    openButton = new QPushButton(tr("Open YUV"));
    labelFormat = new QLabel(tr("Format"));
    comboBoxFormat = new QComboBox;
    comboBoxFormat->addItem(tr("YUV420"));
    comboBoxFormat->addItem(tr("NV12"));
    labelWidth = new QLabel(tr("Width:"));
    labelHeight = new QLabel(tr("Height:"));
    labelFrameIdx = new QLabel(tr("Frame#:"));
    editWidth = new QLineEdit("480");
    editHeight = new QLineEdit("360");
    editFrameIdx = new QLineEdit("1");
    btnPrevFrame = new QPushButton(tr("PrevFrame"));
    btnNextFrame = new QPushButton(tr("NextFrame"));
    labelCurFrameIndex = new QLabel(tr("[0/0]"));
    labelScale = new QLabel(tr("Scale(100%)"));
    scaleSlider = new QSlider(Qt::Horizontal);
    scaleSlider->setFocusPolicy(Qt::StrongFocus);
    //scaleSlider->setTickPosition(QSlider::TicksBothSides);
    scaleSlider->setMinimum(1);
    scaleSlider->setMaximum(200);
    scaleSlider->setSingleStep(10);
    scaleSlider->setValue(100);
    inputLayout->addWidget(openButton);
    inputLayout->addWidget(labelFormat);
    inputLayout->addWidget(comboBoxFormat);
    inputLayout->addWidget(labelWidth);
    inputLayout->addWidget(editWidth);
    inputLayout->addWidget(labelHeight);
    inputLayout->addWidget(editHeight);
    inputLayout->addWidget(labelFrameIdx);
    inputLayout->addWidget(editFrameIdx);
    inputLayout->addWidget(btnPrevFrame);
    inputLayout->addWidget(labelCurFrameIndex);
    inputLayout->addWidget(btnNextFrame);
    inputLayout->addWidget(labelScale);
    inputLayout->addWidget(scaleSlider);
    inputGroupBox->setLayout(inputLayout);
    mainLayout->addWidget(inputGroupBox);

    // preview
    //QLabel *label = new QLabel;
    //label->setFixedSize(PREVIEW_SIZE, PREVIEW_SIZE);
    //previewLabels.append(label);
    //previewLayout = new QHBoxLayout();
    //previewLayout->addWidget(label);
    //previewWidget = new QWidget;
    //previewWidget->setLayout(previewLayout);
    //previewScrollArea = new QScrollArea;
    //previewScrollArea->setWidget(previewWidget);
    //previewScrollArea->setVisible(true);
    //previewScrollArea->setWidgetResizable(true);
    //previewOutLayout = new QVBoxLayout;
    //previewOutLayout->addWidget(previewScrollArea);
    //previewGroupBox = new QGroupBox(tr("Preview"));
    //previewGroupBox->setLayout(previewOutLayout);
    //mainLayout->addWidget(previewGroupBox);

    // image
    imageGroupBox = new QGroupBox(tr("Image"));
    imageLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);
    imageLabel = new QLabel(this);
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
    connect(editFrameIdx, SIGNAL(returnPressed()), SLOT(gotoSpecifiedFrame()));
    connect(btnPrevFrame, SIGNAL(clicked()), SLOT(gotoPrevFrame()));
    connect(btnNextFrame, SIGNAL(clicked()), SLOT(gotoNextFrame()));
    connect(scaleSlider, SIGNAL(valueChanged(int)), SLOT(setScaleFactor(int)));
}

void YuvTool::gotoPrevFrame()
{
    if (curFrameIndex > 1)
    {
        curFrameIndex--;
        refreshImage();
        editFrameIdx->setText(QString::number(curFrameIndex));
        btnNextFrame->setEnabled(true);
    }
    else
    {
        btnPrevFrame->setEnabled(false);
    }
}

void YuvTool::gotoNextFrame()
{
    if (curFrameIndex < totalFrameNum)
    {
        curFrameIndex++;
        refreshImage();
        editFrameIdx->setText(QString::number(curFrameIndex));
        btnPrevFrame->setEnabled(true);
    }
    else
    {
        btnNextFrame->setEnabled(false);
    }
}

void YuvTool::gotoSpecifiedFrame()
{
    getYuvProperty();
    if (curFrameIndex >=1 && curFrameIndex <= totalFrameNum)
    {
        refreshImage();
    } 
}

void YuvTool::setScaleFactor(int factor)
{
    QString text = "Scale[";
    text += QString::number(factor);
    text += "%]";
    labelScale->setText(text);
    refreshDisplay();
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
    

    getYuvProperty();

    // always display the first frame for new image
    curFrameIndex = 1;

    refreshImage();
}

void YuvTool::getYuvProperty()
{
    picWidth = editWidth->text().toInt();
    picHeight = editHeight->text().toInt();
    int factor = scaleSlider->value();
    dstWidth = factor * picWidth / 100;
    dstHeight = factor * picHeight / 100;
    curFrameIndex = editFrameIdx->text().toInt();
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
    //refreshPreview();
}

void YuvTool::refreshImage()
{
    const uchar *data = NULL;
    int bytesPerLine = dstWidth * 3;
    QImage::Format format = QImage::Format_RGB888;

    data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFormat, 
        dstWidth, dstHeight, (curFrameIndex - 1), &totalFrameNum);

    QImage image(data, dstWidth, dstHeight, bytesPerLine, format);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    QString curFrameText = "[" + QString::number(curFrameIndex);
    curFrameText += "/";
    curFrameText += QString::number(totalFrameNum) + "]";
    labelCurFrameIndex->setText(curFrameText);
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


    data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFormat, previewWidth, previewHeight, indexFrame++, &totalFrameNum);
    QImage image(data, previewWidth, previewHeight, bytesPerLine, format);
    previewLabels[0]->setPixmap(QPixmap::fromImage(image));

    while ((data = yuv2rgb(yuvFilePath.toStdString().c_str(), picWidth, picHeight, yuvFormat, previewWidth, previewHeight, indexFrame, &totalFrameNum)) != NULL)
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

