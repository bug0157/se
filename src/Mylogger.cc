#include "../include/Mylogger.h"

Mylogger * Mylogger::_pInstance = nullptr;

Mylogger::Mylogger()
//创建Category对象
: _mycat(Category::getRoot().getInstance("yl"))
{
    //创建两个PatternLayout对象;一个打印到屏幕,一个输入到文件
    PatternLayout * ptnLayout1 = new PatternLayout();
    ptnLayout1->setConversionPattern("%d %c [%p] %m%n");
    PatternLayout * ptnLayout2 = new PatternLayout();
    ptnLayout2->setConversionPattern("%d %c [%p] %m%n");

    //创建目的地对象
    /* OstreamAppender * posAp = new OstreamAppender("console", &cout); */
    FileAppender * fileApp = new FileAppender("fileApp", "../log/My.log");

    //设置目的地的日志布局
    /* posAp->setLayout(ptnLayout1); */
    fileApp->setLayout(ptnLayout2); 
    
    //设置优先级
    _mycat.setPriority(Priority::DEBUG);
    /* _mycat.setAppender(posAp); */
    _mycat.addAppender(fileApp);
    
}

Mylogger::~Mylogger()
{
    Category::shutdown();
}

void Mylogger::warn(const char *msg)
{
    _mycat.warn(msg);
}

void Mylogger::info(const char *msg)
{
    _mycat.info(msg);
}

void Mylogger::debug(const char *msg)
{
    _mycat.debug(msg);
}

void Mylogger::error(const char *msg)
{
    _mycat.error(msg);
}
