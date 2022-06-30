#include "SearchEngineServer.h"

void test(){
    Configuration *conf = Configuration::getInstance();
    SearchEngineServer sesever(5, 10, "127.0.0.1", 8888, conf);
    sesever.start();
}

int main (int argc, char *argv[])
{
    test();
    return 0;
}
