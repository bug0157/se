#include "../../include/offline/PageLibPreprocessor.h"
#include "../../include/offline/SplitTool.h"

#if 0
void test(){
    Configuration *conf = Configuration::getInstance(); 
    printf("读取配置文件成功\n");
    SplitTool *tool = new SplitToolCppJiaba(conf); 
    printf("切词工具生成成功\n");
    PageLibPreProcessor plpp(tool);
    plpp.readInfoFromFile();
    //plpp.cutRedundantPages();
    //plpp.showSize();
    plpp.buildInvertIndexTable();
    plpp.storeOnDisk();
    //plpp.showIndex();
}

int main (int argc, char *argv[])
{
    test();
    return 0;
}
#endif
