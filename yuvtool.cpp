#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const int imageSize = 800;

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h);

QImage scale(const QString &imageFileName)
{
    int width = 480;
    int height = 360;
    const uchar *data = NULL;
    int bytesPerLine = width *3;
    QImage::Format format = QImage::Format_RGB888;

    // QImage(const uchar *data, int width, int height, int bytesPerLine, Format format);
    data = yuv2rgb(imageFileName.toStdString().c_str(), width, height);

    QImage image(data, width, height, bytesPerLine, format);
    // QImage image(imageFileName);
    return image.scaled(QSize(imageSize, imageSize), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

YuvTool::YuvTool(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    imageScaling = new QFutureWatcher<QImage>(this);
    connect(imageScaling, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
    connect(imageScaling, SIGNAL(finished()), SLOT(finished()));

    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));

    labelWidth = new QLabel(tr("Width:"));
    labelHeight = new QLabel(tr("Height:"));
    editWidth = new QLineEdit("480");
    editHeight = new QLineEdit("360");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(labelWidth);
    buttonLayout->addWidget(editWidth);
    buttonLayout->addWidget(labelHeight);
    buttonLayout->addWidget(editHeight);
    buttonLayout->addStretch();

    imagesLayout = new QGridLayout();

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imagesLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

YuvTool::~YuvTool()
{
    imageScaling->cancel();
    imageScaling->waitForFinished();
}

void YuvTool::open()
{
    // Cancel and wait if we are already loading images.
    if (imageScaling->isRunning()) {
        imageScaling->cancel();
        imageScaling->waitForFinished();
    }

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

    QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(imageSize, imageSize);
    imagesLayout->addWidget(imageLabel, 0, 0);
    labels.append(imageLabel);

    // Use mapped to run the thread safe scale function on the files.
    //imageScaling->setFuture(QtConcurrent::mapped(files, scale));

    int width = 480;
    int height = 360;
    const uchar *data = NULL;
    int bytesPerLine = picWidth * 3;
    QImage::Format format = QImage::Format_RGB888;

    data = yuv2rgb(files[0].toStdString().c_str(), picWidth, picHeight);
    QImage image(data, picWidth, picHeight, bytesPerLine, format);

    labels[0]->setPixmap(QPixmap::fromImage(image));

    openButton->setEnabled(false);
}

void YuvTool::showImage(int num)
{
    labels[num]->setPixmap(QPixmap::fromImage(imageScaling->resultAt(num)));
}

void YuvTool::finished()
{
    openButton->setEnabled(true);
}

