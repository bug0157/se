#ifndef SE_INCLUDE_OFFLINE_PAGELIB_H_
#define SE_INCLUDE_OFFLINE_PAGELIB_H_

//#include "congFiguration.hh"
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "DirScanner.h"
#include "tinyxml2.h"

using std::fstream;
using std::map;
using std::ofstream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

typedef struct RssItem {
    string title;
    string link;
    string description;
} RssItem;

//生成网页库，网页偏移库
class PageLib {
public:
    PageLib(DirScanner &dirScanner);
    ~PageLib();

    //这里create（）函数需要DirScanner类中的files（）成员函数
    //返回语料文件路径的vector
    void create();

    //获取xml文件里的的每个<doc>的内容
    void rss(const char *fm, int &count);

    //将Rssitem 插入_file;
    void insertItem(RssItem &item, int num);

    //每个<doc>的偏移信息存入map
    void offset();

    //存储网页库到date/ripepage.data
    void store();

private:
    DirScanner &_dirScanner;
    vector<string> _file;
    map<int, pair<int, int>> _offsetLib;
};
#endif // !SE_INCLUDE_OFFLINE_PAGELIB_H_
