#ifndef SE_INCLUDE_OFFLINE_WEBPAGE_H_
#define SE_INCLUDE_OFFLINE_WEBPAGE_H_


#include "../../include/offline/tinyxml2.h"
#include "../../include/Configuration.h"
#include "../../include/offline/SplitTool.h"

#include <stdlib.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;
using namespace tinyxml2;

class WebPage{
public:
    WebPage(string&);//传入newpage中的一整行doc内容
    ~WebPage();

    int getDocId();//获取文档id
    string getURL();//获取URL
    string getContent();//获取文档内容
    string getTitle();//获取文档标题
    string getDoc();//获取整篇文档
    map<string,int>& getWordsMap();//获取文档词频

    void ProcessDoc(); //将传入的doc进行分割处理
    void show();//打印测试函数 
    
    void WordsHandle();//处理单词词频 
    

private:
    string _doc;//整篇文档
    int _docId;//文档id
    string _docTitle;//文档标题
    string _docURL;//文档URL
    string _docContent;//文档内容
    string _docSummary;//文档摘要
    //vector<string> _topWords;//最高词频前20个
    map<string,int> _wordsMap;//所有单词和词频
};




#endif //!SE_INCLUDE_OFFLINE_WEBPAGE_H_
