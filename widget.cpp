#include "widget.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

widget::widget(QWidget  *parent) : QWidget(parent){



        this->resize(400, 240);
        this->setWindowTitle("GUI, hvor du kan legge til objekt ved å trykke på en knapp");
        QPushButton* but1 = new QPushButton("resize object");

        but1->setCheckable(true);


        QPushButton* but2 = new QPushButton("triangel");
        QPushButton* but3 = new QPushButton("square");

        QHBoxLayout *layout = new QHBoxLayout;

        layout->addWidget(but1);
        layout->addWidget(but2);
        layout->addWidget(but3);
        this->setLayout(layout);


}

void widget::on_push_toggled(bool checked)
{

    mRenderWindow->transformObject(checked);

}

