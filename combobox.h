#ifndef COMBOBOX_H
#define COMBOBOX_H
#include <QComboBox>
#include <QAction>
#include "mainwindow.h"

class ComboBox : public MainWindow
{
    Q_OBJECT

public:
    QAction * firstAction;
    QAction * secondAction;
    QAction * thirdAction;
    QComboBox *box;
    ComboBox();
    MainWindow* mWindow;

protected slots:
    void boxCurrentIndexChanged(int index);
};

#endif // COMBOBOX_H
