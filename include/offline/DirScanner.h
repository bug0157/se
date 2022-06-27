#ifndef SE_INCLUDE_OFFLINE_DIRSCANNER_H
#define SE_INCLUDE_OFFLINE_DIRSCANNER_H

#include <string.h>
#include <dirent.h>

#include <vector>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector; 

class DirScanner 
{
public:
    DirScanner()
    {

    }

    vector<string> files() 
    {
        return this->_files;
    }

    void traverse(const string &dirname)
    {
        DIR *pDir;
        struct dirent *ptr;
        if (!(pDir = opendir(dirname.c_str()))) 
        {
            cout << "Folder doesn't Exist!" << endl;
            return;
        }
        while ((ptr = readdir(pDir)) != 0) 
        {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) 
            {
                char *pFile;
                pFile = strrchr(ptr->d_name, '.');  // 判断输入的文件名最后输出.的位置

                if (pFile != NULL)  // 输入不为空
                {
                    if (strcmp(pFile, ".xml") == 0) 
                    {
                        this->_files.push_back(dirname + "/" + ptr->d_name);
                    }
                }
            }
        }
        closedir(pDir);
    }

private:
    vector<string> _files;
};

/* void test()
{
    vector<string> file_name;
    string path = "../../SE/data/xml";

    DirScanner dirScanner;
    dirScanner.traverse(path);
    file_name = dirScanner.files();

    for (int i = 0; i < file_name.size(); ++i) 
    {
        cout << file_name[i] << endl;
    }
}

int main(int argc, char *argv[]) 
{
    test();
    return 0;
} */



#endif // !SE_INCLUDE_OFFLINE_DIRSCANNER_H
