#ifndef LOGGER_LOG_GCC_X86_64_H
#define LOGGER_LOG_GCC_X86_64_H

#include "Constants.h"
#include "Mutex.h"
#include "AtomicIncrement.h"
#include "Loggers.h"

//namespace LogImpl{

    atomic_t mutexWaitCount=0;
    Mutex loggersMutex;
    Mutex logMutex;
    Mutex poolMutex;

/**
This method is used to get the Log entry information for the log file that is to be
logged in the Logger file.
*/
   void logentry(std::string& entry, const char* level, const std::string& category)
    {
         entry.append("\n\n#################################\n");
         entry.append("\nFile : ");
         entry.append(__FILE__);
         entry.append("\nFunction : ");
         entry.append(__FUNCTION__);
         entry.append("\nLine No : ");
         int lineNo=__LINE__;
         ostringstream convert;
         convert<<lineNo;
         entry.append(convert.str());
         entry.append("\nDate : ");
         entry.append(__DATE__);
         entry.append("\nTime : ");
         entry.append(__TIME__);
         entry.append("\ncategory : ");
         entry.append(category);
         entry.append("\nprocessId : ");
         int processId=getpid();
         convert<<processId;
         entry.append(convert.str());
         entry.append("\nLevel : ");
         entry.append(string(level));
         entry.append("\nInfo :  ");

       /*  long threadId=pthread_self();
         convert<<threadId;
         entry.append("\nThreadId:");
         entry.append(convert.str());
        */
    }

/**
This class increments the value of mutexWaitCount Atomically.
*/
class ScopedAtomicIncrement
{
      private:
        atomic_t& count;
      public:
        explicit ScopedAtomicIncrement(atomic_t &count_)
        :count(count_)
        {
            atomicIncrement(count);
        };
        ~ScopedAtomicIncrement()
        {
            atomicDecrement(count);
        }
};

/**
This class provides the pool objects from the pool , while ensuring the synchronization
issues.
*/
template <typename T, unsigned MaxPoolSize=8>
class LoggerPool
{
      std::vector< T* > objsPool;
      Mutex mutex;
      public:

       ~LoggerPool()
       {
          for(int i=0;i<objsPool.size();i++)
           {
              if(objsPool[i]!=NULL)
                delete objsPool[i];
            }
        }

      T* getInstance()
      {
        /**
        We could return a Pool object without worrying about the synchronization problem.
        because there is no object present in the pool.
        */
        if(objsPool.empty())
        {
           return new T();
        }

        /**
        Getting the lock on the Logger Pool before fetching any object from the LoggerPool.
        */
        T* poolObj;
        {
          MutexLock lockLoggerPool(poolMutex);
          if (objsPool.empty()){
               return new T();
          }
          poolObj=objsPool.back();
          objsPool.pop_back();
        }
         return poolObj;
    }

