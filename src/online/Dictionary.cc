#include "../../include/online//Dictionary.h"
#include <iostream>
#include <sstream>
#include <fstream>

using std::istringstream;
using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

//初始化静态成员
Dictionary * Dictionary::_pInstance = nullptr;

//静态构造
Dictionary* Dictionary::createInstance()
{
    if(nullptr == _pInstance){
        _pInstance = new Dictionary();
    }
    return _pInstance;
}

//通过词典文件路径初始化词典
void Dictionary::init(const string &dictpath)
{
    //初始化词典
    string filename = dictpath + "dict.dat";
    ifstream ifs(filename);
    if(!ifs.good()){
        cout << "ifstream open dict.dat error" << endl;
        return;
    }

    string line;
    map<string, int> dict;
    while(getline(ifs, line)){
        istringstream wfs(line);
        string word;
        wfs >> word;
        int num;
        wfs >> num;
        dict[word] = num;
    }

    for(auto &it : dict)
    {
        _dict.push_back(it);
    }
    ifs.close();

    //初始化索引
    filename = dictpath + "index.dat";
    ifstream ifs1(filename);
    if(!ifs1.good()){
        cout << "ifstream open dictIndex.dat error" << endl;
        return;
    }

    while(getline(ifs1, line)){
        istringstream wfs(line);
        string word;
        wfs >> word;
        int num;
        while(wfs >> num){
            _indexTable[word].insert(num);
        }
    }
    ifs1.close();
}

//查询词典
pair<string, int> Dictionary::getDict(int line)
{
    return _dict[line-1];
}

//获取索引表
map<string, set<int>> & Dictionary::getIndexTable()
{
    return _indexTable;
}

//执行查询
set<int> Dictionary::doQuery(string word)
{
    return _indexTable[word];    
}

Dictionary::Dictionary()
:_dict()
,_indexTable()
{
}
