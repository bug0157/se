#ifndef SE_INCLUDE_OFFICE_PAGELIBPREPROCESSOR_H_
#define SE_INCLUDE_OFFICE_PAGELIBPREPROCESSOR_H_

#include "../../lib/simhash-master/include/simhash/Simhasher.hpp"

#include "../../include/offline/WebPage.h"

using namespace simhash;
using weightMap = unordered_map<string, set<pair<uint64_t, double>>>;
using TFMap = vector<unordered_map<string, int>>;

class PageLibPreProcessor{
public:
    PageLibPreProcessor(SplitTool *tool);
    //void doProcess();              //执行预处理

    void readInfoFromFile();       //读取网页库和偏移库；
    void cutRedundantPages();      //去重
    void buildInvertIndexTable();  //创建倒排索引
    void storeOnDisk();            //存储到磁盘上
    //for test
    void showSize() { cout << "newPagelib size:" << _newPageLib.size() << endl; }
    void showIndex(){
        for (auto &map_it : _invertIndexTable) {
            cout << map_it.first << " ";
            for (auto &set_it : map_it.second) {
                cout << set_it.first << " "
                     << set_it.second << " ";
            }
            cout << endl;
        }
    }

private:

    Simhasher _simhasher;
    //指纹库
    vector<uint64_t> _simhashValue;
    //分词对象
    SplitTool* _tool;
    //网页库的容器对象
    vector<WebPage> _pageLib;
    //去重后的网页库
    vector<WebPage> _newPageLib; 
    //倒排索引表对象
    weightMap _invertIndexTable; 
    TFMap _TfMap;
};





#endif // !SE_INCLUDE_OFFICE_PAGELIBPREPROCESSER_H_
