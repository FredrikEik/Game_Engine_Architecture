#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include "renderwindow.h"

class widget : public QWidget
{
public:
    explicit widget(QWidget  *parent = nullptr);

private slots:
 //void on_push_toggled(bool checked);

private:
RenderWindow *mRenderWindow;
widget *ui;


};

#endif // WIDGET_H
