#include <fstream>
#include <sstream>


#include "../../include/offline/tinyxml2.h"
#include "../../lib/utfcpp-master/source/utf8.h"
#include "../../include/offline/DictProducer.h"

using namespace tinyxml2;
using std::ofstream;
using std::ostringstream;

DictProducer::DictProducer(SplitTool *tool)
: _record()
, _dict()
, _index()
,_cutter(tool){
    buildCnDict();
    buildCnIndex();
}

void DictProducer::buildCnDict(){
    std::cout << "start xml prase" << std::endl;
    XMLDocument xml;
    xml.LoadFile("../../data/xml/hm.xml");
    XMLElement *root = xml.RootElement();//获取根结点
    XMLElement *channel = root->FirstChildElement("channel");
    XMLElement *item = channel->FirstChildElement("item");
    string sentence;
    while (item) {
        //获取标题
        //XMLElement *item_title = item->FirstChildElement("title");
        //获取内容
        XMLElement *item_content = item->FirstChildElement("description");
        string fragment = item_content->GetText();
        sentence += fragment;
        item = item->NextSiblingElement();
    }
    //分词
    std::cout << "start cut" << std::endl;
    vector<string> words = _cutter->cut(sentence);
    for (auto &w_it : words) {
        auto ex_it = _record.find(w_it);
        //词已存在，词典频率++
        if (ex_it != _record.end()) {
            ++_dict[ex_it->second].second;
        }
        //词不存在
        else {
            _dict.push_back(pair<string,int>(w_it, 1));
            _record.insert(pair<string,int>(w_it, _dict.size()));
        }
    }
}

void DictProducer::buildCnIndex(){
    string letter;
    int line = 0;
    for(auto &dict :_dict)
    {
        ++ line;
        letter = dict.first;
        for(auto it = letter.begin(), it2 = letter.begin(); it2 != letter.end(); )
        {
            utf8::next(it2, letter.end());
            ostringstream oss;
            while(it < it2){
                oss << *it;
                ++it;
            }
            _index[oss.str()].insert(line);
        }
    }

}

void DictProducer::storeDict(const char *filepath){
    ofstream ofs(filepath);
    if (!ofs.good()) {
        perror("file open error");
        return;
    }
    for (auto &iter : _dict) {
        ofs << iter.first << " " << iter.second << std::endl;
    }
    ofs.close();
}

void DictProducer::storeIndex(const char *filepath){
    ofstream ofs(filepath);
    if (!ofs.good()) {
        perror("file open error");
        return;
    }
    for (auto &iter : _index) {
        ofs << iter.first << " ";
        for (auto &set_it : iter.second) {
            ofs << set_it << " ";
        }
        ofs << std::endl;
    }
    ofs.close();
}

// for test
void DictProducer::showDict() const{
    std::cout << "show dict:" << std::endl;
    
    for (auto iter : _dict) {
        std::cout << iter.first << " " << iter.second << std::endl;
    }
}

void DictProducer::showIndex() const{
    std::cout << "show index:" << std::endl;
    
    for (auto iter : _index) {
        std::cout << iter.first << " ";
        for (auto &set_it : iter.second) {
            std::cout << set_it << " ";
        }
        std::cout << std::endl;
    }
}
