#include <fstream>
#include <sstream>


#include "../../include/offline/tinyxml2.h"
#include "../../include/offline/DictProducer.h"

using namespace tinyxml2;

DictProducer::DictProducer(SplitTool *tool)
: _record()
, _dict()
, _index()
,_cutter(tool){
    buildCnDict();
    //storeIndex(dir.c_str());
}

void DictProducer::buildCnDict(){
    std::cout << "start xml prase" << std::endl;
    XMLDocument xml;
    xml.LoadFile("../../data/xml/hm.xml");
    XMLElement *root = xml.RootElement();//获取根结点rss
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



// for test
void DictProducer::showDict() const{
    std::cout << "show dict:" << std::endl;
    
    for (auto iter : _dict) {
        std::cout << iter.first << " " << iter.second << std::endl;
    }
}
