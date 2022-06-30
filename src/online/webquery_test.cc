#include "../../include/Configuration.h"
#include "../../include/offline/SplitTool.h"
#include "../../include/online/WebPageQuery.h"
#include "../../include/online/retClass.h"
#include <vector>
#include <iostream>

using namespace std;


#if 0
int main(){
    Configuration * con = Configuration::getInstance();
    SplitTool* sp = new SplitToolCppJiaba(con);

    WebPageQuery webi(sp);
    
    while(1){
         //webi.show();
         string word;
         cout<<"INSERT>>>>";
         cin>>word;
         //cout<<webi.doQuery("蝉鸣荒凉")<<endl;
         vector<Retclass> ret = webi.doQueryRet(word);

         cout<<word<<":"<<endl;

         for(auto&it:ret){
             cout<<"title:"<<it.getTitle()<<endl;
             cout<<"link:"<<it.getLink()<<endl;
             cout<<"摘要："<<it.getContent()<<endl;
             cout<<"\n----------------------\n";
         }
    }  

    return 0;

}
#endif
