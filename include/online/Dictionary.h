#ifndef __YL_DICTIONARY_H__
#define __YL_DICTIONARY_H__

#include <vector>
#include <map>
#include <set>
#include <string>

using std::vector;
using std::string;
using std::map;
using std::set;
using std::pair;


class Dictionary
{
public:
    //静态构造
    static Dictionary* createInstance();
    //通过词典文件路径初始化词典
    void init(const string &dictpath);
    //查询词典
    pair<string, int> getDict(int line);
    //获取索引表
    map<string, set<int>> &getIndexTable();
    //执行查询
    set<int> doQuery(string word);

private:
    Dictionary();
    vector<pair<string, int>> _dict;    //词典
    map<string, set<int>> _indexTable;  //索引表
    static Dictionary * _pInstance;
};

#endif
