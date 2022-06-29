#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    {

    }

    //该函数在线程池中执行的
    void process()
    {
        //在process函数中去进行真正的业务逻辑的处理
        //....
        //...
        /* string newMsg = msg + 1; */
        //现在计算线程（ThreadPool）处理完业务逻辑之后，需要
        //将处理完后数据发送给IO线程（EventLoop/Reactor），
        //IO线程如何与计算线程进行通信？(eventfd)
        _con->sendInLoop(_msg);
        //数据的发送需要在EventLoop里面进行发送
        //TcpConnection需要将数据发送给EventLoop，让EventLoop去
        //进行发送数据IO操作
        //此时TcpConnection需要知道EventLoop存在
    }
private:
    string _msg;
    TcpConnectionPtr _con;
};

ThreadPool *gThreadPool = nullptr;

void onConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    //回显
    string msg = con->receive();
    cout << "recv msg  " << msg << endl;
    //接收与发送之间，消息msg是没有做任何处理的
    //...
    //
    //处理msg的业务逻辑全部在此处实现的话
    //将msg这些信息打个包交给MyTask，然后将MyTask注册给线程池
    //让线程池去处理具体的业务逻辑，此时业务逻辑的处理就不在
    //EventLoop线程中
    
    MyTask task(msg, con);
    /* gThreadPool->addTask(std::bind(&MyTask::process, &task)); */
    gThreadPool->addTask(std::bind(&MyTask::process, task));
}

void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" << endl;
}


void test()
{
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();//此时处于监听状态

    EventLoop loop(acceptor);
    //回调函数的注册
    loop.setConnectionCallback(std::move(onConnection));
    loop.setMessageCallback(std::move(onMessage));
    loop.setCloseCallback(std::move(onClose));

    loop.loop();

}

void test2()
{
    ThreadPool threadPool(5, 10);//局部变量
    threadPool.start();
    gThreadPool = &threadPool;

    TcpServer server("127.0.0.1", 8888);
    server.setAllCallback(std::move(onConnection)
                          , std::move(onMessage)
                          , std::move(onClose));

    server.start();
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}

