#ifndef SE_REACTOR_SEARCHENGINESERVER_H_
#define SE_REACTOR_SEARCHENGINESERVER_H_

#include "TcpServer.h"
#include "ThreadPool.h"
#include "../include/online/KeyRecommander.h"
#include "../include/online/WebPageQuery.h"
#include "ProtocolParser.h"

using namespace std::placeholders;
using std::bind;

class SearchTask{
public:
    SearchTask(string &msg, const TcpConnectionPtr &con, WebPageQuery &webquery)
    : _msg(msg)
    , _con(con)
    , _keyRecommander()
    , _webSearcher(webquery)
    {
    }

    void process(){
        //业务逻辑
        ProtocolParser::res_t res = _parser.decode(_msg);
        //解包分析任务
        int flag = res._id;
        //关键词推荐业务
        if (1 == flag) {
            _keyRecommander.execute(res._content[0]);
            vector<string> res = _keyRecommander.response();
            string ret = _parser.encode(1, res);
            _con->sendInLoop(ret);
        }
        //网页查询业务
        if (2 == flag) {
            string ret = _webSearcher.doQuery(_msg);  
            _con->sendInLoop(ret);
        }
    }

private:
    string _msg;
    TcpConnectionPtr _con;
    KeyRecommander _keyRecommander;
    WebPageQuery &_webSearcher;
    ProtocolParser _parser;
};

class SearchEngineServer{
public:
    SearchEngineServer(size_t threadNum, size_t queSize,
                       const string &ip, unsigned short port,
                       Configuration *conf)
    : _pool(threadNum, queSize)
    , _tcpServer(ip, port)
    , _conf(conf){

    }

    void start(WebPageQuery &webquery){
        _pool.start();//启动计算线程
        //注册所有回调函数
        _tcpServer.setAllCallback(
                bind(&SearchEngineServer::onConnection, this, _1),
                bind(&SearchEngineServer::onMessage, this , _1, webquery),
                bind(&SearchEngineServer::onClose, this, _1));
        //IO线程启动
        _tcpServer.start();
    }

    void stop(){
        _pool.stop();
        _tcpServer.stop();
    }

private:
    void onConnection(const TcpConnectionPtr &con){
        std::cout << con->toString() << " has connected!" << std::endl;
    }

    void onMessage(const TcpConnectionPtr &con, WebPageQuery &webquery){
        //接收数据
        string msg = con->receive();
        if (msg.empty()) {
            std::cout << "invalid msg" << std::endl;
        }
        //封装成SearchTask
        SplitTool *tool = new SplitToolCppJiaba(_conf);
        SearchTask task(msg, con, webquery);
        //将process函数注册进计算线程
        _pool.addTask(bind(&SearchTask::process, task));
    }

    void onClose(const TcpConnectionPtr &con){
        std::cout << con->toString() << " has closed!" << std::endl;
    }

private:
    TcpServer _tcpServer;
    ThreadPool _pool;
    Configuration *_conf;
    //WebPageSearcher _webSearcher;
    //ProtocolParser _protocolParser;
};

#endif // !SE_REACTOR_SEARCHENGINESERVER_H_
