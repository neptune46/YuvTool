#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_yuvtool.h"

class YuvTool : public QMainWindow
{
    Q_OBJECT

public:
    YuvTool(QWidget *parent = Q_NULLPTR);

private:
    Ui::YuvToolClass ui;
};
