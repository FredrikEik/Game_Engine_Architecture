#include "widgettransform.h"
#include "ui_widgettransform.h"

widgettransform::widgettransform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widgettransform)
{
    ui->setupUi(this);
}

widgettransform::~widgettransform()
{
    delete ui;
}

//void widgettransform::widgettransform::on_pushButton_toggled(bool checked)
//{
//    mRenderWindow->transformObject(checked);
//}
