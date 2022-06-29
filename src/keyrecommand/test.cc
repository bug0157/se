#include "../../include/keyrecommand/KeyRecommander.h"

void test1()
{
    string query = "计算机";
    KeyRecommander key(query);
    key.execute();
    key.response();
}

int main()
{
    test1();
    return 0;
}

