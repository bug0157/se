#ifndef SE_INCLUDE_OFFICE_PAGELIBPREPEOCESSOR_H_
#define SE_INCLUDE_OFFICE_PAGELIBPREPEOCESSOR_H_

#include "../../lib/simhash/Simhasher.hpp"
#include "../../lib/simhash/cppjieba/Jieba.hpp"

#include "../../include/offline/WebPage.h"

using namespace simhash;
using weightMap = unordered_map<string, set<pair<uint64_t, double>>>;
using TFMap = vector<unordered_map<string, int>>;

class PageLibPreProcessor{
public:
    PageLibPreProcessor();
    void doProcess();              //执行预处理

    void readInfoFromFile();       //读取网页库和偏移库；
    void cutRedundantPages();      //去重
    void buildInvertIndexTable();  //创建倒排索引
    void storeOnDisk();            //存储到磁盘上

private:

    Simhasher _simhasher;
    //指纹库
    vector<uint64_t> _simhashValue;
    //分词对象
    cppjieba::Jieba _jieba;
    //网页库的容器对象
    vector<WebPage> _pageLib;
    //去重后的网页库
    vector<WebPage> _newPageLib; 
    //倒排索引表对象
    weightMap _invertIndexTable; 
    TFMap _TfMap;
};





#endif // !SE_INCLUDE_OFFICE_PAGELIBPREPEOCESSOR_H_
