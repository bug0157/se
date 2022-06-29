#include "../../include/Configuration.h"
#include "../../include/offline/SplitTool.h"
#include <vector>


SplitToolCppJiaba::SplitToolCppJiaba(Configuration *conf)
: _conf(conf)
, _jieba(DICT_PATH, 
        HMM_PATH, 
        USER_DICT_PATH){
    std::cout << "SplitToolCppJiaba()" << std::endl;
}

vector<string> SplitToolCppJiaba::cut(const string &sentence){
    vector<string> allWords;
    //调用Jieba的cutforsearch方法
    _jieba.CutForSearch(sentence, allWords);
    //std::cout << "allWords:" << allWords.size() << std::endl;

    //去除停用词
    //通过配置类调用停用词文件路径
    unordered_set<string> stop_words = _conf->getStopWords();
    //std::cout << "stop_words:" <<stop_words.size() << std::endl;
    vector<string> temp;
    int cnt = 0;
    for (auto &iter : allWords) {
        //std::cout << iter << std::endl;
        if (stop_words.find(iter) != stop_words.end()) {
            //std::cout << "111" << std::endl;
            continue;
        }
        temp.push_back(iter);
    }
    //清空
    allWords.clear();

    //去除英文及乱码
    for (auto &it : temp) {
        unsigned char x = it[0];
        if (x < 128) {
            //std::cout << cnt++ << std::endl;
            continue;
        } 
        allWords.push_back(it);
    }
    return allWords;
}
