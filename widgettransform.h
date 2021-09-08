#ifndef WIDGETTRANSFORM_H
#define WIDGETTRANSFORM_H
#include "renderwindow.h"
#include <QWidget>

namespace Ui {
class widgettransform;
}

class widgettransform : public QWidget
{
    Q_OBJECT

public:
    explicit widgettransform(QWidget *parent = nullptr);
    ~widgettransform();

    RenderWindow *mRenderWindow;


private slots:

    //void widgettransform::on_pushButton_toggled();
private:
    Ui::widgettransform *ui;
};

#endif // WIDGETTRANSFORM_H
