#ifndef SE_INCLUDE_OFFICE_DICTPRODUCER_H
#define SE_INCLUDE_OFFICE_DICTPRODUCER_H

#include <utility>
#include <set>


#include "SplitTool.h"

using std::pair;
using std::map;
using std::set;

class DictProducer {
public:
    //DictProducer(const string &dir);//处理英文
    DictProducer(SplitTool *cutter);//处理中文
    DictProducer(const string &dir, SplitTool *cutter);//处理中文

    //void buildEnDict();
    //void buildEnIndex();
    void buildCnDict();
    void buildCnIndex();
    void storeDict(const char * filepath);
    void storeIndex(const char * filepath);

    //void showFiles() const;               //查看文件路径，测试用
    void showDict() const;                //查看词典，测试用
    void showIndex() const;
    void getFiles();                      //获取文件的绝对路径
    void pushDict(const string &word);    //存储某个单词
private:
    SplitTool *_cutter;              //分词工具
    vector<string> _files;              //文件路径
    vector<pair<string,int>> _dict;    //词典->词+频率
    map<string,set<int>> _index;       //索引->词+所有出现过的下标
};




#endif // !SE_INCLUDE_OFFICE_DICTPRODUCER_H
