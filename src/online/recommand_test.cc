#include "../../include/online//KeyRecommander.h"

#if 0
void test1()
{
    while (1)
    {
        string query;
        std::cout << "pls input key:";
        std::cin >> query;
        KeyRecommander keyrecommander;
        keyrecommander.execute(query);
        vector<string> res = keyrecommander.response();
        for (auto &it : res) {
            std::cout << it << std::endl; 
        }
    }
}
int main()
{
    test1();
    return 0;
}
#endif
