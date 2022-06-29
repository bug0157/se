#include "../../lib/utfcpp-master/source/utf8.h"
#include "../../include/keyrecommand/KeyRecommander.h"
#include <sstream>

using std::ostringstream;

//构造函数
KeyRecommander::KeyRecommander(string &query, const TcpConnectionPtr& conn)
: _queryWord(query)
//, _conn(conn)
, _resultQue()
{

}

//构造函数
KeyRecommander::KeyRecommander(string &query)
: _queryWord(query)
//, _conn()
, _resultQue()
{

}

//执行查询
void KeyRecommander::execute()
{
    queryIndexTable();
}

//查询索引
void KeyRecommander::queryIndexTable()
{
    Dictionary *dict = Dictionary::createInstance();
    dict->init("../../data/dict/");
    set<int> lines;
    for(auto it = _queryWord.begin(), it2 = _queryWord.begin();
        it2 != _queryWord.end(); )
    {
        utf8::next(it2, _queryWord.end());
        ostringstream oss;
        while(it < it2){
            oss << *it;
            ++it;
        }
        set<int> tmp = dict->doQuery(oss.str());
        lines.insert(tmp.begin(), tmp.end());
    }
    this->statistic(lines);
}

//进行计算
void KeyRecommander::statistic(set<int> &iset)
{
    Dictionary *dict = Dictionary::createInstance();
    for(auto &it : iset)
    {
        MyResult rest;
        pair<string, int> word = dict->getDict(it);         
        rest._word = word.first;
        rest._freq = word.second;
        rest._dist = distance(word.first);

        //插入元素
        _resultQue.push(rest);
        if(_resultQue.size() > 5){
            //队列中超过5个元素,弹出
            _resultQue.pop();
        }
    }
}

//计算最小编辑距离
int KeyRecommander::distance(const string &rhs)
{
    vector<string> a1;
    for(auto it = _queryWord.begin(), it2 = _queryWord.begin();
        it2 != _queryWord.end(); )
    {
        utf8::next(it2, _queryWord.end());
        ostringstream oss1;
        while(it < it2){
            oss1 << *it;
            ++it;
        }
        a1.push_back(oss1.str());
    }

    vector<string> a2;
    for(auto it = rhs.begin(), it2 = rhs.begin();
        it2 != rhs.end(); )
    {
        utf8::next(it2, rhs.end());
        ostringstream oss2;
        while(it < it2){
            oss2 << *it;
            ++it;
        }
        a2.push_back(oss2.str());
    }

    int max1 = a1.size();
    int max2 = a2.size();
    int edit[max1+1][max2+1];
    for(int i = 0; i <= max1; ++i)
    {
        edit[i][0] = i;
    }
    for(int i = 0; i <= max2; ++i)
    {
        edit[0][i] = i;
    }

    for(int i = 1; i <= max1; ++i)
    {
        for(int j = 1; j <= max2; ++j)
        {
            if(a1[i-1] == a2[j-1]){
                edit[i][j] = edit[i-1][j-1];
            }else if(a1[i-1] != a2[j-1]){
                //取最小的值
                if(edit[i-1][j-1]+1 <= edit[i-1][j]+1
                   && edit[i-1][j-1]+1 <= edit[i][j-1]+1){
                    edit[i][j] = edit[i-1][j-1]+1;
                }else if(edit[i-1][j]+1 <= edit[i-1][j-1]+1
                   && edit[i-1][j]+1 <= edit[i][j-1]+1){
                    edit[i][j] = edit[i-1][j]+1;
                }else{
                    edit[i][j] = edit[i][j-1]+1;
                }
            }
        }
    }
    return edit[max1][max2];
}

//响应客户端请求
void KeyRecommander::response()
{
    if(_resultQue.empty()){
        cout << "NULL" << endl;
    }

    while(!_resultQue.empty()){
        cout << _resultQue.top()._word << endl;
        _resultQue.pop();
    }
}
