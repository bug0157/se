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


