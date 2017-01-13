#include "stdafx.h"
#include "yuvtool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YuvTool w;
    w.show();
    return a.exec();
}
