#include "../../include/offline/DictProducer.h"
#include "../../include/Configuration.h"

#if 1
void test(){
    Configuration *conf = Configuration::getInstance(); 
    SplitTool *cutter = new SplitToolCppJiaba(conf);
    DictProducer dp1("../../data/xml/", cutter);
    //dp1.showDict();
    //dp1.showIndex();
    dp1.storeDict("../../data/dict/dict.dat");
    dp1.storeIndex("../../data/dict/index.dat");
}

int main (int argc, char *argv[])
{
    test();
    return 0;
}  
#endif
