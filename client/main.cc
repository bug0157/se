#include "ClientConnect.hpp"
#include "../Reactor/ProtocolParser.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::flush;

int main(){
    ClientConnect conn("127.0.0.1", 8888);
    int ret = conn.connect();
    if (ret < 0){
        perror("connect");
        return -1;
    }
    while(1){
        string str;
        do{
            cout << endl << endl;
            cout << "please select a option:\n"
                 << "    1. keyword Recommand\n"
                 << "    2. webpage search\n"
                 << "input your option:" << flush;
            getline(cin, str);
        }while(str != "1" && str != "2");

        ProtocolParser parser;

        if(str == "1"){
            cout << endl << endl;
            cout << "please input a keyword:" << flush;
            string keyword;
            getline(cin, keyword);
            vector<string> content;
            content.push_back(keyword);
            //打包发送
            string msg = parser.encode(1, content);
            conn.sendMessage(msg);
            
            string result = conn.recvMessage();
            //解包打印
            cout << "关键词推荐结果如下："<< endl;
            cout << endl;
            ProtocolParser::res_t res;
            res = parser.decode(result);
            for (auto iter : res._content) {
                cout << iter << endl;
            }
            cout << endl << endl;
            cout << "~~~~~~~~~~~~我是分割线~~~~~~~~~~~~~" << endl;
            cout << endl << endl;
        }
#if 1
        if(str == "2"){
            cout << endl << endl;
            cout << "please input a sentence:" << flush;
            string sentence;
            getline(cin, sentence);
            vector<string> content;
            content.push_back(sentence);

            string msg = parser.encode(2, content);
            conn.sendMessage(msg);
            
            string result = conn.recvMessage();
            //cout << result << endl;
            //解包打印
            ProtocolParser::res_t res;
            cout << "网页查询结果如下："<< endl;
            cout << endl;
            res = parser.decode(result);
            for (auto iter : res._content) {
                cout << iter << endl;
            }
            cout << endl << endl;
            cout << "~~~~~~~~~~~~我是分割线~~~~~~~~~~~~~" << endl;
            cout << endl << endl;
            //while (1){
            //    retMsg = conn.recvMessage();
            //    size_t webpagesnum = retMsg.at("webpageNumsInOneResultPage");
            //    if (webpagesnum == 0){
            //        cout << "No result!" << endl;
            //        break;
            //    }
            //    cout << std::setw(4) << retMsg << endl;
            //    if (webpagesnum != 10)
            //        break;

            //    cout << "press n to get next result page, or quit search " << sentence << endl;
            //    string searchOption;
            //    getline(cin, searchOption); //使用getline来避免缓冲区还留有换行符
            //    if (searchOption == "n"){
            //        msg["IndexOfResultPages"] = ++IndexOfResultPages;
            //        connect.sendMessage(msg);
            //    }
            //    else
            //        break;
            //}
        }
#endif
    }
}

