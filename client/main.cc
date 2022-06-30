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
            cout << "please select a option:\n"
                 << "    1. keyword Recommand\n"
                 << "    2. webpage search\n"
                 << "input your option:" << flush;
            getline(cin, str);
        }while(str != "1" && str != "2");

        ProtocolParser parser;

        if(str == "1"){
            cout << "please input a keyword:" << flush;
            string keyword;
            getline(cin, keyword);
            vector<string> content;
            content.push_back(keyword);
            //打包发送
            string msg = parser.encode(1, content);
            cout << msg << endl;
            conn.sendMessage(msg);
            cout << "send message" << endl;
            
            string result = conn.recvMessage();
            //解包打印
            ProtocolParser::res_t res;
            res = parser.decode(result);
            for (auto iter : res._content) {
                cout << iter << endl;
            }
        }
#if 0
        if(str == "2"){
            msg["type"] = 2;
            size_t IndexOfResultPages = 1;
            msg["IndexOfResultPages"] = IndexOfResultPages;
            cout << "please input a sentence:" << flush;
            string sentence;
            getline(cin, sentence);
            msg["sentence"] = sentence;

            connect.sendMessage(msg);
            json retMsg;
            while (1){
                retMsg = connect.recvMessage();
                size_t webpagesnum = retMsg.at("webpageNumsInOneResultPage");
                if (webpagesnum == 0){
                    cout << "No result!" << endl;
                    break;
                }
                cout << std::setw(4) << retMsg << endl;
                if (webpagesnum != 10)
                    break;

                cout << "press n to get next result page, or quit search " << sentence << endl;
                string searchOption;
                getline(cin, searchOption); //使用getline来避免缓冲区还留有换行符
                if (searchOption == "n"){
                    msg["IndexOfResultPages"] = ++IndexOfResultPages;
                    connect.sendMessage(msg);
                }
                else
                    break;
            }
        }
#endif
    }
}

