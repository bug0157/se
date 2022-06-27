#include "DirScanner.h"

void test()
{
    vector<string> file_name;
    string path = "../../data/xml";

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
}
