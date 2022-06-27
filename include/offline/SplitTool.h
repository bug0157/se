#ifndef SE_INCLUDE_OFFICE_SPLITTOOL_H_
#define SE_INCLUDE_OFFICE_SPLITTOOL_H_

#include <vector>
#include <string>

#include "../../lib/cppjieba-master/include/cppjieba/Jieba.hpp"

using std::vector;
using std::string;

class Configuration;

class SplitTool
{
public:
    SplitTool() = default;
    virtual ~SplitTool() = default;
    virtual vector<string> cut(const string &sentence) = 0;
};


class SplitToolCppJiaba : public SplitTool{
public:
    SplitToolCppJiaba(Configuration *conf);
    ~SplitToolCppJiaba() = default;
    vector<string> cut(const string &sentence);

private:
    Configuration *_conf;
    cppjieba::Jieba _jieba;
};
#endif // !SE_INCLUDE_OFFICE_SPLITTOOL_H_
