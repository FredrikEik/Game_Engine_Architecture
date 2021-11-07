#ifndef LOGGER_H
#define LOGGER_H

//#include <string>

enum class LColor
{
    LOG = 0,
    HIGHLIGHT,
    WARNING,
    DAMNERROR       //For some reason "ERROR" is taken, even within this class...
};

class Logger
{
public:
    static Logger* getInstance();

    void setMainWindow(class MainWindow *mainWindowIn);

    void logText(std::string input, LColor colorIn = LColor::LOG);

private:
    Logger();
    void operator=(Logger&){};  //Assignment operator
    Logger(const Logger&){};    //Copy constructor

    class MainWindow* mMainWindow{nullptr};
};

#endif // LOGGER_H
