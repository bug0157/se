#ifndef SE_INCLUDE_CACHE_LRUCACHE_H_
#define SE_INCLUDE_CACHE_LRUCACHE_H_

#include <unordered_map>
#include <list>
#include <string>

using std::string;
using std::unordered_map;
using std::list;



class LRUCache{
public:
    LRUCache(int num = 30);
    LRUCache(const LRUCache &cache);

    struct Node{
        //后面可以拓展，加频率，时间，json
        string _key;
        string _value;
        Node() = default;
        Node(const string &key, const string &value)
        : _key(key)
        , _value(value){

        };
    };

    void addElement(const string &key, const string &value);
    void readFromFile(const string &filename);
    void writeToFile(const string &filename);
    void update(const LRUCache &rhs);
    list<string,string> &getPendingUpdateList();
private:
    int _capacity;                                //缓存节点容量
    unordered_map<string,list<Node>::iterator> _hashMap;//hashtable进行查找
    list<Node> _resultsList;       //保存键值对
    list<Node> _pendingUpdateList; //待更新节点信息
};




#endif // !SE_INCLUDE_CACHE_LRUCACHE_H_
