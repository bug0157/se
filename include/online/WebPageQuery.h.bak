#ifndef SE_INCLUDE_ONLINE_WEBPAGEQUERY_H_
#define SE_INCLUDE_ONLINE_WEBPAGEQUERY_H_


#include "../offline/WebPage.h"
#include "../offline/SplitTool.h"
#include "retClass.h"

#include <unordered_map>
#include <utility>
#include <set>
#include <string>
#include <fstream>


using std::unordered_map;
using std::string;
using std::set;
using std::pair;


class WebPageQuery{
    
public:
    WebPageQuery(SplitTool*);
    ~WebPageQuery();
    //void loadLib();//将invertIdx.dat,newpage.lib,newoffset.lib的文件加载到成员数据中
                   //分开三个函数来加载文件内容,loadLib会调用下面三个函数
    void show();//测试函数

    string doQuery(const string& query);
    vector<Retclass> doQueryRet(const string& query); 
private:
    void WordHandle(const string&,vector<string>&,map<string,int>&);//切词处理
    void WordWeightCount(vector<string>&,map<string,int>&,vector<double>&);//要查询的语句每个单词的权重计算函数
    int offsetWordQuery(vector<string>&,map<int,vector<double>>&);//返回值作为标志，为-1表示没有结果
    void cosCountQuery(map<double,int>&,vector<double>&,map<int,vector<double>>&);
   //计算余弦值 
    string resultHandle(map<double,int>&,string);//结果分割函数，包括摘要自动生成

    void loadLib();//将invertIdx.dat,newpage.lib,newoffset.lib的文件加载到成员数据中
                   //分开三个函数来加载文件内容,loadLib会调用下面三个函数
 
    void loadInvertIdx();//加载InvertIdx.dat文件
    void loadOffset();//加载offset.lib文件
    void loadNewpage();//加载newpage.lib文件
    
private:
    SplitTool* _sp;
    unordered_map<int, WebPage> _pageLib;    
    unordered_map<int, pair<int, int>> _offsetLib;
    unordered_map<string, set<pair<int, double>>> _invertIndexLib;
};






#endif // !SE_INCLUDE_ONLINE_WEBPAGEQUERY_H_
