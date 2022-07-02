#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _loop(loop)
, _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())

{

}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const string &msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string &msg)
{
    if(_loop)
    {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));//注册到EventLoop
        //void runInLoop(function<void()> &&cb);
    }
}

string TcpConnection::receive()
{
    char buff[65535] = {0};
    cout << "111" << endl;
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

bool TcpConnection::isClosed() const
{
    char buf[10] = {};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }

    return InetAddress(addr);

}

//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);

}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{
    _onConnectionCb = std::move(cb);
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = std::move(cb);
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = std::move(cb);
}

//三个事件回调函数的执行
void TcpConnection::handleConnectionCallback()
{
    if(_onConnectionCb)
    {
        /* using TcpConnectionPtr = shared_ptr<TcpConnection>; */
        /* using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>; */
        /* TcpConnectionCallback _onConnectionCb; */
        //function<void(const shared_ptr<TcpConnection> &)> _onConnectionCb;
        _onConnectionCb(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }

}
void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
    {
        _onCloseCb(shared_from_this());
    }

}
