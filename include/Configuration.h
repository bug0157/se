#ifndef SE_INCLUDE_CONFIGURATION_H_
#define SE_INCLUDE_CONFIGURATION_H_

#include "offline/SplitTool.h"
#include <pthread.h>

#include <map>

using std::map;

//单例类
class Configuration
{
public:
    static Configuration *getInstance();


    static void init();
    static void destory();

private:
    Configuration();
    ~Configuration();

private:
    static Configuration* _pInstance;
    static pthread_once_t _once;

    map<string, string> _confMap;
};




#endif // !SE_INCLUDE_CONFIGURATION_H_

