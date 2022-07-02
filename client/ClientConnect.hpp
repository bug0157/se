#pragma once
#include <iostream>
#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"

class ClientConnect{
public:
    ClientConnect(const string &ip,unsigned short port)
    : _sockio(_sock.fd())
    , _serverAddr(ip, port){
    }
    int getSockfd() const{
        return _sock.fd();
    }
    string recvMessage(){
        char buf[65535] = {0};
        int ret = _sockio.readLine(buf, sizeof(buf));
        return string(buf);
    }
    void sendMessage(const string &msg){
        int ret = _sockio.writen(msg.c_str(), msg.size());
    }

    int connect() const{
       return ::connect(_sock.fd(), (sockaddr*)&_serverAddr.addr(), sizeof(_serverAddr.addr())); 
    }

private:
    Socket _sock;
    SocketIO _sockio;
    InetAddress _serverAddr;
};
