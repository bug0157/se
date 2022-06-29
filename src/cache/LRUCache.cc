#include "../../include/cache/LRUCahce.h"


LRUCache::LRUCache(int num)
: _capacity(num)
, _hashMap()
, _resultsList()
, _pendingUpdateList(){

}

LRUCache::LRUCache(const LRUCache &cache){
    _capacity = cache._capacity;
    _hashMap = cache._hashMap;
    _resultsList = cache._resultsList;
    _pendingUpdateList = cache._pendingUpdateList;
}

void LRUCache::addElement(const string &key, const string &value ){
    //先判断key是否在队列
    //key已经在队列
    if (_hashMap.count(key)){
        Node newhead = *_hashMap[key];
        _resultsList.erase(_hashMap[key]);
        _resultsList.push_front(newhead);
        _hashMap[key] = _resultsList.begin();
    }
    //key不在队列
    else {
        //判断队列是否满
        //队列不满
        if (_resultsList.size() != _capacity) {
            _resultsList.push_front(Node(key, value));
            _hashMap[key] = _resultsList.begin();
        }
        //队列已满
        else {
            _resultsList.pop_back();
            _resultsList.push_front(Node(key, value));
            _hashMap[key] = _resultsList.begin();
        }
    }
}