      void releaseInstance(T* inst)
      {
         MutexLock lockLoggerPool(poolMutex);
         if(objsPool.size()< MaxPoolSize){
            objsPool.push_back(inst);
         }
         else
         {
            if(inst!=NULL)
                delete inst;
         }

      }
};

    /**
    Pure Virtual class used to put and flush the message in the file.
    */
    class LogAppender
    {
      public:
        virtual ~LogAppender() { }
        virtual void putMessage(const string& msg) = 0;
        virtual void finish(bool flush) = 0;
    };

    //////////////////////////////////////////////////////////////////////
    // FdAppender - writes log to a file descriptor
    //
    class FdAppender : public LogAppender
    {
      protected:
        int _fd;
        string _msg;

      public:
        explicit FdAppender(int fd)
          : _fd(fd)
        { }

        virtual void putMessage(const string& msg);
        virtual void finish(bool flush);
    };

    void FdAppender::putMessage(const string& msg)
    {
          _msg+=msg;
          _msg+="\n";
    }

    void FdAppender::finish(bool flush)
    {
      if(!flush&&_msg.size()<8192)
        return ;
      int writtenBytes=write(_fd,_msg.data(),_msg.size());
      if(writtenBytes!=_msg.size())
        cout<<"\nsome problem in writing the contents in the files.\n";
      _msg.clear();
    }

    /**
    Getting the file specific information with help of this class.
    */
    class FileAppender : public FdAppender
    {
        string _fname;
        public:
        explicit FileAppender(const string& fname);
        virtual void putMessage(const string& msg);
        const std::string& fname() const  {
            return _fname;
        }
        void closeFile();
        void openFile();
    };

    FileAppender::FileAppender(const string& fname)
    :FdAppender(-1)
    {
        _fname=fname;
    }

    void FileAppender::putMessage(const string& msg)
    {
        if (_fd==-1){
           openFile();
        }
        FdAppender::putMessage(msg);
    }

    void FileAppender::openFile(){
        _fd=open( _fname.c_str(),O_WRONLY|O_APPEND|O_CREAT,0666);
    }
    void FileAppender::closeFile()
    {
        close(_fd);
    }

   /**
   LoggerManagerConfiguration Impl class.
   */
   class LoggerManagerConfiguration::Impl
  {
    public:
      typedef map<string, Logger::log_level_type> LogLevels;

    private:
            Logger::log_level_type _rootLevel;
            LogLevels _logLevels;
            string _fname;
            unsigned _maxfilesize;

    public:
      Impl()
        : _maxfilesize(0),_rootLevel(Logger::LOG_LEVEL_FATAL)
    {

    }
      ~Impl()
      {
      }

      const string& fname() const          { return _fname; }
      void setFileName(const string& fname)   {
           _fname=fname;
      }
      unsigned maxfilesize() const              { return _maxfilesize; }

      Logger::log_level_type rootLevel() const  { return _rootLevel; }
      Logger::log_level_type logLevel(const std::string& category) const;
      const LogLevels& logLevels() const        { return _logLevels; }

  };


  //############################################################################
  // Implementaion of methods of LoggerManagerConfiguration.
  LoggerManagerConfiguration::LoggerManagerConfiguration()
  : _impl(new LoggerManagerConfiguration::Impl())
  {
  }

  LoggerManagerConfiguration::LoggerManagerConfiguration(const LoggerManagerConfiguration& c)
    : _impl(new Impl(*c._impl))
  {

  }

  LoggerManagerConfiguration& LoggerManagerConfiguration
  ::operator=(const LoggerManagerConfiguration& c)
  {
    delete _impl;
    _impl = 0;
    _impl = new Impl(*c._impl);
    return *this;
  }

  LoggerManagerConfiguration::~LoggerManagerConfiguration()
  {
      delete _impl;
  }

  Logger::log_level_type LoggerManagerConfiguration::rootLevel() const
  {
      return _impl->rootLevel();
  }

  //END
  //############################################################################


    /**
    Logger Manager Impl class implemetation class.
    */

   class LoggerManager::Impl
  {
      LogAppender  *_appender;
      LoggerManagerConfiguration _config;
      typedef map <string, Logger*>  LoggersMap;
      LoggersMap _loggers;

      Impl(const Impl&);
      Impl& operator=(const Impl&);

    public:
      explicit Impl(const LoggerManagerConfiguration& config);
      ~Impl();

      Logger* getLogger(const std::string& category);

      LogAppender* appender()
      {
           return _appender;
      }

      Logger::log_level_type rootLevel() const
      {
           return _config.rootLevel();
      }

      // Need to look over this method for for getting the logLevel
      //from LoggerManagerConfiguration class..
      Logger::log_level_type logLevel(const std::string& category) const
      {
           return Logger::LOG_LEVEL_FATAL;
      }
  };

    // ##########################################################################
    //implementation of methods of LoggerManager.

    //Static instances of the LoggerManager class.
    bool LoggerManager::_enabled = true;

    LoggerManager& LoggerManager::getInstance()
    {
       static LoggerManager loggerManager;
       return loggerManager;
    }

    void LoggerManager::configure(const LoggerManagerConfiguration& config)
    {
        if(_impl!=NULL)
        {
           delete  impl();
        }
        _impl= new LoggerManager::Impl(config);
        _enabled=true;
    }

    LoggerManager::~LoggerManager()
    {
        MutexLock lock(logMutex);
        if(_impl!=NULL)
          delete _impl;
        _impl=NULL;
        _enabled=false;
    }

    Logger::log_level_type LoggerManager::rootLevel() const
    {
        return _impl->rootLevel();
    }

    Logger::log_level_type LoggerManager::logLevel(const std::string& category) const
    {
        return _impl->logLevel(category);
    }

    Logger* LoggerManager::getLogger(const std::string& category)
    {
        if (_impl == 0)
            return 0;
        return _impl->getLogger(category);
  }

    LoggerManager::LoggerManager()
    {

    }

   /**
     Configuring the LoggerManager by providing the Log File name , that would be used
     for logging the file information..
   */
    void LoggerManager::logInit()
    {
        static bool logInitFlag=false;
        if(!logInitFlag){
        string fname("myapp.log");
        logInit(fname);
        logInitFlag=true;
       }
    }

    void LoggerManager::logInit(const string& fname)
    {
        ifstream in(fname.c_str());
        LoggerManagerConfiguration config;
        config.impl()->setFileName(fname);
        getInstance().configure(config);
    }

    //End.
    //############################################################################



    //############################################################################
    //implementation of Impl class methods of LoggerManager inner class.
    Logger* LoggerManager::Impl::getLogger(const std::string& category)
    {
         MutexLock lock(loggersMutex);
         if(_loggers.find(category)!=_loggers.end())
         {
            return _loggers[category];
         }
         Logger *logger=new Logger(category,logLevel(category));
         _loggers[category]=logger;
         return logger;
    }

    LoggerManager::Impl::Impl(const LoggerManagerConfiguration& config)
    {
         _appender = new FileAppender(config.impl()->fname());
         _config=config;
    }

    LoggerManager::Impl::~Impl(){
        for (LoggersMap::iterator it = _loggers.begin(); it != _loggers.end(); ++it)
            delete it->second;
    }


    //End.
    //############################################################################

    /**
    Impl Inner class of LogMessage.
    */
    class LogMessage::Impl{

      public:
        Logger *_logger;
        const char* _level;
        ostringstream _msg;
      public:

        Impl()
        {
           //cout<<"constructor of LogMessage Impl is called"<<endl;
        }

        ~Impl()
        {
           //printf("\n inside the destructor of Impl LogMessage class.\n");
        }

        void setLogger(Logger* logger)
        {
            _logger = logger;
        }

        Logger* getLogger()
        {
            return _logger;
        }

        void setLevel(const char* level)
        {
            _level = level;
        }

        void finish();

        ostringstream& out()
        {
            return _msg;
        }

        string str()
        {
            return _msg.str();
        }

        void clear()
        {
            _msg.clear();
            _msg.str(string());
        }
    };

    //############################################################################
    //Intializing the LoggerPool object.
    namespace
    {
        LoggerPool<LogMessage::Impl> logMessageImplPool;
    }


    //END
    //############################################################################



    //############################################################################
    //Implenetation of the LogMessage Methods.
      LogMessage::LogMessage(Logger* logger, const char* level)
      {
          LogMessage::Impl *_impl=logMessageImplPool.getInstance();
          _impl->setLogger(logger);
          _impl->setLevel(level);

      }

      LogMessage::LogMessage(Logger* logger, Logger::log_level_type level)
      :_impl(logMessageImplPool.getInstance())
      {
         if(_impl!=0){
         _impl->setLogger(logger);
         _impl->setLevel(level >= Logger::LOG_LEVEL_TRACE ? "TRACE"
                  : level >= Logger::LOG_LEVEL_DEBUG ? "DEBUG"
                  : level >= Logger::LOG_LEVEL_INFO  ? "INFO"
                  : level >= Logger::LOG_LEVEL_WARN  ? "WARN"
                  : level >= Logger::LOG_LEVEL_ERROR ? "ERROR"
                  : "FATAL");
         }
         else
         {
              printf("\n nil object is returned..\n");
          }

      }

     void LogMessage::finish()
    {
        _impl->finish();
        logMessageImplPool.releaseInstance(_impl);
        _impl = 0;
    }

      LogMessage::~LogMessage()
      {
         if(_impl!=NULL)
         {
             _impl->finish();
             logMessageImplPool.releaseInstance(_impl);
             _impl=0;
         }
      }

        ostream& LogMessage::out()
        {
            return _impl->out();
        }

        string LogMessage::str() const
        {
            return _impl->str();
        }

    //END
    //############################################################################




    //############################################################################
    //Implementation of the methods of LogMessege Inner class of Impl

     void LogMessage::Impl::finish()
     {
        try
        {
            ScopedAtomicIncrement inc(mutexWaitCount);
            MutexLock lock(logMutex);
            if (!LoggerManager::isEnabled())
                return;

            string msg;
            logentry(msg,_level,_logger->getCategory());
            msg += _msg.str();
            msg.append("\n\n#################################\n");
            LoggerManager& loggerManager=LoggerManager::getInstance();
            LogAppender *appender =loggerManager.impl()->appender();
            appender->putMessage(msg);
            appender->finish((atomicGet(mutexWaitCount) <= 1));
        }
        catch(const std::exception&)
        {
            printf("\nsome error while writing log info to the file\n");
        }

        clear();
     }

    //END
    //############################################################################


//}

/**
Define here the Logger namespace used by the Log.cpp file.
*/
//using namespace LogImpl;

#endif

