#include "Log.h"
#include "MainThreadProgramm.h"

//log_init();

/**
This is test function to test whether the logging is done correctly or not..
*/
/*
static Logger* getLoggerFromManager(const string& category)
{
    Logger* logger = 0;
    if (!LoggerManager::isEnabled())
      return 0;
    if (logger == 0) {
      LoggerManager::logInit();
      logger =LoggerManager::getInstance().getLogger(category);
    }
    return logger;
}

void test_log(const string& expr,const string& category)
{
    do{
        Logger* _logger = getLoggerFromManager(category);

        if (_logger!=0)// && _logger->isEnabled(Logger::LOG_LEVEL_DEBUG))
        {
            LogMessage _logMessage(_logger,Logger::LOG_LEVEL_DEBUG);
            _logMessage.out()<<expr;
            _logMessage.finish();
        }
     }while(false);
}

int main()
{
    int a=5;
    cout << "Hello world!"<<endl;

    test_log(string("this is the debuging info..1"),string("main.cpp\\logging\\thread1"));
    test_log(string("this is the debuging info..2"),string("main.cpp\\logging\\thread2"));
    test_log(string("this is the debuging info..3"),string("main.cpp\\logging\\thread3"));
    test_log(string("this is the debuging info..4"),string("main.cpp\\logging\\thread4"));
    test_log(string("this is the debuging info..5"),string("main.cpp\\logging\\thread5"));
    return 0;
}

*/

//log_define("main.cpp\logging");

int main()
{
    cout<<"simple Logger program";
    //log_debug("this is debuging info..");
    driver();
    return 0;
}
