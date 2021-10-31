#include "transformwidget.h"
#include "ui_transformwidget.h"

transformWidget::transformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transformWidget)
{
    ui->setupUi(this);
}

transformWidget::~transformWidget()
{
    delete ui;
}
