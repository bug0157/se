#include "../../include/keyrecommand/KeyRecommander.h"

void test1()
{
    while (1)
    {
        string query;
        std::cout << "pls input key:";
        std::cin >> query;
        KeyRecommander key(query);
        key.execute();
        key.response();
    }
}

int main()
{
    test1();
    return 0;
}

