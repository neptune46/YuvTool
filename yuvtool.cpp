#include "stdafx.h"
#include "yuvtool.h"

#include <qmath.h>

const int imageSize = 800;

QImage scale(const QString &imageFileName)
{
    QImage image(imageFileName);
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

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
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
        "*.jpg *.png");

    if (files.count() == 0)
        return;

    // Do a simple layout.
    qDeleteAll(labels);
    labels.clear();

    QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(imageSize, imageSize);
    imagesLayout->addWidget(imageLabel, 0, 0);
    labels.append(imageLabel);

    // Use mapped to run the thread safe scale function on the files.
    imageScaling->setFuture(QtConcurrent::mapped(files, scale));

    //QImage image(files[0]);
    //labels[0]->setPixmap(QPixmap::fromImage(image));

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

