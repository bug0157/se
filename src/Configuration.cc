#include <stdlib.h>
#include <fstream>

#include "../include/Configuration.h"

using std::ifstream;

//饱汉模式，初始化静态数据成员
//pthread_once + atexit实现自动释放
Configuration* Configuration::_pInstance = nullptr;
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;

Configuration* Configuration::getInstance(){
    //std::cout << "Configuration::getInstance()" << std::endl;  
    pthread_once(&_once, init);
    return _pInstance;
}

//初始化单例对象，注册回收函数
void Configuration::init(){
    //std::cout << "Configuration::init()" << std::endl;  
    if (_pInstance == nullptr) {
        _pInstance = new Configuration();
        atexit(Configuration::destory);
    }
}

void Configuration::destory(){
    //std::cout << "Configuration::destory()" << std::endl;  
    if (_pInstance == nullptr) {
        return; 
    }
    delete _pInstance;
    _pInstance = nullptr;
}

Configuration::Configuration(){
    //std::cout << "Configuration::Configuration" << std::endl;  
    //初始化停用词
    stopWordsInit();
}

unordered_set<string> &Configuration::getStopWords(){
    return _stopWords;
}

void Configuration::stopWordsInit(){
    //std::cout << "Configuration::stopWordsInit()" << std::endl;  
    string file = "/home/metaphysic/se/data/stopwords/stop_words_cn.utf8";
    ifstream ifs(file);
    if (!ifs.is_open()) {
        perror("ifs open failed");
        return;
    } 
    ifs.seekg(std::ios_base::beg);
    string words;
    _stopWords.clear();
    while (getline(ifs, words)) {
        //std::cout << words << std::endl;
        _stopWords.emplace(words);
    }
    //std::cout << "读完停用词" << std::endl;

#if 0
    for (auto it = _stopWords.begin(); it != _stopWords.end(); ++it) {
        std::cout << *it << std::endl;  
    }
    std::cout << "存储完停用词" << std::endl;
#endif

    ifs.close();
}
