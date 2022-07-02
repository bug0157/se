#pragma once
#include "sys/types.h"
#include <sys/socket.h>
#include <string>
#include <unistd.h>
using std::string;
class SocketIO{
public:
    SocketIO(int fd) : _fd(fd) {}
    ~SocketIO() {}

    int readn(char *buf, int len){
        int left = len;
        char *pstr = buf;
        int ret = 0;

        while(left > 0)
        {
            ret = read(_fd, pstr, left);
            if(-1 == ret && errno == EINTR)
            {
                continue;
            }
            else if(-1 == ret)
            {
                perror("read error -1");
                return len - ret;
            }
            else if(0 == ret)
            {
                break;
            }
            else
            {
                pstr += ret;
                left -= ret;
            }
        }

        return len - left;

    }

    int readLine(char *buf, int len){
        int left = len - 1;
        char *pstr = buf;
        int ret = 0, total = 0;

        while(left > 0)
        {
            //不会将缓冲区中的数据进行清空
            ret = recv(_fd, pstr, left, MSG_PEEK);
            if(-1 == ret && errno == EINTR)
            {
                continue;
            }
            else if(-1 == ret)
            {
                perror("readLine error -1");
                return len - ret;
            }
            else if(0 == ret)
            {
                break;
            }
            else
            {
                for(int idx = 0; idx < ret; ++idx)
                {
                    if(pstr[idx] == '\n')
                    {
                        int sz = idx + 1;
                        readn(pstr, sz);
                        pstr += sz;
                        *pstr = '\0';
                        return total + sz;
                    }
                }

                readn(pstr, ret);//从内核态拷贝到用户态
                total += ret;
                pstr += ret;
                left -= ret;
            }
        }
        *pstr = '\0';
        return total - left;
    }

    int writen(const char* buf, int len){
        int left = len;
        const char *pstr = buf;
        int ret = 0;

        while(left > 0)
        {
            ret = write(_fd, pstr, left);
            if(-1 == ret && errno == EINTR)
            {
                continue;
            }
            else if(-1 == ret)
            {
                perror("writen error -1");
                return len - ret;
            }
            else if(0 == ret)
            {
                break;
            }
            else
            {
                pstr += ret;
                left -= ret;
            }
        }
        const char *blank = "\n";
        write(_fd, blank, 1);
        return len - left;
    }

private:
    int _fd;
};
