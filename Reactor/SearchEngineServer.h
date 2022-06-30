#ifndef SE_REACTOR_SEARCHENGINESERVER_H_
#define SE_REACTOR_SEARCHENGINESERVER_H_

#include "TcpServer.h"
#include "ThreadPool.h"
#include "../include/online/KeyRecommander.h"
#include "../include/online/WebPageQuery.h"

using namespace std::placeholders;
using std::bind;

class SearchTask{
public:
    SearchTask(string &msg, const TcpConnectionPtr &con, SplitTool *tool)
    : _msg(msg)
    , _con(con)
    , _keyRecommander(msg)
    , _webSearcher(tool)
    {
    }

    void process(){
        //业务逻辑
        //回显
        //_con->sendInLoop(_msg);
        //关键词推荐业务
        int flag = 1;
        if (0 == flag) {
            _keyRecommander.execute();
            string ret = _keyRecommander.response();
            std::cout << "response 返回的结果为：\n" << ret << std::endl;
            _con->sendInLoop(ret);
        }
        //网页查询业务
        if (1 == flag) {
            string ret = _webSearcher.doQuery(_msg);  
            _con->sendInLoop(ret);
        }
    }

private:
    string _msg;
    TcpConnectionPtr _con;
    KeyRecommander _keyRecommander;
    WebPageQuery _webSearcher;
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

    void start(){
        _pool.start();//启动计算线程
        //注册所有回调函数
        _tcpServer.setAllCallback(
                bind(&SearchEngineServer::onConnection, this, _1),
                bind(&SearchEngineServer::onMessage, this , _1),
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

    void onMessage(const TcpConnectionPtr &con){
        //接收数据
        string msg = con->receive();
        if (msg.empty()) {
            std::cout << "invalid msg" << std::endl;
        }
        //封装成SearchTask
        SplitTool *tool = new SplitToolCppJiaba(_conf);
        SearchTask task(msg, con, tool);
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
