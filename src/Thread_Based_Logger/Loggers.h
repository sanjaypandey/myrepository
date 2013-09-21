#ifndef LOGGER_LOGGERS_GCC_X86_64_H
#define LOGGER_LOGGERS_GCC_X86_64_H

#include "Constants.h"

//namespace LoggerInfo
//{
  class Logger
  {
     public:
      typedef enum {
        LOG_LEVEL_FATAL = 0,
        LOG_LEVEL_ERROR = 100,
        LOG_LEVEL_WARN  = 200,
        LOG_LEVEL_INFO  = 300,
        LOG_LEVEL_DEBUG = 400,
        LOG_LEVEL_TRACE = 500
      } log_level_type;

      private:
       string category;
       log_level_type level;

      public:
      Logger(const std::string& c, log_level_type l)
        : category(c), level(l)
      { }
      ~Logger()
      {
          printf("\ndestructor of logger is called..\n");
      }
      bool isEnabled(log_level_type l) const
        { return level <= l; }
      const string& getCategory()
        { return category; }
      log_level_type getLogLevel() const
        { return level; }
  };

 /**
 This is the Logger Manager Coniguration class.
 */
   class LoggerManagerConfiguration
  {
    public:
      class Impl;
    private:
      friend class Impl;
      Impl* _impl;

    public:
      LoggerManagerConfiguration();
      LoggerManagerConfiguration(const LoggerManagerConfiguration&);
      LoggerManagerConfiguration& operator=(const LoggerManagerConfiguration&);
      ~LoggerManagerConfiguration();
      Impl* impl()             { return _impl; }
      const Impl* impl() const { return _impl; }

      Logger::log_level_type rootLevel() const;
      Logger::log_level_type logLevel(const std::string& category) const;
  };

/**
 LoggerManager class , this class is responsible for dealing with the all the Loggers Information
 and configure the LoggerManagerConfiguration class , so that we could write the log
 in any particular log file.
*/
   class LoggerManager
  {
    public:
      class Impl;
    private:
      friend class Impl;
      Impl* _impl;

    public:
      LoggerManager();
      static bool _enabled;
      LoggerManager(const LoggerManager&);
      LoggerManager& operator=(const LoggerManager&);
    public:
      ~LoggerManager();

      Impl* impl()              { return _impl; }
      const Impl* impl() const  { return _impl; }

      static LoggerManager& getInstance();
      static void logInit();
      static void logInit(const std::string& fname);
      void configure(const LoggerManagerConfiguration& config);
      Logger* getLogger(const std::string& category);
      static bool isEnabled()
      {
          return _enabled;
      }

      Logger::log_level_type rootLevel() const;
      Logger::log_level_type logLevel(const std::string& category) const;
  };

/**
This class eventually writes the Loggers information in the Log file.
It gets the LogWriting objects from the LoggersPool and write the particular Logger
Information in the log file, by ensuring the synchronization.
*/
   class LogMessage
  {
    public:
      class Impl;

    private:
      friend class Impl;
      Impl* _impl;

      LogMessage(const LogMessage&);
      LogMessage& operator=(const LogMessage&);

    public:
      LogMessage(Logger* logger, const char* level);
      LogMessage(Logger* logger, Logger::log_level_type level);
      ~LogMessage();
      Impl* impl()             { return _impl; }
      const Impl* impl() const { return _impl; }
      std::ostream& out();
      std::string str() const;
      void finish();
  };

//}

//using namespace LoggerInfo;



#define log_enabled(level)\
        (getLogger() != 0 && getLogger()->isEnabled(Logger::LOG_LEVEL_##level))

#define log(level,expr)\
      do{\
        Logger* _logger = getLogger();\
        if (_logger != 0 && _logger->isEnabled(Logger::LOG_LEVEL_ ## level)) \
        { \
            LogMessage _logMessage(_logger,Logger::LOG_LEVEL_##level); \
            _logMessage.out() << expr; \
            _logMessage.finish(); \
        } }while(false)

#define log_fatal_enabled()     log_enabled(FATAL)
#define log_error_enabled()     log_enabled(ERROR)
#define log_warn_enabled()      log_enabled(WARN)
#define log_info_enabled()      log_enabled(INFO)
#define log_debug_enabled()     log_enabled(DEBUG)
#define log_trace_enabled()     log_enabled(TRACE)

#define log_fatal(expr)     log(FATAL, expr)
#define log_error(expr)     log(ERROR, expr)
#define log_warn(expr)      log(WARN, expr)
#define log_info(expr)      log(INFO, expr)
#define log_debug(expr)     log(DEBUG, expr)

#define log_define(category) \
  static Logger* getLogger()   \
  {  \
    Logger* logger = 0; \
    if (!LoggerManager::isEnabled()) \
      return 0; \
    if (logger == 0) {\
      LoggerManager::logInit();\
      logger =LoggerManager::getInstance().getLogger(category); \
    }\
    return logger; \
  }

//#define log_init()  LoggerManager::logInit()

#endif
