#include "widgetui.h"
#include "ui_widgetui.h"

WidgetUI::WidgetUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetUI)
{
    ui->setupUi(this);
}

WidgetUI::~WidgetUI()
{
    delete ui;
}
