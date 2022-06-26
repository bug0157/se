#include "../../include/offline/DictProducer.h"

void test(){
    SplitTool *tool = new SplitToolCppJiaba();
    DictProducer dp1(tool);
    dp1.showDict();
}

int main (int argc, char *argv[])
{
    test();
    return 0;
}
