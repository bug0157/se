#ifndef __YL_MYLOGGER_H__
#define __YL_MYLOGGER_H__

#include "../lib/log4cpp/Category.hh"
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>

//添加前缀 文件名、函数名、行号、信息
#define addprefix(msg) std::string("[")\
    .append(__FILE__).append(":")\
    .append(__func__).append(":")\
    .append(std::to_string(__LINE__)).append("]")\
    .append(" ").append(msg).c_str()

#define logError(msg) Mylogger::getInstance()->error(addprefix(msg))
#define logInfo(msg) Mylogger::getInstance()->info(addprefix(msg))
#define logDebug(msg) Mylogger::getInstance()->debug(addprefix(msg))
#define logWarn(msg) Mylogger::getInstance()->warn(addprefix(msg))

using namespace log4cpp;

//使用单例模式
class Mylogger
{
public:
    static Mylogger * getInstance()
    {
        if(nullptr == _pInstance){
            _pInstance = new Mylogger();
        }
        return _pInstance;
    }

    static void destroy()
    {
        if(_pInstance){
            delete _pInstance;
        }
    }

	void warn(const char *msg);
	void error(const char *msg);
	void debug(const char *msg);
	void info(const char *msg);

private:
	Mylogger();
	~Mylogger();

private:
    static Mylogger * _pInstance;
    Category & _mycat;
};

#endif
