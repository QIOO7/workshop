#include "workshopwidget.h"
#include "ui_workshopwidget.h"

workshopWidget::workshopWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::workshopWidget)
{
    ui->setupUi(this);

    InitFunc();
}

workshopWidget::~workshopWidget()
{
    delete ui;
}

void workshopWidget::InitFunc()
{
    ;
}

