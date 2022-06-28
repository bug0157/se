#include "../../include/offline/PageLibPreprocessor.h"


void PageLibPreProcessor::readInfoFromFile(){
    string pagelib_path = "";
    string offsetlib = "";
    ifstream ifs(pagelib_path);
    if (!ifs.is_open()) {
        perror("pagelib open failed");
        return;
    }
    //读取每行，创建成webpage对象，存入vector
    string temp;
    while (getline(ifs, temp)) {
        WebPage web(temp);
        _pageLib.push_back(web);
    }
}

void PageLibPreProcessor::cutRedundantPages(){
    vector<pair<string, double>> res;
    size_t topN = 5, docID = 0;
    uint64_t u64 = 0;
    for(int idx = 0; idx < _pageLib.size(); ++idx){
        _simhasher.extract(_pageLib[idx].getContent(), res, topN);
        _simhasher.make(_pageLib[idx].getContent(), topN, u64);
        size_t i;
        for (i = 0; i < _simhashValue.size(); ++ i){
            if (Simhasher::isEqual(_simhashValue[i], u64) == 1)
                break;
            if (i == _simhashValue.size()){
                _simhashValue.push_back(u64); 
                _newPageLib.push_back(_pageLib[idx]);
            }
        }
    }
}

void PageLibPreProcessor::buildInvertIndexTable(){
    ifstream ifsStopWords("STOP_WORD_PATH");
    unordered_set<string> stopWords;
    vector<string> tmpWords;
    string stopWord, tmpWord;
    unordered_map<string, set<pair<uint64_t, int>>> TfMap;
    //停用词存在vector
    while(ifsStopWords >> stopWord){
        stopWords.insert(stopWord);
    }
    for (size_t idx = 0; idx < _pageLib.size(); ++idx){
        _jieba.Cut(_pageLib[idx].getContent(), tmpWords);
        unordered_map<string, int> tfmap;
        for (auto & tmp : tmpWords){
            auto res = tfmap.insert({tmp, 1});
            if (!res.second){
                ++res.first->second;
            }
        }
        //停用词
        for (unordered_map<string, int>::iterator it = tfmap.begin(); it != tfmap.end();){
            if (stopWords.end() != stopWords.find(it->first)){
                tfmap.erase(it++);
                continue;
            }
            it++;
        }
        _TfMap.push_back(std::move(tfmap));
    }
    ifsStopWords.close();
    //遍历vector
    for (size_t idx = 0; idx < _TfMap.size(); ++idx){
        //遍历map中的单词
        auto map_fre = _TfMap[idx];
        unordered_map<string, int>::iterator it1 = map_fre.begin();
        for (; it1 != map_fre.end(); ++it1){
            auto res = TfMap.insert(make_pair(it1->first, set<pair<uint64_t,int>>{make_pair(_pageLib[idx].getDocId(),it1->second)}));
            if (!res.second){
                res.first->second.insert(make_pair(_pageLib[idx].getDocId(), it1->second));
            }
        }
    }
    //遍历每个string
    size_t N = _pageLib.size();
    unordered_map<string, set<pair<uint64_t, int>>>::iterator it2 = TfMap.begin();
    for(; it2 != TfMap.end(); ++it2){
        double df = it2->second.size();      //set的大小
        double idf = log((double)N/(df+1)) + 0.0001;//避免idf为零
        double w1 = 0;
        for (set<pair<uint64_t, int> >::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3){
            w1 += idf * (it3->second) * idf * (it3->second);
        }
        double w2 = sqrt(w1);
        for (set<pair<uint64_t, int> >::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3){
            _invertIndexTable[it2->first].insert(make_pair(it3->first, (double(it3->second))/w2));
        }
    }
}

