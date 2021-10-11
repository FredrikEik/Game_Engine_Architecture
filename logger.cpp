#include "logger.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit>

Logger::Logger()
{}

Logger *Logger::getInstance()
{
    static Logger *mInstance = new Logger();
    return mInstance;
}

void Logger::setMainWindow(MainWindow *mainWindowIn)
{
    mMainWindow = mainWindowIn;
}

void Logger::logText(std::string input, LColor colorIn)
{
    QColor textColor;

    switch (colorIn) {
    case LColor::LOG :
        textColor = Qt::darkGray;
        break;
    case LColor::HIGHLIGHT :
        textColor = Qt::darkGreen;
        break;
    case LColor::WARNING :
        textColor = Qt::darkYellow;
        break;
    case LColor::DAMNERROR :
        textColor = Qt::red;
        break;
    default:
        textColor = Qt::darkGray;
    }

    QTextCharFormat textformat;
    textformat = mMainWindow->ui->outputLog->currentCharFormat();
    textformat.setForeground(QBrush((textColor)));
    mMainWindow->ui->outputLog->setCurrentCharFormat(textformat);
    mMainWindow->ui->outputLog->appendPlainText(input.c_str());
}
