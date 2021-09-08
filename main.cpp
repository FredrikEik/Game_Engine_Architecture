#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QtWidgets>

class MyWidget : public QWidget{
public:
    MyWidget(QWidget *parent =NULL) : QWidget(parent){
        this->resize(400, 240);
        this->setWindowTitle("GUI, hvor du kan legge til objekt ved å trykke på en knapp");
        QPushButton* but1 = new QPushButton("circle");
        QPushButton* but2 = new QPushButton("triangel");
        QPushButton* but3 = new QPushButton("square");

        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(but1);
        layout->addWidget(but2);
        layout->addWidget(but3);
        this->setLayout(layout);
    }
};



int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);

    MyWidget widget;
    widget.show();
    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    return a.exec();
}
