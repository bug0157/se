#include <dirent.h>

#include <fstream>
#include <sstream>

#include "../../include/offline/tinyxml2.h"
#include "../../lib/utfcpp-master/source/utf8.h"
#include "../../include/offline/DictProducer.h"

using namespace tinyxml2;
using std::cout;
using std::endl;
using std::cerr;
using std::ofstream;
using std::ifstream;
using std::ostringstream;

//处理中文
DictProducer::DictProducer(const string& dir, SplitTool *tool)
: _cutter(tool)
, _dict()
, _index(){
    DIR *dirp = opendir(dir.c_str());
    if(dirp == NULL){
        perror("opendir");
        return;
    }
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != NULL){
        string DirName = pdirent->d_name;
        if(DirName != "." && DirName != ".."){
            _files.push_back(DirName);
        }
    }
    closedir(dirp);
    buildCnDict();
    buildCnIndex();
}

void DictProducer::buildCnDict(){
    string text;
    for(auto &file :_files)
    {
        cout << "load " << file << endl;
        //加载xml文件
        XMLDocument doc;
        string filename = "../../data/xml/" + file;
        int ret = doc.LoadFile(filename.c_str());
        if(ret != 0){
            cerr << "load xml file failed" << endl;
        }

        //遍历结点
        XMLElement *root = doc.FirstChildElement("rss")->FirstChildElement("channel");
        XMLElement *node = root->FirstChildElement("item");
        while(node){
            //获取标题
            text += " ";
            text += node->FirstChildElement("title")->GetText();
            
            //获取内容,内容不能为空
            text += " ";
            text += node->FirstChildElement("description")->GetText();

            node = node->NextSiblingElement();
        }
    }

    cout << "cut begin" << endl;
    map<string, int> dict;
    vector<string> word = _cutter->cut(text);
    for(auto &it_word : word)
    {
        auto it = dict.find(it_word);
        if(it != dict.end()){
            it->second ++;
        }else{
            dict[it_word] = 1;
        }
    }

    for(auto &it : dict)
    {
        _dict.push_back(it);
    }
    cout << "cut end" << endl;    
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
