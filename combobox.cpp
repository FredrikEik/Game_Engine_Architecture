#include "combobox.h"

ComboBox::ComboBox()
{
    firstAction = new QAction(this);
    firstAction->setText("Circle");
    secondAction = new QAction(this);
    secondAction->setText("Square");
    thirdAction = new QAction(this);
    thirdAction->setText("Triangle");

    box = new QComboBox(this);
    box->addItem(firstAction->text(), QVariant::fromValue(firstAction)); //add actions
    box->addItem(secondAction->text(), QVariant::fromValue(secondAction));
    box->addItem(thirdAction->text(), QVariant::fromValue(thirdAction));

    connect(box, SIGNAL(currentIndexChanged(const int&)), this, SLOT(boxCurrentIndexChanged(const int&)));
}

void ComboBox::boxCurrentIndexChanged(int index)
{
    QAction * selectedAction = box->itemData(index, Qt::UserRole).value<QAction *>();
    if (selectedAction)
    {
        selectedAction->trigger(); //do stuff with your action
    }
}
