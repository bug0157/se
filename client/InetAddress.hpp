#pragma once
#include <arpa/inet.h>
#include <string.h>
#include <string>

using std::string;

class InetAddress{
public:
    InetAddress(const string &ip, unsigned short port){
    ::bzero(&_addr,sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
    }
    InetAddress(const sockaddr_in &addr){}
    string ip() const{
        return inet_ntoa(_addr.sin_addr);
    }
    unsigned short port() const{
        return ntohs(_addr.sin_port);
    }
    const sockaddr_in &addr() const{
        return _addr;
    }
private:
    sockaddr_in _addr;
};
