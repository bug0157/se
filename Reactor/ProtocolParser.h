#ifndef SE_REACTOR_PROTOCOLPARSER_H_
#define SE_REACTOR_PROTOCOLPARSER_H_

#include <vector>
#include <string>

#include "json.hpp"


using json = nlohmann::json;
using std::string;
using std::vector;

class ProtocolParser{
public:
    struct res_t{
        int _id;
        vector<string> _content;
    };

    //对象序列化成字符串
    string encode(int id, const vector<string> &content){
        _j["id"] = id;
        _j["content"] = content;
        return _j.dump();
    }
    //字符串反序列化成对象
    res_t decode(const string &msg){
        res_t res;
        json temp = json::parse(msg);
        res._id = temp.at("id").get<int>();
        for (auto &iter : temp.at("content")) {
            res._content.push_back(iter);
        }
        return res;
    }

private:
    json _j;
};

#endif // !SE_REACTOR_PROTOCOLPARSER_H_
