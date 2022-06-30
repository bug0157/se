#include "SearchEngineServer.h"
#include "../include/offline/SplitTool.h"
void test(){
    Configuration *conf = Configuration::getInstance();
    SearchEngineServer sesever(5, 10, "127.0.0.1", 8888, conf);
    SplitTool *tool = new SplitToolCppJiaba(conf);
    WebPageQuery webquery(tool);
    sesever.start(webquery);
}

int main (int argc, char *argv[])
{
    test();
    return 0;
}
