#ifndef SE_INCLUDE_CACHE_CACHEMANAGER_H_
#define SE_INCLUDE_CACHE_CACHEMANAGER_H_

#include <pthread.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

class LRUCache;


class CacheManager{
public:
    static CacheManager *getInstance();
    static void initCache(const string &filename);
    static void destory();

    LRUCache &getCache(size_t idx);
    void updateCache();
private:
    CacheManager();
    ~CacheManager();
    
private:
    static CacheManager * _pCacheManager;
    static pthread_once_t _onec;

    vector<LRUCache> _cacheList;
};







#endif // !SE_INCLUDE_CACHE_CACHEMANAGER_H_
