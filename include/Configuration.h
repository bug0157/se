#ifndef SE_INCLUDE_CONFIGURATION_H_
#define SE_INCLUDE_CONFIGURATION_H_

#include "offline/SplitTool.h"
#include <pthread.h>

#include <map>

//using std::map;
using std::unordered_set;

//单例类
class Configuration
{
public:
    static Configuration *getInstance();
    static void init();
    static void destory();

    unordered_set<string> &getStopWords();

private:
    Configuration();
    ~Configuration() = default;
    void stopWordsInit();

private:
    static Configuration* _pInstance;
    static pthread_once_t _once;
    unordered_set<string> _stopWords; //停用词集
    //map<string,string> _confMap;
};




#endif // !SE_INCLUDE_CONFIGURATION_H_

