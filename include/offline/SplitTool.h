#ifndef SE_INCLUDE_OFFICE_SPLITTOOL_H_
#define SE_INCLUDE_OFFICE_SPLITTOOL_H_

#include <vector>
#include <string>

#include "../../lib/simhash-master/deps/cppjieba/Jieba.hpp"

const char* const DICT_PATH = "/home/metaphysic/se/lib/simhash-master/dict/jieba.dict.utf8";
const char* const HMM_PATH = "/home/metaphysic/se/lib/simhash-master/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "/home/metaphysic/se/lib/simhash-master/dict/user.dict.utf8";
const char* const IDF_PATH = "/home/metaphysic/se/lib/simhash-master/dict/idf.utf8";
const char* const STOP_WORD_PATH = "/home/metaphysic/se/lib/simhash-master/dict/stop_words.utf8";

using std::vector;
using std::string;

class Configuration;

class SplitTool
{
public:
    SplitTool() = default;
    virtual ~SplitTool() = default;
    virtual vector<string> cut(const string &sentence) = 0;
    virtual vector<string> cut1(const string &sentence) = 0;
};


class SplitToolCppJiaba : public SplitTool{
public:
    SplitToolCppJiaba(Configuration *conf);
    ~SplitToolCppJiaba() = default;
    vector<string> cut(const string &sentence);
    vector<string> cut1(const string &sentence);

private:
    Configuration *_conf;
    cppjieba::Jieba _jieba;
};
#endif // !SE_INCLUDE_OFFICE_SPLITTOOL_H_
