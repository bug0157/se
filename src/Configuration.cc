#include "../include/Configuration.h"

#include <stdlib.h>


//饱汉模式，初始化静态数据成员
//pthread_once + atexit实现自动释放
Configuration* Configuration::_pInstance = nullptr;
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;

Configuration* Configuration::getInstance(){
    pthread_once(&_once, init);
    return _pInstance;
}

//初始化单例对象，注册回收函数
void Configuration::init(){
    if (_pInstance == nullptr) {
        _pInstance = new Configuration();
        atexit(Configuration::destory);
    }
}

void Configuration::destory(){
    if (_pInstance == nullptr) {
        return; 
    }
    delete _pInstance;
    _pInstance = nullptr;
}
