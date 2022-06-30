#pragma once
#include <sys/socket.h>
#include <functional>
#include <unistd.h>
class Socket{
public:
    Socket(){
        open();
    }
    ~Socket(){
        close();
    }
    int fd() const{
        return _fd;
    }
    void shutDownWrite(){
        int ret = shutdown(_fd, SHUT_RD);
        _fd = -1;
    }
    void open(){
        if (_fd == -1){
            _fd = ::socket(AF_INET, SOCK_STREAM, 0);
        }
    }
    void close(){
        if (_fd != -1){
            ::close(_fd);
            _fd = -1;
        }
    }
private:
    int _fd = -1;   //-1表示关闭状态
};